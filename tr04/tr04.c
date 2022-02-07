/**
 * @author Ana Lívia Ruegger Saldanha (N.USP 8586691)
 * @brief  SCC0201 — Introdução à Ciência de Computação II
 *         Trabalho 04: Tabela Hash
 *         
 * @date   2021-12-20
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node node_t;

struct node
{
    char *word;
    node_t *next;
};

int hash(char *string, int m);
node_t **create_hash_table(int m);
node_t *create_node(char *word);
void insert(char *word, int i, node_t **hash_table);
void delete(char *word, int i, node_t **hash_table);
void print_list(node_t **hash_table, int i);
void check(char *word, int i, node_t **hash_table);
void destroy(node_t **hash_table, int hash_table_size);

int main()
{
    int hash_table_size, n_commands;
    scanf("%d\r\n%d\r\n", &hash_table_size, &n_commands);

    node_t **hash_table = create_hash_table(hash_table_size);

    int i;
    char *word;    
    char command[6];
    for (int j = 0; j < n_commands; j++)
    {
        scanf("%s ", command);

        if (strcmp(command, "add") == 0)
        {
            scanf("%m[^\r\n]", &word);
            insert(word, hash(word, hash_table_size), hash_table);
        }
        else if (strcmp(command, "del") == 0)
        {
            scanf("%m[^\r\n]", &word);
            delete(word, hash(word, hash_table_size), hash_table);
        }
        else if (strcmp(command, "get") == 0)
        {
            scanf("%d\r\n", &i);
            print_list(hash_table, i);
        }
        else if (strcmp(command, "check") == 0)
        {
            scanf("%m[^\r\n]", &word);
            check(word, hash(word, hash_table_size), hash_table);
        }
    }

    destroy(hash_table, hash_table_size);

    return 0;
}

 /**
  * @brief Função hash: encontra a posição correta de uma string em uma tabela
  * hash de tamanho dado.
  * 
  * @param string cadeia de caracteres cuja posição na tabela será calculada;
  * @param m tamanho da tabela hash;
  * @return posição da string na tabela hash (int).
  */
int hash(char *string, int m)
{
    int p = 1000000007;
    int x = 263;

    long int sum = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        sum += (int)string[i] * pow(x, i);
        sum %= p;
    }

    return sum % m;
}

/**
 * @brief Cria uma tabela hash vazia; como se trata de uma implementação com 
 * resolução de colisão por encadeamento, isso significa que todas as listas
 * encadeadas iniciam vazias (NULL).
 * 
 * @param m tamanho da tabela a ser criada;
 * @return ponteiro para a tabela criada (node_t **).
 */
node_t **create_hash_table(int m)
{
    node_t **hash_table = (node_t **) malloc(m * sizeof(node_t *));
    
    for (int i = 0; i < m; i++)
        hash_table[i] = NULL;

    return hash_table;
}

/**
 * @brief Cria um nó de lista encadeada contendo a palavra dada.
 * 
 * @param word string que será inserida no nó;
 * @return ponteiro para o nó criado (node_t *).
 */
node_t *create_node(char *word)
{
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    
    new_node->word = word;
    new_node->next = NULL;

    return new_node;
}

/**
 * @brief Cria e insere na tabela hash um nó com a palavra dada. Caso a palavra
 * já exista, apenas encerra a função.
 * 
 * @param word palavra que deve ser inserida;
 * @param i índice correto da palavra na tabela hash (calculado pela função hash);
 * @param hash_table tabela hash onde a palavra deve ser insiderida.
 */
void insert(char *word, int i, node_t **hash_table)
{
    node_t *current = hash_table[i];

    if (!current) // Lista está vazia.
    {
        hash_table[i] = create_node(word);
    }
    else // Há palavras na lista.
    {
        while (current) // Percorre a lista encadeada.
        {
            // Checa se a palavra que vamos inserir já está na lista.
            if (strcmp(word, current->word) == 0)
            {
                free(word);
                return;
            }

            current = current->next;
        }

        // Palavra ainda não está na lista; insere.
        current = hash_table[i];
        hash_table[i] = create_node(word);
        hash_table[i]->next = current;
    }
}

/**
 * @brief Remove da tabela hash uma dada palavra. Caso a palavra não exista,
 * apenas encerra a função.
 * 
 * @param word palavra que deve ser removida da tabela;
 * @param i índice correto da palavra na tabela hash (calculado pela função hash);
 * @param hash_table tabela hash de onde a palavra deve ser removida.
 */
void delete(char *word, int i, node_t **hash_table)
{
    node_t *current = hash_table[i];
    node_t *previous = NULL;

    while (current) // Percorre a lista encadeada.
    {
        if (strcmp(word, current->word) == 0) // Encontrou a palavra.
        {
            if (previous) previous->next = current->next;
            else hash_table[i] = NULL;

            free(current->word);
            free(current);
            free(word);
            return;
        }

        previous = current;
        current = current->next;
    }

    // Não encontrou a palavra.
    free(word);    
}

/**
 * @brief Dado um índice da tabela hash, imprime na saída padrão o conteúdo
 * da lista encadeada naquela posição. Caso a lista esteja vazia, imprime
 * uma linha em branco.
 * 
 * @param hash_table ponteiro para a tabela hash;
 * @param i índice, na tabela, da lista cujo conteúdo deve ser exibido.
 */
void print_list(node_t **hash_table, int i)
{
    node_t *current = hash_table[i];

    while (current)
    {
        printf("%s ", current->word);
        current = current->next;
    }
    
    printf("\n");
}

/**
 * @brief Checa se uma palavra está na tabela hash. Imprime "sim" ou "não" na
 * saída padrão.
 * 
 * @param word palavra que será buscada;
 * @param i índice correto da palavra na tabela hash (calculado pela função hash);
 * @param hash_table ponteiro para a tabela hash.
 */
void check(char *word, int i, node_t **hash_table)
{
    node_t *current = hash_table[i];

    while (current)
    {
        if (strcmp(word, current->word) == 0)
        {
            printf("sim\n");
            free(word);
            return;
        }

        current = current->next;
    }

    printf("não\n");
    free(word);
}

/**
 * @brief Desaloca toda a memória ocupada pela tabela hash e seus elementos
 * (listas encadeadas).
 * 
 * @param hash_table ponteiro para a tabela hash;
 * @param hash_table_size tamanho da tabela hash.
 */
void destroy(node_t **hash_table, int hash_table_size)
{
    node_t *current;
    for (int i = 0; i < hash_table_size; i++)
    {
        current = hash_table[i];
        while (current)
        {
            hash_table[i] = current->next;
            free(current->word);
            free(current);
            current = hash_table[i];
        }
    }

    free(hash_table);
}