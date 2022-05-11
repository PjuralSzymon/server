#ifndef _COMMON_H_
#define _COMMON_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include <pthread.h>

#define SERVER_PORT 18009

#define MAXLINE 8192
#define SA struct sockaddr

void err_n_die(const char *fmt, ...);
char *bin2hex(const unsigned char *input, size_t len);
char *read_file(char * filename);
char *connect_two_txt(char * orginal, char *new);

#endif