//DFS (profundidade)
//assistindo a aula e implementando a estrutura
#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

#define maxV 1024
#define BRANCO 0
#define CINZA 1
#define PRETO 2 

static int dist[maxV], cor[maxV], pred[maxV];


int fim;

typedef int bool;
typedef int TIPOPESO;

//Estrututa que vai guardando a lista de adjacencias
typedef struct adjacencia{
    int vertice;
    TIPOPESO peso;
    struct adjacencia *prox;
}ADJACENCIA;

typedef struct vertice{
    ADJACENCIA *cab;
}VERTICE;

typedef struct grafo{
    int vertices;
    int arestas;
    VERTICE *adj;
}GRAFO;

//criando pilha
typedef struct item{
    int u;
    ADJACENCIA *p;
}ITEM;

//funções para controlar pilha
ITEM *pilha;

void pilhaInit(int maxN){
    pilha = (ITEM *) malloc(maxN*sizeof(ITEM));
    fim = 0;
}

int pilhaEmpty(){
    return fim == 0;
}

ITEM desempilhar(){
    return pilha[--fim];
}

void empilhar(int item, ADJACENCIA *px){
    pilha[fim].u = item;
    pilha[fim].p = px;
}

//limpar a memoria
void pilhaFREE(){
    free(pilha);
}

// Criando um grafo
GRAFO *criarGrafo(int v){
    GRAFO *g =  (GRAFO *) malloc(sizeof(GRAFO));

    g->vertices = v;
    g->arestas = 0;
    g->adj = (VERTICE *) malloc(v*sizeof(VERTICE)); //criando vetor auxiliar

    for (int i=0; i < v; i++){
        g->adj[i].cab = NULL;
    } 
    return g;
}

ADJACENCIA *criarAdj (int v, int peso){
    ADJACENCIA *temp = (ADJACENCIA *) malloc(sizeof(ADJACENCIA));
    temp->vertice = v;
    temp->peso = peso;
    temp->prox =NULL;
    return temp;
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){
    if(!gr)
        return (false);
    if((vf < 0) || (vf >= gr->vertices)) 
        return (false);
    if((vi < 0) || (vi >= gr->vertices)) 
        return (false);

    ADJACENCIA *novo = criarAdj(vf, p);
    gr-> adj[vi].cab = novo;

    ADJACENCIA *novo2 = criarAdj(vi, p);
    novo2->prox = gr->adj[vf].cab;
    gr->adj[vf].cab = novo2;

    gr->arestas++;
}

//Busca em Grafos em profundidade
void DFS(GRAFO *G, int raiz){
    int u, v;
    pilhaInit(G->vertices);

    dist[raiz] = 0;
    cor[raiz] = CINZA;
    pred[raiz] = -1;
    
    for (int v1 = 0; v1 <= G->vertices; v1++){
        dist[v1] = maxV;
        cor[v1] = BRANCO;
        pred[v1] = -1;
    }
    empilhar(raiz, G->adj[raiz].cab);
    
    while(!pilhaEmpty()){
        ITEM Q = desempilhar(raiz);
        ADJACENCIA *p = Q.p;
        
        while((p != NULL) && (cor[p->vertice] != BRANCO)){
            p = p->prox;
        }
        if (p != NULL){
            empilhar(u, p->prox);
            
            v = p->vertice;
            dist[v] = dist[u] +1;
            cor[v] = u;
            pred[v] = CINZA;
            empilhar(v, G->adj[v].cab);
        }else{
            cor[u] = PRETO;
        }
    }
}

void imprime (GRAFO *gr){
    printf("vertices %d. Arestas: %d \n", gr->vertices, gr->arestas);

    for (int i = 0; i < gr->vertices; i++){
        printf("v %d", i);
        ADJACENCIA *ad = gr->adj[i].cab;
        while(ad){
            printf(" adj[%d,%d]", ad->vertice, ad->peso);
            ad = ad->prox; //jamais esquecer após o print, tive que voltar o video!
        }
        printf("\n");
    }
}

void imprimeTree(GRAFO *gr){
    for(int v=0; v < gr->vertices; v++){
        printf("(%d,%d)\n", pred[v], v);
    }
}

int main(){
    GRAFO *gr = criarGrafo(12);

    criaAresta(gr,0,2,1);
    criaAresta(gr,1,3,1);
    criaAresta(gr,0,1,1);
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
    DFS(gr, 0);
    imprimeTree(gr);

    return 0; 
}
