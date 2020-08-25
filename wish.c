#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include<sys/wait.h>
#include<limits.h>
#include<fcntl.h>
void process_tokens(char*);
char* search_path[100] = {NULL};
int check_for_redirection(char**);
char* extract_output_file(char**,int);
//void add_Path(char*[],int);
int num_paths = 1;
void clear_path();
void remove_trailing_space(char**);
void tokenize(char*,char**,char*[],int*,int,char*);
void remove_leading_space(char**);
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
remove_trailing_space(&line);
remove_leading_space(&line);

printf("Input is %s of length %d\n",line,(int)strlen(line));
//line[strlen(line)-1] ='\0';
if((int)strlen(line) == 0)
    continue;
if((cmd == -1) || (strcmp(line,"exit") ==0))
{
    clear_path();
    exit(0);
}
else
{
    char* executable;
    //char* tokens;
    //printf("%s",line);
    //printf("%s",line);
    char* duplicate;
    duplicate = strdup(line);

    if(strncmp(line,"path ",5) == 0)
    {  
  //      printf("dup is %s",duplicate);
        tokenize(duplicate,&executable,search_path,&num_paths,1," ");
//	printf("exec is %s\n",executable);
    }
    /*if(strcmp(executable,"path") == 0)
    {
	printf("Num_path %d",num_paths);
    }*/
    else if(strncmp(line ,"cd ",3) == 0)
    {
	char* param_list[20];
	int num_params = 0;
	char cur_path[PATH_MAX];
        tokenize(duplicate,&executable,param_list,&num_params,1," ");
        if(num_params != 1)
	    printf("Incorrect number of params");
	else
	{
	   printf("Execu is %s",executable);
	   printf("num is %d",num_params);
	   printf("Param for cd is %s",param_list[0]);
	   char* cwd;
	    //char* path = NULL;
	    //size_t nchars = 0;
	    //path = (char*)malloc(sizeof(get_current_dir_name())+1);
	    cwd = getcwd(cur_path,PATH_MAX);
	    //strcpy(path,get_current_dir_name());
	    strcat(cwd,"/");
	    strcat(cwd,param_list[0]);
	    printf("Param for cd is %s",cwd);
	    if(chdir(cwd) == -1)
	    {
		printf("Cant change to directory\n");
	    }
	}
    }
    else
        process_tokens(line);
}
free(line);
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
void remove_trailing_space(char** input)
{
char buff[(int)strlen(*input)+1];
strcpy(buff,*input);
int j = 0;
int loc = -1;
while(buff[j] != '\0')
{
    if(buff[j] != ' ' && buff[j] != '\t' && buff[j] != '\n')
	loc = j;
    j++;
}
buff[loc+1] = '\0';
strcpy(*input,buff);
printf("after trial,input is %s of length %d",*input,(int)strlen(*input));
}
void remove_leading_space(char** input)
{
    char buff[(int)strlen(*input)+1];
    int loc = 0,i,j;
    strcpy(buff,*input);
    printf("input is %s of size %d",*input,(int)strlen(*input));
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
    printf("buffer is %s of lenght %d",buff,(int)strlen(buff));
    strcpy(*input,buff);
}
void process_tokens(char* input)
{
    char* param_list[100];
    char* executable = NULL;
    int is_redirected = check_for_redirection(&input);
    char* output_file;
    if(is_redirected)
    {
	printf("The index is %d\n",is_redirected);
	output_file = extract_output_file(&input,is_redirected);
	if(strlen(output_file) <= 0)
	{
	    printf("Invalid file");
	    exit(1);
	}
	
    }
    char* duplicate = strdup(input);
    int num_params = 0;
    tokenize(duplicate,&executable,param_list,&num_params,0," ");
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
	printf("File path is %s\n", file_path);
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
		if(is_redirected)
		{
		    close(STDOUT_FILENO);
		    open(output_file,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
		}
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
		else
		{
		    printf("Chilid process terminated succesfully");
		    printf("Ret status is %d\n",retStatus);
		}
                //for(int i = 0;i<num_params;i++)
		  //  free(param_list[i]);
		//free(executable);	       
	    }
	    free(file_path);
	    break;
	}
	else
	{
	    free(file_path);
	    printf("Cant be accessed");
	}
    }
    for(int i = 0;i<num_params;i++)
	free(param_list[i]);
    free(executable);
}
int check_for_redirection(char** input)
{
    char* pos;
    int index;
    pos = strchr(*input,'>');
    if(pos != NULL)
    {
	index = (int)(pos-*input);
	return index;
    }
    else
	return 0;
}
char* extract_output_file(char** input, int is_redirected)
{
    char *executable = NULL;
    char* param_list[20];
    int num_params = 0;
    tokenize(*input,&executable,param_list,&num_params,0,">");
    if(num_params > 2)
    {
	printf("Error in redirection");
	exit(1);
    }
    strcpy(*input,param_list[0]);
    char* out_file = (char*)malloc(strlen(param_list[1]+1));
    strcpy(out_file,param_list[1]);
    //int out_file_len = strlen(*input) - is_redirected;
    //char* out_file = (char*)malloc(out_file_len);
    //char* input_sliced = (char*)malloc(is_redirected+1);
    //strncpy(out_file,*input+is_redirected+1,out_file_len);
     
    //strncpy(input_sliced,*input,is_redirected);
    remove_trailing_space(input);
    //remove_trailing_space(&out_file);
    remove_leading_space(&out_file);
    printf("in file size %s of len %d \n",*input,(int)strlen(*input));
    printf("out file size %s of len %d\n",out_file,(int)strlen(out_file));
    free(executable);
    for(int i = 0; i< num_params; i++)
	free(param_list[i]);
    //free(*input);
    //*input = input_sliced;
    return out_file;
    //printf("position arg is %d file is %s\n",pos,*input);
    //return NULL;
}
void tokenize(char* input, char** exec, char* params[],int *num_params,int isPath,char* sep)
{
    int isFirstUpdate = 1;
    int i = 0;
    char* tokens = NULL;
    printf("inputs is %s",input);
    while((tokens = strsep(&input,sep))!= NULL)
    {
    if(isFirstUpdate)
    {
        *exec =malloc(sizeof(tokens));
	strcpy(*exec,tokens);
    }
    if((isPath) && (isFirstUpdate))
    {
	isFirstUpdate = 0;
	continue;
    }
    isFirstUpdate = 0;
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

