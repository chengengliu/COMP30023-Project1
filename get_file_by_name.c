#include "my_server.h"

int get_file_by_name(char * file_name){
  DIR *dir;
  struct dirent *d;
  directory = opendir("./test");
  //No need to check if the path is correct or not
  //Default as existent
  /*
  while(d= readdir(directory)!=NULL){
    if()
  }*/
