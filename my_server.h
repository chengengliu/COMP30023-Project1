/*
* Written by Liu Chengeng   Student ID : 813174
* This is the project 1 for COMP30023 Computer Systems.
* All rights reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>

#define TRUE 1
#define MAXSIZE 1024
#define QUEUESIZE 1024
#define THREADNUM 200
#define NOTFOUND "HTTP/1.0 404 NOT FOUND\r\n"
#define FOUND "HTTP/1.0 200 OK\r\n"

struct thread{
  char * root_path;
  int sockfd;
  pthread_t thread_id;
};
typedef struct thread thread_t;


void * thread_handler(void * arg);
