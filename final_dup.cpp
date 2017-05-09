
#include <iostream>
#include <string>
#include <cstdlib>
// fork
#include <unistd.h>
 
// wait
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
using namespace std;
 
 
char * a[3],*b[3],*c[3],*d[4];
int main()
{
	int status;
	string s;
	//getline(cin,s);
 
	a[0]="ls";  // First Command
	a[1]="-l";
	// Second Command
	b[0]="grep";
	b[1]="Debug";
	//Third
	c[0]="cat";
	d[0]="cut";
	d[1]="-d";
	d[2]=" ";
	d[3]="-f2";

	cout<<"Enter number of pipes\n";
	int n;
	cin>>n;
	int fd[n][2];
	for(int i=0;i<n;i++)
	{
		pipe(fd[i]);
	}	
	for(int i=0;i<n;i++)
	{
		int pid =fork();
		if(pid)
		{
			//parent
			waitpid(pid,&status,0);
			cout<<"Parent Finished\n";
			close(fd[i][1]);
			// if(i==1 )
			// 	close(fd[i][1]);
			// if(i==2)
			// {
			// 	close(fd[0][0]);
			// 	close(fd[0][1]);
			// 	close(fd[1][0]);
			// 	close(fd[1][1]);
			// }

		}
		else if(pid==0)
		{
			cout<<"Inside Child " <<i <<endl;
 
			if(i==0)
			{

				/// First Command 
				dup2(fd[i][1],1);
				//close(fd[1]);
				close(fd[i][0]);
				execvp(a[0],a);
				//dup2(d,1);				//cout<<"After Exec \n";
 
			}
			else if(i==1)
			{
				dup2(fd[i-1][0],0);
				dup2(fd[i][1],1);
				close(fd[i-1][0]);
				//close(fd[1]);
				execvp(b[0],b);
				//dup2(d,1);
			}

			else if(i==2)
			{
				dup2(fd[i-1][0],0);
				dup2(fd[i][1],1);
				close(fd[i-1][0]);
				//close(fd[1]);
				execvp(c[0],c);
				//dup2(d,1);
			}
			else
			{
				// Last command 
				dup2(fd[i-1][0],0);
				close(fd[i-1][0]);
				//close(fd[1]);
				//dup2(d,1);
				cout<<"Here"<<endl;
				execvp(d[0],d);
				
			}
			// if(i&1)
			// {
			// 	// I want it to be my second command
			// 	dup2(d,1);
			// 	execvp(b[0],b);
			// }
			// else
			// {
			// 	// I want it to be my first command
			// 	execvp(a[0],a);
			// }
			cout<<"Exit Child "<<i<<endl;
			exit(0);
		}
	}

 
}