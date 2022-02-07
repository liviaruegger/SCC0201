/**
 * @author Ana Lívia Ruegger Saldanha (N.USP 8586691)
 * @brief  SCC0201 — Introdução à Ciência de Computação II
 *         Exercício 02: Compressão de Imagens com Quad-Tree
 *         
 * @date   2021-10-23
 * 
 */

#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;

int **read_matrix(int n_rows, int n_columns);
void free_matrix(int **matrix, int n_rows);

void compress_image(int **image, int i_start, int j_start, int d);

int main()
{
    int n_rows, n_columns;
    scanf("%d %d", &n_rows, &n_columns);

    if (n_rows != n_columns)
    {
        printf("A matriz deve ser quadrada!");
        return 0;
    }

    int **image = read_matrix(n_rows, n_columns);

    compress_image(image, 0, 0, n_rows);
    printf("\n");

    free_matrix(image, n_rows);

    return 0;
}

/**
 * @brief Faz a leitura de uma matriz de inteiros da entrada padrão, alocando memória
 * dinamicamente.
 * 
 * @param n_rows número de linhas da matriz;
 * @param n_columns numero de colunas da matriz;
 * @return ponteiro para a matriz lida (int **). 
 */
int **read_matrix(int n_rows, int n_columns)
{
    int **matrix = (int **)malloc(n_rows * sizeof(int *));

    for (int i = 0; i < n_rows; i++)
    {
        matrix[i] = (int *)malloc(n_columns * sizeof(int));

        for (int j = 0; j < n_columns; j++)
            scanf("%d", &matrix[i][j]);
    }

    return matrix;
}

/**
 * @brief Desaloca a memória ocupada por uma matriz de inteiros.
 * 
 * @param matrix ponteiro para a matriz;
 * @param n_rows número de linhas da matriz.
 */
void free_matrix(int **matrix, int n_rows)
{
    for (int i = 0; i < n_rows; i++)
        free(matrix[i]);

    free(matrix);
}

/**
 * @brief Função recursiva que simula a compressão de uma imagem (matriz de inteiros)
 * com quad-tree, imprimindo um valor numérico para cada nó criado. Funciona apenas
 * com matrizes quadradas de tamanho divisível por 4.
 * 
 * @param image ponteiro para a matriz de inteiros (imagem);
 * @param i_start coordenada i (linha) do canto superior esquerdo do quadrante;
 * @param j_start coordenada j (coluna) do canto superior esquerdo do quadrante;
 * @param d tamanho da imagem (altura = largura).
 */
void compress_image(int **image, int i_start, int j_start, int d)
{
    bool finished = true;
    for (int i = i_start; i < i_start + d; i++)
    {
        for (int j = j_start; j < j_start + d; j++)
        {
            if (image[i][j] != image[i_start][j_start])
                finished = false;
        }
    }

    if (finished)
    {
        printf("%d ", image[i_start][j_start]);
        return;
    }

    printf("-1 ");

    compress_image(image, i_start, j_start, d / 2);
    compress_image(image, i_start, j_start + d / 2, d / 2);
    compress_image(image, i_start + d / 2, j_start, d / 2);
    compress_image(image, i_start + d / 2, j_start + d / 2, d / 2);
}