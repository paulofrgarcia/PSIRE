#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define CONTINUE 0
#define EXIT 1

int not_white(char e)
{
	switch(e)
	{
	case ' ':
	case '\t':	
	case '\r':
	case '\n':
	case ')':
		return 0;
	default:
		return 1;
	}
		
	
}


//evaluates an s-expression
//starting at index i
char *eval(char *exp, int i)
{
	int j;
	char *result=NULL;
	char tmp[100];
	char e1[100];
	char e2[100];
	int cnt;
	int k=0;

	if(exp[i]!='(')
	{
		printf("Malformed s-expression\n");
		free(exp);
		return NULL;
	}
	
	for(j=i;exp[j]!=')';j++)
	{
		if(j==(int)strlen(exp) && exp[j]!=')')
		{
			printf("Malformed s-expression\n");
			free(exp);
			return NULL;
		}		
	}
		
	//here, we are at the end of current s-expression

	cnt=i+3;
	while(not_white(exp[cnt]))
	{
		e1[k]=exp[cnt];
		cnt++;
		k++;
	}
	e1[k]='\0';
	k=0;
	cnt++;
	
	while(not_white(exp[cnt]))
	{
		e2[k]=exp[cnt];
		cnt++;
		k++;
	}
	e2[k]='\0';
	

	switch(exp[i+1])
	{
	
	case '+': sprintf(tmp,"%d",atoi(e1)+atoi(e2)); break; 
	case '-': sprintf(tmp,"%d",atoi(e1)-atoi(e2)); break; 
	case '*': sprintf(tmp,"%d",atoi(e1)*atoi(e2)); break; 
	case '/': sprintf(tmp,"%d",atoi(e1)/atoi(e2)); break;
	case '%': sprintf(tmp,"%d",atoi(e1)%atoi(e2)); break;  
	}	
	//printf("tmp %s\n",tmp);
	
	result=(char *)malloc(strlen(exp)+1 - (j-i));
	//printf("mal\n");
	if(result==NULL)
	{
		printf("Failed to allocate memory\n");
		return NULL;
	}	

	for(cnt=0;cnt!=i;cnt++)
	{
		//printf("for1\n");
		result[cnt]=exp[cnt];
	}
	//printf("here\n");
	for(k=0;k!=strlen(tmp);k++,cnt++)
	{
		result[cnt]=tmp[k];
		//printf("for2 %c c=%d k=%d\n",result[cnt],cnt,k);
	}	
	//result[cnt]=tmp;
	//cnt++;
	cnt++;
	//printf("here2\n, len=%d\n",strlen(exp)+1 - (j-i));
	for(;cnt<strlen(exp)+1 - (j-i);cnt++)
		result[cnt]=exp[cnt + (j-i)];

	free(exp);
	printf("%s\n",result);
	return result;
}



int repl()
{
	char *line;
    	size_t bufsize = 32;
    	size_t characters;

	printf(">> ");


    	line = (char *)malloc(bufsize * sizeof(char));
    	if( line == NULL)
    	{
        	perror("Unable to allocate buffer");
        	return EXIT;
    	}
	characters = getline(&line,&bufsize,stdin);
	
	line[((int)characters)-1]='\0';
	
	
	if(strcmp(line,"exit")==0)
		return EXIT;

	//"eval" assumes the expression is an s-expression
	line=eval(line,0);

	if(line!=NULL)
	{
		//printf("%s\n",line);
		free(line);
	}
	return CONTINUE;
}



int main()
{
	//Initializations

	printf("PSIRE Lambda Interpreter loaded\n");

	//REPL
	while(repl()!=EXIT);

	printf("PSIRE Lambda Interpreter exiting\n");

	return 0;
}