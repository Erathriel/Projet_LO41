#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
if(argc>1)
{
//printf("argument=%s\n",argv[1]);
int id = atoi(argv[1]);
write(id,"Coucou papa",100);
}
}
