/* chrisping.h */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <chrisutils.h>

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int64;

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $16 (int128)
#define $c (char *)
#define $i (int)

struct s_icmp {
    int8 type;
    int8 code;
    int16 checksum;
    int8 data[];
};
typedef struct s_icmp icmp;

icmp *mkicmp(int8,int8,int8*,int16);

int main(int,char**);
void copy(int8*,int8*,int16);