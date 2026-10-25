#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define true 1
#define false 0
#define INT_MAX 32000 //"infinito"
typedef int bool;
typedef int TIPOPESO;

typedef struct adjacencia{
    int vertice;
    TIPOPESO peso;
    struct adjacencia *prox; //ad
} ADJACENCIA;

typedef struct vertice{
    int grau;
    /* Dados armazenados vao aqui */
    ADJACENCIA *cab;
} VERTICE;

typedef struct grafo {
    int vertices;
    int arestas;
    VERTICE *adj;
} GRAFO;

/* Criando um grafo */
GRAFO *criarGrafo(int v){
    GRAFO *g = (GRAFO *) malloc(sizeof(GRAFO));
    
    g->vertices     = v;
    g->arestas      = 0;
    g->adj          = (VERTICE *) malloc(v*sizeof(VERTICE));
    int i;
    
    for (i=0; i<v; i++){
        g->adj[i].cab = NULL;
        g->adj[i].grau=0;
    }
    
    return g;
}

ADJACENCIA *criaAdj(int v,int peso){
    ADJACENCIA *temp = (ADJACENCIA *) malloc(sizeof(ADJACENCIA));
    temp->vertice   = v;
    temp->peso      = peso;
    temp->prox      = NULL;
    return (temp);
}

ADJACENCIA *inseriListAdj(ADJACENCIA *adj, int vi, int vf, TIPOPESO p)
{

    ADJACENCIA *ant = adj;
    ADJACENCIA *atual = adj;
    int control = 0;

    ADJACENCIA *novo = criaAdj(vf, p);

    while (atual != NULL && atual->vertice < vf)
    {
        ant = atual;
        atual = atual->prox;
        control = 1;
    }
    if (ant == NULL && control == 0)
    {
        novo->prox = atual;
        
        return novo;
    }
    else if (control == 0)
    {
        novo->prox = atual;
        return novo;
    }else{
        novo->prox = atual;
        ant->prox = novo;
    }

    return adj;
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){
    if (!gr)
        return(false);
    if((vf<0) || (vf >= gr->vertices))
        return(false);
    if((vi<0) || (vi >= gr->vertices))
        return(false);
    
    gr->adj[vi].cab = inseriListAdj(gr->adj[vi].cab, vi, vf, p);
    gr->adj[vf].cab = inseriListAdj(gr->adj[vf].cab, vf, vi, p);
    
    gr->arestas++;
    return (true);
}



void imprime(GRAFO *gr){
    printf("Vertices: %d. Arestas: %d, \n", gr->vertices,gr->arestas);
    
    int i;
    for(i=0;i<gr->vertices; i++){
        printf("v%d: ",i);
        ADJACENCIA *ad = gr->adj[i].cab;
        while(ad){
            printf("v%d(%d) ", ad->vertice,ad->peso);
            ad = ad->prox;
        }
        
        printf("\n");
    }
}

// ----------- PRIM ---------------------------
void agm_prim(GRAFO *gr, int orig, int *pai){
    int n = gr->vertices;
    int *chave = malloc(n *sizeof(int));
    bool *na_arvore = malloc(n *sizeof(bool));

    for (int i = 0; i < n; i++){
        chave[i] = INT_MAX; //menor aresta = infinito inicialmnete
        pai[i] = -1; // representando ausencia de vertice
        na_arvore[i] = false; //todos os vetices começam na fila
    }
    chave[orig] = 0; // Como a raiz eh a primeira a sair, seu custo 0

    for (int count = 0; count < n; count++){
        if (count % 10000 == 0){
            printf("Progresso: %d / %d vertices \n", count, n);
        }
        int minimo = INT_MAX;
        int u = -1;

        for(int i = 0; i < n; i++){
            if(!na_arvore[i] && chave[i] < minimo){
                minimo = chave[i];
                u = i;
        }
    }
    if (u == -1)break; //isso se o grafo for desconexo
    na_arvore[u] = true; // agora u pertence a mts

    for (ADJACENCIA *ad = gr->adj[u].cab; ad; ad = ad->prox){
//AQUI EU PAREI O CODETRACK SEM QUERER AO CLICAR EM "ERROS" que fica ao lado
//feedback: Coloque se o usuario tem certeza em querer parar a gravacao 
        int vizinho = ad->vertice;
        int peso = ad->peso;

        //verificacao de caso vizinho na arvore e se o seu peso eh menor que o custo atual
        if (!na_arvore[vizinho] && peso < chave[vizinho]){
            chave[vizinho] = peso; //atualiza para menor valor
            pai [vizinho] = u; //muda o predecessor para u
        }
    }
}
free(chave);
free(na_arvore);
}

int main(){
    const char *instancias[] = {"data/USA-road-d.CAL.gr", "data/USA-road-d.NY.gr"};
    
    FILE *saida = fopen("resultado_prim.txt", "w");

    fprintf(saida, "RESULTADOS DO ALGORITMO DE PRIM \n\n");

    for(int i = 0; i < 2; i++){
        printf("\n Processando: %s", instancias[i]);

        //Carregar grafo
        FILE *f = fopen(instancias[i], "r");
        if (!f){
            printf("\n Erro ao abrir %s\n", instancias[i]);
            continue;
        }

        GRAFO *gr = NULL;
        char linha [256];
        while(fgets (linha, sizeof(linha), f)){
            if(linha[0] == 'c') continue; 
            if(linha[0] == 'p'){
                int V, E;
                sscanf(linha, "p sp %d %d", &V, &E);
                gr = criarGrafo(V);
            }else if (linha[0] == 'a' && gr) {
                int u, v, w;
                sscanf(linha, "a %d %d %d", &u, &v, &w);
                u--;
                v--;
                criaAresta(gr, u, v, w);
            }
        }
        fclose(f);
        if (!gr){
            printf("\nErro ao carregar %s\n", instancias[i]);
            continue;
        }
        int *pai = malloc(gr->vertices *sizeof(int));

        clock_t ini = clock();
        agm_prim (gr, 0, pai);
        double tempo = (double)(clock() - ini) /CLOCKS_PER_SEC;

        //calcular o custo de tempo
        long long custo = 0;
        for (int v = 1; v < gr->vertices; v++){
            if(pai[v] < 0) continue;
            for (ADJACENCIA *a = gr->adj[pai[v]].cab; a; a = a->prox)
                if(a->vertice == v){
                    custo += a->peso;
                    break;
                }
        }

        fprintf(saida, "Instancias: %s\n Custo Total: %lld\n Tempo: %f s\n--\n", instancias[i], custo, tempo);
        puts ("CONCLUIDO");
        
        //liberar grafo:
        free(pai);
        for(int v = 0; v < gr->vertices; v++){
            ADJACENCIA *a = gr->adj[v].cab;
            while(a){
                ADJACENCIA *t = a;
                a = a->prox; 
                free(t);
            }
        }
        free(gr->adj);
        free(gr);
    }
    fclose(saida);
    puts ("\n instancias Processadas! Verifique o .txt");

    return 0;
}