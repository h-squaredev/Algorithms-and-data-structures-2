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

// Função de busca
int buscar(No* raiz, int valor) {
    while(raiz != NULL){
        if(raiz->valor == valor)
            return 1;


        if(valor < raiz->valor)
            raiz = raiz->esquerda;
        else
            raiz = raiz->direita;
    }
    printf("ERRO! Nao ha esse valor na arvore!");
    return 0;
}