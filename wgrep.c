#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc, char*argv[])
{

    if(argc < 2)
    {
	printf("wgrep: searchterm [file]");
	exit(1);
    }
    for(int i = 1;i < argc-1;i++)
    {
	FILE* file_to_search;
	int printAll = 0;
	if(argc == 2)
	    file_to_search = fopen("stdin", "r");
	else
	    file_to_search = fopen(argv[i+1],"r");
	if(file_to_search == NULL)
	{
	    printf("wgrep:Ccannot open file\n");
	    exit(1);
	}
	size_t nchar = 0;
	char* line = "NULL";
	if(argv[1] == NULL)
	    printAll = 1;
	    
	while(getline(&line,&nchar,file_to_search)!=-1)
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
}

