/* chrisping.c */
#include "chrisping.h"

ip *mkip(type kind, const int8 *src, const int8 *dst, int16 id_, int16 *cntptr) {
    int16 id;
    int16 size;
    ip *pkt;

    if (!kind || !src || !dst)
        return (ip *)0;
    
    if (id_)
        id = id_;
    else
        id = *cntptr++;

    size = sizeof(struct s_ip);
    pkt = (ip *)malloc($i size);
    assert(pkt);
    zero($1 pkt, size);

    pkt->kind = kind;
    pkt->id = id;
    pkt->src = inet_addr($c src);
    pkt->dst = inet_addr($c dst);
    pkt->payload = (icmp *)0;

    if (!pkt->dst) {
        free(pkt);
        return (ip*)0;
    }

    return pkt;
}

void showip(int8 *ident, ip *pkt) {
    if (!pkt)
        return;

    printf("(ip *)%s = {\n", $c ident);
    printf(" kind:\t 0x%.02hhx\n", (char)pkt->kind);
    printf(" id:\t 0x%.02hhx\n", $i pkt->id);
    printf(" src:\t %s\n", $c todotted(pkt->src));
    printf(" dst:\t %s\n", $c todotted(pkt->dst));
    printf("}\n");

    if (pkt->payload)
        show(pkt->payload);

    return;
}

// 0xaabb
int16 endian16(int16 x) {
    int8 a, b;
    int16 y;

    b = (x & 0x00ff);
    a = ((x & 0xff00)>>8);
    y = (b << 8)
            | a;

    return y;
}

int16 checksum(int8 *pkt, int16 size) {
    int16 *p;
    int32 acc, b;
    int16 carry;
    int16 n;
    int16 sum;
    int16 ret;

    acc = 0;
    for (n=size, p = (int16 *)pkt; n; n -= 2, p++) {
        b = *p;
        acc += b;
    }
    carry = ((acc & 0xffff0000)>>16);
    sum = (acc & 0x0000ffff);

    ret = ~(sum+carry);

    return endian16(ret);
}

int8 *evalip(ip *pkt) {
    struct s_rawip rawpkt;
    struct s_rawip *rawptr;
    int16 check;
    int8 *p, *ret;
    int8 protocol;
    int16 lengthle;
    int16 lengthbe;
    int8 *icmpptr;
    int16 size;

    if (!pkt)
        return $1 0;

    protocol = 0;
    switch(pkt->kind) {
        case L4icmp:
            protocol = 1;
            break;
        default:
            return $1 0;
            break;
    }

    rawpkt.checksum = 0;
    rawpkt.dscp = 0;
    rawpkt.dst = pkt->dst;
    rawpkt.ecn = 0;
    rawpkt.flags = 0;
    rawpkt.id = endian16(pkt->id);
    rawpkt.ihl = (sizeof(struct s_rawip)/4);

    lengthle = 0;
    if (pkt->payload) {
        lengthle = (rawpkt.ihl*4) + pkt->payload->size
            + sizeof(struct s_rawicmp);
        lengthbe = endian16(lengthle);
        rawpkt.length = lengthbe;
    } else
        lengthle = rawpkt.length = (rawpkt.ihl*4);

    rawpkt.offset = 0;
    rawpkt.protocol = protocol;
    rawpkt.src = pkt->src;
    rawpkt.ttl = 250;
    rawpkt.version = 4;

    if (lengthle%2)
        lengthle++;

    size = sizeof(struct s_rawip);
    p = $1 malloc($i lengthle);
    ret = p;
    assert(p);
    zero(p, lengthle);
    copy(p, $1 &rawpkt, size);
    p += size;

    if (pkt->payload) {
        icmpptr = evalicmp(pkt->payload);
        if (icmpptr) {
            copy(p, icmpptr, pkt->payload->size);
            free(icmpptr);
        }
    }

    check = checksum(ret, lengthle);
    rawptr = (struct s_rawip *)ret;
    rawptr->checksum = check;

    return ret;
}

int8 *evalicmp(icmp *pkt) {
    int8 *p, *ret;
    int16 size;
    struct s_rawicmp rawpkt;
    struct s_rawicmp *rawptr;
    int16 check;

    if (!pkt || !pkt->data)
        return $1 0;

    switch (pkt->kind) {
        case echo:
            rawpkt.type = 8;
            rawpkt.code = 0;
            break;
    
        case echoreply:
            rawpkt.type = 0;
            rawpkt.code = 0;

            break;

        default:
            return $1 0;
            break;
    }

    rawpkt.checksum = 0;
    size = sizeof(struct s_rawicmp) + pkt->size;
    if (size%2)
        size++;

    p = $1 malloc($i size);
    ret = p;
    assert(p);
    zero($1 p, size);

    copy(p, $1 &rawpkt, sizeof(struct s_rawicmp));
    p += sizeof(struct s_rawicmp);
    copy(p, pkt->data, pkt->size);

    check = checksum(ret, size);
    rawptr = (struct s_rawicmp *)ret;
    rawptr->checksum = check;

    return ret;
}

void copy(int8 *dst, int8* src, int16 size) {
    int16 n;
    int8 *sptr, *dptr;

    for (dptr=dst, sptr=src, n=size; n; n--)
        *dptr++ = *sptr++;

    return;
}

icmp *mkicmp(type kind, const int8 *data, int16 size) {
    int16 n;
    icmp *p;

    if (!data || !size)
        return (icmp *)0;

    n = sizeof(struct s_icmp) + size;
    p = (icmp *)malloc($i n);
    assert(p);
    zero($1 p, n);

    p->kind = kind;
    p->size = size;
    p->data = $1 data;

    return p;
}

void showicmp(int8 *ident, icmp *pkt) {
    if (!pkt)
        return;

    printf("(icmp *)%s = {\n", $c ident);
    printf("  kind:\t  %s\n  size:\t  %d\n}\npayload:\n",
        (pkt->kind == echo) ? "echo" : "echo reply",
        $i pkt->size);
    if (pkt->data)
        printhex(pkt->data, pkt->size, 0);
    printf("\n");

    return;
}

int main(int argc, char *argv[]) {
    int8 *str;
    int8 *raw;
    icmp *icmppkt;
    int16 rnd;
    ip *ippkt;
    int16 size;

    (void)rnd;
    srand(getpid());
    rnd = (rand() % 50000);

    str = $1 malloc(6);
    assert(str);
    zero(str, $2 6);
    strncpy($c str, "Hello", 5);

    icmppkt = mkicmp(echo, str, $2 5);
    assert(icmppkt);

    ippkt = mkip(L4icmp, $1 "192.168.10.5", $1 "8.8.8.8", 0, &rnd);
    assert(ippkt);
    ippkt->payload = icmppkt;

    raw = evalip(ippkt);
    size = sizeof(struct s_rawip) + sizeof(struct s_rawicmp)
        + ippkt->payload->size;
    show(ippkt);
    printhex(raw, size, ' ');

    free(icmppkt->data);
    free(icmppkt);
    free(ippkt);
    
    return 0;
}