#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#endif

typedef struct Comentario* Comentario;
typedef struct TodosComentarios* TodosComentarios;


Comentario createComentario(int id, char* nome, char* data, int replies, char* comentario);


TodosComentarios createTodosComentarios();

void insereComentario(TodosComentarios tc, Comentario co);

void imprimeComentarios(TodosComentarios tc);

void imprimeFormatoJson(char const *file, TodosComentarios tc);

void imprimeFormatoJson_ToCompile(char const *file, TodosComentarios tc);