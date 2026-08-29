/*
Compilation command:
flex --outfile=./src/Compiler/tmp-06-flex.cpp ./src/Compiler/06-flex.l && ./run ./src/Compiler/tmp-06-flex.cpp && rm ./src/Compiler/tmp-06-flex.cpp
*/

%option noyywrap
%{
#include <stdio.h>
#include <string.h>
#include "src/commons.hpp"
static int lineno = 1;
%}

DIGIT      [0-9]
DATA_TYPE       ("int"|"float"|"bool"|"void"|"char"|"struct"|"union")
CONTROL_FLOW    ("if"|"else"|"for"|"do"|"while"|"return"|"break"|"continue")
BOOL_CONST      ("true"|"false")
ID         [A-Za-z_][A-Za-z0-9_]*
INT_CONST   {DIGIT}+
FLOAT_CONST {DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)?

%%
[ \t\r]+            ;
\n                  { lineno++; }
\/\/.*              ;
\/\*([^*]|\*+[^*/])*\*+\/   ;

{DATA_TYPE}         { printf("<KEYWORD,%s,%d>\n", yytext, lineno); }
{CONTROL_FLOW}      { printf("<KEYWORD,%s,%d>\n", yytext, lineno); }
{FLOAT_CONST}       { printf("<FLOAT_CONST,%s,%d>\n", yytext, lineno); }
{INT_CONST}         { printf("<INT_CONST,%s,%d>\n", yytext, lineno); }
{BOOL_CONST}        { printf("<BOOL_CONST,%s,%d>\n", yytext, lineno); }
{ID}                { printf("<IDENTIFIER,%s,%d>\n", yytext, lineno); }

"+"|"-"|"*"|"/"|"%" { printf("<ARITH_OP,%s,%d>\n", yytext, lineno); }
"=="|"!="|"<="|">="|"<"|">" { printf("<REL_OP,%s,%d>\n", yytext, lineno); }
"&&"|"||"|"!"   { printf("<LOGICAL_OP,%s,%d>\n", yytext, lineno); }
"="|"\+="|"-="|"\*="|"/=" { printf("<ASSIGN_OP,%s,%d>\n", yytext, lineno); }

";"|","|"\("|"\)"|"\{"|"\}" { printf("<DELIM,%s,%d>\n", yytext, lineno); }

.                   { printf("<UNKNOWN,%s,%d>\n", yytext, lineno); }
%%

int main(int argc, char** argv) {
    Footer();
    Header("Flex Lexical Scanner");
    Surab::Log("Enter a c/c++ expression: ");
    yylex();
    return 0;
}
