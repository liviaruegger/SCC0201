/**
 * @author Ana Lívia Ruegger Saldanha (N.USP 8586691)
 * @brief  SCC0201 — Introdução à Ciência de Computação II
 *         Trabalho 03: Simulador de Escalonador de Processos
 *         
 * @date   2021-12-02
 * 
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct no no_t;

struct no
{
    int identificador;
    int t_inicial;
    int t_execucao;
    int prioridade;
    no_t *proximo;
};

typedef struct lista
{
    no_t *cabeca;
    no_t *p_exec;
}   lista_t;

int **ler_matriz_de_processos(int *n_linhas);
void ordenar_por_t_inicial(int **matriz, int n_processos);
void desalocar_matriz(int **matriz, int n_processos);

void simular(int **matriz, int n_processos);
lista_t *criar_lista_de_processos();
void inserir_processo(lista_t *lista, int *info);
void remover_processo(lista_t *lista);
void desalocar_lista(lista_t *lista);

int main()
{
    int n_processos;
    int **matriz_de_processos = ler_matriz_de_processos(&n_processos);
    ordenar_por_t_inicial(matriz_de_processos, n_processos);
    
    simular(matriz_de_processos, n_processos);

    desalocar_matriz(matriz_de_processos, n_processos);

    return 0;
}

/**
 * @brief Lê da entrada padrão as informações de processos a serem executados pelo
 * simulador e as armazena em uma matriz de inteiros alocada dinamicamente; cada linha
 * da matriz corresponde a um processo. 
 * 
 * @param n_linhas endereço onde será armazenado o número de processos (linhas da matriz);
 * @return matriz de inteiros contendo as informações dos processos (int **). 
 */
int **ler_matriz_de_processos(int *n_linhas)
{
    int **matriz = NULL;
    int n_processos = 0;

    while (!feof(stdin))
    {
        matriz = (int **) realloc(matriz, (n_processos + 1) * sizeof(int *));
        
        int *novo_processo = (int *) malloc(4 * sizeof(int));

        // Ler identificador.
        int identificador;
        scanf("%d", &identificador);

        // Alterar caso já exista um processo com o mesmo identificador.
        for (int i = 0; i < n_processos; i++)
        {
            if (identificador == matriz[i][0])
                identificador++;
        }
        
        // Armazenar todas as informações do processo na matriz.
        novo_processo[0] = identificador;
        for (int i = 1; i < 4; i++)
            scanf("%d", &novo_processo[i]);
        
        matriz[n_processos] = novo_processo;
        n_processos++;
    }

    *n_linhas = n_processos;
    return matriz;
}

/**
 * @brief Ordena as linhas da matriz (cada uma referente a um processo) de acordo com o
 * tempo em que o processo é reconhecido pelo simulador (inteiro na posição 1 do vetor/
 * linha da matriz). A ordenação é feita utilizando o algoritmo insertion sort.
 * 
 * @param matriz matriz de inteiros contendo as informações dos processos;
 * @param n_processos número de processos (linhas da matriz).
 */
void ordenar_por_t_inicial(int **matriz, int n_processos)
{
    int j;
    for (j = 1; j < n_processos; j++)
    {
        int *chave = matriz[j];
        int i = j - 1;
        while (i >= 0 && matriz[i][1] > chave[1])
        {
            matriz[i + 1] = matriz[i];
            i--;
        }
        matriz[i + 1] = chave;
    }
}

/**
 * @brief Desaloca a memória ocupada por uma matriz de inteiros alocada dinamicamente.
 * 
 * @param matriz matriz de inteiros alocada dinamicamente;
 * @param n_linhas numero de linhas da matriz.
 */
void desalocar_matriz(int **matriz, int n_linhas)
{
    for (int i = 0; i < n_linhas; i++)
        free(matriz[i]);

    free(matriz);
}

/**
 * @brief A partir de uma matriz de inteiros contendo informações acerca de processos a
 * serem executados, simula um escalonador de processos, imprimindo na saída padrão o
 * código de cada processo seguido do tempo (quantum) no qual ele foi finalizado.
 * 
 * @param matriz matriz de inteiros contendo as informações dos processos;
 * @param n_processos número de processos (linhas da matriz).
 */
