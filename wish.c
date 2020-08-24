#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include<sys/wait.h>
void process_tokens(char*);
char* search_path[100] = {NULL};
//void add_Path(char*[],int);
int num_paths = 1;
void clear_path();
void tokenize(char*,char**,char*[],int*);
int main(int argc, char*argv[])
{
if(argc > 2)
{
    printf("Ussage ./wish or wish [batch]");
    exit(0);
}
search_path[0] = (char*)malloc(sizeof("/bin")); 
strcpy(search_path[0],"/bin");
while(1)
{
char* line = NULL;
size_t nchars = 0;
int cmd = 0;
//char* input_tokens[1024];
//int num_tokens = 0;
printf("wish>");
cmd = getline(&line,&nchars,stdin);
line[strlen(line)-1] ='\0';
if((int)strlen(line) == 0)
    continue;
if((cmd == -1) || (strcmp(line,"exit") ==0))
{
    for(int i = 0; i < num_paths; i++)
	free(search_path[i]);
    exit(0);
}
else
{
    char* duplicate;
    char* tokens;
    //printf("%s",line);
    //printf("%s",line);
    duplicate = strdup(line);
    if(strncmp(line,"path ",5) == 0)
    {
    int i = 0;
    while((tokens = strsep(&duplicate," "))!= NULL)
    {
	if(i == 0)
	{ 
            i++;
	    continue;
	}
        search_path[i-1] =(char*) malloc(sizeof(tokens));
        strcpy(search_path[i-1],tokens);
        i++;
    }
    num_paths = i-1;
    }
    else
        process_tokens(line);
}
printf("\n");
}
}
/*void add_Path(char*tokens[],int n)
{
int i = 0;
printf("Num is %d\n",n);
for(i = 0;i < n;i++)
{
    search_path[i] = (char*)malloc(sizeof(tokens[i+1]));
    strcpy(search_path[i],tokens[i+1]);
}
num_paths = i;

printf("%d\n",num_paths);
for(int j = 0;j<num_paths;j++)
    printf("%s\n",search_path[j]);*/
//}
void process_tokens(char* input)
{
    char* param_list[100];
    char* executable = NULL;
    char* duplicate = strdup(input);
    int num_params = 0;
    tokenize(duplicate,&executable,param_list,&num_params);
    for(int i = 0;i <num_params;i++)
	printf("Params is %s\n",param_list[i]);
    param_list[num_params] = NULL;
    printf("Executable is %s\n",executable);
    char* file_path = NULL;
    for(int i = 0;i<num_paths;i++)
    {
        file_path = (char*)malloc(strlen(search_path[i]));
        strcpy(file_path,search_path[i]);
	strcat(file_path,"/");
	strcat(file_path,executable);
	printf("%s", file_path);
	if(access(file_path,F_OK) != -1)
	{
            int rc = fork();
            if(rc < 0)
	    {
	        printf("Fork failed");
	        exit(1);
	    }
	    else if(rc == 0)
	    {
                execv(file_path,param_list);
	    }
	    else
	    {
	        int retStatus;
		retStatus = wait(NULL);
		if(rc == 1)
		{
		    printf("Error while executing chile");
		    exit(1);
		}
	        if(rc == 0)
		{
		    printf("Child terminated succesfully");
		}
                for(int i = 0;i<num_params;i++)
		    free(param_list[i]);
		free(executable);
	       
	    }
	}
	else
	    printf("Cant be accessed");
    }
}
void tokenize(char* input, char** exec, char* params[],int *num_params)
{
    int i = 0;
    char* tokens = NULL;
    printf("inputs is %s",input);
    while((tokens = strsep(&input," "))!= NULL)
    {
    if(i == 0)
    {
        *exec =malloc(sizeof(tokens));
	strcpy(*exec,tokens);
    }
    params[i] = (char*)malloc(sizeof(tokens));
    strcpy(params[i],tokens);
    i++;
    }
    *num_params = i;

}
void clear_path()
{
    for(int i = 0;i<num_paths;i++)
    {
	free(search_path[i]);
    }
    num_paths = 0;
}

