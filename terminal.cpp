// Suryansh Agnihotri :)
// suryansh94 


#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
// waiting
#include <sys/types.h>
#include <sys/wait.h>

#include <fstream> // ifstream
// fork
#include <unistd.h>
//open
#include <fcntl.h>

// exit(0)
#include <cstdlib>

#include <signal.h>

#define MAXARGS 100
using namespace std;
char * a[MAXARGS];
string input;
vector<int> separator_of_commands;
vector<string> commands, tokens, echo_tokens;
string out_file_name,input_file,history_commands;
int pipe_present, redirection_present,inp_redirection_present,bang_operator_present,bang_value_from_last, first_time = 0,pid;
set<string> builtin_commands;
char current_working_directory[1024];
map<int ,string>history;
int count_history_items=1;
volatile sig_atomic_t flag = 0,flag1=0;
void my_function(int sig){ // can be called asynchronously
  flag = 1; // set flag
}
void update_history_map()
{

 	ifstream file_read("history.txt");
		if (file_read.is_open()) {
			
			while (getline(file_read, history_commands)) {

				history[count_history_items++]=history_commands;
			}

		}
}
void update_history_file()
{	
	ofstream file("history.txt");
	if (file.is_open()) 
	{
		for(unsigned int i=1;i<=history.size();i++)
		{
			file<< history[i]<<endl;
		}

	} 
	else
	{
		perror("file");
	}
}

void print_history()
{
	// cout<<"History size = "<<history.size()<<endl;
	// int n;
	// cin>>n;
	if(history.size()==0)
	{
		cout<<"No items till now\n";
	}
	for(unsigned int i=1;i<=history.size();i++)
	{
		cout<<i<<" "<<history[i]<<endl;
	}
}
void enter_command() {
	getline(cin, input);
}
void get_file_name(string input) {
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
				redirection_present = 1;
			}
		}
	}
	// Till here I got my separators , ie unguarded  pipes
	if(last_position!=-1)
	{
		int x=last_position+1;
		while(x<length &&  ( input[x]==' ' || x=='\t')  )
			x++;
		int start=x;
		int end = x+1;
		while(end <length &&  (input[end]!=' ' && input[end]!='\t') )
		{
			
			end++;
		}
		out_file_name = input.substr(start,end-start);
			
	}
	
}
void get_input_file_name(string input) {
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
		if (input[i] == '<') {
			if (double_quote_prefix[i] % 2 == 0 && (single_quote_prefix[i] % 2
					== 0)) {
				last_position=i;
				inp_redirection_present = 1;
			}
		}
	}
	// Till here I got my separators , ie unguarded  pipes
	if(last_position!=-1)
	{
		int x=last_position+1;
		while( (x<length) && (input[x]==' '  || (x=='\t') ) )
			x++;
		int start=x;
		int end = x+1;
		while(end <length &&  (input[end]!=' ' && input[end]!='\t') )
		{
			
			end++;
		}
		input_file = input.substr(start,end-start);
			
	}
	
}
void echo_tokenise(string s) {

	echo_tokens.clear();
	int start = 0, l = s.length();
	string token;
	for (int i = 0; i < l;) {
		if (s[i] == ' ' || s[i] == '\t') {

			//cout<<"Start is "<<start<<endl;
			//token=s.substr(start,i-start);
			//cout<<"Space wala token is "<<token<<endl;
			//tokens.push_back(token);
			while (s[i] == ' ' || s[i] == '\t')
				i++;

			if (start != 0)
				echo_tokens.push_back(" ");
			start = i;
			//cout<<"Start becomes "<<start<<endl;
		}
		if (s[i] == '"') {
			start = i + 1;
			i++;
			while (s[i] != '"') {
				i++;
			}

			token = s.substr(start, i - start);
			//cout<<"token is "<<token<<endl;
			echo_tokens.push_back(token);
			i++;
			start = i;
			//  cout<<"Entering start "<<start<<endl;
			// cout<<"Start at index"<<"D"<<s[start]<<"D"<<endl;
			// while(start<l && s[start]==' ' || s[start]=='\t')
			// {
			// 	start++;
			// 	i++;
			// }
			// cout<<"Exiting start "<<start<<endl;
		} else if (s[i] == '\'') {

			start = i + 1;
			i++;
			while (s[i] != '\'') {
				i++;
			}
			//cout<<"start is "<<start<<"i is"<<i<<endl;
			token = s.substr(start, i - start);
			//cout<<"token ' is "<<token<<endl;
			echo_tokens.push_back(token);
			i++;
			start = i;
			// while(start<l && s[start]==' ' || s[start]=='\t')
			// {
			// 	start++;
			// 	i++;
			// }

			//cout<<"New start is "<<start<<endl;
		} else {

			while (i < l && s[i] != ' ' && s[i] != '\t' && s[i] != '"' && s[i]
					!= '\'')
				i++;
			token = s.substr(start, i - start);
			//cout<<"token is "<<token<<endl;
			echo_tokens.push_back(token);
			start = i;
		}
	}
	if (start < l) {
		token = s.substr(start, l - start);
		//cout<<"Last token is "<<token<<endl;		
		echo_tokens.push_back(token);
	}

}
void tokenise(string s) {
	int start = 0, l = s.length();
	string token;
	for (int i = 0; i < l;) {
		if (s[i] == ' ' || s[i] == '\t') {
			//cout<<"Start is "<<start<<endl;
			//token=s.substr(start,i-start);
			//cout<<"Space wala token is "<<token<<endl;
			//tokens.push_back(token);
			while (s[i] == ' ' || s[i] == '\t')
				i++;
			start = i;
			//echo_tokens.push_back(" ");
		}
		if (s[i] == '"') {
			start = i + 1;
			i++;
			while (s[i] != '"') {
				i++;
			}

			token = s.substr(start, i - start);
			tokens.push_back(token);
			i++;
			start = i;
			//  cout<<"Entering start "<<start<<endl;
			// cout<<"Start at index"<<"D"<<s[start]<<"D"<<endl;
			// while(start<l && s[start]==' ' || s[start]=='\t')
			// {
			// 	start++;
			// 	i++;
			// }
			// cout<<"Exiting start "<<start<<endl;
		} else if (s[i] == '\'') {

			start = i + 1;
			i++;
			while (s[i] != '\'') {
				i++;
			}
			//cout<<"start is "<<start<<"i is"<<i<<endl;
			token = s.substr(start, i - start);

			tokens.push_back(token);
			i++;
			start = i;
			// while(start<l && s[start]==' ' || s[start]=='\t')
			// {
			// 	start++;
			// 	i++;
			// }

			//cout<<"New start is "<<start<<endl;
		} else {

			while (i < l && s[i] != ' ' && s[i] != '\t' && s[i] != '"' && s[i]
					!= '\'')
				i++;
			token = s.substr(start, i - start);
			tokens.push_back(token);
			//i++;
			start = i;
		}
	}
	if (start < l) {
		token = s.substr(start, l - start);

		tokens.push_back(token);
	}

}

