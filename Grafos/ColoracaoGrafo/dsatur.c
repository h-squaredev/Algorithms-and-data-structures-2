#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
typedef int bool;
typedef int TIPOPESO;


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
    GRAFO *g = (GRAFO *) malloc(sizeof(GRAFO));

    g->vertices     = v;
    g->arestas      = 0;
    g->adj          = (VERTICE *) malloc(v*sizeof(VERTICE));
    int i;

    for (i=0; i<v; i++)
        g->adj[i].cab = NULL;

    return g;
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
    ADJACENCIA *temp = (ADJACENCIA *) malloc(sizeof(ADJACENCIA));
    temp->vertice   = v;
    temp->peso      = peso;
    temp->prox      = NULL;
    return (temp);
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
    if (!gr)
        return(false);
    if((vf<0) || (vf >= gr->vertices))
        return(false);
    if((vi<0) || (vi >= gr->vertices))
        return(false);

    ADJACENCIA *novo = criaAdj(vf,p);

    novo->prox      = gr->adj[vi].cab;
    gr->adj[vi].cab = novo;

    ADJACENCIA *novo2 = criaAdj(vi,p);

    novo2->prox      = gr->adj[vf].cab;
    gr->adj[vf].cab = novo2;

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
            printf("u%d(%d) ", ad->vertice,ad->peso);
            ad = ad->prox;
        }

        printf("\n");
    }
}


/* ---------------------------------------------------------------------
 * Coloracao de grafos - heuristica DSATUR
 * (Brelaz, D. "New methods to color the vertices of a graph".
 *  Communications of the ACM, 22(4), 251-256, 1979)
 * --------------------------------------------------------------------- */


/*
 * colorirDSATUR: colore os vertices de gr aplicando a heuristica DSATUR de
 * Brelaz (1979):
 *   (1) ordena implicitamente os vertices por grau decrescente ao escolher
 *       o vertice inicial;
 *   (2) atribui a cor 1 ao vertice de maior grau;
 *   (3) enquanto houver vertice nao colorido, seleciona o de maior grau de
 *       saturacao (empate: maior grau no sub-grafo nao colorido);
 *   (4) atribui a esse vertice a menor cor disponivel;
 *   (5) repete ate colorir todos os vertices.
 *
 * Entrada:
 *   gr - ponteiro para o grafo a ser colorido
 *
 * Saida:
 *   vetor alocado dinamicamente com gr->vertices posicoes, onde a posicao i
 *   contem a cor (>= 1) atribuida ao vertice i. O chamador eh responsavel
 *   por liberar esse vetor (free) apos o uso.
 */
int *colorirDSATUR(GRAFO *gr){
    int n = gr->vertices;
    int *cor = (int *)malloc(n * sizeof(int));
    int *dsat = (int *)malloc(n * sizeof(int)); //dsat = Grau de Saturacao

    for (int i = 0; i < n; i++){
        cor[i] = 0; //0 = nao foi colorido ainda
        dsat[i] = 0;
    }

        int max_grau = -1;
        int v_inicial = -1;
        
        //ordenando os vertices do grafo em ordem decrescente
        for (int i= 0; i < n; i++){
            int grau_atual = 0;
            ADJACENCIA *adj = gr->adj[i].cab;
            while (adj != NULL){
                grau_atual++;
                adj = adj->prox;
            }
            if (grau_atual > max_grau){
                max_grau = grau_atual;
                v_inicial = i;
            }
        }
        //atribuir cor 1 para o vertice de maior grau
        cor[v_inicial] = 1;

        //loop que colore os vertices restantes do grafo
        for (int count = 1; count < n; count++){
            // Atualiza o "dsat" para vertices nao coloridos
            for(int i = 0; i < n; i++){
                if(cor[i] == 0){
                    int cores_adj[n +1];
                    for(int k; k<= n; k++){
                        cores_adj[k] = 0;
                    }
                    int saturacao = 0;
                    ADJACENCIA *adj = gr->adj[i].cab;
                    
                    while (adj != NULL){
                        int c = cor[adj->vertice];
                        if(c != 0 && cores_adj[c] == 0){
                            cores_adj[c] = 1;
                            saturacao++;
                        }
                        adj = adj->prox;
                    }
                    dsat[i] = saturacao;
                }
            }
            //selecione o proximo vertice de maior grau de saturacao
            int max_saturacao = -1;
            int max_grau_nao_colorido = -1;
            int u = -1;
    
            for (int i = 0; i < n; i++){
                if(cor[i] == 0){

                    //Calculo do grau de ligacao só com subgrafo nao colorido
                    int grau_nao_colorido = 0;
                    ADJACENCIA *adj = gr->adj[i].cab;
    
                    while(adj != NULL){
                        if (cor [adj->vertice] == 0){
                            grau_nao_colorido++;
                        }
                        adj = adj->prox;
                    }
                    if(dsat[i] > max_saturacao){
                        max_saturacao = dsat[i];
                        max_grau_nao_colorido = grau_nao_colorido;
                        u = i;
                    }else if(dsat[i] == max_saturacao){
                        //Regra de desempate pelo subgrafo nao colorido
                        if(grau_nao_colorido > max_grau_nao_colorido){
                            max_grau_nao_colorido = grau_nao_colorido;
                            u = i;
                        }
                    }
                }
            }
            // Atribuindo a cor de menor indice disponivel para o vertice selecionado
            int cores_usadas[n + 1];
            for (int k = 0; k <= n; k++){
                cores_usadas[k] = 0;
            }
                ADJACENCIA *adj = gr->adj[u].cab;
                while(adj != NULL){
                    if (cor[adj->vertice] != 0){
                        cores_usadas[cor[adj->vertice]] = 1;
                    }
                    adj = adj->prox;
                }
                int menor_cor_disponivel = 1;
                while(cores_usadas[menor_cor_disponivel] != 0){
                    menor_cor_disponivel++;
                }
                cor[u] = menor_cor_disponivel;
        }
        free(dsat);

        return cor; //array contendo cada cor do vertice
}

/*
 * imprimeCores: imprime a cor atribuida a cada vertice e o numero de cores
 * distintas utilizadas (numero cromatico obtido pela heuristica).
 *
 * Entrada:
 *   gr  - ponteiro para o grafo
 *   cor - vetor de cores retornado por colorirDSATUR
 */
void imprimeCores(GRAFO *gr, int *cor){
    int numCores = 0;
    int i;

    for(i=0; i<gr->vertices; i++){
        printf("v%d -> cor %d\n", i, cor[i]);

        if(cor[i] > numCores)
            numCores = cor[i];
    }

    printf("Numero de cores utilizadas (heuristica DSATUR): %d\n", numCores);
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

    printf("\n--- Coloracao DSATUR ---\n");
    int *cor = colorirDSATUR(gr);
    imprimeCores(gr, cor);
    free(cor);

    return 0;
}