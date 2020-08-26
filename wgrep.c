#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc, char*argv[])
{

    if(argc < 2)
    {
	printf("wgrep:searchterm [file...]\n");
	exit(1);
    }
    int printAll = 0;
    if(argv[1] == NULL)
	printAll = 1;
    if(argc == 2)
    {
	size_t nchar_stdin = 0;
	char* stdin_line = "NULL";
	while(getline(&stdin_line,&nchar_stdin,stdin) != -1)
	{
	    if((strstr(stdin_line,argv[1]) != NULL) || printAll)
		printf("%s",stdin_line);
	}	
	free(stdin_line);
	exit(0);
    }
    for(int i = 1;i < argc-1;i++)
    {
	FILE* file_to_search;
	
	file_to_search = fopen(argv[i+1],"r");
	if(file_to_search == NULL)
	{
	    printf("wgrep: cannot open file\n");
	    exit(1);
	}
	size_t nchar = 0;
	char* line = "NULL";
	    
	while(getline(&line,&nchar,file_to_search) != -1)
	{
	    //printf("%s",line);
	    if((strstr(line,argv[1])!=NULL) || printAll)
	    {
		printf("%s",line);
	    }
	}
	free(line);
	fclose(file_to_search);
    }
    exit(0);
}

