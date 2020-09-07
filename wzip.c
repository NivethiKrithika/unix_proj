#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void compress_and_write(char,unsigned int);
void write_to_file(char**);
//unsigned int check_for_continuation(char*, unsigned int);
void concat(char*);
int main(int argc, char* argv[])
{
    if(argc < 2)
    {
	printf("wzip: file1 [file2 ...]\n");
	exit(1);
    }
    //ff = fopen("merged.txt","w");
    int first_run = 1;
    unsigned int count = 0;
    char in_char = '\0';
    for(int i = 1;i < argc;i++)
    {
        FILE *fp;
        fp = fopen(argv[i],"r");
        if(fp == NULL)
        {
	    printf("File cannot be opened\n");
	    exit(1);
        }
//	unsigned int count = 0;
//	char in_char = '\0';
	//char temp = '\0';
	char c = '\0';
	//int first_run = 1;
	while((c = fgetc(fp))!=EOF)
	{
	  //  printf("incoming char is %c,inchar is %c", c,in_char);
	    //printf("%d",strcmp(&in_char,&c));

	    if((in_char == c) || first_run)
	    {
		//printf("first iter\n");
		in_char = c;
	        count += 1;
		first_run = 0;
	//	printf("first iter %c, %d\n",in_char,count);
		continue;
	    }
	    else
	    {
	//	printf("Hitting else!!!\n");
                compress_and_write(in_char,count);
		count = 1;
		in_char = c;
	    }
	    //printf("%c",in_char);
	    //printf("%d",count);
	}
    }
	//printf("Char is %c ,count is %d\n",in_char,count);
	compress_and_write(in_char,count);
    }

/*        else
        {
	    char* line = NULL;
	    size_t nchars = 0;
	    while(getline(&line,&nchars,fp) != -1)
	    {
	        if(strcmp(line,'') == 0)
		{
	            if(fgetc(fp+1)
		    line = realloc(line,1)
		    continue;
		}
		compress_and_write
	       // compress_and_write(line);
	       //printf("%s\n",line);
	       //printf("length is %d\n",(int)strlen(line));
	       //if(strlen(line) == 1)
	         //  printf("one char is %s",line);
	       //printf("%ld",nchars);
	    }
	    free(line);
        }
        fclose(fp);
    }
}*/

/*struct comp_unit
{
    unsigned int count;
    char letter;
}*/
//FILE *f1;
//f1 = fopen("merged.txt","w")
void compress_and_write(char input, unsigned int count )
{
    //unsigned int i;
    unsigned int *cont_index;
   // printf("Hitting here %c,%d",input,count);
    //printf("input is %s",input);
    cont_index = (unsigned int *)malloc(sizeof(unsigned int));
    char* letter;
    letter = (char*)malloc(1);
    strncpy(letter,&input,1);
	//fprintf(stdout,"letter is %c input[i] is %c",*letter,input[i]);
    *cont_index = count;
	//fprintf(stdout,"cont_index %d\n",*cont_index);
	//fprintf(stdout,"%d%c\n",*cont_index,*letter);
    fwrite(cont_index,sizeof(*cont_index),1,stdout);
    fwrite(letter,1,1,stdout);
    free(letter);
    free(cont_index);
}
/*unsigned int check_for_continuation(char* input,unsigned int index)
{
 //   char* string_to_check;
    unsigned int out_index = 1;
    
    char comparision_char = input[index];
    //printf("input index %c , comp char%c\n",input[index],comparision_char);
    for(int i = index+1; i < strlen(input); i++)
    {
        if(input[i] == comparision_char)
	    out_index++;
	else
	    break;
    }
  //  printf("out index %d\n",out_index);
    return out_index;
}
*/
