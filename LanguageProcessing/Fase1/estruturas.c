#include <stdio.h>
#include <glib.h>
#include <gmodule.h>
#include "estruturas.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
//////////////////////////////////// Comentarios e Respostas ////////////////////////////////////
extern char* strdup(const char*);
struct Comentario{
    int id;
    char* user;
    char* date;
    int replies;
    char* commentText;
    GList* respostas;
};

Comentario createComentario(int id, char* nome, char* data, int rep, char* comentario){
    Comentario co = malloc(sizeof(struct Comentario));
    co->id = id;
    co->user = strdup(nome);
    co->date = data;
    co->replies = rep;
    co->commentText = strdup(comentario);
    co->respostas = NULL;

    return co;
}



//////////////////////////////////// Hash de Comentarios ////////////////////////////////////


struct TodosComentarios{
    GHashTable* comentarios;
};



TodosComentarios createTodosComentarios(){
    TodosComentarios tc = malloc(sizeof(struct TodosComentarios));
    tc->comentarios = g_hash_table_new(g_str_hash, g_str_equal);
  return tc;
}

void insereComentario(TodosComentarios tc, Comentario co){
    Comentario comment = g_hash_table_lookup(tc->comentarios, (gpointer) &co->id);
    if(comment == NULL){
        g_hash_table_insert(tc->comentarios,(gpointer) &co->id, (gpointer) co);
        return;
    }
    else{
        comment->replies++;
        comment->respostas = g_list_append(comment->respostas,co);
    }
    return;
}


void imprimeComentarios(TodosComentarios tc){
    GList* values = g_hash_table_get_values(tc->comentarios);
    while(values != NULL){
        Comentario co = (Comentario) values->data;
        printf("\n\n [%d] -- %s -- Comentario: %s\n",co->id,co->user,co->commentText);
        printf( "\n Numero de respostas: %d\n",co->replies);

        int nrRespostas = 1;
        while(co->respostas != NULL){
            Comentario c = co->respostas->data;
            printf("[%d] -- %s -- Resposta ao comentario: %s\n", co->id,c->user,(char*)c->commentText);
            co->respostas = g_list_next(co->respostas);
            nrRespostas++;
        }
        
        values = g_list_next(values);
    }
}

void imprimeFormatoJson(char const *file, TodosComentarios tc){
    int fd = open(file,O_CREAT | O_TRUNC | O_WRONLY, 0640);
    close(1);
    dup(fd);
    GList* values = g_hash_table_get_values(tc->comentarios);
    printf("[\n\t");
    while(values != NULL){
        Comentario co = (Comentario) values->data;
            char*timestamp="";
            char*date="";
            char* tok= strtok (co->date,"T");
            if (tok!=NULL){
            date=tok;
            tok=strtok(NULL,"T");
            timestamp=tok;
            }
                printf("\t{\n\t");
                printf("\"id\": \"%d\",\n\t",co->id);
                printf("\"user\": \"%s\",\n\t",co->user);
                printf("\"date\": \"%s\",\n\t",date);
                printf("\"Timestamp\": \"%s\",\n\t",timestamp);
                printf("\"likes\": 0,\n\t");
                printf("\"commentText\": \"%s\",\n\t",co->commentText);
                if((co->replies)>0){
                printf("\"hasReplies\": true,\n\t");
                }
                else{
                printf("\"hasReplies\": false,\n\t");
                }
                printf("\"numberOfReplies\": %d,\n\t",(co->replies));
                printf("\"replies\": [");

            int nrRespostas = 1;

        while(co->respostas != NULL){
            Comentario c = co->respostas->data;
            char*timestamp="";
            char*date="";
            char* tok= strtok (c->date,"T");
            if (tok!=NULL){
            date=tok;
            tok=strtok(NULL,"T");
            timestamp=tok;
            }
            printf("\n\t\t{\n\t");
            printf("\"user\": \"%s\",\n\t",c->user);
            printf("\"date\": \"%s\",\n\t",date);
            printf("\"Timestamp\": \"%s\",\n\t",timestamp);
            printf("\"likes\": 0,\n\t");
            printf("\"commentText\": \"%s\"\n\t}",(char*)c->commentText);
            co->respostas = g_list_next(co->respostas);
            if(co->replies>nrRespostas){
                printf(",");
            }
            nrRespostas++;

        }
        printf("]\n");
        printf("},");
        values = g_list_next(values);

    }
    printf("]");
    close(fd);

}






char *transform_Errors(char* coment){
char *result = malloc(strlen(coment)+50);
int i,j;
for(i = 0,j=0; i < strlen(coment); i++,j++)
{
	if(coment[i] == '"')  
	{
        
		result[j] = '\\';
        j++;
        result[j] = '"';
        
	}
    else result[j] = coment[i];
}

if(j>i){
        strcpy(coment,result);
        free(result);
     return coment;
}
return NULL;
}

void imprimeFormatoJson_ToCompile(char const *file, TodosComentarios tc){
    int fd = open(file,O_CREAT | O_TRUNC | O_WRONLY, 0640);
    int stdout=dup(1);
    dup2(fd,1);
    GList* values = g_hash_table_get_values(tc->comentarios);
    printf("[\n\t");
    while(values != NULL){
        Comentario co = (Comentario) values->data;
            char*timestamp="";
            char*date="";
            char* tok= strtok (co->date,"T");
            if (tok!=NULL){
            date=tok;
            tok=strtok(NULL,"T");
            timestamp=tok;
            }
                printf("\t{\n\t");
                printf("\"id\": \"%d\",\n\t",co->id);
                printf("\"user\": \"%s\",\n\t",co->user);
                printf("\"date\": \"%s\",\n\t",date);
                printf("\"Timestamp\": \"%s\",\n\t",timestamp);
                printf("\"likes\": \"0\",\n\t");
            char* coment; 
            coment = transform_Errors(co->commentText);
            if(coment!=NULL){
                        printf("\"commentText\": \"%s\",\n\t",coment);   
            }
                    else{
                        printf("\"commentText\": \"%s\",\n\t",(char*)co->commentText);
            }
                if((co->replies)>0){
                printf("\"hasReplies\": true,\n\t");
                }
                else{
                printf("\"hasReplies\": false,\n\t");
                }
                printf("\"numberOfReplies\": %d,\n\t",(co->replies));
                printf("\"replies\": [");

            int nrRespostas = 1;

        while(co->respostas != NULL){
            Comentario c = co->respostas->data;
            char*timestamp="";
            char*date="";
            char* tok= strtok (c->date,"T");
            if (tok!=NULL){
            date=tok;
            tok=strtok(NULL,"T");
            timestamp=tok;
            }
            printf("\n\t\t{\n\t");
            printf("\"user\": \"%s\",\n\t",c->user);
            printf("\"date\": \"%s\",\n\t",date);
            printf("\"Timestamp\": \"%s\",\n\t",timestamp);
            printf("\"likes\": \"0\",\n\t");
            char* coment1; 
            coment1 = transform_Errors(c->commentText);
            if(coment1!=NULL){
            printf("\"commentText\": \"%s\"\n\t}",coment1);   
            }
            else{
            printf("\"commentText\": \"%s\"\n\t}",(char*)c->commentText);
            }
            co->respostas = g_list_next(co->respostas);
            if(co->replies>nrRespostas){
                printf(",");
            }
            nrRespostas++;

        }
        printf("]\n");
        printf("},");
        values = g_list_next(values);

    }
    printf("]");
    close(fd);

}
