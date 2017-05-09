#include <bits/stdc++.h>
using namespace std;
string input;
vector<string>tokens,echo_tokens;
	vector<int>separator_of_commands;

void echo_tokenise(string s)
{
	int start=0,l=s.length();
	string token;
	for(int i=0;i<l;)
	{
		if(s[i]==' ' || s[i]=='\t')
		{
			while(s[i]==' ' || s[i]=='\t')
				i++;
			if(start!=0)
				echo_tokens.push_back(" ");
			start=i;
			//cout<<"Start becomes "<<start<<endl;
		}
		if(s[i]=='"')
		{
			start=i+1;
			i++;
			while(s[i]!='"')
			{
				i++;
			}

			token=s.substr(start,i-start);
			//cout<<"token \"is "<<token<<endl;
			echo_tokens.push_back(token);
			i++;
			start=i;
		
		}
		else if(s[i]=='\'')
		{

			start=i+1;
			i++;
			while(s[i]!='\'')
			{
				i++;
			}
			//cout<<"start is "<<start<<"i is"<<i<<endl;
			token=s.substr(start,i-start);
			//cout<<"token ' is "<<token<<endl;
			echo_tokens.push_back(token);
			i++;
			start=i;
	
		}
		else
		{
			cout<<"I is "<<i<<endl;
			while(i<l && s[i]!=' ' && s[i]!='\t' && s[i]!='"' && s[i]!='\'')
				i++;
			token=s.substr(start,i-start);
			echo_tokens.push_back(token);
			start=i;
		}
	}
	if(start<l)
	{
		token=s.substr(start,l-start);
		//cout<<"Last token is "<<token<<endl;		
		echo_tokens.push_back(token);
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
			{
				i++;
			}
		}
	}


for(int i=0;i<separator_of_commands.size();i++)
	cout<<separator_of_commands[i]<<endl;
		// Till here I got commands without leading and trailing spaces ...
		// If cd | then two commands are stored in which one is null ,handle it separately

	
}
int main()
{
	getline(cin,input);
	//tokenise();
	parse_the_command();
	cout<<"hello\n";
	cout<<"Size = "<<separator_of_commands.size()<<endl;
	// int l = echo_tokens.size();
	// cout<<"Tokens are\n";
	// for(int i=0;i<l;i++)
	// 	cout<<"!"<<echo_tokens[i]<<"!"<<endl;
	// cout<<"Size of tokens is "<<tokens.size()<<endl;;
}