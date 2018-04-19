#include "my_server.h"



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

  // Need to check this later.
  while(*temp){
    if(*temp == '.' && *(temp+1)!= '.' && *(temp+1)!='/'){
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
  int port_number, listenfd, n, thread_num=0;
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


  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if(listenfd<0)
    perror("Error on listening\n");

  if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,&optvalue,
    sizeof(optvalue)) < 0 ){
    perror("Error optmising socket\n");
    return -1;
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_number);
  server_address.sin_addr.s_addr = INADDR_ANY;
  if(bind(listenfd,(struct sockaddr*)&server_address,sizeof(server_address))<0){
    perror("Error bind\n");
    exit(1);
  }

  // Announce willingness to accept incoming connection
  if(listen(listenfd, QUEUESIZE)<0){
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
    int client_sock = accept(listenfd, (struct sockaddr *)&client_addrress,
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

    if(pthread_create(&(threads[thread_num]), NULL, thread_handler,
      (void *)args)){
      perror("Error Pthread");
      continue;
    }
    //printf("%d\n", thread_num);
    //free(args);
    thread_num ++;
  }
  close(listenfd);
  return 0;
}
