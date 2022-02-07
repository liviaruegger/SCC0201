/**
 * @file   main.c
 * @author Ana Lívia Ruegger Saldanha (N.USP 8586691)
 * @brief  SCC0201 — Introdução à Ciência de Computação II
 *         Exercício 03: Busca Indexada com Lista Encadeada
 *         
 * @date   2021-12-01
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "busca.h"

#define FINALIZAR         0
#define LER_ARQUIVO       1
#define ATUALIZAR_INDICES 2
#define BUSCAR_PALAVRA    3

typedef enum {false, true} bool;

char *read_line(FILE *stream);
void ler_arquivo_e_adicionar_palavras(lista_t *lista);

int main()
{
    indice_t *indice = NULL;
    lista_t *lista = NULL;

    bool indice_atualizado = false;

    int comando;
    do
    {
        scanf("%d", &comando);
        getchar(); // Consome '\r'
        getchar(); // Consome '\n'

        if (comando == FINALIZAR)
        {
            desalocar_indice(indice);
            desalocar_lista(lista);
        }
        else if (comando == LER_ARQUIVO)
        {
            if (lista == NULL) lista = criar_lista();
            ler_arquivo_e_adicionar_palavras(lista);
            imprimir_tres_primeiras_palavras(lista);
            indice_atualizado = false;
        }
        else if (comando == ATUALIZAR_INDICES)
        {
            if (indice == NULL) indice = criar_indice();
            atualizar_indice(lista, indice);
            indice_atualizado = true;
        }
        else if (comando == BUSCAR_PALAVRA)
        {
            char *palavra = read_line(stdin);
            if (!indice_atualizado) printf("Vetor de indices nao atualizado.\r\n");
            else buscar_palavra(lista, indice, palavra);
            free(palavra);
        }

    } while (comando != FINALIZAR);
    
    return 0;
}

/**
 * @brief Lê e armazena uma linha do arquivo dado até encontrar uma quebra de linha ou
 * o fim do arquivo (EOF).
 * 
 * @param stream entrada (pode ser um arquivo ou a entrada padrão);
 * @return ponteiro para a string lida (char *). 
 */
char *read_line(FILE *stream)
{
    char *string = (char *) malloc(sizeof(char));
    int size = 0;

    do
    {
        string[size] = fgetc(stream);
        size++;

        if (string[size - 1] != '\r' && string[size - 1] != '\n' && string[size - 1] != EOF)
            string = (char *) realloc(string, size + 1);

    } while (string[size - 1] != '\r' && string[size - 1] != '\n' && string[size - 1] != EOF);

    if (string[size - 1] == '\r') getchar();

    string[size - 1] = '\0';

    return string;
}

/**
 * @brief Lê um nome de arquivo da entrada padrão e faz a leitura das palavras contidas
 * nesse arquivo, inserindo-as na lista encadeada passada como parâmetro.
 * 
 * @param lista ponteiro para a lista encadeada na qual as palavras devem ser inseridas.
 */
void ler_arquivo_e_adicionar_palavras(lista_t *lista)
{
    char *nome_do_arquivo = read_line(stdin);
    FILE *arquivo = fopen(nome_do_arquivo, "r");

    char temp = fgetc(arquivo);
    while (temp != EOF)
    {
        ungetc(temp, arquivo);
        char *palavra = read_line(arquivo);
        inserir(lista, palavra);
        temp = fgetc(arquivo);
    }

    fclose(arquivo);
    free(nome_do_arquivo);
}