string trim(string s) {
	string new_string;
	int l = s.length();
	int start = 0, end = l - 1;
	for (int i = 0; i < l; i++) {
		if (s[i] == ' ' || s[i] == '\t')
			continue;
		else {
			start = i;
			break;
		}
	}
	for (int i = l - 1; i >= 0; i--) {
		if (s[i] == ' ' || s[i] == '\t') {
			continue;
		} else {
			end = i;
			break;
		}
	}

	// Check this condition if debugging 

	if (start == 0 && end == l - 1 && s[start] == ' ' && s[end] == ' ')
		return "";

	new_string = s.substr(start, end - start + 1);
	return new_string;
}


// return command number
string bang(string s)
{
	string out="";
	if(s[0]=='!')
	{
		bang_operator_present=1;
		out=s.substr(1);
	}
	else
	{
		bang_operator_present=0;
	}
	return out;
}
void bang_parse_the_command(string input) {
	// separate different commands in input by '|' which is not guarded
	// After that deal with the spaces (trailing , leading and intermediate ) and give me standard format command
	int l = input.length();
	// int double_quote_prefix[length], single_quote_prefix[length];
	// memset(double_quote_prefix, 0, sizeof(double_quote_prefix));
	// memset(single_quote_prefix, 0, sizeof(single_quote_prefix));
	// if (input[0] == '"')
	// 	double_quote_prefix[0] = 1;
	// if (input[0] == '\'')
	// 	single_quote_prefix[0] = 1;
	// for (int i = 1; i < length; i++) {
	// 	if (input[i] == '\"') {
	// 		double_quote_prefix[i] = double_quote_prefix[i - 1] + 1;
	// 	} else {
	// 		double_quote_prefix[i] = double_quote_prefix[i - 1];
	// 	}
	// 	if (input[i] == '\'') {
	// 		single_quote_prefix[i] = single_quote_prefix[i - 1] + 1;
	// 	} else {
	// 		single_quote_prefix[i] = single_quote_prefix[i - 1];
	// 	}
	// }
	// Till here I got my prefix sum
	// for (int i = 0; i < length; i++) {
	// 	if (input[i] == '|') {
	// 		if (double_quote_prefix[i] % 2 == 0 && (single_quote_prefix[i] % 2
	// 				== 0)) {
	// 			separator_of_commands.push_back(i);
	// 			pipe_present = 1;
	// 		}
	// 	}
		
	// }

	for(int i=0;i<l;)
	{
		if(input[i]=='|')
		{
			separator_of_commands.push_back(i);
			i++;
		}
		else
		{
			if(input[i]=='"')
			{
				i++;
				while(i<l && input[i]!='"')
				{
					i++;
				}
				i++;
			}
			else if(input[i]=='\'')
			{
				i++;
				while(i<l && input[i]!='\'')
				{
					i++;
				}
				i++;	
			}
			else
				i++;
		}
	}

	// Till here I got my separators , ie unguarded  pipes
	if (separator_of_commands.size() == 0) {
		// No pipe , single command
		
		commands.push_back(trim(input));
	} else {
		int start = 0;
		string raw_commands;
		for (unsigned int i = 0; i < separator_of_commands.size(); i++) {
			//cout << start << " " << separator_of_commands[i] << endl;
			raw_commands
					= input.substr(start, separator_of_commands[i] - start);
			start = separator_of_commands[i] + 1;
			commands.push_back(trim(raw_commands));
		}
		commands.push_back(
				trim(
						input.substr(
								start,
								separator_of_commands[separator_of_commands.size()
										- 1] - start)));

		// Till here I got commands without leading and trailing spaces ...
		// If cd | then two commands are stored in which one is null ,handle it separately

	}
}
void parse_the_command() {
	// separate different commands in input by '|' which is not guarded
	// After that deal with the spaces (trailing , leading and intermediate ) and give me standard format command
	int l = input.length();
	// int double_quote_prefix[length], single_quote_prefix[length];
	// memset(double_quote_prefix, 0, sizeof(double_quote_prefix));
	// memset(single_quote_prefix, 0, sizeof(single_quote_prefix));
	// if (input[0] == '"')
	// 	double_quote_prefix[0] = 1;
	// if (input[0] == '\'')
	// 	single_quote_prefix[0] = 1;
	// for (int i = 1; i < length; i++) {
	// 	if (input[i] == '\"') {
	// 		double_quote_prefix[i] = double_quote_prefix[i - 1] + 1;
	// 	} else {
	// 		double_quote_prefix[i] = double_quote_prefix[i - 1];
	// 	}
	// 	if (input[i] == '\'') {
	// 		single_quote_prefix[i] = single_quote_prefix[i - 1] + 1;
	// 	} else {
	// 		single_quote_prefix[i] = single_quote_prefix[i - 1];
	// 	}
	// }
	// Till here I got my prefix sum
	// for (int i = 0; i < length; i++) {
	// 	if (input[i] == '|') {
	// 		if (double_quote_prefix[i] % 2 == 0 && (single_quote_prefix[i] % 2
	// 				== 0)) {
	// 			separator_of_commands.push_back(i);
	// 			pipe_present = 1;
	// 		}
	// 	}
		
	// }

	for(int i=0;i<l;)
	{
		if(input[i]=='|')
		{
			separator_of_commands.push_back(i);
			i++;
		}
		else
		{
			if(input[i]=='"')
			{
				i++;
				while(i<l && input[i]!='"')
				{
					i++;
				}
				i++;
			}
			else if(input[i]=='\'')
			{
				i++;
				while(i<l && input[i]!='\'')
				{
					i++;
				}
				i++;	
			}
			else
				i++;
		}
	}

	// Till here I got my separators , ie unguarded  pipes
	if (separator_of_commands.size() == 0) {
		// No pipe , single command
		
		commands.push_back(trim(input));
			
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

				int k=0;
				
				if(bang_args[k]=='-')
				{
				
					k++;
					bang_value_from_last=1;
					// negative
				}
				if(bang_args[k]<='9' && bang_args[k]>='1')
				{
					
					// It is a number
					if(bang_value_from_last)
					{
						// negative number

						bang_args=bang_args.substr(1);
					
					}
					const char * buffer;
					buffer  = new char[256];
					buffer=bang_args.c_str();
					int number = atoi(buffer);
					
					if(!bang_value_from_last)
						commands[0]=history[number];
					else
					{
						commands[0]=history[history.size()-number+1];
					}
					
				}
				else
				{
					// Not a number
					// search for last occurrence of that string in file
					for(int i=history.size();i>=1;i--)
					{
						if (history[i].find(bang_args[0]) != std::string::npos) 
						{
						    commands[0]=history[i];
						}
					}
				}
			}
		}





















	} else {
		int start = 0;
		string raw_commands;
		for (unsigned int i = 0; i < separator_of_commands.size(); i++) {
			//cout << start << " " << separator_of_commands[i] << endl;
			raw_commands
					= input.substr(start, separator_of_commands[i] - start);
			start = separator_of_commands[i] + 1;
			commands.push_back(trim(raw_commands));





// Bang Bang 
		bang_operator_present=0;
		bang_value_from_last=0;
		string bang_args = bang(commands[i]);
		if(bang_operator_present)
		{	
			if(bang_args.size()==1)
			{
				if(bang_args[0]=='!')
				{
					// last command execute
					commands[i]=history[history.size()];
				}
				if(bang_args[0]<='9' && bang_args[0]>='1')
				{
					commands[i]=history[bang_args[0]-'0'];
				}
				else
				{
					// search for last occurrence of that letter in file
					for(int j=history.size();j>=1;j--)
					{
						if (history[j].find(bang_args[0]) != std::string::npos) 
						{
						    commands[i]=history[j];
						}
					}	
				}
			}
			else
			{

				int k=0;
				
				if(bang_args[k]=='-')
				{
				
					k++;
					bang_value_from_last=1;
					// negative
				}
				if(bang_args[k]<='9' && bang_args[k]>='1')
				{
					
					// It is a number
					if(bang_value_from_last)
					{
						// negative number

						bang_args=bang_args.substr(1);
					
					}
					const char * buffer;
					buffer  = new char[256];
					buffer=bang_args.c_str();
					int number = atoi(buffer);
					
					if(!bang_value_from_last)
						commands[i]=history[number];
					else
					{
						commands[i]=history[history.size()-number+1];
					}
					
				}
				else
				{
					// Not a number
					// search for last occurrence of that string in file
					for(int j=history.size();j>=1;j--)
					{
						if (history[j].find(bang_args[0]) != std::string::npos) 
						{
						    commands[i]=history[j];
						}
					}
				}
			}
		}


















		}
		commands.push_back(
				trim(
						input.substr(
								start,
								separator_of_commands[separator_of_commands.size()
										- 1] - start)));

		// Till here I got commands without leading and trailing spaces ...
		// If cd | then two commands are stored in which one is null ,handle it separately


		// Bang Bang 
		bang_operator_present=0;
		bang_value_from_last=0;
		string bang_args = bang(commands[commands.size()-1]);
		if(bang_operator_present)
		{	
			if(bang_args.size()==1)
			{
				if(bang_args[0]=='!')
				{
					// last command execute
					commands[commands.size()-1]=history[history.size()];
				}
				if(bang_args[0]<='9' && bang_args[0]>='1')
				{
					commands[commands.size()-1]=history[bang_args[0]-'0'];
				}
				else
				{
					// search for last occurrence of that letter in file
					for(int j=history.size();j>=1;j--)
					{
						if (history[j].find(bang_args[0]) != std::string::npos) 
						{
						    commands[commands.size()-1]=history[j];
						}
					}	
				}
			}
			else
			{

				int k=0;
				
				if(bang_args[k]=='-')
				{
				
					k++;
					bang_value_from_last=1;
					// negative
				}
				if(bang_args[k]<='9' && bang_args[k]>='1')
				{
					
					// It is a number
					if(bang_value_from_last)
					{
						// negative number

						bang_args=bang_args.substr(1);
					
					}
					const char * buffer;
					buffer  = new char[256];
					buffer=bang_args.c_str();
					int number = atoi(buffer);
					
					if(!bang_value_from_last)
						commands[commands.size()-1]=history[number];
					else
					{
						commands[commands.size()-1]=history[history.size()-number+1];
					}
					
				}
				else
				{
					// Not a number
					// search for last occurrence of that string in file
					for(int j=history.size();j>=1;j--)
					{
						if (history[j].find(bang_args[0]) != std::string::npos) 
						{
						    commands[commands.size()-1]=history[j];
						}
					}
				}
			}
		}






		
















	}
}

