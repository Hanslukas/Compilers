#include <ctype.h> 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 10

int minus=1; //I use this variable for manage the minus at start expression.

//Enumerated Types allow us to create our own symbolic names for a list of related ideas.

typedef enum { 
  ADD, SUB, MUL, DIV, MOD, POW, OBR, CBR, VAL, NON, //These comply with the order of precTable
  NEXT,
  EXP, //Expression
  ERR, ERR1, ERR2 //Mangage of errors
  } tokens;

typedef struct token {
  tokens type;
  double value;
} Token;

typedef enum { L, G, E, B, A } cells; //Enumerated for precTable, L=Less, G=Greater, E=Egual, B=Blank; A=Accepted


/*This part of a compiler breaks the source code into meaningful symbols that the parser can work with. 
 Typically, the scanner returns an enumerated type  representing the symbol just scanned. */

Token scanner(int *p)
{
  Token token = {NON, 0};
  int i, fl; //flag
  if (*p==' ')
    while(isspace(*p = getchar()));// check whether the passed character is white-space.

  switch(*p)
  {
    case '+': 
              token.type = ADD; //check the enumeration type to the type of the token of the structure
              *p=getchar();
              minus=-1;
              return token;
    case '-': if(minus==1)
              {
                *p=getchar();
                minus=0;
                break;
              }
                
              token.type = SUB; 
              *p=getchar();
              minus=-1;
              return token;
    case '*': 
              token.type = MUL;
              *p=getchar();
              minus=-1;
              return token;
    case '/': token.type = DIV;
              *p=getchar();
              minus=-1;
              return token;
    case '%': token.type = MOD;
              *p=getchar();
              minus=-1;
              return token;
    case '^': token.type = POW;
              *p=getchar();
              minus=-1;
              return token;
    case '(': minus=1;
              token.type = OBR;
              *p=getchar();
              return token;
    case ')': 
              token.type = CBR;
              *p=getchar();
              minus=-1;
              return token;

    case '\n': return token;
    default:
              if(!isdigit(*p)) //checks if the passed character is a decimal digit character. 
              {
                token.type = ERR; //If not a return error digits.
                fprintf(stderr, "Error. It's not a digit.\n");
                fprintf(stderr, "Use this syntax: (3+5)^3+7-2\n");
                return token;
              }
  }

  token.type = VAL;

  for(i=0; isdigit(*p); i++)
  {
    if((token.value) == 0 && i == 1 && *p=='0')
    {
      token.type=ERR1;
      return token;
    }
      token.value = token.value * 10 + *p - '0'; //I use this expression for take the integer numbers. '0' references the table ASCII
      *p=getchar();
  }
//Then if *p is apoint I managed the floating number
  if(*p == '.')
    for(fl=-1; isdigit(*p=getchar()); fl--)
      token.value+=(*p-'0')*pow(10,fl); //I use this expression for take floating numbers
//If *p is e then will be an elevation
  if(*p == 'e')
  {
    *p=getchar();
    switch(*p)
    {
      case '+': if(!isdigit(*p = getchar()))
                {
                  token.type=ERR2;
                  return token;
                }
                else
                {
                  if(*p=='0')
                  {
                    if ((*p=getchar())==' ')
                      return token;
                    else
                    {
                      token.type = ERR2;
                      return token;
                    }
                  }
                  fl=*p-'0';
                    for ( ; isdigit(*p=getchar()); ) 
                      fl = fl * 10 + *p - '0'; 
                    token.value=token.value*pow(10,fl);
                  }
                  break;
      case '-':
                if(!isdigit(*p = getchar()))
                {
                  token.type=ERR2;
                  return token;
                }
                else
                {
                  if(*p=='0')
                  {
                    if ((*p=getchar())==' ')
                      return token;
                    else
                    {
                      token.type = ERR2;
                      return token;
                  } 
                } 
                    fl=*p-'0';
                    for ( ; isdigit(*p=getchar()); ) 
                      fl = fl * 10 + *p - '0'; 
                    token.value=token.value*pow(10,-fl);
                  }
                  break;

          default: token.type=ERR;
                  return token;
                } 
        }
        if(minus==0)
        {
          token.value = token.value * (-1);
        }
        minus=0;
        return token;
}


/*PrecTable*/
static int precTable[ROWS][10] = {
  {G,G,L,L,L,L,L,G,L,G},// +
  {G,G,L,L,L,L,L,G,L,G},// -
  {G,G,G,G,G,L,L,G,L,G},// *
  {G,G,G,G,G,L,L,G,L,G},// /
  {G,G,G,G,G,L,L,G,L,G},// %
  {G,G,G,G,G,L,L,G,L,G},// ^
  {L,L,L,L,L,L,L,E,L,B},// (
  {G,G,G,G,G,G,B,G,B,G},// )
  {G,G,G,G,G,G,B,G,B,G},// NUM
  {L,L,L,L,L,L,L,B,L,A},// $ endmarker
  };


