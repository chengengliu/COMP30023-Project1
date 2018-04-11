#include "my_server.h"

int create_socket(int port_number){
  //struct sockaddr_in server_addr;
  int listenid, optvalue=0;

  if((listenid = socket(AF_INET, SOCK_STREAM, 0))<0){
    //printf("Error when creating the socket\n");
    perror("Listen Error ");

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

/*This handler should do :
Read
Manipulate/Call the function to find specific file.
Send
Close
*/
// Thread_arg has the client side socket information.
void * thread_handler(void * thread_arg){
  int sock, read_len;
  char *buffer, rec_message[MAXSIZE], root_path;
  thread_t *arg = (thread_t *)thread_arg;

  sock = arg->sockid;
  root_path = *(arg->root_path);
  /*
  if(ead_len = read(sock, rec_message, MAXSIZE)){
  }*/
  //Keep reading
  // On success, the number of bytes read is returned (zero indicates end
  // of file), and the file position is advanced by this number.
  while((read_len = read(sock, rec_message, MAXSIZE))>0){
    // End of message specified by the read length
    rec_message[read_len] = '\0';

    write(sock, rec_message, strlen(rec_message));
  }


  if(read_len == 0){
    printf("Client disconnected\n");
  }
  if(read_len == -1){
    printf("Failed to receive\n");
  }
  free((thread_t *) thread_arg);
  pthread_exit(NULL);
  return 0;
}

void process_request(int client_sock){

}





int main(int argc, char **argv) {
  //struct sockaddr_in client_addr;
  //char buffer[MAXSIZE];
  int port_number, listenid, newsockid, n, thread_num=0;
  struct sockaddr_in server_address, client_addrress;
  pthread_t threads[THREADNUM];

  //pthread_t thread_id;
  char buffer[256];
  char* root_path;

  if (argc < 2){
    perror("Error. Need port number \n");
    exit(1);
  }
  if(argc < 3){
    perror("Error. Need root path \n");
    exit(1);
  }
  port_number = atoi(argv[1]);
  root_path = argv[2];
  //printf("%s\n", path);

  listenid = create_socket(port_number);
  //printf("Listen id is %d\n", listenid);
  if(bind_socket(&server_address, port_number, listenid)<1){
    //printf("Bind failed\n");
    perror("Bind failed\n");
    exit(1);
  }
  //printf("%d\n", thread_num);
  // Announce willingness to accept incoming connection
  if(listen(listenid, QUEUESIZE)<0){
    //printf("Listen failed \n");
    perror("Listen failed\n");
    exit(1);
  }
  /*Now accept a connection */
  /*For now only create one connection */
  socklen_t client_len = sizeof(client_addrress);
  /*Add pthread???? Multi-threading test*/
  // I'm thinking maybe the first version(pthread creation) is better than
  // The second version.
  // This is the main loop for threading.

  while(TRUE){
    int client_sock = accept(listenid, (struct sockaddr *)&client_addrress,
      &client_len);

    if(client_sock < 0){
      perror("Error on accepting connection ");
      continue;
    }

    //pthread_t thread_id = threads[i];
    thread_t *args = malloc(sizeof(thread_t));
    args-> sockid = client_sock;
    args-> thread_id = threads[thread_num];
    args-> root_path = root_path;
    //Craete successfully will return value of 0.
    if(pthread_create((threads+thread_num), NULL, thread_handler,
      (void *)&args)){
      perror("Error Pthread");
      continue;
    }
    //printf("%d\n", thread_num);
    thread_num ++;
  }


  /**********************Second Version of thread creating
  while(client_sock = accept(listenid,(struct sockaddr * )&client_addrress,
    &client_len)){
      printf("Connection accepted\n");
      if(pthread_create(&thread_id, NULL, thread_handler,
        (void *)client_addrress)){
          perror("Error creating thread\n");
          continue;
      }
      printf("Assigned connection to thread \n");

    }*/


  //**********single acception (used when doing socket connection test)*******
  //These read and write procedure will be dedicated to
  // pthread to do. But for now, it is just for test use.


  newsockid = accept(listenid, (struct sockaddr *)&client_addrress,
    &client_len);

  if(newsockid < 0){
    printf("Client socket creation failed. \n");
    exit(1);
  }

  n = read(newsockid, buffer, 255);
  //Now the buffer holds information we need
  if(n < 0){
    //printf("Error reading from the socket\n");
    perror("Read Error");
    exit(1);
  }
  /*************************************************************************/

  // Now move to the phase of getting content of GET request in http 1.0


  //Write back to the client side

  n = write (newsockid, "Hello World", 18);
  if(n< 0 ){
    printf("ERROR writing to the socket \n");
    exit(1);
  }
  close(listenid);
  return 0;
}
