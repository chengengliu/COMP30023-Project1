#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void test(char *a);

int main(int argc, char const *argv[]) {
  /*
  int a= 10;

  //printf("%d\n", a);
  int b[10];
  b[0] = 0;
  b[1 ]= 0;
  //test(a,b);
  //printf("%d\n", b[1]);
  char *c = "Dance";
  //test(a,b,c);
  // /printf("%s\n", c);*/
  char * url = "/somedir/page.html";
  test(url);

  return 0;
}
/*Test for string split in c */

void test(char *a){
  char* temp;
  char *token, *str, *tofree;
  tofree = str = strdup("/somedir/page.html");
  while ((token = strsep(&str, "/")))
    //my_fn(token);
  printf("%s\n", token);
  free(tofree);
  //strsep(&a,"/");



  int m[10];
  //printf("%d %d\n", &m, (m+1));
  /*
  //printf("%p   %p\n", b, b+1);
  *(b+1) = 10;
  //printf("%d\n", *(b+1));
  a = a+1;
  char *copy;
  char message[50];
  //message = *c;
  copy = "Hel";
  strcat(message, copy);
  //printf("%lu", strlen(message));
  //message = copy;
  //strcat(message, copy);
  printf("%s\n", message);*/
}
