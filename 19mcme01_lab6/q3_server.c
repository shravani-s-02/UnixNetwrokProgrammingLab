#include "unp.h"
#include <math.h>

struct stack{
	float num;
	struct stack *next;
	struct stack *prev;
};

void push(float data, struct stack **top) {	
	struct stack *newNodePtr = (struct stack*)malloc(sizeof(struct stack));
	newNodePtr->num = data;
	newNodePtr->next = NULL;
	
	if(*top == NULL) {
		newNodePtr->prev = NULL;
		*top = newNodePtr;
	} 
	else{
		(*top)->next = newNodePtr;
		newNodePtr->prev = *top;
		*top = newNodePtr;
	}
}

float pop(struct stack **top) {
	float val = 0;
	if(*top == NULL) {
		return INFINITY;
	}
	val = (*top)->num;
	if((*top)->prev != NULL) {
		struct stack *nodePtr;
		(*top)->prev->next = NULL;
		nodePtr = *top; 
		*top = (*top)->prev;
		free(nodePtr);
	}
	else {
		*top = NULL;
		free(*top);
	}
	return val;
}

int isOperator(char *op) {
	if(strcmp(op,"+") == 0 || strcmp(op,"-") == 0 || strcmp(op,"*") == 0  || strcmp(op,"/") == 0 ) {
		return 1;
	}   
	
	return 0;
}

int isNum(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); 
}

int precedence(char *ch) {   
	switch (ch[0]) {   
	case '+':   
	case '-':   
		return 1;   

	case '*':   
	case '/':   
		return 2;   
	}   
	return -1;   
}  

char *intToOperator(int num) {
	switch (num) {   
	case 1:  
		return "+"; 
	case 2:   
		return "-";   

	case 3:   
		return "*";
	case 4:   
		return "/";   
	case 5:
		return "(";
	} 
	return "invalid"; 
}

int operatorToInt(char *op) {
	switch (op[0]) {   
	case '+':  
		return 1; 
	case '-':   
		return 2;   

	case '*':   
		return 3;
	case '/':   
		return 4;     
	case '(':   
		return 5;   
	}   
	return -1; 
}

char *toPostfix(char *infix) {
	struct stack *top2 = NULL;
	float popVal;
        char *expression, *token, *value, *temp = " ";
        expression = (char*)malloc( 100 * sizeof(char) );
        value = (char*)malloc( 10 * sizeof(char) );
        token = strtok(infix, " ");
	 while(token != NULL) { 
       	if (isOperator(token) == 0 && strcmp(token,"(") != 0 && strcmp(token,")") != 0) {
       		if(isNum(token[0]) == 1) {
       			return "invalid expression";
       		}
       		strncat(expression, token, strlen(token));
       		strncat(expression, temp, strlen(temp));
        	}
        	else if (strcmp(token,"(") == 0) {
        		push(5, &top2); 
            	}
        	else if (strcmp(token, ")") == 0) { 
            		while (top2 != NULL && top2->num != 5 ) {
                		if((popVal = pop(&top2)) == INFINITY) {
					return "invalid expression";
				}
                		strcpy(value, intToOperator(popVal));
                		strncat(expression, value, strlen(value)); 
                		strncat(expression, temp, strlen(temp));
            		}
            		if (top2 != NULL && top2->num != 5) {   		
             			return "invalid expression"; 
             		}             
           		else {
                		pop(&top2); 
        		}
        	}
        	else {
            		while (top2 != NULL && precedence(token) <= precedence(intToOperator(top2->num))) { 
            			if((popVal = pop(&top2)) == INFINITY) {
					return "invalid expression";
				}
                		strcpy(value, intToOperator(popVal));
                		strncat(expression, value, strlen(value)); 
                		strncat(expression, temp, strlen(temp));
                	}
            		push(operatorToInt(token), &top2); 
        	}
        	token = strtok(NULL," ");
        }  
    	while (top2 != NULL) {
    		if((popVal = pop(&top2)) == 5 || popVal == INFINITY) {
			return "invalid expression";
		}
        	value = intToOperator(popVal);
        	strncat(expression, value, strlen(value));
        	strncat(expression, temp, strlen(temp));
    	} 
    	return  expression;
}

char *calcuate(char *line) {
	if(strcmp(line, "") == 0 ) {
		return "Invalid expression\n";
	}
	struct stack *top1 = NULL;
	float val1, val2;
	char *token, *retVal;
	
	line[strcspn(line, "\n")] = '\0';
	line = toPostfix(line);
	if(strcmp(line, "invalid expression") == 0 ) {
		return "Invalid expression\n";
	}
	token = strtok(line," ");
	
	while(token != NULL) {
		if(isOperator(token) == 0) {
			push(atof(token), &top1);
		}
		else {
			val1 = pop(&top1);
			val2= pop(&top1);
			if(val1 == INFINITY || val2 == INFINITY) {
				return "invalid expression";
			}
			switch(token[0]) {
			case '+':
				push(val2 + val1, &top1);
				break;
			case '-':
				push(val2 - val1, &top1);
				break;
			case '*':
				push(val2 * val1, &top1);
				break;
			case '/':
				push(val2 / val1, &top1);
				break;
			}
		}
		token = strtok(NULL," ");
	}
	retVal = (char*)malloc( 50 * sizeof(char) );
	float result = pop(&top1);
	if(result == INFINITY) {
		return "invalid expression";
	}
	sprintf(retVal, "%f\n", result);
	return retVal;
}

void str_echo(int sockfd) {
	ssize_t n;
	char line[MAXLINE];
	char *result;
	
	for( ; ; ) {
		if((n=Readline(sockfd, line, MAXLINE)) ==  0) {
			return;
		}
		result = calcuate(line);
		Writen(sockfd, result , strlen(result)); 
	}
}

void main() {
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
	while(1) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

		if((childpid = fork()) == 0 ) {
			Close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		Close(connfd);
	}

}
