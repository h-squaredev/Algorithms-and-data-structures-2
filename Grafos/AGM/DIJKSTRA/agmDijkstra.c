#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define true 1
#define false 0
#define INT_MAX 32000
#define START 1
typedef int bool;
typedef int TIPOPESO;

typedef struct adjacencia{
    // registra o número do vertice
    int vertice;
    // registra o peso do vertices
    TIPOPESO peso;

    // proximo elemento da lista
    struct adjacencia *prox;
} ADJACENCIA;

typedef struct vertice{
    /* Dados armazenados vao aqui */
    ADJACENCIA *cab;
} VERTICE;

typedef struct grafo{
    // Guarda o número de vertives
    int vertices;
    // guarda o numero de arestas
    int arestas;
    // listas com os vértices adjacentes
    VERTICE *adj;
} GRAFO;

/* Criando um grafo */
GRAFO *criarGrafo(int v){

    // aloca memoria para uma strutura do tipo grafo
    GRAFO *g = (GRAFO *)malloc(sizeof(GRAFO));

    // inicializa o quantidade de vertives
    g->vertices = v;
    // inicializa a quantidade arestas
    g->arestas = 0;
    // aloca memoria para uma vetor da quantidade de vertices
    g->adj = (VERTICE *)malloc(v * sizeof(VERTICE));
    int i;

    // inicializar as posições do vertives com NULL
    for (i = 0; i < v; i++)
        g->adj[i].cab = NULL;

    return g;
}

ADJACENCIA *criaAdj(int v, int peso){
    // Aloca memoria para um vertices adjacente
    ADJACENCIA *temp = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));
    // define qual é o vertice
    temp->vertice = v;
    // atribui o peso
    temp->peso = peso;
    // define o proximo da lista como NULL
    temp->prox = NULL;
    return (temp);
}

