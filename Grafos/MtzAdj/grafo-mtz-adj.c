#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
typedef int bool;
typedef int TIPOPESO;


int inicio,fim;

typedef struct adjacencia{
    int vertice;
    TIPOPESO peso;
    struct adjacencia *prox;
} ADJACENCIA;

typedef struct vertice{
    /* Dados armazenados vao aqui */
    ADJACENCIA *cab;
} VERTICE;

typedef struct grafo {
    int vertices;
    int arestas;
    VERTICE *adj;
} GRAFO;

/*
 * criarGrafo: aloca e inicializa um novo grafo com um numero fixo de vertices,
 * sem nenhuma aresta.
 *
 * Entrada:
 *   v - numero de vertices do grafo
 *
 * Saida:
 *   ponteiro para o GRAFO alocado, com gr->vertices = v, gr->arestas = 0
 *   e todas as listas de adjacencia vazias (cab = NULL)
 */
GRAFO *criarGrafo(int v){
    GRAFO *gr = (GRAFO *)malloc(sizeof(GRAFO));
    //ponteiro para o GRAFO alocado 
    gr->vertices = v;
    gr->arestas = 0;
    //aloca array de vertices
    gr->adj = (VERTICE *)malloc(v * sizeof(VERTICE)); 

    //inicializa todas as listas de adjacencia vazias
    for (int i = 0; i<v; i++){
        gr->adj[i].cab = NULL;
    }

    return gr;
}

/*
 * criaAdj: cria e inicializa um novo no de adjacencia (aresta) para ser
 * inserido em uma lista de adjacencia.
 *
 * Entrada:
 *   v    - indice do vertice de destino apontado por esta adjacencia
 *   peso - peso associado a esta adjacencia
 *
 * Saida:
 *   ponteiro para o novo no ADJACENCIA alocado, com campo prox iniciado
 *   como NULL (o no ainda nao esta ligado a nenhuma lista)
 */
ADJACENCIA *criaAdj(int v,int peso){
    ADJACENCIA *novaAresta = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));

    novaAresta->vertice = v;
    novaAresta->peso = peso;
    novaAresta->prox = NULL;

    return novaAresta;

}

/*
 * criaAresta: insere uma aresta ponderada entre dois vertices de um grafo nao direcionado.
 *
 * Entrada:
 *   gr - ponteiro para o grafo onde a aresta sera inserida
 *   vi - indice do vertice de origem (0 a gr->vertices-1)
 *   vf - indice do vertice de destino (0 a gr->vertices-1)
 *   p  - peso da aresta
 *
 * Saida:
 *   true  - aresta criada com sucesso
 *   false - gr eh NULL ou algum dos indices (vi, vf) esta fora do intervalo valido
 *
 * Efeito: como o grafo eh nao direcionado, adiciona vf na lista de adjacencia
 * de vi e vi na lista de adjacencia de vf (cada insercao no inicio da lista),
 * e incrementa o contador de arestas do grafo.
 */
bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){
    //validacao
    if (gr != NULL && vi >= 0 && vi < gr->vertices && vf >= 0 && vf < gr->vertices){
        //chama a afuncao de criar adjacencia para gerar novo no
        ADJACENCIA *ida = criaAdj(vf, p);
        //Efeito:
        ida->prox = gr->adj[vi].cab;
        gr->adj[vi].cab = ida;
        
        //chama a afuncao de criar adjacencia para gerar novo no
        ADJACENCIA *volta = criaAdj(vi, p);
        volta->prox = gr->adj[vf].cab;
        gr->adj[vf].cab = volta;

        //contagem de arestas
        gr->arestas++;
        
        return true;

    }else{
        printf("Erro: Opa man, deu erro ai. Vertices estao fora do limite do grafo \n");
        return false;
    }
}


void imprime(GRAFO *gr){
    printf("Vertices: %d. Arestas: %d, \n", gr->vertices,gr->arestas);

    int i;
    for(i=0;i<gr->vertices; i++){
        printf("v%d: ",i);
        ADJACENCIA *ad = gr->adj[i].cab;
        while(ad){
            printf("u%d(%d) ", ad->vertice,ad->peso);
            ad = ad->prox;
        }

        printf("\n");
    }
}

int main(){

    GRAFO *gr = criarGrafo(12);
    criaAresta(gr,0,1,1);
    criaAresta(gr,0,2,1);
    criaAresta(gr,1,3,1);
    criaAresta(gr,2,3,1);
    criaAresta(gr,2,5,1);
    criaAresta(gr,2,4,1);
    criaAresta(gr,4,5,1);
    criaAresta(gr,5,6,1);
    criaAresta(gr,3,7,1);
    criaAresta(gr,3,8,1);
    criaAresta(gr,3,9,1);
    criaAresta(gr,8,9,1);
    criaAresta(gr,7,8,1);
    criaAresta(gr,7,9,1);
    criaAresta(gr,10,11,1);
    imprime(gr);


    return 0;
}