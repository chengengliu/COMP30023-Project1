#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char a[] = "Hello World. I am dancing. ";
	char *b;
	b = strtok(a," ");
	while(b != NULL){
		printf("%s\n", b);
		b = strtok(NULL, " .");
	}
	return 0;
}