void simular(int **matriz, int n_processos)
{
    lista_t *lista = criar_lista_de_processos();

    int t = 1; // Contador de quantum.
    int i = 0; // Índice do próximo processo a ser inserido.

    int em_execucao = n_processos; // Contador de processos ainda não finalizados.
    
    while (em_execucao)
    {
        // Inserir processos reconhecidos.
        while (i < n_processos && matriz[i][1] == t)
        {
            inserir_processo(lista, matriz[i]);
            i++;
        }
        
        // Executar um processo (1 quantum).
        lista->p_exec->t_execucao--;

        if (lista->p_exec->t_execucao == 0)
        {
            printf("%d %d\r\n", lista->p_exec->identificador, t);
            remover_processo(lista);
            em_execucao--;
        }
        else
        {
            if (lista->p_exec->proximo == NULL) lista->p_exec = lista->cabeca;
            else lista->p_exec = lista->p_exec->proximo;
        }

        t++;
    }

    desalocar_lista(lista); 
}

/**
 * @brief Cria uma lista de processos vazia (uma struct contendo dois ponteiros: cabeça
 * da lista e próximo processo a ser executado).
 * 
 * @return ponteiro para a lista criada (lista_t *). 
 */
lista_t *criar_lista_de_processos()
{
    lista_t *lista = (lista_t *) malloc(sizeof(lista_t));

    lista->cabeca = NULL;
    lista->p_exec = NULL;

    return lista;
}

/**
 * @brief A partir de um vetor de inteiros contendo informações acerca de um processo,
 * faz uma inserção ordenada desse processo como um nó em uma lista encadeada. Caso a
 * prioridade do novo processo seja maior que a do próximo a ser executado, o ponteiro
 * de execução é redefinido.
 * 
 * @param lista lista encadeada de processos em execução;
 * @param info vetor de inteiros contendo as informações acerca do processo.
 */
void inserir_processo(lista_t *lista, int *info)
{
    // Criar nó.
    no_t *novo_processo = (no_t *) malloc(sizeof(no_t));
    novo_processo->identificador = info[0];
    novo_processo->t_inicial = info[1];
    novo_processo->t_execucao = info[2];
    novo_processo->prioridade = info[3];
    novo_processo->proximo = NULL;

    //Inserir processo na posição correta da lista encadeada.
    if (lista->cabeca == NULL) // Lista está vazia.
    {
        lista->cabeca = novo_processo;
        lista->p_exec = novo_processo;
    }
    else // Lista já contém processos.
    {
        no_t *p = lista->cabeca;
        no_t *anterior = NULL;
        
        // Ordenar por prioridade.
        while (p != NULL && novo_processo->prioridade < p->prioridade)
        {
            anterior = p;
            p = p->proximo;
        }

        // Na mesma prioridade, a ordenação é feita pelo indentificador.
        while (p != NULL && novo_processo->prioridade == p->prioridade
        && novo_processo->identificador > p->identificador)
        {
            anterior = p;
            p = p->proximo;
        }

        // Inserir na posição correta.
        novo_processo->proximo = p;
        if (anterior == NULL) lista->cabeca = novo_processo;
        else anterior->proximo = novo_processo;

        // Corrigir ponteiro de execução se necessário.
        if (novo_processo->prioridade > lista->p_exec->prioridade
        || (novo_processo->prioridade == lista->p_exec->prioridade
        && novo_processo->identificador < lista->p_exec->identificador))
            lista->p_exec = novo_processo;
    }
}

/**
 * @brief Remove da lista encadeada o processo que está no ponteiro de execução.
 * 
 * @param lista lista encadeada de processos em execução.
 */
void remover_processo(lista_t *lista)
{
    no_t *aux = lista->p_exec;

    if (aux == lista->cabeca) // Remover o primeiro nó.
    {
        lista->p_exec = aux->proximo;
        lista->cabeca = aux->proximo;
    }
    else // Remover nó do meio ou do fim.
    {
        // Redefine o ponteiro de execução para o próximo nó da lista.
        if (aux->proximo == NULL) lista->p_exec = lista->cabeca;
        else lista->p_exec = aux->proximo;

        // Encontrar nó e remover.
        no_t *atual = lista->cabeca;
        while (atual != NULL && atual->proximo != aux)
        {
            atual = atual->proximo;
        }
        atual->proximo = aux->proximo;
    }
   
    free(aux);
}

/**
 * @brief Desaloca a memória utilizada para armazenar a lista encadeada de processos.
 * 
 * @param lista lista encadeada de processos em execução.
 */
void desalocar_lista(lista_t *lista)
{
    if (lista != NULL && lista->cabeca != NULL)
    {
        no_t *p = lista->cabeca;
        while (p != NULL)
        {
            lista->cabeca = p->proximo;
            free(p);
            p = lista->cabeca;
        }
    }
    
    free(lista);
}