%{
#include "y.tab.h"
%}

%option yylineno
%%
[ \t\r] ;


[+*\-/=(),\.\[\]\"]  return *yytext;
[\n]             return SEP;

0[xob][A-Za-z_0-9]+ {yylval.cvalue= strdup(yytext); return numSis;}
(?i:title) {yylval.cvalue= strdup(yytext); return title;}
(?i:True|false) {yylval.cvalue= strdup(yytext); return booleano;}

[A-Za-z_-]+[0-9]* {yylval.cvalue= strdup(yytext); return string;}
[+-](?i:inf|nan) {yylval.cvalue= strdup(yytext); return math;}
[0-2][0-9]:[0-6][0-9]:[0-6][0-9] {yylval.cvalue= strdup(yytext);return tempo;}
[0-9][0-9][0-9][0-9]-[0-1][0-9]-[0-3][0-9] {yylval.cvalue= strdup(yytext);return date;}
[+-]?[0-9]+[.]?[0-9]*[Ee]?[+-]?[0-9]* {yylval.cvalue= strdup(yytext);return num;}
[\"\'][\<\>\*\\\/\$a-zA-Z0-9 ?:.-]+[\"\'] {yytext[yyleng-1]=0;yylval.cvalue= strdup(yytext+1);return StringAspas;}

. ;
