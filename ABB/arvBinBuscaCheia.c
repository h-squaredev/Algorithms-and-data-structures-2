#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    int valor;
    struct no* esquerda;
    struct no* direita;
} No;

No* novoNo(int valor) {
    No* raiz = (No*)malloc(sizeof(No));
    raiz->valor = valor;
    raiz->esquerda = NULL;
    raiz->direita = NULL;
    return raiz;
}

// Função de inserção
No* inserir(No* raiz, int valor) {
    if (raiz == NULL){
        return novoNo(valor);
    }else if(valor < (raiz->valor)){
        raiz->esquerda = inserir(raiz->esquerda, valor);
    }else if (valor > (raiz->valor)){
        raiz->direita = inserir(raiz->direita, valor);
    }
    return raiz;
}

// retorna 1 se cheia, 0 se não
int arvore_cheia(No* raiz){
    // 1. Árvore vazia
    if (raiz == NULL) return 1;

    // 2. Nó folha (0 filhos)
    if (raiz->esquerda == NULL && raiz->direita == NULL) return 1;

    // 3. Nó com 2 filhos (Verifica recursivamente os dois lados)
    if (raiz->esquerda != NULL && raiz->direita != NULL) {
        return arvore_cheia(raiz->esquerda) && arvore_cheia(raiz->direita);
    }

    // 4. Se chegou aqui, tem apenas 1 filho
    return 0;
}
