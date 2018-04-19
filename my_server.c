/*
* Autor name: Liu Chengeng
* Login ID : c.liu75@student.unimelb.edu.au
* Student ID : 813174
* This is the project 1 for COMP30023 Computer Systems.
* All copyrights reserved.
*/
#include "my_server.h"

/*
* Thread handler is used to handle read/write in socket, including
* looking for specific file. If success return the file and if fail
* to fetch the file, return 404.
*/
void * thread_handler(void * arg){
  thread_t targ = *(thread_t *)arg;
  char mes[MAXSIZE];

  int read_len= read(targ.sockfd, mes, MAXSIZE);
  if(read_len<0){
    perror("Error reading\n");
    exit(1);
  }

  char version[MAXSIZE] = {0}, filepath[MAXSIZE] = {0},
    method[MAXSIZE] = {0};
  sscanf(mes, "%s %s %s", method, filepath, version);

  char url[MAXSIZE] = {0};
  //Combine root path and file path together to get abs path.
  strcat(url,targ.root_path);
  strcat(url,filepath);

  char header[MAXSIZE] = {0}, extension[MAXSIZE] = {0},
    content_type[MAXSIZE] = {0};
  int counter =0;
  char* temp = filepath;

  //Extract entension of the requested file.
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

  FILE *fp;
  char buffer[MAXSIZE]={0};
  fp = fopen(url, "r");
  //If find the file under the dirc. Write back the header and file.
  if(fp){
    write(targ.sockfd,header, strlen(header));
    write(targ.sockfd, content_type, strlen(content_type));
    //While the file is not ended, keep writing.
    while(!feof(fp)){
      int nread = fread(buffer,1,sizeof(buffer),fp);
      if(nread < 0)
        perror("Error reading the file");
      write(targ.sockfd, buffer, nread);
      bzero(buffer, sizeof(buffer));
    }
  }
  //If the file is not found under the direc, return 404
  if(fp== NULL){
    write(targ.sockfd,NOTFOUND,strlen(NOTFOUND));
    close(targ.sockfd);
    free((thread_t*) arg);
    return NULL;
  }
  close(targ.sockfd);
  free((thread_t *)arg);
  return NULL;
}

/*
* A simple http server.
* Main function will build socket on server side and accept connection.
* Pthread will be created and the task of thread is in thread_handler.
*/

int main(int argc, char **argv) {

  int port_number, listenfd, thread_num=0;
  struct sockaddr_in server_address, client_addrress;
  int optvalue=1;
  pthread_t threads[THREADNUM];
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
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  //Bind address to the socket.
  if(bind(listenfd,(struct sockaddr*)&server_address,
    sizeof(server_address))<0){
    perror("Error binding\n");
    exit(1);
  }
  // Announce willingness to accept incoming connection
  if(listen(listenfd, MAXSIZE)<0){
    perror("Error listening\n");
    exit(1);
  }
  socklen_t client_len = sizeof(client_addrress);
  //Loop to build threads to handle each incoming socket connection
  while(TRUE){
    //Build incoming connection.
    int client_sock = accept(listenfd, (struct sockaddr *)&client_addrress,
      &client_len);

    if(client_sock < 0){
      perror("Error on accepting connection ");
      continue;
    }
    thread_t *args = malloc(sizeof(thread_t));
    if(!args){
      perror("Error allocating memory for thread");
    }
    args-> sockfd = client_sock;
    args-> thread_id = threads[thread_num];
    args-> root_path = root_path;

    if(pthread_create(&(threads[thread_num]), NULL, thread_handler,
      (void *)args)){
      perror("Error Pthread");
      continue;
    }
    thread_num ++;
  }
  close(listenfd);
  return 0;
}
