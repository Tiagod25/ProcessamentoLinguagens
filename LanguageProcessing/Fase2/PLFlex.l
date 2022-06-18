%{
#include "y.tab.h"
%}

%option yylineno
%%
[ \t\r] ;


[+*\-/=(),\.\[\]\"]  return *yytext;
[\n]             return SEP;

(?i:title) {yylval.cvalue= strdup(yytext); return title;}
(?i:True|false) {yylval.cvalue= strdup(yytext); return bool;}

[A-Za-z_-]+ {yylval.cvalue= strdup(yytext); return string;}
[0-2][0-9]:[0-6][0-9]:[0-6][0-9] {yylval.cvalue= strdup(yytext);return time;}
[0-9][0-9][0-9][0-9]-[0-1][0-9]-[0-3][0-9] {yylval.cvalue= strdup(yytext);return date;}
[0-9]+ {yylval.cvalue= strdup(yytext);return num;}
\"[a-zA-Z0-9 .-]+\" {yytext[yyleng-1]=0;yylval.cvalue= strdup(yytext+1);return StringAspas;}

. ;
