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


char * a[3],*b[3];
int main()
{
	int status;
	string s;
	//getline(cin,s);

	a[0]="ls";
	b[0]="cat";

	int length = s.length();
	// int fd1[ 2 ];
	// pipe( fd1 );
	int pid =fork();


	if(pid!=0)
	{
		waitpid(pid,&status,0);

		cout<<"Parent finished\n";
	}
	else if ( pid == 0 ) {
        cout << "Inside child" << endl;

	        
        execvp(a[0],a);
	        // if ( pid = fork() > 0 ) {
	        //     if (dup2( fd1[ 1 ] , 1 ) < 0) {
	        //       cerr << "Err dup2 in child" << endl;
	        //     }
	        //     close( fd1[ 0 ] );
	        //     close( fd1[ 1 ] ); // important; see below
	        //     // Note: /usr/bin, not /bin
	        //     execvp( a[0], a );
	        //     cerr << "Err execing in child" << endl;
	        // }
	        // else if ( pid == 0 ) {
	        //     cout << "Inside grand child" << endl;
	        //     if (dup2( fd1[ 0 ] , 0 ) < 0) {
	        //       cerr << "Err dup2 in gchild" << endl;
	        //     }
	        //     close( fd1[ 0 ] );
	        //     close( fd1[ 1 ] );
	        //     execvp( b[0], b );
	        //     cerr << "Err execing in grandchild" << endl;
	        // }
	}



}
ofstream file("history.txt", ios_base::app);
							if (file.is_open()) {
								file << "hello\n";

							} else {
								perror("file");
					





					// Check for bang operator !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Bang Bang :)
		bang_operator_present=0;
		bang_value_from_last=0;
		string bang_args = bang(commands[0]);
		if(bang_operator_present)
		{	
			if(bang_args.size()==1)
			{
				if(bang_args[0]=='!')
				{
					// last command execute
					commands[0]=history[history.size()];
				}
				if(bang_args[0]<='9' && bang_args[0]>='1')
				{
					commands[0]=history[bang_args[0]-'0'];
				}
				else
				{
					// search for last occurrence of that letter in file
					for(int i=history.size();i>=1;i--)
					{
						if (history[i].find(bang_args[0]) != std::string::npos) 
						{
						    commands[0]=history[i];
						}
					}	
				}
			}
			else
			{
				// int k=0;
				
				// if(bang_args[k]=='-')
				// {
				
				// 	k++;
				// 	bang_value_from_last=1;
				// 	// negative
				// }
				// if(bang_args[k]<='9' && bang_args[k]>='1')
				// {
					
				// 	// It is a number
				// 	if(bang_value_from_last)
				// 	{
				// 		// negative number

				// 		bang_args=bang_args.substr(1);
					
				// 	}
				// 	const char * buffer;
				// 	buffer  = new char[256];
				// 	buffer=bang_args.c_str();
				// 	int number = atoi(buffer);
				// 	commands[0]=history[number];
					
					
				// }
				// else
				// {
				// 	// Not a number
				// 	// search for last occurrence of that string in file
				// 	// for(int i=history.size();i>=1;i--)
				// 	// {
				// 	// 	if (history[i].find(bang_args[0]) != std::string::npos) 
				// 	// 	{
				// 	// 	    commands[0]=history[i];
				// 	// 	}
				// 	// }
				// }
			}
		}