typedef struct pushdown Stack;
/*We use a pushdown automata. The memory is constituted by a stack, 
a data structure whose data can be extracted in order necessarily opposite to the insertion. */
struct pushdown {
  Token token;
  Stack *next;
};

//Function for push in stack
Stack *push(Stack *stack, Token token) {
  Stack *tmp;
  if((tmp = (Stack *) malloc(sizeof(Stack))) == NULL)
    return NULL;
  tmp->token = token;
  tmp->next = stack;
  return tmp;
  }

//Function for pop in stack
Stack *pop(Stack *stack) {
  if(stack != NULL) {
    Stack *tmp = stack->next;
    free(stack);
    stack = tmp;
    }
  return stack;
  }

//This function takes the first token type different from Exp
int topTerminal(Stack *stack)
{
  for(; stack != NULL; stack = stack->next)
    if(stack->token.type != EXP)
      return stack->token.type;
  //Error
}


//MAIN
int main() 
{
  int p=' ';
  double v;
  Token token = {NON, 0};
  Stack *stack = NULL;
  stack = push(stack, token);
  token.type = NEXT;

  for(;;)
  {
    if(token.type == NEXT)
      token = scanner(&p);
    if(token.type == ERR)
    {
      fprintf(stderr, "\nError: character not accepted!\n");
      return 1;
    }
    if(token.type == ERR1) 
    {
      fprintf(stderr, "Error: double zero not permitted!\n");
      return 1;
    }
    if(token.type == ERR2) 
    {
      fprintf(stderr, "Error: invalid float number!\n");
      return 1;
    }
    printf("%d|%g\n", token.type, token.value);
  


    switch(precTable[topTerminal(stack)][token.type])
    {
      case L:
      case E:
           stack = push(stack, token); //If less or Egual push into pushdown
           token.type = NEXT;
           break;
      case G: switch(topTerminal(stack))
              {
                case ADD: // E -> E+E
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  v = stack->token.value;
                  stack = pop(stack);
                  if(stack->token.type != ADD)
                    goto syntaxError;
                  stack = pop(stack);
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  stack->token.value += v;
                  continue;

                case SUB: // E -> E+E
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  v = stack->token.value;
                  stack = pop(stack);
                  if(stack->token.type != SUB)
                    goto syntaxError;
                  stack = pop(stack);
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  stack->token.value -= v;
                  continue;

                 case MUL: // E -> E+E
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  v = stack->token.value;
                  stack = pop(stack);
                  if(stack->token.type != MUL)
                    goto syntaxError;
                  stack = pop(stack);
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  stack->token.value *= v;
                  continue;

                 case DIV: // E -> E+E
                  if(stack->token.type != EXP || stack->token.value == 0) //Check division by 0
                  {
                    fprintf(stderr, "You can not divide by 0\n");
                    goto syntaxError;
                  }
                  v = stack->token.value;
                  stack = pop(stack);
                  if(stack->token.type != DIV)
                    goto syntaxError;
                  stack = pop(stack);
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  stack->token.value /= v;
                  continue;

                 case MOD: // E -> E+E
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  v = stack->token.value;
                  stack = pop(stack);
                  if(stack->token.type != MOD)
                    goto syntaxError;
                  stack = pop(stack);
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  stack->token.value = (int) stack->token.value % (int)v;
                  continue;

                 case POW: // E -> E+E
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  v = stack->token.value;
                  stack = pop(stack);
                  if(stack->token.type != POW)
                    goto syntaxError;
                  stack = pop(stack);
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  stack->token.value = pow(stack->token.value, v);
                  continue;
              

      case CBR: // E-> ( E )
                  if(stack->token.type != CBR)
                    goto syntaxError;
                  stack = pop(stack);
                  if(stack->token.type != EXP)
                    goto syntaxError;
                  v = stack->token.value;
                  stack = pop(stack);
                  if(stack->token.type != OBR)
                    goto syntaxError;
                  stack->token.value = v;
                  stack->token.type = EXP;
                  continue;

      case VAL: // E-> i
                  if(stack->token.type != VAL)
                    goto syntaxError;
                  stack->token.type = EXP;
                  continue;
        }   
      case B: 
      syntaxError:
              fprintf(stderr, "Syntax error\n");
              return 2;

      case A: 
              if(stack->token.type == EXP)
                printf("%g\n", stack->token.value);
      default: return 0;
    }
  }
  return 0;
}