ADJACENCIA *inseriListAdj(ADJACENCIA *adj, int vi, int vf, TIPOPESO p){

    ADJACENCIA *ant = adj;
    ADJACENCIA *atual = adj;
    int control = 0;

    ADJACENCIA *novo = criaAdj(vf, p);

    while (atual != NULL && atual->vertice < vf){
        ant = atual;
        atual = atual->prox;
        control = 1;
    }
    if (ant == NULL && control == 0){
        novo->prox = atual;
        return novo;
    }else if (control == 0){
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
        return (false);
    if ((vf < 0) || (vf >= gr->vertices))
        return (false);
    if ((vi < 0) || (vi >= gr->vertices))
        return (false);

    
    gr->adj[vi].cab = inseriListAdj(gr->adj[vi].cab, vi, vf, p);
    // gr->adj[vf].cab = inseriListAdj(gr->adj[vf].cab, vf, vi, p);
    gr->arestas++;
    return (true);
}

void imprime(GRAFO *gr){
    printf("Vertices: %d. Arestas: %d, \n", gr->vertices, gr->arestas);

    int i;
    for (i = START; i < gr->vertices; i++){
        printf("v%d: ", i);
        ADJACENCIA *ad = gr->adj[i].cab;
        while (ad){
            printf("v%d(%d) ", ad->vertice, ad->peso);
            ad = ad->prox;
        }
        
        printf("\n");
    }
}

// ----------- DIJKSTRA ---------------------------
//Funcao auxiliar para encontrar o vértice nao visitado com menor distancia atual
int minDistancia(int *dist, bool *sptSet, int V){
    int min = INT_MAX, min_index = -1;

    for (int v = START; v < V; v++){
        if (sptSet[v] == false && dist[v] <= min){
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}
int *dijkstra(GRAFO *g, int s, int *p){
    int V = g->vertices;

    //Vetor que guarda a menor distancia de 's' para outros vertices
    int *dist = (int *)malloc(V * sizeof(int));
    
    //Vetor de boooleanos para restrear os vertices ja visitados (SPTS)
    bool *sptSet = (bool *)malloc(V * sizeof(bool));
    
    //Inicializa todas as distancias como infinito (INT_MAX) e spts processados como false
    for (int i = 0; i < V; i++){
        dist [i] = INT_MAX;
        sptSet[i] = false;
        p[i] = -1; // -1 indicando nao ter predecessor ainda
    }
    //distancia inicializa como 0 pq sai dela mesmo
    dist [s] = 0;

    //Encotrar o caminho mais curto de todos os vertices 
    for (int count = START; count < V - 1; count++){
        //dos vertices nao processados, pega o de menor distancia
        int u = minDistancia(dist, sptSet, V);

        //Se o min_index for -1 ou a distancia for INT_MAX, os nós restantes estao inalcancaveis
        if (u == -1 || dist[u] == INT_MAX) break;

        //Marca o vértice escolhdo como ja visitado/processado
        sptSet[u] = true;

        //Atualiza a distancia dos vertices adjacentes do vertice escolhido
        ADJACENCIA *adj = g->adj[u].cab;
        while (adj != NULL){
            int v = adj->vertice;

            //Atualiza se nao foi processado, dist ate u nao for infinito, dist total + peso < dist atual registrada em dist[v]
            if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + adj->peso < dist[v]){
                dist[v] = dist[u] + adj->peso;
                p[v] = u; // salva 'u' como predecessor de 'v' para poder reconstruir a rota final
            }
            adj = adj->prox;
        }
    }

    free(sptSet);
    return dist; // retorna o vetor com menores dist encontradas
}

///-------------------OU (EXCLUSIVO) ----------------------------

// ----------- BELLMAN FORD ---------------------------

int *bellman_ford(GRAFO *g, int s, int *p){
    //implemente a função que realiza o algoritmo de bellman_ford
    //implemente outras funções auxiliares necessárias
}

void caminho(GRAFO* g, int venhoDe ,int vouPara, int *p){
    //Verificacao se o no destino eh valido ou alcancavel pelo menos
    if (vouPara < 0 || vouPara >= g->vertices || (p[vouPara] == -1 && venhoDe != vouPara)){
        printf("Não há rota alcançável de v%d para v%d\n", venhoDe, vouPara);
        return;
    }

    //Aloca um array temporario para armazenar o caminho de tras para frente
    int *rota = (int *) malloc(g->vertices * sizeof(int));
    int count = 0;
    int atual = vouPara;
    
    //faz o caminho reverso usando os predecessores
    while (atual != -1 && atual != venhoDe){
        rota[count++] == atual;
        atual = p[atual];
    }
    rota[count++] = venhoDe; //add o no de origem

    //imprime a rota na ordem correta (invertendo o array)
    printf("Rota de v%d para v%d: ", venhoDe, vouPara);
    for (int i = count -1; i >= 0; i--){
        printf("%d", rota[i]);
        if ("i > 0") printf(" -> ");
    }
    printf("\n");

    free(rota);
}


int main(){
    const char *instancias[] = {"data/USA-road-d.CAL.gr", "data/USA-road-d.NY.gr"};
    //scanf("%s","data/USA-road-d.CAL.gr");

    
    for(int i = 0; i < 2; i++){
        printf("\n Processando: %s\n", instancias[i]);
        
        GRAFO *gr = NULL;
        FILE *arq = fopen(instancias[i], "r");
        if (arq == NULL){
            puts("Deu ruim");
            return 0;
        }
        
        char entrada[50], c;
        int qtdnos, arcs, n1, n2, peso;
        int *p = NULL;
        
        puts("Carregando o grafo na memória...");
        
        while (fscanf(arq, "%c", &c) != EOF){ //Tirei o "cont < 100 && f" para nao limitar o data
            if (c == 'c'){
                fscanf(arq, "%[^\n]", entrada);
            }
            else if (c == 'p'){
                fscanf(arq, " sp %d %d", &qtdnos, &arcs);
                gr = criarGrafo(qtdnos + 1);
                puts("#vertices criados#...");
                
                // Cria o vetor para armazenar os predecessores
                p = (int *)malloc(gr->vertices * sizeof(int));
            }
            else if (c == 'a'){
                fscanf(arq, " %d %d %d", &n1, &n2, &peso);
                
                //printf("a-%d (%d,%d)[%d]\n", j++, n1, n2, peso); --- comentado para saida menos lenta
                criaAresta(gr, n1, n2, peso);
            }
        }
        fclose(arq);
        puts("Grafo carregados com sucesso #...");
        //imprime(gr); --- Como vai varrer o grafo inteiro, vai estourar memoria
        
        int find = 1; //Origem fixa
        
        //VARIAVEIS DE TEMPO
        clock_t inicio, fim;
        double tempo_processamento;
        
        // Medindo o TEMPO do DIJKSTRA
        inicio = clock();
        int *r = dijkstra(gr, find, p);
        fim = clock();
        
        tempo_processamento = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Tempo do Dijkstra: %f segundos\n", tempo_processamento);
        
        //Gerando ARQUIVO de SAIDA
        FILE *saida = fopen("resultado_DIJKSTRA.txt", "w");
        if (saida != NULL) {
            fprintf(saida," RESULTADO DO ALGORITMO DIJKSTRA \n");
            fprintf(saida,"Tempo de processamento: %f segundos\n\n", tempo_processamento);

            //Escolhendo 3 vertices distantes
            int destinos[3] = {1000, 50000, 100000};

            for (int i = 0; i < 3; i++){
                int dest = destinos[i];
                if (dest < gr->vertices){
                    fprintf(saida, "Distancia de v%d para v%d: %d\n", find, dest, r[dest]);
                }
            }
            fclose(saida);
            puts("Resultados salvos em 'resultados_DIJKSTRA.txt'!");
        }
        
        //int *r = bellman_ford(gr, find, p);
        
        
    }

    return 0;
}