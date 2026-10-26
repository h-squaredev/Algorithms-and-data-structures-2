// Aluno: Humberto Henrique Castro Rocha

#include <stdio.h>
#include <stdlib.h>

typedef struct lista{
	int info;
	struct lista* prox;
}LISTA;

LISTA* cria_no(int v){
	LISTA* novo = (LISTA*) malloc(sizeof(LISTA));
	novo->info = v;
	novo->prox = NULL;
	return novo;
}

LISTA* insere_fim(LISTA* li, int i){
    LISTA* novo = cria_no(i);

    // Se a lista estiver vazia, o novo no se torna o primeiro elemento
    if (li == NULL) {
        return novo;
    }

    LISTA* p = li; // ponteiro para percorrer a lista
    while (p->prox != NULL) { // percorre até o último elemento
        p = p->prox;
    }

    p->prox = novo; // encadeia o novo elemento no final da lista
    
    return li;
}
