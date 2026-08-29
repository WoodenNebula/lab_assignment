%{
#include <cstdio>
#include <cstdlib>
#include <string>
#include "src/commons.hpp"

extern int yylex();
void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
%}

%union { char* sval; double fval; }
%token <sval> ID
%token <fval> NUM
%token TYPE
%left '+' '-'
%left '*' '/'
%right'<' '>'

%%
program: decl_list ;

decl_list: /* empty */
         | decl_list declaration ;

declaration: TYPE ID ';' { printf("Decl %s\n", $2); }
           | expr ';' { printf("Expr evaluated\n"); }
;

expr: expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '<' expr
    | expr '>' expr
    | '(' expr ')'
    | NUM
    | ID
;

%%

int main(){
    Header("Parser: Flex/Bison");
    Footer();
    Surab::Log("\nEnter declarations or expressions:");
    yyparse();
    return 0;
}
