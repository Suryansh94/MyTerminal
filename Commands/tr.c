// Suryansh Agnihotri :)

// Show error for converting lower to digits etc

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

#define OPTION_D 0
#define OPTION_C 0
#define OPTION_S 0
// 0 --- > 95

// USE \ for '  and \ also , Lesson learnt :)
char ascii[] = {' ','!','"','#','$','%','&','\'','(', ')', '*',	'+',',','-','.','/','0','1','2','3','4','5','6','7',				'8',				'9',				':',	';',				'<',	 			'=',				'>',	 			'?',				'@',			'A',				'B',				'C',				'D',				'E',		'F',		'G',		'H',	'I',	 		'J',				'K',	 		'L',				'M',				'N',				'O',				'P',				'Q',				'R',				'S',				'T',				'U',				'V',	 		'W',	 		'X',	 		'Y',	 		'Z',	 		'[',	'\\',']',			'^',				'_',			'`',				'a',	  			'b',	  			'c',	  			'd',  'e',  'f',  			'g',  			'h',  			'i',  			'j',  			'k',  			'l',  			'm',  			'n',  			'o',  			'p',  			'q',  			'r',  			's',  			't',  			'u',  			'v',  			'w',  			'x',  			'y',  			'z',  			'{',			'|',			'}' ,'~'}; 
int translate[96],delete[96];
char input[10000000],output[10000000];
int search(char a)
{
	int i=0;
	for(i=0;i<=95;i++)
	{
		if(ascii[i]==a)
			return i;
	}
}
/// It will return true for range inputs
int ContainHyphen(char *p)
{
	if(p[0]!='-' && p[2]!='-')
	{
		if(p[1]=='-')
			return 1;
		else
			return 0;
	}

	return 0;
}
//   upper , lower,digit,punct , space  
int ContainBracketSet(char *p)
{	
	int len=strlen(p);
	if(len>=9)
	{
		if(p[0]=='[' && p[len-1]==']')
		{
			if(p[1]==':' && p[len-2]==':')
			{
				return 1
			}
			else
			{
				return 0;
			}
		}	
	}
	
	return 0;
}

