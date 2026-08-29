%option noyywrap
%{
#include "15-parser.tab.hpp"
#include <cstdio>
int lineno = 1;
%}

ID      [A-Za-z_][A-Za-z0-9_]*
NUM     [0-9]+(\.[0-9]+)?

%%
[ \t\r]+               ;
\n                      { lineno++; }
"int"|"float"|"double"  { return TYPE; }
{ID}                    { yylval.sval = strdup(yytext); return ID; }
"+"|"-"|"*"|"/"     { return yytext[0]; }
";"                    { return ';'; }
{NUM}                   { yylval.fval = atof(yytext); return NUM; }
.                       { return yytext[0]; }
%%