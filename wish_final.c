#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include<sys/wait.h>
#include<limits.h>
#include<fcntl.h>
int process_tokens(char*);
char* search_path[100] = {NULL};
int check_for_spcl_cmnd(char**,char);
char* extract_output_file(char**,int);
//void add_Path(char*[],int);
int num_paths = 1;
void clear_path();
void remove_trailing_space(char**);
void tokenize(char*,char**,char*[],int*,int,char*);
void remove_leading_space(char**);
void extract_commands(char**,char*[],int*);
int fork_execute(char*, char*[],int,char*);

int main(int argc, char*argv[])
{
if(argc > 2)
{
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
}
//search_path[0] = (char*)malloc(sizeof("/bin")); 
//strcpy(search_path[0],"/bin");
FILE* fp;
if(argc == 2)
{
fp = fopen(argv[1],"r");
if(fp == NULL)
{
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
    /*printf("Error while opening the file\n");
    exit(1);*/
}
}
search_path[0] = (char*)calloc(sizeof("/bin"),1);
strcpy(search_path[0],"/bin");
while(1)
{
//int ret = 0;
char* line = NULL;
size_t nchars = 0;
int cmd = 0;
//char* input_tokens[1024];
//int num_tokens = 0;
if(argc == 1)
{
    printf("wish> ");
    cmd = getline(&line,&nchars,stdin);
}
else
{
if((cmd = getline(&line,&nchars,fp)) == -1)
{
    fclose(fp);
    clear_path();
    exit(0);
}
}

//char* pos;
//pos = strchr(line,'s');
//printf("%ld\n",pos-line);
//printf("len is %d",(int)strlen(line));
remove_trailing_space(&line);
remove_leading_space(&line);
//char *pos;
//pos = strchr(line,'\0');
//printf("%ld\n",pos-line);
//printf("len is %d",(int)strlen(line));
//printf("lCrossed");
//printf("Input is %s of length %d\n",line,(int)strlen(line));
//line[strlen(line)-1] ='\0';
if((int)strlen(line) == 0)
    continue;
if((cmd == -1) || (strcmp(line,"exit") ==0))
{
    clear_path();
    free(line);
    exit(0);
}
else
{
    char* executable;
    //char* tokens;
    //printf("%s",line);
    //printf("%s",line);
    //char* duplicate;
    //duplicate = strdup(line);

    if(strncmp(line,"path",4) == 0)
    {  
  //      printf("dup is %s",duplicate);
        char* duplicate;
	duplicate = strdup(line);
	free(search_path[0]);
        tokenize(duplicate,&executable,search_path,&num_paths,1," ");
	free(executable);
        free(duplicate);
//	printf("exec is %s\n",executable);
    }
    /*if(strcmp(executable,"path") == 0)
    {
	printf("Num_path %d",num_paths);
    }*/
    else if(strncmp(line ,"cd ",3) == 0)
    {
	char* duplicate;
	duplicate = strdup(line);
	char* param_list[20];
	int num_params = 0;
	//char cur_path[PATH_MAX];
        tokenize(duplicate,&executable,param_list,&num_params,1," ");
        if(num_params != 1)
        {
           char error_message[30] = "An error has occurred\n";
           write(STDERR_FILENO, error_message, strlen(error_message));
	    //printf("Incorrect number of params");
	}
	else
	{
	   //printf("Execu is %s",executable);
	   //printf("num is %d",num_params);
	   //printf("Param for cd is %s",param_list[0]);
	    char cwd[PATH_MAX];
	    //char* path = NULL;
	    //size_t nchars = 0;
	    //path = (char*)malloc(sizeof(get_current_dir_name())+1);
	    getcwd(cwd,PATH_MAX);
	    //strcpy(path,get_current_dir_name());
	    strcat(cwd,"/");
	    strcat(cwd,param_list[0]);
	    //printf("Param for cd is %s",cwd);
	    if(chdir(cwd) == -1)
	    {
		char error_message[30] = "An error has occurred\n";
		write(STDERR_FILENO, error_message, strlen(error_message));
		printf("cd command failed");
        
	    }
	    //
	}
	free(duplicate);
	free(executable);
	for(int i = 0; i < num_params;i++)
	    free(param_list[i]);
    }
    else
        process_tokens(line);
}
free(line);
//printf("\n");
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
//

void remove_trailing_space(char** input)
{
char buff[(int)strlen(*input)+1];
strncpy(buff,*input,strlen(*input)+1);
int j = 0;
int loc = -1;
while (buff[j] != '\0')
{
    if(buff[j] != ' ' && buff[j] != '\t' && buff[j] != '\n')
	loc = j;
    j++;
}
buff[loc+1] = '\0';
*input = realloc(*input,strlen(buff)+1);
strncpy(*input,buff,strlen(buff)+1);
//printf("after trial,input is %s of length %d",*input,(int)strlen(*input));
}


void remove_leading_space(char** input)
{
    char buff[(int)strlen(*input)+1];
    int loc = 0,i;
    strncpy(buff,*input,strlen(*input)+1);
    //printf("input is %s of size %d",*input,(int)strlen(*input));
    while(buff[loc] == ' '||buff[loc] == '\t' || buff[loc] == '\n')
        loc++;
    if(loc != 0)
    {
        i = 0;
        while(buff[i+loc] !='\0')
        {
            buff[i] = buff[i+loc];
            i++;
        }
        buff[i] = '\0';
    }
   // printf("buffer is %s of lenght %d",buff,(int)strlen(buff));
    *input = realloc(*input,strlen(buff)+1);
    strcpy(*input,buff);
}



int process_tokens(char* input)
{
    //char* param_list[100];
    //char* executable = NULL;
    /*int is_redirected = check_for_spcl_cmnd(&input,'>');
    char* output_file;
    if(is_redirected)
    {
	//printf("The index is %d\n",is_redirected);
	char* duplicate_redirection;
	duplicate_redirection = strdup(input);
	output_file = extract_output_file(&duplicate_redirection,is_redirected);
	if(strlen(output_file) <= 0)
	{
	    printf("Invalid file");
	    exit(1);
	}
	
    }*/
    int ret = 0;
    int is_parallelization = check_for_spcl_cmnd(&input,'&');
    char* arg_list[100];
    int num_of_args = 1;
    if(is_parallelization)
    {
	//printf("Hitting here\n");
	//int num_of_args = 0;
	char* duplicate_parallel;
	duplicate_parallel = strdup(input);
	num_of_args = 0;
    	extract_commands(&duplicate_parallel,arg_list,&num_of_args);
	//num_of_args = num_of_args -2;
	//printf("nu of arg is %d\n",num_of_args);
	//for(int i =0;i< num_of_args;i++)
	  //  printf("The string is %s",arg_list[i]);
	free(duplicate_parallel);
    }
    //char* duplicate;
    char* duplicates[100];
    //int num_params = 0;
    //if(is_parallelization)
   // {
	char* param_list_parallel[100][100];
	char* executables[100];
	char* output_file[100];
	int is_redirected[100] = {0};
	//for(int i=0;i<100;i++)
	  //  *param_list_parallel[i] = malloc(100*sizeof(char*));
	int num_params_parallel[100];
	int j;
	if(!is_parallelization)
            arg_list[0] = strdup(input);
	//printf("No of args is %d",num_of_args);
        for(int i = 0;i < num_of_args;i++)
	{
	    //duplicates[i] = strdup(arg_list[i]);
	    is_redirected[i] = check_for_spcl_cmnd(&arg_list[i],'>');
	    if(is_redirected[i])
	    {
		char* duplicate_redirection;
		duplicate_redirection = strdup(arg_list[i]);
		output_file[i] = extract_output_file(&duplicate_redirection,is_redirected[i]);
	//	free(duplicate_redirection);
		if(output_file[i] == NULL) 
		{
		    //if(!is_parallelization)
		    //{
		        free(duplicate_redirection);
	              //  free(arg_list[0]);
		     if(!is_parallelization)
		     {
		        free(arg_list[0]);
		        //return(1);
		     }	  
		     return(1);   
		}
		//printf("arg list is %s",arg_list[i]);
		strcpy(arg_list[i],duplicate_redirection);
	        
	    }
	    duplicates[i] =strdup(arg_list[i]);
	    tokenize(duplicates[i],&executables[i],param_list_parallel[i],&num_params_parallel[i],0," ");
	    //for(j = 0;j<num_params_parallel[i];j++)
	    //{
		//printf("final params are %s\n",param_list_parallel[i][j]);
		//printf("Executable is %s\n",executables[i]);
	    //}
	    j = num_params_parallel[i];
	    param_list_parallel[i][j] = NULL;
	    free(duplicates[i]);
	    ret = fork_execute(executables[i],param_list_parallel[i],is_redirected[i],output_file[i]);
	}
	int num_processes = num_of_args;
	//int childPid;
	int retStatus;
	if(ret == 0)
	{
	while(num_processes)
	{
	    waitpid(WAIT_ANY,&retStatus,0);
	    //printf("Child %d\n",retStatus);
	    num_processes--;
	}
	}
	for(int j = 0; j <num_of_args;j++)
	{
	    free(executables[j]);
            free(arg_list[j]);
	    //free(duplicates[j]);
	    if(is_redirected[j])
	    {
	        free(output_file[j]);
	    }
	    for(int k = 0;k < num_params_parallel[j];k++)
	        free(param_list_parallel[j][k]);
	    //for(int i = 0; i < 100; i++)
	      //  free(param_list_parallel[i]);
	    
	}
        return(0);	
	//int retStatus;
	//retStatus = wait(NULL);
	//if(retStatus == 1)
	  //  printf("Error while executing child");
	//else
	  //  printf("Child executed succesfully %d", retStatus);
    
}       //printf("Hitting!!!");
  
   /* else
    {
    duplicate = strdup(input);
    tokenize(duplicate,&executable,param_list,&num_params,0," ");
    for(int i = 0;i <num_params;i++)
	printf("Params is %s\n",param_list[i]);
    param_list[num_params] = NULL;*/
    //printf("Executable is %s\n",executable);
 int fork_execute(char* executable, char* param_list[],int is_redirected,char* output_file)
{
    if(num_paths == 0)
    {
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO,error_message,strlen(error_message));
	return(1);
    }
    //char* file_path = NULL;
    for(int i = 0;i<num_paths;i++)
    {
	char file_path[PATH_MAX];
        //file_path = (char*)malloc(strlen(search_path[i])+1);
        strcpy(file_path,search_path[i]);
	strcat(file_path,"/");
	strcat(file_path,executable);
	//printf("File path is %s\n", file_path);
	if(access(file_path,F_OK) != -1)
	{
	    int rc = fork();
	    //printf("pid is %d\n",rc);   
            if(rc < 0)
	    {
	        char error_message[30] = "An error has occurred\n";
		write(STDERR_FILENO, error_message, strlen(error_message));
		return(1);
	    }
	    else if(rc == 0)
	    {
		if(is_redirected)
		{
		    close(STDOUT_FILENO);
		    open(output_file,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
		}
                execv(file_path,param_list);
	    }
	
	    else
	    {
	  //  free(file_path);
	    break;
	}
	}
	else
	{
	    if(i == num_paths-1)
            {
	    char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
	    return(1);
	    }
	    else
		continue;
            //free(file_path);
	}
    }
    return 0;
  /*  for(int i = 0;i<num_params;i++)
	free(param_list[i]);
    free(executable);*/
}

void extract_commands(char** input,char* param_list[],int* num_params)
{
    char* executable = NULL;
    //char* param_list[100];
    //int num_params;
    int num_args = 0;
    tokenize(*input,&executable,param_list,num_params,0,"&");
    num_args = *num_params;
    for(int i = 0;i<*num_params;i++)
    {
	remove_trailing_space(&param_list[i]);
	remove_leading_space(&param_list[i]);
	    if((int)strlen(param_list[i]) == 0)
	    {
		free(param_list[i]);
		num_args-=1;

	     }
	}
    *num_params = num_args;
	//printf("strlen is %d",(int)strlen(param_list[i]));
	  // for(int j = 0;j<strlen(param_list[i]);j++)
	   //{
	//	   printf("%d",atoi(&param_list[i][j]));
	  // }
	    //printf("Yes null");
	   // num_params -= 1;
	//}
	//printf("params are %s of length %d \n",param_list[i],(int)strlen(param_list[i]));
    free(executable);    

}

int check_for_spcl_cmnd(char** input,char spcl_char)
{
    char* pos;
    //int index;
    pos = strchr(*input,spcl_char);
    if(pos != NULL)
    {
//	index = (int)(pos-*input);
//	free(pos);
	return 1;
    }
    else
	return 0;

}

char* extract_output_file(char** input, int is_redirected)
{
    char *executable = NULL;
    char* param_list[20];
    int num_params = 0;
    if(strncmp(*input,">",1) == 0)
    {
        char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO,error_message,strlen(error_message));
	return NULL;
    }
    tokenize(*input,&executable,param_list,&num_params,0,">");
    if(num_params > 2)
    {
	char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
        return NULL;
    }
    memset(*input,'\0',strlen(*input)); 
    strncpy(*input,param_list[0],strlen(param_list[0]));
    //char* out_file = (char*)calloc(strlen(param_list[1]+1),1);
    //char* out_file;
    //out_file = strdup(param_list[1]);
    //strcpy(out_file,param_list[1]);
    char* out_file;
    out_file = strdup(param_list[1]);
    remove_trailing_space(input);
    remove_leading_space(&out_file);
    //printf("in file size %s of len %d \n",*input,(int)strlen(*input));
    //printf("out file size %s of len %d\n",out_file,(int)strlen(out_file));
    free(executable);
    for(int i = 0; i< num_params; i++)
	free(param_list[i]);
    int num_out_files = 0;
    char* tokens;
    char* check_for_multi_files = strdup(out_file);
    while((tokens = strsep(&check_for_multi_files," "))!=NULL)
	num_out_files++;
    free(check_for_multi_files);
    if(num_out_files > 1)
    {
        char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
	return NULL;
    }
    if(strlen(out_file) <= 0)
    {
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
	//printf("Invalid file");
	return NULL;
    }
    //free(check_for_multi_files);
    return out_file;
}


void tokenize(char* input, char** exec, char* params[],int *num_params,int isPath,char* sep)
{
    int isFirstUpdate = 1;
    int i = 0;
    char* tokens = NULL;
    //printf("inputs is %s",input);
    while((tokens = strsep(&input,sep))!=NULL)
    {	
    if(isFirstUpdate)
    {
	*exec = strdup(tokens);
        //*exec =calloc(sizeof(tokens)+1,1);
	//strcpy(*exec,tokens);
    }
    if((isPath) && (isFirstUpdate))
    {
	isFirstUpdate = 0;
	continue;
    }
    isFirstUpdate = 0;
    params[i] = strdup(tokens);
    //params[i] = (char*)calloc(sizeof(tokens)+1,1);
    //strncpy(params[i],tokens,strlen(tokens));
    i++;
    }
    *num_params = i;
   // free(tokens);

}


void clear_path()
{
    for(int i = 0;i<num_paths;i++)
    {
	free(search_path[i]);
    }
    num_paths = 0;//Add your code here
}
