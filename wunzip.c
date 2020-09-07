#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(int argc,char* argv[])
{
    if(argc < 2)
    {
	printf("wunzip: file1 [file2 ...]\n");
	exit(1);
    }
    else
    {
	for(int i = 1; i < argc; i++)
	{
	    FILE* fp;
	    fp = fopen(argv[i],"r");
	    if(fp == NULL)
	    {
		printf("Cant open file\n");
		exit(1);
	    }
	    void* line;
	    line = malloc(5);
	    //int nchars = 5;
	    int readd;
	    while((readd =fread(line,1,5,fp))!= 0)
	    {
                int count = *(int*)line;
	        char letter = *(char*)(line+4);
	       // printf("%d",count);
	       // printf("%c",letter);
	       // printf("bytes read %d\n",readd);
		for(int j = 0;j<count;j++)
		    printf("%c",letter);
	    }
            free(line);
	    fclose(fp);
	}
        exit(0);
     }
}
