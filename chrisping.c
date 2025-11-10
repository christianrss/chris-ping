/* chrisping.c */
#include "chrisping.h"

void copy(int8 *dst, int8* src, int16 size) {
    int16 n;
    int8 *sptr, *dptr;

    for (dptr=dst, sptr=src, n=size; n; n--)
        *dptr++ = *sptr++;

    return;
}

icmp *mkicmp(int8 type, int8 code, int8 *data, int16 size) {
    int16 n;
    icmp *p;

    if (!data || !size)
        return (icmp *)0;

    n = sizeof(struct s_icmp) + size;
    p = (icmp *)malloc($i n);
    assert(p);
    zero($1 p, n);

    p->type = type;
    p->code = code;

    copy($1 &p->data, data, size);
    p->checksum = checksum(p);

    return p;
}

int main(int argc, char *argv[]) {
    return 0;
}