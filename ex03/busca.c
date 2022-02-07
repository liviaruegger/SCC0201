/**
 * @file   busca.c
 * @author Ana Lívia Ruegger Saldanha (N.USP 8586691)
 * @brief  SCC0201 — Introdução à Ciência de Computação II
 *         Exercício 03: Busca Indexada com Lista Encadeada
 *         
 * @date   2021-12-01
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "busca.h"

struct no_indice
{
    char letra;
    no_t *inicio;
};

struct no_lista
{
    char *palavra;
    no_t *prox;
};

struct lista
{
    no_t *inicio;
};

/**
 * @brief Cria uma lista encadeada vazia.
 * 
 * @return ponteiro para a lista criada (lista_t *). 
 */
lista_t *criar_lista()
{
    lista_t *lista = (lista_t *) malloc(sizeof(lista_t));

    lista->inicio = NULL;

    return lista;
}

/**
 * @brief Desaloca toda a memória utilizada por uma lista encadeada.
 * 
 * @param lista ponteiro para a lista que deve ser desalocada.
 */
void desalocar_lista(lista_t *lista)
{
    if (lista != NULL)
    {
        no_t *temp = lista->inicio;
        while (temp != NULL)
        {
            lista->inicio = temp->prox;
            
            free(temp->palavra);
            free(temp);

            temp = lista->inicio;
        }

        free(lista);
    }
}

/**
 * @brief Insere uma palavra em uma lista encadeada de forma "semi" ordenada: a ordenação
 * é feita apenas considerando a primeira letra de cada palavra; se já existem palavras
 * iniciando com aquela letra, a nova palavra é inserida antes delas.
 * 
 * @param lista ponteiro para a lista na qual a nova palavra deve ser adicionada;
 * @param palavra string contendo a palavra a ser adicionada.
 */
void inserir(lista_t *lista, char *palavra)
{
    no_t *aux = lista->inicio;
    no_t *anterior = NULL;

    // Encontrar a posição correta:
    while (aux != NULL && aux->palavra[0] < palavra[0])
    {
        anterior = aux;
        aux = aux->prox;
    }

    // Criar novo nó:
    no_t *novo_no = (no_t *) malloc(sizeof(no_t));
    novo_no->palavra = palavra;

    if (anterior == NULL) // Inserir no início da lista.
    {
        novo_no->prox = lista->inicio;
        lista->inicio = novo_no;
    }
    else // Inserir no meio ou no fim da lista.
    {
        novo_no->prox = anterior->prox;
        anterior->prox = novo_no;
    }
}

/**
 * @brief Imprime as palavras contidas nos três primeiros nós de uma lista encadeada.
 * 
 * @param lista ponteiro para a lista contendo as palavras que devem ser impressas.
 */
void imprimir_tres_primeiras_palavras(lista_t *lista)
{
    no_t *p = lista->inicio;

    for (int i = 0; i < 3; i++)
    {
        printf("%s\r\n", p->palavra);
        p = p->prox;
    }
}

/**
 * @brief Busca uma palavra na lista passada como parâmetro utilizando um vetor de índices
 * do tamanho do alfabeto, no qual cada elemento contém uma letra diferente do alfabeto e
 * um ponteiro para o primeiro nó da lista cuja palavra se inicia com a mesma letra.
 * Imprime na saída padrão o número de nós percorridos na lista até encontrar a palavra
 * buscada ou uma mensagem mostrando que a palavra não está na lista.
 * 
 * @param lista ponteiro para a lista encadeada na qual a busca deve ser feita;
 * @param indice vetor de índices contendo as letras do alfabeto e respectivos ponteiros;
 * @param palavra palavra que deve ser buscada na lista.
 */
void buscar_palavra(lista_t *lista, indice_t *indice, char *palavra)
{
    char chave = palavra[0];
    no_t *p = NULL;

    // Encontrar a letra no índice.
    for (int i = 0; i < 26; i++)
    {
        if (indice[i].letra == chave)
        {
            p = indice[i].inicio;
            break;
        }
    }

    if (p == NULL) // Não há palavras iniciadas com a chave.
    {
        printf("Palavra nao existe na lista.\r\n");
    }
    else // Procurar palavra na lista.
    {
        int contador = 0;
        while (strcmp(p->palavra, palavra) != 0 && p->palavra[0] == chave)
        {
            p = p->prox;
            contador++;
        }

        if (strcmp(p->palavra, palavra) == 0) printf("%d\r\n", contador);
        else printf("Palavra nao existe na lista.\r\n");
    }
}

/**
 * @brief Cria um vetor de índices (cada elemento consiste em uma struct contendo um
 * caractere do alfabeto e um ponteiro que deverá apontar para um nó da lista encadeada
 * contendo palavras). O vetor de índices é iniciado contendo todas as letras do alfabeto
 * e respectivos ponteiros com valor NULL. 
 * 
 * @return ponteiro para o início de vetor de índices (indice_t *). 
 */
indice_t *criar_indice()
{
    indice_t *indice = (indice_t *) malloc(26 * sizeof(indice_t));

    char letra = 'a';
    for (int i = 0; i < 26; i++)
    {
        indice[i].inicio = NULL;
        indice[i].letra = letra++;
    }    

    return indice;
}

/**
 * @brief Desaloca toda a memória utilizada por um vetor de índices.
 * 
 * @param indice ponteiro para o início do vetor de índices que deve ser desalocado.
 */
void desalocar_indice(indice_t *indice)
{
    if (indice != NULL)
        free(indice);
}

/**
 * @brief Atualiza o vetor de índices de acordo com as palavras presentes na lista
 * encadeada. Imrpime na saída padrão a quantidade de elementos no índice que contém
 * o seu ponteiro apontando para algum nó da lista.
 * 
 * @param lista ponteiro para a lista encadeada contendo palavras;
 * @param indice ponteiro para o início do vetor de índices.
 */
void atualizar_indice(lista_t *lista, indice_t *indice)
{
    // Conta a quantidade de elementos que tem o seu ponteiro
    // apontando pra algum nó da lista.
    int contador = 0;

    if (lista == NULL || indice == NULL) return;

    no_t *p = lista->inicio;

    for (int i = 0; i < 26; i++)
    {
        if (p != NULL && indice[i].letra == p->palavra[0])
        {
            indice[i].inicio = p;
            contador++;

            // Avançar até a próxima letra.
            while (p != NULL && p->palavra[0] == indice[i].letra)
                p = p->prox;
        }
    }

    printf("%d\r\n", contador);
}