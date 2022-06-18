#include <stdio.h>
#include <glib.h>
#include <gmodule.h>
#include "estruturas.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

extern char *strdup(const char *);
//////////////////////////////////// Hash de Comentarios ////////////////////////////////////
struct Igual
{
    char *variavel;
    GList *valores;
};

struct Conjunto
{
    char *nome;
    GList *igualdades;
    GHashTable *sublistas;
};
struct TodosConjuntos
{
    char *tituloS;
    GHashTable *todos_conjuntos;
};

Igual createIgualdade(char *variavel)
{
    Igual ig = malloc(sizeof(struct Igual));
    ig->variavel = variavel;
    ig->valores = NULL;
    //printf("CreateIgualdade = %s \n", variavel);
    return ig;
}
void inserirValores_Sub(TodosConjuntos tc, Conjunto co1, Conjunto co2, Igual ig, char *values)
{
    //printf("entrei no insereValores_ SUB \n");
    Conjunto conju = g_hash_table_lookup(tc->todos_conjuntos, g_strdup(co1->nome));
    Conjunto conju1 = g_hash_table_lookup(conju->sublistas, g_strdup(co2->nome));
    if (conju == NULL || conju1 == NULL)
    {
        //printf("conj nao existe");
        //printf("\n conj name = %s \n", conju->nome);
        return;
    }
    else
    {
        //printf("valor inserido:");
        //printf(" %s \n", values);
        ig->valores = g_list_append(ig->valores, values);
        conju1->igualdades = g_list_append(conju1->igualdades, (gpointer)ig);
    }
    return;
    //printf("inserirValores = %s \n", values);
    return;
}
void inserirValores(TodosConjuntos tc, Conjunto conj, Igual ig, char *values)
{
    //printf("entrei no insereValores \n");
    Conjunto conju = g_hash_table_lookup(tc->todos_conjuntos, g_strdup(conj->nome));
    if (conju == NULL)
    {
        //printf("conj nao existe");
        //printf("\n conj name = %s \n", conj->nome);
        return;
    }
    else
    {
        //printf("valor inserido:");
        //printf(" %s \n", values);
        ig->valores = g_list_append(ig->valores, values);

        conju->igualdades = g_list_append(conju->igualdades, (gpointer)ig);
    }
    return;
    //printf("inserirValores = %s \n", values);
    return;
}
Conjunto createconj(char *name)
{
    Conjunto co = malloc(sizeof(struct Conjunto));
    co->nome = name;
    co->igualdades = NULL;
    co->sublistas = g_hash_table_new(g_str_hash, g_str_equal);
    //printf("createconj = %s \n", name);
    return co;
}

TodosConjuntos createTodosConjuntos(char *titulo)
{

    TodosConjuntos tc = malloc(sizeof(struct TodosConjuntos));
    tc->todos_conjuntos = g_hash_table_new(g_str_hash, g_str_equal);
    tc->tituloS = titulo;
    //printf("TodosConjuntos = %s \n", titulo);
    return tc;
}
void insereSubConjunto(TodosConjuntos tc, Conjunto co1, Conjunto co2)
{
    //printf("entrei no insereSubconjunto \n");
    Conjunto conj = g_hash_table_lookup(tc->todos_conjuntos, g_strdup(co1->nome));
    if (conj == NULL)
    {
        //printf("conjunto principal ainda nao existe \n");

        GList *values = g_hash_table_get_values(tc->todos_conjuntos);
        Conjunto com = (Conjunto)values->data;
        while (values != NULL)
        {
            Conjunto com = (Conjunto)values->data;
            //printf(" \n TC = %s\n", (char *)com->nome);
            values = g_list_next(values);
        }
        return;
    }
    else
    {
        //printf("conjunto principal existe, a adicionar subconjunto \n");
        g_hash_table_insert(conj->sublistas, g_strdup(co2->nome), (gpointer)co2);
    }
    return;
}
void insereConjunto(TodosConjuntos tc, Conjunto co)
{
    //printf("entrei no insereconjunto \n");
    Conjunto conj = g_hash_table_lookup(tc->todos_conjuntos, g_strdup(co->nome));
    bool kk = g_hash_table_contains(tc->todos_conjuntos, (gpointer) & "owner");
    //printf("%d", kk);
    if (conj == NULL)
    {
        //printf("conjunto ainda nao existe \n");
        g_hash_table_insert(tc->todos_conjuntos, g_strdup(co->nome), (gpointer)co);

        GList *values = g_hash_table_get_values(tc->todos_conjuntos);
        Conjunto com = (Conjunto)values->data;
        while (values != NULL)
        {
            Conjunto com = (Conjunto)values->data;
            //printf(" \n TC = %s\n", (char *)com->nome);
            values = g_list_next(values);
        }
        return;
    }
    //printf("\nconjunto ja existe \n");
    GList *values = g_hash_table_get_values(tc->todos_conjuntos);
    Conjunto com = (Conjunto)values->data;
    while (values != NULL)
    {
        Conjunto com = (Conjunto)values->data;
        //printf("%s", (char *)com->nome);
        values = g_list_next(values);
    }
    return;
}