int change_directory() {
	if (tokens.size() == 1) {
		// No arg with cd , so change to home
		chdir(getenv("HOME"));
	} else {
		// Change the directory as the first arg
		if (tokens[1] == "~") {
			chdir(getenv("HOME"));
		} else if (chdir(tokens[1].c_str()) == -1) {
			// No such dir
			printf("suryansh94: cd: %s: No such file or directory\n",
					tokens[1].c_str());
		}
	}
	return 1;
}
int present_directory() {
	getcwd(current_working_directory, 1024);
	printf("%s\n", current_working_directory);
	return 1;
}

int echo() {

	if (redirection_present) {

		
		
		for (unsigned int i = 0; i < echo_tokens.size(); i++) {
			if( (i-1>=0 && echo_tokens[i]==out_file_name && echo_tokens[i-1] ==">") || echo_tokens[i] ==">" || (i-1>=0 && echo_tokens[i] ==" " && echo_tokens[i-1] ==">") || (i-1>=0 && echo_tokens[i]==out_file_name && echo_tokens[i-1] ==" ")) 
			{

			}
			else
				cout << echo_tokens[i];
		}
		//Output has to be redirected
	} else {
		for (unsigned int i = 0; i < echo_tokens.size(); i++) {
			cout << echo_tokens[i];
		}
		cout << "\n";
	}
	return 1;
}

