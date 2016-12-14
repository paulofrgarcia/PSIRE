#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define CONTINUE 0
#define EXIT 1

#define ID 0
#define LPAR 1
#define RPAR 2
#define LAMBDA 3
#define DOT 4

//AST types
#define ANONYMOUS 0
#define APPLICATION 1
#define IDENT 2


//Scanner/Lexer
char *lex_line;
int lex_cnt=0;

int is_whitespace(char c)
{
	switch(c)
	{
	case ' ':
	case '\t':	
	case '\r':
	case '\n':
		return 1;
	default:
		return 0;
	}	
}

int is_char(char c)
{
	if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c=='_'))
		return 1;
	else
		return 0;
}

//true if the next token matches "t"
//does not consume token
int l_next(int t)
{
	while(is_whitespace(lex_line[lex_cnt]))
		lex_cnt++;
	//printf("next token %c\n",lex_line[lex_cnt]);
	switch(t)
	{
	case LPAR: if(lex_line[lex_cnt]=='(') return 1; else return 0;
	case RPAR: if(lex_line[lex_cnt]==')') return 1; else return 0;
	case LAMBDA: if(lex_line[lex_cnt]=='\\') return 1; else return 0;
	case DOT: if(lex_line[lex_cnt]=='.') return 1; else return 0;
	default: if(is_char(lex_line[lex_cnt])) return 1; else return 0;
	}	
}
//true if the next token matches "t"
//consumes token if it matches
int l_skip(int t)
{
	while(is_whitespace(lex_line[lex_cnt]))
		lex_cnt++;
	//printf("skip token %c\n",lex_line[lex_cnt]);
	switch(t)
	{
	case LPAR: if(lex_line[lex_cnt]=='(') {lex_cnt++; return 1;} else return 0;
	case RPAR: if(lex_line[lex_cnt]==')') {lex_cnt++; return 1;} else return 0;
	case LAMBDA: if(lex_line[lex_cnt]=='\\') {lex_cnt++; return 1;} else return 0;
	case DOT: if(lex_line[lex_cnt]=='.') {lex_cnt++; return 1;} else return 0;
	default: while(is_char(lex_line[lex_cnt])){lex_cnt++;} return 1;
	}	
}

//Skip the token
void l_match()
{
	while(is_whitespace(lex_line[lex_cnt]))
		lex_cnt++;
	//printf("match token %c\n",lex_line[lex_cnt]);
	switch(lex_line[lex_cnt])
	{
	case '(': lex_cnt++; return;
	case ')': lex_cnt++; return;
	case '\\': lex_cnt++; return;
	case '.': lex_cnt++; return;
	default: while(is_char(lex_line[lex_cnt])){lex_cnt++;} return;
	}	
}

//skip and return the token
char *l_token()
{
	char buf[256];
	char *ret;
	int i=0;
	while(is_whitespace(lex_line[lex_cnt]))
		lex_cnt++;
	//printf("token token %c\n",lex_line[lex_cnt]);
	while(is_char(lex_line[lex_cnt]))
	{
		buf[i]=lex_line[lex_cnt];
		lex_cnt++;
		i++;
		if(i==256)
		{
			printf("Error: Identifier name too long.\n");
			return NULL;
		}
	}
	buf[i]='\0';
	ret=(char *)malloc(strlen(buf)+1);
	strcpy(ret,buf);
	return ret;
}

//Creates an AST from the input


struct AST_node
{
	int type;
	void *right;
	void *left;
};
typedef struct AST_node AST_node;

AST_node *term();
AST_node *atom();
AST_node *application();

AST_node *term()
{
	AST_node *node;
	
	node=(AST_node *)malloc(sizeof(AST_node));

	//printf("term\n");
	if (l_skip(LAMBDA)) 
	{
		//printf("LAMBDA ");
		node->type=ANONYMOUS;
		node->left=l_token();
		//printf("token ");
		l_match(DOT);
		//printf("DOT ");
		node->right=term();
		//printf("EDN term\n");
		return node;  	
	}  
	else 
	{
    		return application();
  	}
}



AST_node *application() 
{
	AST_node *nodel, *noder, *tmp;

	//printf("\napplication\n");

	nodel=atom();

    
  	while (1) 
	{
		noder=atom();
    		if (!noder) 
		{
			//printf("empty application\n");
      			return nodel;
    		} 
		else 
		{
			//printf("not empty aplication\n");
			tmp=(AST_node *)malloc(sizeof(AST_node));
			tmp->type=APPLICATION;
			tmp->right=noder;
			tmp->left=nodel;
			nodel=tmp;
    		}
  	}
}

AST_node *atom() 
{
	AST_node *my_term;

	//printf("atom\n");

  	if (l_skip(LPAR)) 
	{
		//printf(" ( ");
    		my_term=term();
    		l_match(RPAR);
    		return my_term;
  	} 
	else 
	{
		if (l_next(ID)) 
		{
			//printf("ID ");
			my_term=(AST_node *)malloc(sizeof(AST_node));
			my_term->type=IDENT;
			my_term->left=l_token();
			my_term->right=NULL;
			return my_term;
  		} 
		else 
		{
			//printf("null\n");
    			return NULL;
  		}
	}
}


void print_AST(AST_node *n);


/*
Grammar:

term: application | LAMBDA ID DOT term

application: application atom | atom

atom: ID | LPAR term RPAR

*/
void parse(char *line) 
{
	lex_line=line;
	lex_cnt=0;

	AST_node *AST_head;
	printf("Parsing... %s\n", line);
	AST_head=term();
	print_AST(AST_head);
	printf("\n");
}




void print_AST(AST_node *n)
{
	if(n==NULL)
	{
		//printf("null\n");
		return;
	}
	if(n->type==IDENT)
	{
		//printf("ident\n");
		printf("%s",(char *)(n->left));
	}
	if(n->type==ANONYMOUS)
	{
		//printf("anon\n");
		printf("\\%s . ",(char *)(n->left));
		print_AST(n->right);
	}
	if(n->type==APPLICATION)
	{
		//printf("app\n");
		print_AST(n->left);
		print_AST(n->right);
	}
}











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
	//native s-expressions
	case '+': sprintf(tmp,"%d",atoi(e1)+atoi(e2)); break; 
	case '-': sprintf(tmp,"%d",atoi(e1)-atoi(e2)); break; 
	case '*': sprintf(tmp,"%d",atoi(e1)*atoi(e2)); break; 
	case '/': sprintf(tmp,"%d",atoi(e1)/atoi(e2)); break;
	case '%': sprintf(tmp,"%d",atoi(e1)%atoi(e2)); break;  
	case '\\': break; //anonymous function
	case '|': break; //function application
	default: break; //variable reference	
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


	parse(line);
	return CONTINUE;
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
