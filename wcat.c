#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc, char* argv[])
{
    if(argc < 2)
	printf("Invalid number of arguemnts. Please enter a valid file name");
    else
    {
	for(int i = 1; i <=argc-1;i++)
	{
	FILE *file_to_read;
	char* line = NULL;
	line =  (char*)malloc(1024);
	memset(line,'\0',sizeof(line));
	int nchar = 1024;
	file_to_read =fopen(argv[i],"r");
	if(file_to_read == NULL)
	{
	    fprintf(stderr,"wcat:cannot open file\n.");
	    exit(1);
	}
        while(fgets(line,nchar,file_to_read)!=NULL)
	{
	    printf("%s",line);
	}
	printf("Done printing\n");
	free(line);
	fclose(file_to_read);
	}
	exit(0);
    }
}
