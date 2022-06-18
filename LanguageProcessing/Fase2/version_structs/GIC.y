%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "estruturas.c"

extern int yylex();
extern int yylineno;
extern char* yytext;
int yyerror();

int comp = 0;
int contaN = 0;
int contaP = 0;
float soma = 0;
int contadorSec=0;
int contadorSave=0;
int seccoes=0;
int parentesesAb=0;
char* lista[50];
int i=0;
int flagLista=0;
TodosConjuntos tc;
Conjunto conjun;
Conjunto subconjunto;
int sub=0;
Igual ig;
%}

%union{
    int ivalue;
    char* cvalue;
}
%token string title SEP booleano date tempo num StringAspas math numSis
%type <cvalue> string date tempo booleano num StringAspas elemento title lista elemsLista elemLista math numSis
%%
TOML: titulo SEP seccoes
	;

titulo: title '=' StringAspas {tc =createTodosConjuntos($3);}

seccoes: seccoes SEP seccao
		|seccao
		;

seccao: nomesec SEP corposec SEP
		;

nomesec:'['string']' {conjun = createconj($2) ;insereConjunto(tc,conjun);}
		|'['string '.' string']' {subconjunto = createconj($4) ; conjun = createconj($2) ;insereSubConjunto(tc,conjun,subconjunto);sub=1;}
		;
corposecUNI: declaracao
	   		| seccao  {sub=0;}
			;

corposec: corposec SEP corposecUNI
		| corposecUNI
		;

declaracao: string '=' elemento { if(flagLista==0){
										if (sub==1) {ig =createIgualdade($1);inserirValores_Sub(tc,conjun,subconjunto,ig,$3);}
										else{ig =createIgualdade($1);inserirValores(tc,conjun,ig,$3);}
										}
										else{
										ig =createIgualdade($1);
										for(int k=0;k<i;k++){
											if (sub==1) {inserirValores_Sub(tc,conjun,subconjunto,ig,lista[k]);}
											else{inserirValores(tc,conjun,ig,lista[k]);}
											}
											flagLista=0;
											i=0;
										}
									}
		  ;

elemento: num
		| booleano
    	| date
		| tempo
        | StringAspas
		| lista {flagLista=1;}
		| math
		| numSis
        ;

lista:   '['lb elemsLista lb']'
	 	;

elemLista: num
		  |StringAspas
		  ;

elemsLista: elemsLista ',' lb elemLista {lista[i]=$4; i++;}
	  	   | elemLista				{lista[i]=$1; i++;}
		   ;
lb: SEP
	|
	;
%%

int yyerror(char *s) {
	return 0;
	}

int main(){
	//truncate("log.json",0);
    //int fd=open("log.json",O_CREAT|O_WRONLY,0666);
    //dup2(fd,1);
    yyparse();

	GList* values = g_hash_table_get_values(tc->todos_conjuntos);
	Conjunto co = (Conjunto) values->data;
	GList* sub = g_hash_table_get_values(co->sublistas);
  printf("{ \n\"%s\" :{ \n ",(char *)tc->tituloS);
  int contsec=0;
	while(values!=NULL){
		Conjunto co = (Conjunto) values->data;
		if (contsec>0) {printf(",");}
		printf("	\"%s\": {\n",(char *)co->nome);
		int cont=1;
		int igualFlag=0;
			while(co->igualdades!=NULL){
				Igual ig = co->igualdades->data;
				if((g_list_next(ig->valores))){
				igualFlag=1;
				if (cont>1) {printf(",\n");}
				printf("		\"%s\":[\"%s\"",(char *)ig->variavel,(char *)ig->valores->data);
				ig->valores = g_list_next(ig->valores);
				while(ig->valores!=NULL){
					printf(",");
				printf("\"%s\"",(char *)ig->valores->data);
				ig->valores = g_list_next(ig->valores);
				}
				printf("]");
				}
				else if (ig->valores!=NULL){
					igualFlag=1;
					if (cont>1) {printf(",\n");}
					printf("		\"%s\":\"%s\"",(char *)ig->variavel,(char *)ig->valores->data);
				}
				cont++;
				co->igualdades = g_list_next(co->igualdades);

	}
	GList* sub = g_hash_table_get_values(co->sublistas);
	if(sub!=NULL){
		Conjunto sublista = (Conjunto) sub->data;
		}
	int count =0;
	while(sub!=NULL){
		Conjunto sublista = (Conjunto) sub->data;
		if(igualFlag==1 && count==0) {printf(",");}
		if(count>0) {printf(",");}
		printf("\n	\"%s\": {\n",(char *)sublista->nome);
		int cont=1;
			while(sublista->igualdades!=NULL){
				Igual ig = sublista->igualdades->data;
				if((g_list_next(ig->valores))){
					if (cont>1) {printf(",\n");}
					printf("		\"%s\":[\"%s\"",(char *)ig->variavel,(char *)ig->valores->data);
					ig->valores = g_list_next(ig->valores);
				while(ig->valores!=NULL){
				printf(",");
				printf("\"%s\"",(char *)ig->valores->data);
				ig->valores = g_list_next(ig->valores);
				}
				printf("]");
				}
				else if (ig->valores!=NULL){
					if (cont>1) {printf(",\n");}
					printf("		\"%s\":\"%s\"",(char *)ig->variavel,(char *)ig->valores->data);
				}
				cont++;
				sublista->igualdades = g_list_next(sublista->igualdades);
	}
		printf("\n}");
		sub=g_list_next(sub);
		count++;
	}
		values=g_list_next(values);
		contsec++;
    	printf("\n}");
	}
	printf("\n}");
	printf("\n}");
	return 0;
}


int yywrap(){return(1);}
