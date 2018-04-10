#include "my_server.h"



int main(int argc, char **argv) {
  //struct sockaddr_in client_addr;

  //char buffer[MAXSIZE];
  int port_number, listenid, newsockid, n;
  struct sockaddr_in server_address, client_addrress;
  char buffer[256];


  if (argc < 2){
    printf("Error. Need port number \n");
  }
  port_number = atoi(argv[1]);
  listenid = create_socket(port_number);
  printf("Listen id is %d\n", listenid);
  if(bind_socket(&server_address, port_number, listenid)<1){
    printf("Bind failed\n");
    exit(1);
  }
  // Announce willingness to accept incoming connection
  if(listen(listenid, QUEUESIZE)<0){
    printf("Listen failed \n");
    exit(1);
  }

  /*Now accept a connection */
  /*For now only create one connection */
  socklen_t client_len = sizeof(client_addrress);
  newsockid = accept(listenid, (struct sockaddr *)&client_addrress,
    &client_len);

  if(newsockid < 0){
    printf("Client socket creation failed. \n");
    exit(1);
  }

  n = read(newsockid, buffer, 255);
  if(n < 0){
    printf("Error reading from the socket\n");
    exit(1);
  }

  n = write (newsockid, "Hello World", 18);
  if(n< 0 ){
    printf("ERROR writing to the socket \n");
    exit(1);
  }

  close(listenid);

  return 0;
}

int create_socket(int port_number){
  //struct sockaddr_in server_addr;
  int listenid, optvalue=0;

  if((listenid = socket(AF_INET, SOCK_STREAM, 0))<0){
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

/*The socket will accept connection to All/Any IPs  */
int bind_socket(struct sockaddr_in *server_address, int port_number,
    int listenid){
  server_address-> sin_family = AF_INET;
  server_address-> sin_port = htons(port_number);
  // INADDR_ANY is "Any address" in IPV4
  // after binding successuffly , It means the socket
  //Will listen at any interface
  server_address-> sin_addr.s_addr = INADDR_ANY;
  if(bind(listenid, (struct sockaddr *)server_address,
    sizeof(*server_address))<0){
    return -1;
  }
  return 1;
}
