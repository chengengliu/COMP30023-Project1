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
#define NOTFOUND "HTTP/1.0 404\r\n\r\n"
#define FOUND "HTTP/1.0 200 OK\r\nConten-type: text/html\r\n\r\n"

struct thread{
  char * root_path;
  int sockid;
  pthread_t thread_id;
};
typedef struct thread thread_t;


//********  NOT SURE HERE. WANNA DO SOMETHING LIKE DIC IN PYTHON???
//// OR MAYBE TRY ENUM ???


/*Craete the socket, get ready to listen I assume?*/
int create_socket(int port_number);
/*Bind the socket with specified address and port Number*/
int bind_socket(struct sockaddr_in *server_address, int port_number,
    int listenid);

int get_file_by_name();
void * thread_handler(void * thread_arg);
void process_request(thread_t arg, char *message, char *buffer);
void respond(char *abs_path, int sock, char *buffer);

void process_url(char *filename, char* buffer, int sock);

void re_success(int fd, char *msg, char *type, char *version);

void * thread_handle_test(void * arg);
//void respond_file(FILE *p, char *buffer, int sock);
