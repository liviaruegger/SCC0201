/**
 * @file   busca.h
 * @author Ana Lívia Ruegger Saldanha (N.USP 8586691)
 * @brief  SCC0201 — Introdução à Ciência de Computação II
 *         Exercício 03: Busca Indexada com Lista Encadeada
 *         
 * @date   2021-12-01
 * 
 */

typedef struct no_indice indice_t;
typedef struct no_lista no_t;
typedef struct lista lista_t;

lista_t *criar_lista();
void desalocar_lista(lista_t *lista);
void inserir(lista_t *lista, char *palavra);
void imprimir_tres_primeiras_palavras(lista_t *lista);
void buscar_palavra(lista_t *lista, indice_t *indice, char *palavra);

indice_t *criar_indice();
void desalocar_indice(indice_t *indice);
void atualizar_indice(lista_t *lista, indice_t *indice);