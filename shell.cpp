#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

// waiting
#include <sys/types.h>
#include <sys/wait.h>

// fork
#include <unistd.h>

// exit(0)
#include <cstdlib>

#define MAXARGS 100
using namespace std;
char * a[MAXARGS];
string input;
vector<int> separator_of_commands;
vector<string> commands, tokens;

int pipe_present, redirection_present;

void enter_command() {
	getline(cin, input);
}

void tokenise(string s) {
	int start = 0, l = s.length();
	string token;
	for (int i = 0; i < l;) {
		if (s[i] == ' ' || s[i] == '\t') {
			//cout<<"Start is "<<start<<endl;
			token = s.substr(start, i - start);

			tokens.push_back(token);
			while (s[i] == ' ' || s[i] == '\t')
				i++;
			start = i;

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

			while (start < l && s[start] == ' ' || s[start] == '\t') {
				start++;
				i++;
			}
			//cout<<"Exiting start "<<start<<endl;
		} else if (s[i] == '\'') {

			start = i + 1;
			i++;
			while (s[i] != '\'') {
				i++;
			}
			//cout<<"start is "<<start<<"i is"<<i<<endl;
			token = s.substr(start, i - start);
			//cout<<"token ' is "<<token<<endl;
			tokens.push_back(token);
			i++;
			start = i;
			while (start < l && s[start] == ' ' || s[start] == '\t') {
				start++;
				i++;
			}

			//cout<<"New start is "<<start<<endl;
		} else {

			i++;
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
	new_string = s.substr(start, end - start + 1);
	return new_string;
}
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
	for (int i = 0; i < length; i++) {
		if (input[i] == '|') {
			if (double_quote_prefix[i] % 2 == 0 && (single_quote_prefix[i] % 2
					== 0)) {
				separator_of_commands.push_back(i);
				pipe_present = 1;
			}
		}
		if (input[i] == '>') {
			if (double_quote_prefix[i] % 2 == 0 && (single_quote_prefix[i] % 2
					== 0)) {

				redirection_present = 1;
			}
		}
	}
	// Till here I got my separators , ie unguarded  pipes
	if (separator_of_commands.size() == 0) {
		// No pipe , single command
		commands.push_back(trim(input));
	} else {
		int start = 0;
		string raw_commands;
		for (int i = 0; i < separator_of_commands.size(); i++) {
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

void execute() {

	// For not built in
	int number_of_commands = commands.size();
	int status;
	if (number_of_commands == 1) {
		// No pipe used
		tokenise(commands[0]);
		//		for (int i = 0; i < tokens.size(); i++) {
		//			cout << tokens[i] << endl;
		//		}

		// Do initialisation for char *a[]
		int n = tokens.size();

			for (int i = 0; i < n; i++) {
				a[i] = (char*) tokens[i].c_str();
			
		}
		
		// Clear tokens
		tokens.clear();

		//		for (int i = 0; i < n; i++) {
		//			printf("%s\n", a[i]);
		//		}
		int pid = fork();
		if (pid != 0) {
			// parent
			waitpid(pid, &status, 0);

		} else {
		
			execvp(a[0], a);
		}
	} else {
		int pipefd[number_of_commands][2];
		for (int i = 0; i < number_of_commands; i++) {
			pipe(pipefd[i]);
		}
		int cmds = commands.size();
		for (int i = 0; i < cmds; i++) {

			// Tokenising and iniatalisation of char* a[] 

			//			cout<<"Currently Tokenising "<<commands[i]<<endl;
			tokenise(commands[i]);
			int n = tokens.size();
			//			cout<<"Tokens size is "<<n<<endl; 

			// This was my mistake  :) 
			// So set it to null :)
			for (int j = 0; j < n; j++) {
				a[i] = '\0';
			}



			
				for (int j = 0; j < n; j++) {
					a[j] = (char*) tokens[j].c_str();

				}

			

			//						cout << "Token are :\n";
			//			for (int j = 0; j < n; j++) {
			//				cout << tokens[j] << endl;
			//			}

			// Clearing the tokens for next round
			tokens.clear();

			int pid = fork();
			if (pid) {
				//parent
				waitpid(pid, &status, 0);
				//cout << "Parent Finished\n";
				close(pipefd[i][1]);
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
				cout << "Inside Child " << i << endl;

				if (i == 0) {

					/// First Command 
					dup2(pipefd[i][1], 1);
					//close(fd[1]);
					close(pipefd[i][0]);
					execvp(a[0], a);
					//dup2(d,1);				//cout<<"After Exec \n";

				} else if (i != cmds - 1) {
					dup2(pipefd[i - 1][0], 0);
					dup2(pipefd[i][1], 1);
					close(pipefd[i - 1][0]);
					//close(fd[1]);
					execvp(a[0], a);
					//dup2(d,1);
				} else {
					// Last command 
					dup2(pipefd[i - 1][0], 0);
					close(pipefd[i - 1][0]);
					//close(fd[1]);
					//dup2(d,1);
					//cout << "Here" << endl;
					execvp(a[0], a);

				}

				cout << "Exit Child " << i << endl;
				exit(0);
			}

		}
	}
}

int main() {
	enter_command();
	parse_the_command();
	execute();

}
