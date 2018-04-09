#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define MAXSIZE 1024
#define QUEUESIZE 1024

/*Craete the socket, get ready to listen I assume?*/
int create_socket(int);
/*Bind the socket with specified address and port Number*/
int bind_socket(struct sockaddr_in *server_address, int port_number,
    int listenid);
