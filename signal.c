#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// void my_handler(int s){
//            printf("Caught signal %d\n",s);
//            exit(1); 

// }
int pid;
void signalHandler_int(int p){
 // We send a SIGTERM signal to the child process
  if (kill(pid,SIGTERM) == 0){
    printf("\nProcess %d received a SIGINT signal\n",pid);
    //no_reprint_prmpt = 1;     
  }else{
    printf("\n");
  }
}

int main(int argc,char** argv)
{
  int status;
   struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = signalHandler_int;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);

   
    
    pid =fork();
    if(pid==0)
    {
      signal(SIGINT, SIG_IGN);
      while(1)
      {
        printf(" %d %s\n", getpid(),"Child");
      }
    }
    else
    {
      waitpid(pid,&status,0);
      printf("%s\n", "Parent");
    }

   return 0;    
}