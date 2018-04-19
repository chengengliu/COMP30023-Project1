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
int bind_socket(struct sockaddr_in *server_address, int port_number,int listenid){

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

void * thread_handler(void * arg){
  thread_t targ = *(thread_t *)arg;
  char mes[1024];

  int read_len= read(targ.sockid, mes, 1024);
  if(read_len<0){
    perror("Error reading\n");
    exit(1);
  }

  char version[1024] = {0}, filepath[1024] = {0}, method[20] = {0};
  sscanf(mes, "%s %s %s", method, filepath, version);

  char url[1024] = {0};
  strcat(url,targ.root_path);
  strcat(url,filepath);

  char header[200] = {0}, extension[200] = {0}, content_type[200] = {0};
  int counter =0;
  char* temp = filepath;

  while(*temp){
    if(*temp == '.'){
      break;
    }
    counter++, temp++;
  }

  strncpy(extension, filepath+counter, strlen(filepath)-counter);
  strcat(header, FOUND);
  strcat(content_type, "Content-Type: ");

  if(strcmp(extension, ".html")==0){
    strcat(content_type, "text/html");
  }
  if(strcmp(extension,".css") == 0){
    strcat(content_type, "text/css");
  }
  if(strcmp(extension,".jpg")==0){
    strcat(content_type, "image/jpeg");
  }
  if(strcmp(extension, ".js")==0){
    strcat(content_type, "text/javascript");
  }

  strcat(content_type, "\r\n\r\n");


  printf("%s", header);
  printf("%s", content_type);

  FILE *fp;
  char buffer[1024]={0};
  fp = fopen(url, "r");

  if(fp){
    write(targ.sockid,header, strlen(header));
    write(targ.sockid, content_type, strlen(content_type));
    while(!feof(fp)){
      int nread = fread(buffer,1,sizeof(buffer),fp);
      if(nread < 0)
        perror("Error reading the file");
      write(targ.sockid, buffer, nread);
      bzero(buffer, sizeof(buffer));
    }
  }

  if(fp== NULL){
    write(targ.sockid,NOTFOUND,strlen(NOTFOUND));
    close(targ.sockid);
    return NULL;
  }
  close(targ.sockid);
  return NULL;
}


int main(int argc, char **argv) {
  //struct sockaddr_in client_addr;
  //char buffer[MAXSIZE];
  int port_number, listenid, n, thread_num=0;
  struct sockaddr_in server_address, client_addrress;
  int optvalue=1;

  pthread_t threads[THREADNUM];

  //pthread_t thread_id;
  char buffer[1024];
  char* root_path;

  if (argc < 2){
    perror("Error. Need port number\n");
    exit(1);
  }
  if(argc < 3){
    perror("Error. Need root path\n");
    exit(1);
  }
  port_number = atoi(argv[1]);
  root_path = argv[2];


  listenid = socket(AF_INET, SOCK_STREAM, 0);
  if(listenid<0)
    perror("Error on listening\n");

  if(setsockopt(listenid, SOL_SOCKET, SO_REUSEADDR,&optvalue,
    sizeof(optvalue)) < 0 ){
    perror("Error optmising socket\n");
    return -1;
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_number);
  server_address.sin_addr.s_addr = INADDR_ANY;
  if(bind(listenid,(struct sockaddr*)&server_address,sizeof(server_address))<0){
    perror("Error bind\n");
    exit(1);
  }

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
    //printf("%d client socket\n", args->sockid);
    if(pthread_create(&(threads[thread_num]), NULL, thread_handler,
      (void *)args)){
      perror("Error Pthread");
      continue;
    }
    //printf("%d\n", thread_num);
    //free(args);
    thread_num ++;
  }
  close(listenid);


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
  /*


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
  }*/
  /*************************************************************************/

  // Now move to the phase of getting content of GET request in http 1.0


  //Write back to the client side
  /*
  n = write (newsockid, "Hello World", 18);
  if(n< 0 ){
    printf("ERROR writing to the socket \n");
    exit(1);
  }*/

  return 0;
}
