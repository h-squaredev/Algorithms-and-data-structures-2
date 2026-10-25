//MATRIZ ADJACENTE DE UM GRAFO ORIENTADO

#include <stdio.h>
#include <stdlib.h>

typedef struct vizinho{
    int id_vizinho;
    struct vizinho *prox;
}TVizinho;

typedef struct grafo{
    int id_vertice;
    TVizinho *prim_vizinho;
    struct grafo *prox;
}TGrafo;

TGrafo *inicializa (){
    return NULL;
}

void imprime (TGrafo *g) {
    //verificamos se o grafo é diferente de NULL
    while (g != NULL){
        printf("Vertice %d", g->id_vertice);
        TVizinho *v = g->prim_vizinho;
        while (v != NULL){
            printf("->%d", v->id_vizinho);
            v = v->prox;
        }
        printf ("\n\n");
        g = g->prox;
    }
}

/*Funcao que libera da memoria as listas encadeadas que representam as ARESTAS
isto eh, o verice adjacentes */
void libera_vizinhos(TVizinho *v){
    while (v != NULL){
        TVizinho *temp = v;
        v = v->prox;
        free(temp);
    }
}

//funcao libera da memoria a lista encadeada que representa os VERTICES
void libera (TGrafo *g){
    while ((g != NULL)){
        libera_vizinhos(g->prim_vizinho);
        TGrafo *temp = g;
        g = g->prox;
        free(temp);
    }
}

TGrafo* busca_vertice(TGrafo* g, int x){
    while ((g != NULL) && (g->id_vertice != x)){
        g = g->prox;
    }
    return g;
}

//Funcao de INSERIR vertice no grafo
TGrafo *insere_vertice(TGrafo *g, int x){
    TGrafo *p = busca_vertice(g,x);

    if (p == NULL){
        p = (TGrafo*) malloc(sizeof(TGrafo));
        p->id_vertice = x;
        p->prox = g;
        p->prim_vizinho = NULL;
        return p;
    }
    return g;
}

//Funcao para inserir o sentido (ARESTAS)
void insere_um_sentido (TGrafo *g, int v1, int v2){
    TGrafo *p = busca_vertice(g, v1);
    if (p != NULL){ //verifica se o vertice existe
        TVizinho *novo = (TVizinho *) malloc(sizeof(TVizinho));
        novo->id_vizinho = v2;
        novo->prox = p->prim_vizinho; //"p->prim_vizinho" aponta para o primeiro elemento
        p->prim_vizinho = novo;
    }
}

TVizinho* busca_aresta(TGrafo *g, int v1, int v2){
    TGrafo *pv1 = busca_vertice(g, v1);
    TGrafo *pv2 = busca_vertice(g, v2);
    TVizinho *resp = NULL;
    //checa se ambos os vertices existem
    if ((pv1 != NULL) && (pv2 != NULL)){
        resp = pv1->prim_vizinho;
        while ((resp != NULL) && (resp->id_vizinho != v2)){
            resp = resp->prox;
        }
    }
    return resp;
}

void insere_aresta (TGrafo *g, int v1, int v2){
    TVizinho *v = busca_aresta(g, v1, v2);
    if (v == NULL){
        insere_um_sentido(g, v1, v2);
        //insere_um_sentido(g, v2, v1); //descomente esta linha para tornar o grafo nao orientado
    }
}

int main(){
    TGrafo* g;
    g = inicializa();

    g = insere_vertice(g, 1);
    g = insere_vertice(g, 2);
    g = insere_vertice(g, 3);
    g = insere_vertice(g, 4);

    printf("Grafo Inicial (Apenas os vertices): \n");
    imprime(g);

    insere_aresta(g, 1, 2);
    insere_aresta(g, 1, 3);
    insere_aresta(g, 1, 4);
    insere_aresta(g, 2, 3);

    printf("Grafo apos insercao das arestas: \n");
    imprime(g);

    libera(g); //libera a memoria alocada para o grafo
    
    return 0;
}