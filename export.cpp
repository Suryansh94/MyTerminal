#include <iostream>
#include <stdlib.h>
#include <cstdio>
using namespace std;
int main()
{
	char * name;
	setenv(name,"hello",1);
	char *ret = getenv(name);
	if(ret!=NULL)
		printf("%s\n",ret);
	else
		printf("LOL\n");
}