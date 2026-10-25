//Aluno: 202512722003
//BFS (largura)
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
#define maxV 1024
#define BRANCO 0
#define CINZA 1
#define PRETO 2 

static int dist[maxV], cor[maxV], pred[maxV];

int *fila;
int inicio, fim;

typedef int bool;
typedef int TIPOPESO;

typedef struct adjacencia{
    int vertice;
    TIPOPESO peso;
    struct adjacencia *prox;
} ADJACENCIA;

typedef struct vertice{
    ADJACENCIA *cab;
} VERTICE;

typedef struct grafo{
    int vertices;
    int arestas;
    VERTICE *adj;
} GRAFO;

// Criando um grafo
GRAFO *criarGrafo(int v){
    GRAFO *g = (GRAFO *) malloc(sizeof(GRAFO));

    g->vertices = v;
    g->arestas = 0;
    g->adj = (VERTICE *) malloc(v * sizeof(VERTICE));

    for (int i = 0; i < v; i++){
        g->adj[i].cab = NULL;
    } 
    return g;
}

ADJACENCIA *criarAdj (int v, int peso){
    ADJACENCIA *temp = (ADJACENCIA *) malloc(sizeof(ADJACENCIA));
    temp->vertice = v;
    temp->peso = peso;
    temp->prox = NULL;
    return temp;
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){
    if(!gr) return (false);
    if((vf < 0) || (vf >= gr->vertices)) return (false);
    if((vi < 0) || (vi >= gr->vertices)) return (false);

    ADJACENCIA *novo = criarAdj(vf, p);
    novo->prox = gr->adj[vi].cab; // CORREÇÃO: Previne que arestas antigas sejam perdidas
    gr->adj[vi].cab = novo;

    ADJACENCIA *novo2 = criarAdj(vi, p);
    novo2->prox =  gr->adj[vf].cab;
    gr->adj[vf].cab = novo2;

    gr->arestas++;
    return true; // CORREÇÃO: Retorno explícito adicionado
}

void imprime (GRAFO *gr){
    printf("Vertices: %d. Arestas: %d \n", gr->vertices, gr->arestas);

    for (int i = 0; i < gr->vertices; i++){
        printf("v %d", i);
        ADJACENCIA *ad = gr->adj[i].cab;
        while(ad){
            printf(" -> adj[%d, peso %d]", ad->vertice, ad->peso);
            ad = ad->prox; // CORREÇÃO: Impede loop infinito
        }
        printf("\n");
    }
}

//Iniciando uma fila 
void QUEUEinit(int maxN){
    fila = (int *) malloc(maxN * sizeof(int));
    inicio = 0;
    fim = 0;
}

//para fila estar vazia, inicio e fim devem tbm estar vazios 
int QUEUEempty(){ 
    return inicio == fim;
}

//Quando insere na fila, fim inclementa +1
void QUEUEput(int item){
    fila[fim++] = item;
}

//Quando exclui um elemento da fila, inicio inclementa +1
int QUEUEget(){ 
    return fila[inicio++]; 
}

void BFS(GRAFO *gr, int raiz){
    int u;
    QUEUEinit(gr->vertices);
    
    //Incrementando os vetores auxiliares
    for(int v = 0; v < gr->vertices; v++){
        dist[v] = maxV;
        cor[v] = BRANCO; //Quando o vertice e seus vizinhos nao foram visitados
        pred[v] = -1;
    }
    
    dist[raiz] = 0;
    cor[raiz] = CINZA; //Quando o vertice foi visitado mas seus vizinhos nao
    pred[raiz] = -1;

    QUEUEput(raiz);

    while (!QUEUEempty()){ 
        u = QUEUEget();
        ADJACENCIA *v = gr->adj[u].cab;
        
        //para percorrer as adjacencias
        while(v){
            if (cor[v->vertice] == BRANCO){
                dist[v->vertice] = dist[u] + 1;
                pred[v->vertice] = u;
                cor[v->vertice] = CINZA;

                QUEUEput(v->vertice);
            }
            v = v->prox; //proximo adjacente
        }
        cor[u] = PRETO;
    }
}

void imprimeTree(GRAFO *gr){ // CORREÇÃO: Nome da função de imprime para imprimeTree
    printf("\nLista de Predecessores (BFS):\n");
    for(int v = 0; v < gr->vertices; v++){
        printf("Vertice %d -> Predecessor: %d\n", v, pred[v]);
    }
}

int main(){
    GRAFO *gr = criarGrafo(12);

    criaAresta(gr, 0, 2, 1);
    criaAresta(gr, 1, 3, 1);
    criaAresta(gr, 0, 1, 1);
    criaAresta(gr, 2, 3, 1);
    criaAresta(gr, 2, 5, 1);
    criaAresta(gr, 2, 4, 1);
    criaAresta(gr, 4, 5, 1);
    criaAresta(gr, 5, 6, 1);
    criaAresta(gr, 3, 7, 1);
    criaAresta(gr, 3, 8, 1);
    criaAresta(gr, 3, 9, 1);
    criaAresta(gr, 8, 9, 1);
    criaAresta(gr, 7, 8, 1);
    criaAresta(gr, 7, 9, 1);
    criaAresta(gr, 10, 11, 1);
    
    imprime(gr);
    BFS(gr, 0);
    imprimeTree(gr);

    return 0; 
}