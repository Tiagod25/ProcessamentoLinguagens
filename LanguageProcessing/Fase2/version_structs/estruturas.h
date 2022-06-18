#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#endif

typedef struct Conjunto *Conjunto;
typedef struct TodosConjuntos *TodosConjuntos;
typedef struct Igual *Igual;
Conjunto createconj(char *name);
TodosConjuntos createTodosConjuntos(char *titulo);
void insereConjunto(TodosConjuntos tc, Conjunto co);
void insereSubConjunto(TodosConjuntos tc, Conjunto co1, Conjunto co2);
Igual createIgualdade(char *variavel);
void inserirValores(TodosConjuntos tc, Conjunto conj, Igual ig, char *values);
void inserirValores_Sub(TodosConjuntos tc, Conjunto co1, Conjunto co2, Igual ig, char *values);
