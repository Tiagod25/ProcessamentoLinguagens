%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

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
%}

%union{
    int ivalue;
    char* cvalue;
}
%token string title SEP bool date time num StringAspas
%type <cvalue> string date time bool num StringAspas elemento title lista elemsLista elemLista
%%
TOML: titulo SEP seccoes
	;

titulo: title '=' StringAspas {printf("{ \n\"%s\" :{\n",$3);parentesesAb=parentesesAb+2;}

seccoes: seccoes SEP seccao
		|seccao
		;

seccao: nomesec SEP corposec SEP {printf("\n}\n");parentesesAb--;}
		;

nomesec:'['string']' {if (seccoes>0){printf(",");}
					  seccoes++;parentesesAb++;contadorSec=0; printf("	\"%s\": {\n",$2);contadorSave=0;}
		|'['string '.' string']' {parentesesAb++;
									if (contadorSec==0){printf("	\"%s\": {\n",$4);}
									if (contadorSec>0){printf(",\n	\"%s\": {\n",$4);}contadorSave=contadorSec+1;contadorSec=0;}
		;
corposecUNI: declaracao
	   		| seccao {contadorSec=contadorSave;}
			;

corposec: corposec SEP corposecUNI
		| corposecUNI
		;

declaracao: string '=' elemento { if(contadorSec>0) {
										if(flagLista==0){
											printf(",\n		\"%s\":\"%s\"",$1,$3);
										}
										else{
											printf(",\n		\"%s\":[",$1);
											int j=0;
											while(j<(i-1)){
												printf("\"%s\",",lista[j]);
												j++;
											}
											printf("\"%s\"",lista[i-1]);
											printf("]");
											flagLista=0;
											i=0;
										}
									}
									else{
										if (flagLista==0){
											printf("		\"%s\":\"%s\"",$1,$3);
										}
										else{
											printf("		\"%s\":[",$1);
											for(int k=0;k<(i-1);k++){
												printf("\"%s\",",lista[k]);
											}
											printf("\"%s\"",lista[i-1]);
											printf("]");
											flagLista=0;
											i=0;
										}
										}
									contadorSec++;												}
		  ;

elemento: num
		| bool
    	| date
		| time
        | StringAspas
		| lista {flagLista=1;}
        ;

lista: '['lb elemsLista lb']'
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
	while(parentesesAb>0){
	printf("\n}");
	parentesesAb--;
	}
	return(0);
	}

int main(){
	truncate("log.json",0);
    int fd=open("log.json",O_CREAT|O_WRONLY,0666);
    dup2(fd,1);
    yyparse();
	return 0;
}


int yywrap(){return(1);}
