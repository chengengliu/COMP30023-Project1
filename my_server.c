#include "my_server.h"
typedef MAXSIZE 1024

int main(int argc, char const *argv[]) {
  //struct sockaddr_in client_addr;
  char buffer[MAXSIZE];
  int port_number, listenid;

  if (argc < 2){
    printf("Error. Need port number \n");
  }
  port_number = atoi(argv[1]);


}

int create_socket(int port_numebr){
  struct sockaddr_in server_addr;
  int listenid, optvalue=0;
  if(listenid = socket(AF_INET, SOCK_STREAM, 0)<0){
    printf("Error when creating the socket\n");
    return -1;
  }
  // SO_REUSEADDR: Reuse of the local address is supported
  // SOL_SOCKET: Options to be accessed at socket level, not protocol level.
  if(setsockopt(listenid, SOL_SOCKET, SO_REUSEADDR,(const void *)&optvalue,
          sizeof(optvalue)) < 0 ){
            printf("Error when validating the socket. Abbort ");
            return -1;
  }


  return listenid;
}
