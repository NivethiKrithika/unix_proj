#include <stdio.h>
#include <malloc.h>
#include<string.h>
#include <stdlib.h>
#include <sys/stat.h>
void display();
int insert_line(char**);
int write_to_file();
void read_file(char*);
void free_list();
int hard_linked(char*, char*);

#define MAX_NUM_ARGUMENTS 3
int main(int argc, char* argv[])
{
    //int ret = 0;
    int stdoutput = 0;
    //printf("No of arguments is %d",argc);
    if((argc > MAX_NUM_ARGUMENTS) || (argc == 0))
    {
        fprintf(stderr,"usage: reverse <input> <output>\n");
	exit(1);
    }
    if(argc == 1)
    {
	char* line = NULL;
	size_t nchars = 0;
	stdoutput = 1;
	while (getline(&line,&nchars,stdin)!= -1)
	{
	    insert_line(&line);
        }
	write_to_file(stdoutput, "NULL");
	free(line);
	exit(0);
    }
    if(argc == MAX_NUM_ARGUMENTS)
    { 
        if((strcmp(argv[1],argv[2]) == 0) || (hard_linked(argv[1],argv[2])))
        {
	    fprintf(stderr,"reverse: input and output file must differ\n");
	    exit(1);
        }
    }
    
    if((argc == MAX_NUM_ARGUMENTS)||(argc == MAX_NUM_ARGUMENTS -1))
    {
	FILE *fp;
	fp = fopen(argv[1],"r");
	if(fp == NULL)
	{
	    fprintf(stderr,"reverse: cannot open file '%s'\n", argv[1]);
	    exit(1);
	}
	else
	{   
            //printf("Entering");
	    char* read_line = NULL;
	    size_t nchars = 0;
	    while(getline(&read_line, &nchars, fp)!= -1)
	    {
		insert_line(&read_line);
	    }
	    //display();
	    if(argc < MAX_NUM_ARGUMENTS)
	    {
		stdoutput = 1;
		//printf("sent out");
	    	write_to_file(stdoutput,"NULL");
	    }
	    else
	        write_to_file(stdoutput,argv[2]);
	    free(read_line);
	    //read_file(argv[2]);
	}
	fclose(fp);
	exit(0);
    }
    
}

void* list_head = NULL;
void* list_tail = NULL;

int hard_linked(char* file1,char* file2)
{
    FILE *fp1,*fp2;
    fp1 = fopen(file1,"r");
    fp2 = fopen(file2,"r");
    if(fp1 == NULL)
    {
        fprintf(stderr,"reverse: cannot open file '%s'\n",file1);
        exit(1);	
    }
    struct stat first,second;
    int stat_first_file = lstat(file1, &first);
    int stat_second_file = lstat(file2, &second);
    if(fp2 != NULL)
    {
        if((stat_first_file != 0) || (stat_second_file != 0))
        {
            fprintf(stderr,"lsat failed\n");
	    fclose(fp1);
	    fclose(fp2);
            exit(1);
        }
	fclose(fp2);
    }
    fclose(fp1);
    if ((first.st_ino == second.st_ino))
        return 1;
    else
        return 0;
}

typedef struct Lines_node Lines_node;
struct Lines_node
{
    char* line;
    Lines_node *next;
    Lines_node *prev;
};


int insert_line(char** input_line)
{
    char* new_string = malloc(strlen(*input_line)+1);
    if(new_string == NULL)
    {
        fprintf(stderr,"malloc failed\n");
	exit(1);
    }
    strcpy(new_string,*input_line);
    Lines_node* lines_node =  (Lines_node*)malloc(sizeof(Lines_node));
    //printf("lines node is %p\n",lines_node);
    if(lines_node == NULL)
    {
	    fprintf(stderr,"malloc failed\n");
	    exit(1);
    }
    lines_node->line = new_string;
    lines_node->next = NULL;

    if(list_head == NULL)
    {
	list_head = list_tail = lines_node;
	lines_node->prev = NULL;
    }
    else
    {
        Lines_node* traverse = (Lines_node*)list_head;
	while(traverse != list_tail)
	{
	    traverse = traverse->next;
	}
	traverse->next = lines_node;
	lines_node->prev = list_tail;
	list_tail = traverse->next;
    }
    return(0);
}

int write_to_file(int stdoutput,char* file)
{
    FILE* output_file;
    if(!stdoutput)
    {
    	output_file = fopen(file,"w");
    	if(output_file == NULL)
    	{
	    fprintf(stderr,"reverse: cannot open file '%s'\n",file);
	    exit(1);
        }
    }
    Lines_node* traverse = (Lines_node*)list_tail;
    while(traverse != NULL)
    {
	 if(stdoutput)
	     fprintf(stdout,"%s",traverse->line);
	 else
	     fputs(traverse->line,output_file);
	 traverse = traverse->prev;
    }
    if(!stdoutput)
        fclose(output_file);
    free_list();
    return(0);
	 
}
void free_list()
{
    Lines_node* traverse = (Lines_node*)list_head;
    while(traverse != NULL)
    {
	Lines_node* temp = traverse;
	traverse = traverse->next;
	//printf("freeing inside while  %p",temp->line);
	//printf("freeing inside while %p",temp);
	free(temp->line);
	free(temp);
    }
    //printf("Done freeing\n");
}
/*void display()
{
    Lines_node* traverse = (Lines_node*)list_head;
    if(traverse!=NULL)
    {
	while(traverse != list_tail)
	{
            //printf("%p\n",traverse->line);
	    printf("%s\n",(traverse->line));
	    traverse = traverse->next;
        }
	//printf("%p\n",traverse->line);
	printf("%s\n",(traverse->line));
    }
}
  
void read_file(char* file)
{
    FILE *ofile;
    ofile = fopen(file,"r");
    char* read;
    size_t nchars1;
    printf("file opened for read");
    while(getline(&read,&nchars1,ofile)!=-1)
    {
	printf("%s",read);
    }
}*/

