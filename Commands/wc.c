
// Suryansh Agnihotri :)

/// Char count , printf , strlen	, wc ENTER , For every file print it....

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#define BUF_SIZE 1024
char buffer[BUF_SIZE]; 
long newline_count,word_count,byte_count,char_count,longest_line,f=1,fr,myargs=1,rbytes,wbytes,wrongoption;
int option_lines,option_bytes,option_chars,option_longestline,option_words,no_option=1;

char input[10000000];
void CountNewLines()
{
	
	while((rbytes=read(fr,buffer,BUF_SIZE))>0)
	{
		int i=0,len=strlen(buffer);
		// reading 1 characer
		for(i=0;i<rbytes;i++)
			if(buffer[i]=='\n')
			{			
				newline_count++;
			}
	}
}

void CountBytes()
{
	int i=0;
	while((rbytes=read(fr,buffer,BUF_SIZE))>0)
	{
		// reading 1 characer
		
		
		for(i=0;i<rbytes;i++)
			byte_count++;
	}
}
void CountChar()
{
	int i=0;
	while((rbytes=read(fr,buffer,BUF_SIZE))>0)
	{
		// reading 1 characer
	
		for(i=0;i<rbytes;i++)
			char_count++;
	}
}
void LongestLine()
{
	long curr_length=0;
	while((rbytes=read(fr,buffer,BUF_SIZE))>0)
	{
		int i=0;
		for(i=0;i<rbytes;i++)
		{
			if(buffer[i]=='\n')
			{
				longest_line=longest_line>curr_length?longest_line:curr_length;
				//printf("%ld\n",longest_line);
				curr_length=0;
			}
			else
			{
				if(buffer[i]==' ')
				{
					//printf("%c",buffer);
					curr_length++;
				}
					
				else if(buffer[i]=='\t')
				{
					//printf("%c",buffer);
					curr_length+=4;	
				}
					
				else
				{
					curr_length++;
					//printf("%c",buffer);	
				}
						
			}	
		}
		
		
	}
}
void CountWords()
{
	f=1;
	while((rbytes=read(fr,buffer,BUF_SIZE))>0)
	{
		int i;
		for(i=0;i<rbytes;i++)
		{
				if(buffer[i]==' ' || buffer[i]=='\t' || buffer[i]=='\n')
			{
				f=1;
			}
			else if(f)
			{
				word_count++;
				f=0;
			}	
		}
		
	}
}
int main(int argc ,char *argv[])
{
	//fr=open(argv[1],O_RDONLY);
	//CountNewLines();
	//printf("%ld\n", newline_count);
	long int counter=1,nl=0,words=0,bytes=0,chars=0,longest=0,f1=1,f2=1,curr=0,temp=1,Ol=0,Oc=0,Om=0,OL=0,Ow=0,wo=0,i,file_present=0,file_location=1;
	while(counter<argc )
	{
		if(argv[counter][0]!='-')
			file_present=1;
		counter++;
		if(!file_present)
			file_location++;
	}
	if(!file_present)
	{
		// Standard Input
		char c =getchar();
		while(c!=EOF)
		{
			bytes++;
			chars++;
			if(c=='\n')
			{
				nl++;
				longest=longest>curr?longest:curr;
				curr=0;
			}
			if(c=='\t')
				curr+=4;
			if(c==' ')
				curr++;
			if(c==' ' || c=='\t' || c=='\n')
			{
				f1=1;
			}
				
			else 
			{
				if(f1)
				{
					words++;f1=0;
				}
				curr++;
			}
			c=getchar();
		}
		longest=longest>curr?longest:curr;
		//printf("%ld %ld %ld ",nl ,words,bytes);
		while(temp<argc && argv[temp][0]=='-')
		{
			int lop = strlen(argv[temp]);
			for(i=1;i<lop;i++)
			{
				if(argv[temp][i]=='l')
				{
					Ol=1;
				}
				else if(argv[temp][i]=='c')
				{
					Oc=1;
				}
				else if(argv[temp][i]=='m')
				{
					Om=1;
				}
				else if(argv[temp][i]=='L')
				{
					OL=1;
				}
				else if(argv[temp][i]=='w')
					Ow=1;
				
				else {
						wo=1;
						printf("wc: invalid option -- \'%c\'\n",argv[temp][i]);
						printf("Try 'wc --help' for more information.\n");	
				}
					
			}
			temp++;
		}

		if(Ol &&!wo)
			{
				printf("	%ld ",nl);
					nl=0;
			}
		if(Ow&&!wo)
		{

			printf("	%ld ",words);
			words=0;
		}
		if(Oc&&!wo)
		{
			printf("	%ld ",chars);
					chars=0;
		}
		if(Om&&!wo)
		{

			printf("	%ld ",bytes);
			byte_count=0;
		}
		if(OL&&!wo)
		{
			printf("	%ld",longest);
					longest=0;
		}

		printf("\n");
	}
	else
	{
		// File is present
		counter=1;
		while(counter<argc && argv[counter][0]=='-')
			counter++;
		fr = open(argv[counter],O_RDONLY);
	//int fw = open(argv[2],O_WRONLY | O_CREAT);
	if(fr==-1)
	{
		printf("wc: %s: No such file or directory\n",argv[argc-1]);
	}
	/*if(fw==-1)
	{
		printf("ERROR W\n");
	}*/
	else
	{
		if(myargs==argc-1)
		{
			// No options; Print new line ,word bytecount
			while((rbytes=read(fr,&buffer,BUF_SIZE))>0)
			{
				
				for(i=0;i<rbytes;i++)
				{
					// reading 1 character

					//printf("%c\n",buffer);
					byte_count++;
					if(buffer[i]=='\n')	
					{
						newline_count++;
					}
				    if(buffer[i]==' ' || buffer[i]=='\n'|| buffer[i]=='\t')
					{
						f=1;
					}
			   		else if(f)
					{
						f=0;
						word_count++;
					}	
				}				
									

			}
			printf("%ld %ld %ld ",newline_count,word_count,byte_count);



		}
		while(myargs<argc )
		{
			if(argv[myargs][0]!='-')
			{
				myargs++;
				
				continue;
			}
			
			int len = strlen(argv[myargs]);
			int k=1;
			while(k<len)
			{
				
				char option = argv[myargs][k];
				
					switch(option)
					{
						case 'l':
									// Printing Number of new Lines
							if(option_lines)
								break;

							fr = open(argv[file_location],O_RDONLY);
							CountNewLines();
							//myargs++;
							option_lines=1;
							break;
						case 'c':
							if(option_bytes)
								break;
							option_bytes=1;
							fr = open(argv[file_location],O_RDONLY);
							CountBytes();
							
							//myargs++;
							break;
						case 'm':
						if(option_chars)
							break;
							option_chars=1;
							fr = open(argv[file_location],O_RDONLY);
							CountChar();
							
							//myargs++;	
							break;
						case 'L':
						if(option_longestline)
							break;
							option_longestline=1;
							fr = open(argv[file_location],O_RDONLY);
							LongestLine();
							//myargs++;	
							break;
						case 'w':
							if(option_words)
								break;
							option_words=1;
							fr=open(argv[file_location],O_RDONLY);
							CountWords();//myargs++;
							break;
						default:
							printf("wc: invalid option -- \'%c\'\n",option);
							printf("Try 'wc --help' for more information.\n");
							wrongoption=1;
							//myargs++;
							break;
							/*	wc: invalid option -- 'W'
		Try 'wc --help' for more information.*/

				}	
				k++;
			}
			myargs++;
			

			
		}
		/*The  options
       below  may  be  used to select which counts are printed, always in the following order: newline, word, character, byte, maximum
       line length.*/
		
		if(option_lines &&!wrongoption)
			{
				printf("%ld ",newline_count);
					newline_count=0;
			}
		if(option_words&&!wrongoption)
		{

			printf("%ld ",word_count);
			word_count=0;
		}
		if(option_chars&&!wrongoption)
		{
			printf("%ld ",char_count);
					char_count=0;
		}
		if(option_bytes&&!wrongoption)
		{

			printf("%ld ",byte_count);
			byte_count=0;
		}
		if(option_longestline&&!wrongoption)
		{
			printf("%ld ",longest_line);
					longest_line=0;
		}
		if(!wrongoption)
			printf("%s\n",argv[file_location]);	
		/*int rbytes,wbytes;
		while((rbytes=read(fr,&buffer,BUF_SIZE))>0)
		{
			// reading 1 character

			//printf("%c\n",buffer);
			byte_count++;
			if(buffer=='\n')
			{
				newline_count++;
			}
			if(buffer==' ' || buffer=='\n'|| buffer=='\t')
			{
				f=1;
			}
			else if(f)
			{
				f=0;
				word_count++;
			}

		}
		printf("%d %d %d %s\n",newline_count,word_count,byte_count,argv[1]);*/
		close(fr);	
	}

	}
	
}