/// Translation to hoga if second <first , then also
int main(int argc,char* argv[])
{
	int i,k=1,ind,l1,l2,j,input_length,file_present=0,counter=1,file_location=1,option_length,Set_One_location=1; 
	while(file_location<argc)
	{
		if(argv[file_location][0]=='-' &&argv[file_location][0]=='I')
		{
			file_present=1;
			file_location++;
			break;
		}
		file_location++;			
		
	}
	//for(i=0;i<95;i++)
		//printf("%d= %c\n",k++,ascii[i]);
	if(argc<3)
	{
		printf("Error\n");
	}
	else
	{
		if(!file_present)
		{

			// Standard IO case


			/// Checking which options are there
			counter=1;
			while(counter<argc)
			{
				if(argv[counter][0]=='-')  ////////////////////////////// See for invalid options
				{

					option_length=strlen(argv[counter]);
					for(i=1;i<option_length;i++)
					{
						if(argv[counter][i]=='s')
							OPTION_S=1;
						if(argv[counter][i]=='d')
							OPTION_D=1;
						if(argv[counter][i]=='c')
							OPTION_C=1;	
					}
					
					counter++;
				}
				else
				{
					break;
				}
				
			}
			while(1)
			{
				//printf("Hello\n");
				if(OPTION_D)
				{
					Set_One_location=counter;
					if(ContainHyphen(argv[counter]))
					{
						l1=argv[counter][2]-argv[counter][0];
						if(l1<0)
							printf("ERROR\n");//tr: range-endpoints of 'z-a' are in reverse collating sequence order
						else
						{
							
							
						}
					}
				}





				memset(translate,0,sizeof(translate));
				l1=strlen(argv[argc-2]);
				l2 = strlen(argv[argc-1]);
				if(ContainHyphen(argv[argc-2]))
				{
					//printf("Yo, It contains\n");
					if(ContainHyphen(argv[argc-1]))
					{
						//printf("Yo\n");
						l1=argv[argc-2][2]-argv[argc-2][0]+1; // Range of input 1
						l2=argv[argc-1][2]-argv[argc-1][0]+1; // Range of input 2


						if(l1<0 || l2<0)
							printf("ERROR\n");
						//printf("%d %d\n",l1,l2 );
						
						if(l1<=l2)
						{
							k=0;
							while(k<l1)
							{
								ind = search(argv[argc-1][0]+k);
								translate[argv[argc-2][0]-' '+k]=ind+1;
								//printf("%d ",ind+1);
								k++;
							}
							scanf("%s",input);
							input_length = strlen(input);
							for(i=0;i<input_length;i++)
							{
								if(translate[input[i]-' '])
								{
							//printf("YO\n");
							//printf("%d\n",translate[input[i]-' ']-1);
								printf("%c",ascii[translate[input[i]-' ']-1]); // Remember we added one :)
								}
								else
								{
									printf("%c",input[i]);
								}
							}
							printf("\n");
						}
						else
						{
							k=0;
							while(k<l2)
							{
								ind = search(argv[argc-1][0]+k);
								translate[argv[argc-2][0]-' '+k]=ind+1;
									//printf("%d ",ind+1);
								k++;
							}
							j=k;
							while(j<l1)
							{

								translate[argv[argc-2][0]-' '+k]=ind+1;
									//printf("%d ",ind+1);
								k++;
								j++;
							}
							scanf("%s",input);
							input_length = strlen(input);
							for(i=0;i<input_length;i++)
							{
								if(translate[input[i]-' '])
								{
							//printf("YO\n");
							//printf("%d\n",translate[input[i]-' ']-1);
								printf("%c",ascii[translate[input[i]-' ']-1]); // Remember we added one :)
								}
								else
								{
									printf("%c",input[i]);
								}
							}
							printf("\n");

						}
					}	
					else
					{
						//printf("Yo It doesn't\n");
						l1=argv[argc-2][2]-argv[argc-2][0]+1; // Range of input 1


						if(l1<0 )
							printf("ERROR\n");


						l2=strlen(argv[argc-1]); // Range of input 2
						if(l1<=l2)
						{

							k=0;
							while(k<l1)
							{
								ind = search(argv[argc-1][k]);
								translate[argv[argc-2][0]-' '+k]=ind+1;
								//printf("%d ",ind+1);
								k++;
							}
							scanf("%s",input);
							input_length = strlen(input);
							for(i=0;i<input_length;i++)
							{
								if(translate[input[i]-' '])
								{
							//printf("YO\n");
							//printf("%d\n",translate[input[i]-' ']-1);
								printf("%c",ascii[translate[input[i]-' ']-1]); // Remember we added one :)
								}
								else
								{
									printf("%c",input[i]);
								}
							}
							printf("\n");
						}
						else
						{
							//printf("YOoo\n");
							k=0;
							while(k<l2)
							{
								ind = search(argv[argc-1][k]);
								translate[argv[argc-2][0]-' '+k]=ind+1;
									//printf("%d ",ind+1);
								k++;
							}
							j=k;
							while(j<l1)
							{

								translate[argv[argc-2][0]-' '+k]=ind+1;
									//printf("%d ",ind+1);
								k++;
								j++;
							}
							scanf("%s",input);
							 input_length = strlen(input);
							for(i=0;i<input_length;i++)
							{
								//printf("%c %d\n",input[i],translate[input[i]-' '] );
								if(translate[input[i]-' '])
								{
							//printf("YO\n");
							//printf("%d\n",translate[input[i]-' ']-1);
								printf("%c",ascii[translate[input[i]-' ']-1]); // Remember we added one :)
								}
								else
								{

									printf("%c",input[i]);
								}
							}
							printf("\n");

						}

					} 
				}
				else
				{
					//printf("%d %d\n",l1,l2 );
					if(ContainHyphen(argv[argc-1]))
					{
						l2=argv[argc-1][2]-argv[argc-1][0]+1;

						
						if(l2<0)
							printf("ERROR\n");


						l1=strlen(argv[argc-2]);
						if(l1<=l2)
						{	
							for(i=0;i<l1;i++)
							{
								ind = search(argv[argc-1][0]+i);
								//printf("INDEX of %c %d\n",argv[argc-1][i],ind);
								translate[argv[argc-2][i]-' ']=ind+1; // making >=1	
								//printf("%d",translate[argv[argc-2][i]-' ']);
							}
							scanf("%s",input);
							input_length = strlen(input);
							for(i=0;i<input_length;i++)
							{
								if(translate[input[i]-' '])
								{
									//printf("YO\n");
									//printf("%d\n",translate[input[i]-' ']-1);
									printf("%c",ascii[translate[input[i]-' ']-1]); // Remember we added one :)
								}
								else
								{
									printf("%c",input[i]);
								}
							}
							printf("\n");
						}
						else
						{
							for(i=0;i<l2;i++)
							{
								ind = search(argv[argc-1][0]+i); // index of second string
								//printf("INDEX of %c %d\n",argv[argc-1][i],ind);
								translate[argv[argc-2][i]-' ']=ind+1; // making >=1	
								//printf("%d",translate[argv[argc-2][i]-' ']);
							}
							
							j=i;
							i--;
							while(j<l1)
							{
								
								translate[argv[argc-2][j]-' ']=ind+1;
								//	printf("%d",translate[argv[argc-2][i]-' ']);
								j++;
							}
							scanf("%s",input);
							input_length = strlen(input);
							for(i=0;i<input_length;i++)
							{
								if(translate[input[i]-' '])
								{
									//printf("YO\n");
									//printf("%d\n",translate[input[i]-' ']-1);
									printf("%c",ascii[translate[input[i]-' ']-1]); // Remember we added one :)
								}
								else
								{
									printf("%c",input[i]);
								}
							}
							printf("\n");	
						}
					}
					else
					{
						l1=strlen(argv[argc-2]);l2=strlen(argv[argc-1]);
						if(l1<=l2)
						{
							//printf("YO\n");
							for(i=0;i<l1;i++)
							{
								ind = search(argv[argc-1][i]);
								//printf("INDEX of %c %d\n",argv[argc-1][i],ind);
								translate[argv[argc-2][i]-' ']=ind+1; // making >=1	
								//printf("%d",translate[argv[argc-2][i]-' ']);
							}
							scanf("%s",input);
							int input_length = strlen(input);
							for(i=0;i<input_length;i++)
							{
								if(translate[input[i]-' '])
								{
									//printf("YO\n");
									//printf("%d\n",translate[input[i]-' ']-1);
									printf("%c",ascii[translate[input[i]-' ']-1]); // Remember we added one :)
								}
								else
								{
									printf("%c",input[i]);
								}
							}
							printf("\n");
						
						}
						else
						{
							for(i=0;i<l2;i++)
							{
								ind = search(argv[argc-1][i]); // index of second string
								//printf("INDEX of %c %d\n",argv[argc-1][i],ind);
								translate[argv[argc-2][i]-' ']=ind+1; // making >=1	
								//printf("%d",translate[argv[argc-2][i]-' ']);
							}
							
							j=i;
							i--;
							while(j<l1)
							{
								ind = search(argv[argc-1][i]);
								translate[argv[argc-2][j]-' ']=ind+1;
								//	printf("%d",translate[argv[argc-2][i]-' ']);
								j++;
							}
							scanf("%s",input);
							input_length = strlen(input);
							for(i=0;i<input_length;i++)
							{
								if(translate[input[i]-' '])
								{
									//printf("YO\n");
									//printf("%d\n",translate[input[i]-' ']-1);
									printf("%c",ascii[translate[input[i]-' ']-1]); // Remember we added one :)
								}
								else
								{
									printf("%c",input[i]);
								}
							}
							printf("\n");
						
						}	
					}
						
				}
				
			}
			
		}
	}
}