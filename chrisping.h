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

#define packed __attribute__((packed))

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int64;

#define show(x) _Generic((x),           \
    ip*:    showip($1 # x, (ip *)x),    \
    icmp*:  showicmp($1 # x, (icmp *)x) \
)

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $16 (int128)
#define $c (char *)
#define $i (int)

enum e_type {
    unassigned = 0,
    echo,
    echoreply,
    L4icmp,
    L4tcp,
    L4udp
} packed;
typedef enum e_type type;

struct s_rawicmp {
    int8 type;
    int8 code;
    int16 checksum;
    int8 data[];
} packed;

struct s_icmp {
    type kind:3;
    int16 size;
    int8 *data;
} packed;
typedef struct s_icmp icmp;

struct s_ip {
    type kind:3;
    int32 src;
    int32 dst;
    int16 id;
    icmp *payload;
} packed;
typedef struct s_ip ip;

struct s_rawip {
    int8 version:4;
    int8 ihl:4;
    int8 dscp:6;
    int8 ecn:2;
    int16 length;
    int16 id;
    int8 flags:3;
    int16 offset:13;
    int8 ttl;
    int8 protocol;
    int16 checksum;
    int32 src;
    int32 dst;
    int8 options[];
} packed;

int main(int,char**);
void copy(int8*,int8*,int16);
int16 checksum(int8*,int16);
int16 endian(int16);

// icmp
icmp *mkicmp(type,const int8*,int16);
int8 *evalicmp(icmp*);
void showicmp(int8*,icmp*);

// ip
ip *mkip(type,const int8*,const int8*,int16,int16*);
int8 *evalip(ip*);
void showip(int8*,ip*);