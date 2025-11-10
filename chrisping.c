/* chrisping.c */
#include "chrisping.h"

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
    p->checksum = checksum(p);
}

int main(int argc, char *argv[]) {
    return 0;
}