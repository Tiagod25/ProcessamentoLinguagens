%{
#include <stdio.h>
#include "estruturas.h"
#include <stdlib.h>
#include <string.h>
extern char* strdup(const char*);

int fileno(FILE *stream);
TodosComentarios tc= NULL;

char* autor;
char* comentario;
int respostas = 0;
int nComentarios=0;
int id_comment=0;
int resposta=0;
char* data;
int restotal=0;

%}
espacos        [ ]*
conteudo_comentario     <p>[^A-Za-z0-9.?!"]*
inicio_data    <time\ class=\"dateline\ comment__dateline\"\ datetime=\"
nome_autor     rel=\"(nofollow|follow)\">
fim_comentario          <\/li>
inicio_comentario       <li
inicio_respostas        <ol\ class=\"comments__list\">
fim_respostas           <\/ol>
fim_comentario_resposta  <\/li>\n<\/ol>

%x COMMENT AUTOR DATA CONTEUDO
%%
{inicio_comentario}                     {id_comment++;nComentarios++;respostas=0;resposta=0;BEGIN COMMENT;}

<COMMENT>{nome_autor}              {if(resposta==1) {respostas++;}
                                     BEGIN AUTOR;}
<COMMENT>{inicio_data}             {BEGIN DATA;}
<COMMENT>{conteudo_comentario}      {BEGIN CONTEUDO;}
<COMMENT>{inicio_respostas}         {resposta=1;}
<COMMENT>{fim_respostas}            {BEGIN INITIAL;}

<AUTOR>(.*?)<\/a>             {yytext[yyleng-5] = '\0'; autor = strdup(yytext);BEGIN COMMENT;}

<DATA>(.*?)\">                  {yytext[yyleng-2] = '\0'; data = strdup(yytext);BEGIN COMMENT;}

<CONTEUDO>[^<]*      {yytext[yyleng-1] = '\0';Comentario com= createComentario(id_comment,autor,data,respostas,strdup(yytext));insereComentario(tc,com); BEGIN COMMENT;}

<*>{fim_comentario_resposta}      {BEGIN INITIAL;}
<*>{fim_comentario}               {if(resposta==0) BEGIN INITIAL;}
<*>.|\n                           {;}
%%

int yywrap(){ return 1; }


int main(int argc, char* argv[]){
    tc = createTodosComentarios();
    printf("Organizados ID's de comentários.\n");
    yylex();


    if(argc>1){
        
        imprimeFormatoJson_ToCompile(argv[1],tc);
        //imprimeFormatoJson(argv[1],tc);
        

    }
    else{
            imprimeComentarios(tc);
    }
    return 0;
}