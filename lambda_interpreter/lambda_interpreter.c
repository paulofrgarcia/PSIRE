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
char *eval(char *exp)
{
	int j;
	char *result=NULL;
	char *rec_exp;
	char tmp[100];
	char e1[100];
	char e2[100];
	int cnt;
	int k=0;

	printf("exp %s\n",exp);

	if(exp[0]!='(')
	{
		printf("Malformed s-expression\n");
		//free(exp);
		return NULL;
	}
	
	for(j=1;exp[j]!=')';j++)
	{
		if(j==(int)strlen(exp) && exp[j]!=')')
		{
			printf("Malformed s-expression\n");
			//free(exp);
			return NULL;
		}		
		//recursion to parse inner expressions
		if(exp[j]=='(')
		{
			rec_exp=eval(exp+j);
			exp[j]='\0';
			strcat(exp,rec_exp);
			free(rec_exp);
			printf("new exp %s\n",exp);
		}
	}
		
	//here, we are at the end of current s-expression

	cnt=3;
	while(not_white(exp[cnt]))
	{
		e1[k]=exp[cnt];
		cnt++;
		k++;
	}
	e1[k]='\0';
	//printf("e1 %s\n",e1);
	k=0;
	cnt++;
	
	while(not_white(exp[cnt]))
	{
		e2[k]=exp[cnt];
		cnt++;
		k++;
	}
	e2[k]='\0';
	//printf("e2 %s\n",e2);

	switch(exp[1])
	{
	
	case '+': sprintf(tmp,"%d",atoi(e1)+atoi(e2)); break; 
	case '-': sprintf(tmp,"%d",atoi(e1)-atoi(e2)); break; 
	case '*': sprintf(tmp,"%d",atoi(e1)*atoi(e2)); break; 
	case '/': sprintf(tmp,"%d",atoi(e1)/atoi(e2)); break;
	case '%': sprintf(tmp,"%d",atoi(e1)%atoi(e2)); break;  
	}	
	//printf("tmp %s\n",tmp);
	
	result=(char *)malloc(strlen(exp)+1 -j + strlen(tmp)+1);
	for(k=0;k!=strlen(tmp);k++)
	{
		result[k]=tmp[k];
	}
	//printf("result 1 %s\n",result);
	exp+=j+1;
	strcat(result,exp);
	//printf("result %s\n",result);
	return result;
}



int repl()
{
	char *line;
	char *result;
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
	result=eval(line);
	printf("result %s\n",result);

	if(line!=NULL)
	{
		//printf("%s\n",line);
		free(line);
	}
	if(result!=NULL)
	{
		//printf("%s\n",line);
		free(result);
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
