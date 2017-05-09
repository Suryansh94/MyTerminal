#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
char directory_name[1000],curr_path[1000];
char buffer[1000],src[1000],temp[1000],dest[1000],file_name[1000],number[100];
char Lines[1000000][2100];


// Creating the Destination Directory 
void create_dir(int argc, char const *argv[])
{
    struct stat st = {0};
    int ind=0,i,path=0;
    int len = strlen(argv[2]),start=0,curr=0; 
    if(argv[2][0]=='/')
        {
            start++;
            curr++;
            curr_path[path++]='/';
        }
    
    while(curr<len)
    {
        if(argv[2][curr]=='/')
        {
            ind=0;
            for(i=start;i<curr;i++)
            {
                curr_path[path++]=argv[2][i];
            }
            curr_path[path++]='/';
            if (stat(curr_path, &st) == -1)
            {
                mkdir(curr_path, 0700); 
                
            }
            else
            {
                opendir(curr_path);
            }       
            start=curr+1;
        }
        curr++;
    }
    if(start!=curr)
    {
        ind=0;
            for(i=start;i<curr;i++)
            {
                curr_path[path++]=argv[2][i];
            }
            curr_path[path++]='/';  
            if (stat(curr_path, &st) == -1)
            {
                mkdir(  curr_path, 0700);   
            }
            
        
    }
    

    return ;
}
int main(int argc, char const *argv[])
{
    int fd,rbytes;
    DIR* FD;
    struct dirent* files;
    int count=0,lines=0,x=0,y=0,i=0;
    struct stat st = {0};
    int n = atoi(argv[3]);
    
    if (stat(argv[2], &st) == -1) 
    {

        // Creating Directory with location stored in curr_path
        create_dir(argc,argv);
    }
    else
    {
        strcpy(curr_path,argv[2]);
    }
    
    if (NULL == (FD = opendir (argv[1]))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        
        return 1;
    }
    while ((files = readdir(FD))) 
    {
        lines=0,x=0,y=0,count=0;
        if (!strcmp (files->d_name, "."))
            continue;
        if (!strcmp (files->d_name, ".."))    
            continue;
        int len= strlen(argv[1]);
        strcpy(src,argv[1]);
        if(argv[1][len-1]!='/')
            strcat(src,"/");
        strcat(src,files-> d_name);

        // Opening Source File 

        fd=open(src,O_RDONLY);
        if(fd==-1)
        {
            printf("Error\n");
        }
        else
        {
            int f1,k,l;
            while((rbytes=read(fd,buffer,1))>0)
            {
                for(i=0;i<rbytes;i++)
                {
                    if(buffer[i]=='\n')
                    {

                        Lines[x][y++]=buffer[i];
                        lines++;
                        x++;
                        y=0;
                        //printf("x= %d\n", x);
                    }

                    if(lines==n)
                    {
                        count++;
                        strcpy(file_name,files-> d_name);
                        sprintf(number, "%d", count);
                        strcat(file_name,"_");
                        strcat(file_name,number);
                        strcpy(dest,curr_path);
                        strcat(dest,file_name);
                       
                        f1 = creat(dest,S_IRWXU|S_IRGRP|S_IROTH);
                        
                        
                   
                        // Write into file
                        for(k=x-1;k>=x-n && k>=0;k--)
                        {
                            l=strlen(Lines[k]);
                            write(f1,Lines[k],l);
                        }
                        lines=0;
                        
                    }
                    else
                    {
                    
                        if(buffer[i]!='\n')
                            Lines[x][y++]=buffer[i];    
                    }
           

                }
                
            }
            if(lines!=n && lines!=0)
            {
                count++;
                        strcpy(file_name,files-> d_name);
                        sprintf(number, "%d", count);
                        strcat(file_name,"_");
                        strcat(file_name,number);
                        strcpy(dest,curr_path);
                        strcat(dest,file_name);

                       
                        // Creating the file with the desired name
                        f1 = creat(dest,S_IRWXU|S_IRGRP|S_IROTH);
                        
                        
                        int l,k;
                        // Write into file
                        for(k=x-1;k>=x-lines && k>=0;k--)
                        {
                            l=strlen(Lines[k]);
                            write(f1,Lines[k],l);
                        }
            }
            for(i=0;i<x;i++)
                memset(Lines[i],'\0',sizeof(Lines[i]));
            
           close(fd);
        }
        
    }


    return 0;
}