int execute() {

	//  not for built in
	int number_of_commands = commands.size();
	int status,fd,fdi;

	int dd = dup(1);
	if (number_of_commands == 1) {
		
		// No pipe useds


		// history work
		// insert in history if last command and this command are different

		
		
	
		if(history.size()==0)
		{
			
			// Nothing in it
			history[count_history_items++]=commands[0];
			update_history_file();

		}  
		else
		{
				
			// check last command
			if(history[history.size()]!=commands[0])
			{
				history[count_history_items++]=commands[0];
				update_history_file();
			}
		}

		
		// If it is history command then print history file 
		if(commands[0]=="history")
		{
			print_history();
			return 1;
		}








		if (commands[0] == "exit") {
			return 0;
		}
		if (commands[0] == "") {
			return 1;
		}

		// Tokenised 
		tokenise(commands[0]);
		//cout<<"Tok "<<tokens[0]<<endl;


		// cout<<"Tokens are\n";
		// for (int i = 0; i < tokens.size(); i++) {
		// 		cout << tokens[i] << endl;
		// 	}
		//Check Redirection 
		redirection_present=0;
		 get_file_name(commands[0]);
		 inp_redirection_present=0;
		 get_input_file_name(commands[0]);
			 




		// Check built in or not
		if (builtin_commands.find(tokens[0]) != builtin_commands.end()) {

			int output_redirect=redirection_present;
		
		if(output_redirect)
		{
			
			
			fd = open(out_file_name.c_str(),O_CREAT | O_WRONLY,S_IRWXU);
			dup2(fd,1);
			//close(fd);
			 
		}
		if(inp_redirection_present)
		{
			// cout<<"Inside\n";
			// fdi = open(input_file.c_str(),O_RDONLY);
			// dup2(fdi,0);
			//close(fd);
			 
		}
			// Built in command
		// 	if(output_redirect)
		// {
			
			
		// 	fd = open(out_file_name.c_str(),O_CREAT | O_WRONLY,S_IRWXU);
		// 	dup2(fd,1);
		// 	//close(fd);
			 
		// }
			string check = tokens[0];
			if (check == "cd") {
				// Change the current working dir
				return change_directory();
				// return value specifically for checking exit() , rest all commands return 1
				
			}
			if (check == "pwd") {
				return present_directory();
							}
			if (check == "history") {

				
			}
			if (check == "echo") {
				string echo_it = commands[0].substr(5);
				
				if(echo_it[0]=='$')
				{
					printf("%s\n",getenv(echo_it.substr(1).c_str()) );
				}
				else
				{

					echo_tokens.clear();
					
					echo_tokenise(echo_it);
			

					echo();

					echo_tokens.clear();
					dup2(dd,1);
					//exit(0);	
				}
				
			}
			if (check == "export") {


				 // string toexport = commands[0].substr(7);
				 // string to ,val;
				 // int j;
				 // for(j=0;j<toexport.length();j++)
				 // {
				 // 	if(toexport[j]=='=')
				 // 	{
				 // 		to=toexport.substr(0,j);
				 // 		break;
				 // 	}
				 // }
				 // val=toexport.substr(j+1);
				 // setenv(to.c_str(),val.c_str(),1);
				 // exit(0);
			}
			

		} else {

			 // cout << "Not a built in command\n";
			// cout<<"Command is "<<commands[0]<<endl;
			// cout<<"Commands are "<<commands.size()<<endl;

			



			int output_redirect=redirection_present;
		int d = dup(1);
		int di = dup(0);
		if(output_redirect)
		{

			tokens.erase(std::remove(tokens.begin(), tokens.end(), ">"), tokens.end());
			tokens.erase(std::remove(tokens.begin(), tokens.end(), out_file_name), tokens.end());
			fd = open(out_file_name.c_str(),O_CREAT | O_WRONLY,S_IRWXU);
			dup2(fd,1);
			//close(fd);
			 
		}
		if(inp_redirection_present)
		{
			
			tokens.erase(std::remove(tokens.begin(), tokens.end(), "<"), tokens.end());
			tokens.erase(std::remove(tokens.begin(), tokens.end(), input_file), tokens.end());
			fdi = open(input_file.c_str(),O_RDONLY);
			dup2(fdi,0);
			//close(fdi);
		}




			// Do initialisation for char *a[]
			int n = tokens.size();
			// cout<<"Size is "<<n<<endl;
			for (int j = 0; j < MAXARGS; j++) {
				a[j] = '\0';
			}

			if (tokens[0] == "grep") {
				int flag = 0, k = 0;
				for (int j = 0; j < n + 1;) {
					if (j == 1 && !flag) {
						a[k++] = (char *) "--color=auto";
						flag = 1;
					} else {
						if (j == n) {
							j++;
						} else {
							a[k++] = (char*) tokens[j].c_str();
							j++;
						}
					}
				}

			} else if (tokens[0] == "ls") {
					int flag = 0, k = 0;
					for (int j = 0; j < n + 1;) {
						if (j == 1 && !flag) {

							a[k++] = (char *) "--color=auto";
							flag = 1;
						} else {
							if (j == n) {
								j++;
							} else {

								a[k++] = (char*) tokens[j].c_str();
								j++;
							}
						}
					}

			}

			else
				for (int i = 0; i < n; i++) {
					a[i] = (char*) tokens[i].c_str();
				}

			// Clear tokens
			tokens.clear();

			// for (int i = 0; i < n; i++) {
			// 	printf("%s\n", a[i]);
			// }
			
			int pid = fork();
			if (pid != 0) {
				// parent
			
				waitpid(pid, &status, 0);
				close(fdi);
				dup2(d,1);
				dup2(di,0);

			
				

			} else {	
	
					if( execvp(a[0], a) == -1 )
					{
						printf("%s: command not found\n",a[0]);
					}
			}

		}

	} else {

// history work begins
 // insert in history if last command and this command are different

		
	
		// I am putting input directly without parsing , so hwn bang come first parse it...
		if(history.size()==0)
		{
			
			// Nothing in it
			history[count_history_items++]=input;
			update_history_file();

		}  
		else
		{
				
			// check last command
			if(history[history.size()]!=input)
			{
				history[count_history_items++]=input;
				update_history_file();
			}
		}

		

		// History works end

		
		
		int pipefd[number_of_commands][2];
		for (int i = 0; i < number_of_commands; i++) {
			pipe(pipefd[i]);
		}
		int cmds = commands.size();
		for (int i = 0; i < cmds; i++) {

			
		
		




























		// If it is history command then print history file 
		if(commands[i]=="history")
		{
			print_history();
			return 1;
		}	

			//cout<<"Command is "<<commands[i]<<endl;
			
			if (commands[i] == "exit") {
				return 0;
			}
			if (commands[i] == "") {
				if (i != 0)
					cout
							<< "please try something else , Don't play with me _/\\_\n";
				continue;
			}

			// Tokenising and iniatalisation of char* a[] 

						//cout<<"Currently Tokenising "<<commands[i]<<endl;
			tokenise(commands[i]);



			// Check redirection
			redirection_present=0;
		 get_file_name(commands[i]);
		 inp_redirection_present=0;
		 get_input_file_name(commands[i]);
		 int output_redirect=redirection_present;
		int d = dup(1);
		int di = dup(0);
		if(output_redirect)
		{

			//cout<<"OUT\n";
			tokens.erase(std::remove(tokens.begin(), tokens.end(), ">"), tokens.end());
			tokens.erase(std::remove(tokens.begin(), tokens.end(), out_file_name), tokens.end());
			fd = open(out_file_name.c_str(),O_CREAT | O_WRONLY,S_IRWXU);
			dup2(fd,1);
			//close(fd);
			 
		}
		if(inp_redirection_present)
		{
			
			tokens.erase(std::remove(tokens.begin(), tokens.end(), "<"), tokens.end());
			tokens.erase(std::remove(tokens.begin(), tokens.end(), input_file), tokens.end());
			fdi = open(input_file.c_str(),O_RDONLY);
			dup2(fdi,0);
			//close(fdi);
		}





			// check_built_in_command= 1 when built in commands
			
			int check_built_in_command = (builtin_commands.find(tokens[0])
					!= builtin_commands.end());

			int n = tokens.size();
						 

			// This was my mistake  :) 
			// So set it to null :)

			for (int j = 0; j < MAXARGS; j++) {
				a[j] = '\0';
			}

			if (tokens[0] == "grep") {
				int flag = 0, k = 0;
				for (int j = 0; j < n + 1;) {
					if (j == 1 && !flag) {
						a[k++] = (char *) "--color=auto";
						flag = 1;
					} else {
						if (j == n) {
							j++;
						} else {
							a[k++] = (char*) tokens[j].c_str();
							j++;
						}
					}
				}

			} else if (tokens[0] == "ls") {

				int flag = 0, k = 0;
				for (int j = 0; j < n + 1;) {
					if (j == 1 && !flag) {

						a[k++] = (char *) "--color=auto";
						flag = 1;
					} else {
						if (j == n) {
							j++;
						} else {

							a[k++] = (char*) tokens[j].c_str();
							j++;
						}
					}
				}
			}

			else
				for (int i = 0; i < n; i++) {
					a[i] = (char*) tokens[i].c_str();

				}

			//						cout << "Token are :\n";
			//			for (int j = 0; j < n; j++) {
			//				cout << tokens[j] << endl;
			//			}

			// Clearing the tokens for next round
			//tokens.clear();

			int pid = fork();
			if (pid) {
				//parent
				waitpid(pid, &status, 0);
				//cout << "Parent Finished\n";
				close(pipefd[i][1]);
				dup2(d,1);
				dup2(di,0);
				if(inp_redirection_present)
					close(fdi);
				
				tokens.clear();
				// if(i==1 )
				// 	close(fd[i][1]);
				// if(i==2)
				// {
				// 	close(fd[0][0]);
				// 	close(fd[0][1]);
				// 	close(fd[1][0]);
				// 	close(fd[1][1]);
				// }

			} else if (pid == 0) {
				//cout << "Inside Child " << i << endl;


				//cout<<"Token 0 is "<<tokens[0]<<endl;
				if (i == 0) {

					/// First Command 
					dup2(pipefd[i][1], 1);
					//cout<<"Not printed\n";
					//close(fd[1]);
					close(pipefd[i][0]);

					if (check_built_in_command) {

						// cout<< " It is built in command " <<commands[i]<<endl;
						// cout<<"token 0 is "<<tokens[0]<<endl;
						string check = tokens[0];
						//cout<<"check is "<<check<<endl;
						if (check == "cd") {
							// Change the current working dir
							change_directory();
							exit(0);
							// return value specifically for checking exit() , rest all commands return 1
						}
						if (check == "pwd") {

							// If i am doing return present_directory than "Infinite wait :) "
							present_directory();
							exit(0);
						}
						if (check == "history") {
							exit(0);
						}
						if (check == "echo") {
							 string echo_it = commands[i].substr(5);
								//cout<<"echoit is "<<echo_it<<endl;

							echo_tokens.clear();
							//cout<<"Size of echo tokens is"<<echo_tokens.size()<<endl;
							echo_tokenise(echo_it);
							echo();
							echo_tokens.clear();
							exit(0);
						}
						if (check == "export") {
							exit(0);
						}
						tokens.clear();
					} else {
						if( execvp(a[0], a) == -1 )
						{
							printf("%s: command not found\n",a[0]);
						}
					}

					//dup2(d,1);				//cout<<"After Exec \n";

				} else if (i != cmds - 1) {
					dup2(pipefd[i - 1][0], 0);
					dup2(pipefd[i][1], 1);
					close(pipefd[i - 1][0]);

					if (check_built_in_command) {
						string check = tokens[0];
						if (check == "cd") {
							// Change the current working dir
							change_directory();
							exit(0);
							// return value specifically for checking exit() , rest all commands return 1
						}
						if (check == "pwd") {
							present_directory();
							exit(0);
						}
						if (check == "history") {
							exit(0);
						}
						if (check == "echo") {

							string echo_it = commands[i].substr(5);
							//cout<<"echoit is"<<echo_it<<endl;
							echo_tokens.clear();
							//cout<<"Size of echo tokens is"<<echo_tokens.size()<<endl;
							echo_tokenise(echo_it);
							echo();
							echo_tokens.clear();
							exit(0);
						}
						if (check == "export") {
							exit(0);
						}
						tokens.clear();
					} else {
						if( execvp(a[0], a) == -1 )
						{
							printf("%s: command not found\n",a[0]);
						}
					}

					//close(fd[1]);
				
					//dup2(d,1);
				} else {
					// Last command 
					//cout<<"Lat command\n"<<check_built_in_command<<endl;
					dup2(pipefd[i - 1][0], 0);
					close(pipefd[i - 1][0]);

					if (check_built_in_command) {
						string check = tokens[0];
						if (check == "cd") {
							// Change the current working dir
							change_directory();
							exit(0);
							// return value specifically for checking exit() , rest all commands return 1
						}
						if (check == "pwd") {
							present_directory();
							exit(0);
						}
						if (check == "history") {

						}
						if (check == "echo") {



							// If last command is echo make reading from IO 
















							string echo_it = commands[i].substr(5);
							//cout<<"echoit is"<<echo_it<<endl;
							echo_tokens.clear();
							//cout<<"Size of echo tokens is"<<echo_tokens.size()<<endl;
							echo_tokenise(echo_it);
							echo();
							echo_tokens.clear();
						}
						if (check == "export") {

						}
						tokens.clear();
					} else {
						if( execvp(a[0], a) == -1 )
						{
							printf("%s: command not found\n",a[0]);
						}
					}
					//close(fd[1]);
					//dup2(d,1);
					//cout << "Here" << endl;
					

				}

			}

		}

	}

	return 1;
}
void builtin_commands_init() {
	builtin_commands.insert("cd");
	builtin_commands.insert("pwd");
	builtin_commands.insert("export");
	builtin_commands.insert("history");
	builtin_commands.insert("echo");
}
void __clear() {
	separator_of_commands.clear();
	commands.clear();
	tokens.clear();
	pipe_present = redirection_present = 0;
	out_file_name="";
	pipe_present=redirection_present=first_time = 0;

}
void __prompt() {
	char host_name[1204] = "";
	gethostname(host_name, sizeof(host_name));
	getcwd(current_working_directory, 1024);
	printf("%s@%s: %s$ ", getenv("LOGNAME"), host_name,
			current_working_directory);
}
void signalHandler_int(int p){
  //We send a SIGTERM signal to the child process
  if (kill(pid,SIGTERM) == 0){
    printf("\nProcess %d received a SIGINT signal\n",pid);
    //no_reprint_prmpt = 1;     
  }else{
    printf("\n");
  }
}


int main() {

    signal(SIGINT, my_function);
	int first_time = 0;
	builtin_commands_init();
	history.clear();
	update_history_map();
	while (1) {

		 if(flag){ // my action when signal set it 1
		 	printf("\n");
        flag = 0;
    } 

		if (!first_time)
			cout << "Welcome\n";
		first_time = 1;
		__prompt();
		enter_command();
		parse_the_command();
		//cout<<commands[0]<<endl;
		int status = execute();
		if (!status)
			return 0;
		__clear();
	}

}
