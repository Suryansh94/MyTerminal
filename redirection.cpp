#include <iostream>
#include <string>
#include <cstdlib>
#include <fcntl.h>
 #include <sys/stat.h>
// fork
#include <unistd.h>

// wait
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
using namespace std;

string input,out_file_name;
void parse_the_command() {
	// separate different commands in input by '|' which is not guarded
	// After that deal with the spaces (trailing , leading and intermediate ) and give me standard format command
	int length = input.length();
	int double_quote_prefix[length], single_quote_prefix[length];
	memset(double_quote_prefix, 0, sizeof(double_quote_prefix));
	memset(single_quote_prefix, 0, sizeof(single_quote_prefix));
	if (input[0] == '"')
		double_quote_prefix[0] = 1;
	if (input[0] == '\'')
		single_quote_prefix[0] = 1;
	for (int i = 1; i < length; i++) {
		if (input[i] == '\"') {
			double_quote_prefix[i] = double_quote_prefix[i - 1] + 1;
		} else {
			double_quote_prefix[i] = double_quote_prefix[i - 1];
		}
		if (input[i] == '\'') {
			single_quote_prefix[i] = single_quote_prefix[i - 1] + 1;
		} else {
			single_quote_prefix[i] = single_quote_prefix[i - 1];
		}
	}
	// Till here I got my prefix sum
	int last_position=-1;
	for (int i = 0; i < length; i++) {
		if (input[i] == '>') {
			if (double_quote_prefix[i] % 2 == 0 && (single_quote_prefix[i] % 2
					== 0)) {
				last_position=i;
				//	redirection_present = 1;
			}
		}
	}
	// Till here I got my separators , ie unguarded  pipes
	string file_name;
	if(last_position!=-1)
	{
		int x=last_position+1;
		while(x<length && input[x]==' ' || x=='\t')
			x++;
		int start=x;
		int end = x+1;
		while(end <length &&  (input[end]!=' ' && input[end]!='\t') )
		{
			
			end++;
		}
		out_file_name = input.substr(start,end-start);
		cout<<out_file_name<<endl;	
	}
	
}
char * a[3],*b[3];
int main()
{
	// getline(cin,input);
	// parse_the_command();
	a[0]="cat";
	int f,status;
	f = open("hello",O_RDONLY);
	//int d = dup(1);
	dup2(f,0);
	int p = fork();
	if(p!=0)
	{
		cout<<"Parent started\n";
		waitpid(p,&status,0);
		//dup2(d,1);
		cout<<"Parent finished\n";
	}
	else if(p==0)
	{
		cout<<"Child Started\n";
		execvp(a[0],a);	
		cout<<"Child Finished\n";
	}
}
