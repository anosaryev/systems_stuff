#ifndef ZERO_DIVIDED_BY_ZERO
#define ZERO_DIVIDED_BY_ZERO

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define READ 0
#define WRITE 1
#define CLIENTNUM 10
#define INPLEN 100
#define USERLEN 15
#define CLDEBUG 0

struct cons {int kid; int s2ss; int ss2s;};
struct conss {int sd; int ss2s; int s2ss;};
struct thr {int *sd; struct cons *ssray; int *ssnum;};

#endif
