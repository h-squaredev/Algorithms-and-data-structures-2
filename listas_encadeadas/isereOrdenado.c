// Aluno: Humberto Henrique Castro Rocha
// RGA: 202512722003

#include <stdio.h>
#include <stdlib.h>

typedef struct lista{
	int info;
	struct lista* prox;s
}LISTA;

LISTA* cria_no(int v){
	LISTA* novo = (LISTA*) malloc(sizeof(LISTA));
	novo->info = v;
	novo->prox = NULL;
	return novo;
}


LISTA* insere_ordenado(LISTA* li, int i){
    LISTA* ant = NULL; // ponteiro para o elemento anterior
    LISTA* p = li; // ponteiro para percorrer a lista
    
    // Percorre a lista para encontrar o local de insercao
    while (p != NULL && p->info < i) {
        ant = p;
        p = p->prox;
    }
    
    // Chama a funcao para criar um novo no com o valor a ser inserido
    LISTA* novo = cria_no(i);
    
    // Encadeamento do novo elemento
    if (ant == NULL) { 
        novo->prox = li;
        li = novo;
    } else { 
        novo->prox = p;
        ant->prox = novo;
    }
    
    return li;
}