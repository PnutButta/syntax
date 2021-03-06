//
//  syntax.c
//  
//
//  Created by Angel Chara'e Mitchell on 3/23/17.
//
//

/* front.c - a lexical analyzer system for simple
 arithmetic expressions */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
char expression [100];
int curr;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
void factor();
void term();
void expr();
void error();
void clear();

int lex();
/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define NEW_LINE 30

/* main driver */
int main(int argc, char *argv[]) {
    curr = 0; //start over for each line
    if (argc != 2) {
        printf("No filename input\n");
    }
    else {
        /* Open the input data file and process its contents */
        if ((in_fp = fopen(argv[1], "r")) == NULL)
            printf("ERROR - cannot open front.in \n");
        else {
            getChar(); do {
                lex();
                expr();
                if (nextToken == NEW_LINE) {
                    printf("\nNew expression \n");
                }
            } while (nextToken != EOF);
        }

    }
}

/******************************************************/

/* lookup 
 - a function to lookup operators and parentheses
and return the token */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
            
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
            
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
            
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
            
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
            
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
            
        case '\n':
            nextToken = NEW_LINE;
            break;
            
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/* clear
 - a function to clear the expression array for new expressions*/
void clear() {
    memset(expression, 0, sizeof expression);
    //for(int i = 0; i < curr; i=i+1) {
      //  expression[i] = ' ';
    //}
}
/* addChar 
 - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}

/* getChar 
 - a function to get the next character of
 input and determine its character class */
void getChar() {
     if ((nextChar = getc(in_fp)) != EOF) {
         if (isalpha(nextChar))
             charClass = LETTER;
         else if (isdigit(nextChar))
             charClass = DIGIT;
            else charClass = UNKNOWN;
         expression[curr] = nextChar;          //add to expression array
         curr = curr+1;
         if (nextChar == '\n') {
             clear();
             curr = 0;
         }
     }
     else
         charClass = EOF;
 }

/* getNonBlank
 - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank() {
    if (nextChar != '\n') {            // ensure \n is not skipped as whitespace
     while (isspace(nextChar))
         getChar();
    }
 }

/* lex 
 - a simple lexical analyzer for arithmetic
 expressions */
int lex() {
     lexLen = 0;
     getNonBlank();
     switch (charClass) {
        /* Parse identifiers */
         case LETTER:
             addChar();
             getChar();
             while (charClass == LETTER || charClass == DIGIT) {
                 addChar();
                 getChar();
             }
             nextToken = IDENT;
             break;
             
        /* Parse integer literals */
         case DIGIT:
             addChar();
             getChar();
             while (charClass == DIGIT) {
                 addChar();
                 getChar();
             }
             nextToken = INT_LIT;
             break;

             
        /* Parentheses and operators */
         case UNKNOWN:
             lookup(nextChar);
             getChar();
             
            if (nextToken == NEW_LINE) {    // '\n' as unknown in charClass
                if (nextChar != EOF) {
                    nextToken = NEW_LINE;
                    lexeme[0] = 'N';
                    lexeme[1] = 'E';
                    lexeme[2] = 'W';
                    lexeme[3] = 0;
                    break;
                }
                
                else                        // If nextChar is EOF, skip newline
                    goto LINE209;           // goto EOF case
             }
             break;
             
        /* EOF */
         LINE209:
         case EOF:
             nextToken = EOF;
             lexeme[0] = 'E';
             lexeme[1] = 'O';
             lexeme[2] = 'F';
             lexeme[3] = 0;
             break;
     } /* End of switch */
     printf("Next token is: %d, Next lexeme is %s\n",
           nextToken, lexeme);
     return nextToken;
 }

/* expr
 - Parses strings in the language generated by the rule:
 <expr> -> <term> {(+ | -) <term>} */
void expr(){
    printf("Enter <expr>\n");
    
    /* Parse the first term */
    term();
    
    /* As long as the next token is + or -, get
     the next token and parse the next term */
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
    printf("Exit <expr>\n");
 }

/* term
 - Parses strings in the language generated by the rule:
 <term> -> <factor> {(* | /) <factor>) */
void term() {
    printf("Enter <term>\n");
    /* Parse the first factor */
    
    factor();
    /* As long as the next token is * or /, get the
     next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }
    printf("Exit <term>\n");
 }

/* factor
 - Parses strings in the language generated by the rule:
 <factor> -> id | int_constant | ( <expr ) */
void factor() {
    printf("Enter <factor>\n");
    /* Determine which RHS */
    if (nextToken == IDENT || nextToken == INT_LIT)
        
    /* Get the next token */
        lex();
    
    /* If the RHS is ( <expr>), call lex to pass over the
     left parenthesis, call expr, and check for the right
     parenthesis */
    else {
        if (nextToken == LEFT_PAREN) {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN)
                lex();
            else
                error();
        }  /* End of if (nextToken == ... */
        
        /* It was not an id, an integer literal, or a left
         parenthesis */
        else
            error();
    }  /* End of else */
    
    printf("Exit <factor>\n");
 }

/* erorr
 - If error is found in line, line is printed from start
 to place where error is found
 */
void error() {
    printf("\n");
    for (int i = 0; i < curr; i=i+1) {         //iterate to show expr before error
        printf("%c", expression[i]);
    }
    printf("\nError has occured at '%s' \n\n", lexeme);
    
 }

