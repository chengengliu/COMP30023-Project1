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

void *thread_handle_test(void *arg){
  thread_t targ = *(thread_t *)arg;
  char msg[1024]={0};

  int readlen= read(targ.sockid, msg, 1024);
  write(targ.sockid, NOTFOUND, strlen(NOTFOUND));
  return NULL;
}

void re_success(int fd, char *msg, char *type, char *version){
  char buffer[1024];
  char body[1024];

  sprintf(body, "%s", msg);
  sprintf(buffer, "%s 200 OK\r\n", version);
  write(fd,buffer, strlen(buffer));
  sprintf(buffer, "Content-type: %s\r\n\r\n", type);

  write(fd, buffer, strlen(buffer));
  write(fd, body, strlen(body));

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

/*
char * ext_extension(char *filepath){
  char extension[20] = {0};
  int counter =0;
  char* temp = filepath;
  while(*temp){
    if(*temp == '.'){
      break;
    }
    counter++, temp++;
  }
  return extension;
}*/

/*
This is the first version of thread handler.
Not working as i thought. No more use.
*/
// Thread_arg has the client side socket information.
void * thread(void * thread_arg){
  int sock, read_len, nread;
  //FILE *p;
  //char *return_message;
  char rec_message[MAXSIZE];

  char buffer[1024];
  //char *no_found = "404 NOT FOUND";
  thread_t arg = *(thread_t *)thread_arg;

  sock = arg.sockid;

  read_len = read(sock,rec_message,MAXSIZE);
  if(read_len < 0){
    perror("Error reading\n");
    exit(1);
  }
  rec_message[read_len] = '\0';

  char method[1024]={0},re_url[1024] = {0}, version[1024] = {0};
  char filename[40]={0}, extension[10]={0};
  sscanf(rec_message, "%s %s %s", method, re_url, version);

  printf("%s\n", re_url);
  char *temp;
  temp = re_url;


  int counter =0;
  while(*temp){
    if(*temp == '.'){
      break;
    }
    temp ++;
    counter ++;
  }




  //sscanf(re_url,"/%s.%s",filename,extension);

  //printf("Extension is %s\n", extension);

  char abs_path[1024]={0};

  strcpy(abs_path,arg.root_path);
  strcpy(abs_path+strlen(arg.root_path),re_url);
  printf("%s\n", abs_path);

  process_url(abs_path,buffer, sock);
  int nwrite = write(sock,buffer, strlen(buffer));
  if(nwrite < 0)
    perror("Error writing back");


  //write(sock,rec_message,strlen(rec_message));
  /*

  while((read_len = read(sock, rec_message, MAXSIZE))>0){
    // End of message specified by the read length
    rec_message[read_len] = '\0';
    //printf("%s\n", rec_message);
  }*/
  /*This part is important.
   After reading from client side, parse that information down
   And find corresponding fiel and reply*/


   /*
  printf("%s\n", rec_message);
  process_request(*arg, rec_message, buffer); // 404 or 200

  if(read_len == -1){
    printf("Failed to receive\n");
  }*/

  //free((thread_t *) thread_arg);
  //pthread_exit(NULL);
  return 0;
}

void process_url(char *filename, char *buffer, int sock){
  FILE *fp;
  fp =fopen(filename, "r");
  /*
  if(fp==NULL){
    strcpy(buffer, NOTFOUND);
    write(sock,buffer, sizeof(buffer));
    printf("%s\n", buffer);
    return;
  }*/
  if(fp == NULL){
    return;
  }

  strcpy(buffer, FOUND);
  //printf("FOUND FILE\n");
  int read = fread(buffer+strlen(FOUND),1,1024,fp);
  /*
  char response[] = "HTTP/1.0 200 OK\r\n"
          "Content-Type: text/html; charset=UTF-8\r\n\r\n"
          "<doctype !html><html><head><title></title>"
          "<style>body"
          "h1 { font-size:4cm; text-align: center; color: black;"
          "}</style></head>"
          "<body><h1>Not Found  (HTTP 404)</h1></body></html>\r\n";*/
  //printf("%d client socket\n", sock);
  //int nwrite = send(sock, buffer, read, 0);
  //write(sock,buffer, read);
  printf("%s\n", buffer);
}



void process_request(thread_t arg, char *message, char *buffer){
  FILE *p;
  DIR *dir;
  char *root_path, *file_path, abs_path[1024]={0}, *return_message;
  int counter =0;

  root_path = arg.root_path;
  // To read a first space, the beginning of path
  // And if the pointer meets the second space, it is the
  // end of path.
  char temp[1024];
  strcpy(temp,message);
  file_path = strtok(temp," ");

  while(file_path!=NULL){
    file_path = strtok(NULL, " ");
    counter++;
    if(counter == 1){
      break;
    }
  }
  /*File path is extracted hopefully. */
  /*Now that the absolute path is root_path + file_path*/
  //printf("%s\n", file_path);
  strcat(abs_path, root_path);
  strcat(abs_path, file_path);

  //respond(abs_path,arg.sockid,buffer);

  //return p;
  // Extract url of the file
  //dir = opendir();
}
void respond(char *abs_path, int sock, char *buffer){
  FILE *fp;
  //char * return_message;
  fp = fopen(abs_path, "r");
  //respond_file(p,buffer,sock);
  if(fp == NULL){
    strcat(buffer, NOTFOUND);
    printf("HelloNotFoundFile\n");
    printf("HTTP/1.0 404 \n");
  }
  else{
    strcat(buffer, FOUND);
    int read = fread(buffer+strlen(FOUND),1,1024,fp);
    int nwrite = write(sock,buffer, read);
    if(nwrite<0){
      perror("Write fiale\n");
      return;
    }
    printf("HTTP/1.0 200 OK \n");
  }
  //Append body after the header. Since only when file is valid.
  //int read = fread(buffer+strlen(FOUND),1,1024,fp);
  //write(sock,buffer, read);
  //printf("HELLO\n");
}

//void respond_file(FILE *p, char *buffer, int sock){
  /*
  while(TRUE){
    //unsigned char buffer[1024];
    int read = fread(buffer,1,1024,p);
    if(read > 0){
      write(sock,buffer,read);
    }
    if(read < 1024){
      if(feof(p)){
        printf("File read complete\n");
      }
      if(ferror(p)){
        perror("File reading error");
        break;
      }
    }
  }*/
//}


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



  //listenid = create_socket(port_number);
  /*
  if(bind_socket(&server_address, port_number, listenid)<1){
    //printf("Bind failed\n");
    perror("Bind failed\n");
    exit(1);
  }*/

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
