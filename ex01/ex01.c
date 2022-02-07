#include <stdio.h>
#include <stdlib.h>

#define PERSON  '#'
#define PATH    '.'
#define VISITED '*'

#define TRUE  1
#define FALSE 0

char *read_line(FILE *stream);
char **read_matrix(FILE *stream, int n_rows);

int  count_char(char **maze_matrix, int n_rows, int n_columns, char type);
void escape_maze(char **maze_matrix, int n_rows, int n_columns, int i, int j, int *escaped);

int main()
{
    char *file_name = read_line(stdin);

    FILE *maze_file = fopen(file_name, "r");

    int n_rows, n_columns;
    fscanf(maze_file, "%d %d\n", &n_rows, &n_columns);

    int start_i, start_j;
    fscanf(maze_file, "%d %d\n", &start_i, &start_j);

    char **maze_matrix = read_matrix(maze_file, n_rows);
    fclose(maze_file);

    int people_count = count_char(maze_matrix, n_rows, n_columns, PERSON);
    int path_count   = count_char(maze_matrix, n_rows, n_columns, PATH);

    int escaped = FALSE;
    escape_maze(maze_matrix, n_rows, n_columns, start_i, start_j, &escaped);

    for (int i = 0; i < n_rows; i++)
        printf("%s\n", maze_matrix[i]);

    int visited_count = count_char(maze_matrix, n_rows, n_columns, VISITED);
    int explored      = (visited_count * 100) / path_count;
 
    printf("\nVoce escapou de todos! Ninguem conseguiu te segurar!\n");
    printf("Veja abaixo os detalhes da sua fuga:\n");
    printf("----Pessoas te procurando: %d\n", people_count);
    printf("----Numero total de caminhos validos:   %d\n", path_count);
    printf("----Numero total de caminhos visitados: %d\n", visited_count);
    printf("----Exploracao total do labirinto: %.1lf%%\n", (double)explored);

    free(file_name);
    for (int i = 0; i < n_rows; i++)
        free(maze_matrix[i]);
    free(maze_matrix);
    
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
    int string_size = 0;

    do
    {
        string[string_size] = fgetc(stream);
        string_size++;

        if (string[string_size - 1] != '\n' && string[string_size - 1] != EOF)
            string = (char *) realloc(string, string_size + 1);

    } while (string[string_size - 1] != '\n' && string[string_size - 1] != EOF);

    string[string_size - 1] = '\0';

    return string;
}

/**
 * @brief Lê uma matriz (labirinto) do arquivo passado como parâmetro.
 * 
 * @param stream arquivo contendo a matriz do labirinto;
 * @param n_rows número de linhas da matriz;
 * @return ponteiro para a matriz do labirinto (char **). 
 */
char **read_matrix(FILE *stream, int n_rows)
{
    char **maze_matrix = (char **) malloc(n_rows * sizeof(char *));
    
    for (int i = 0; i < n_rows; i++)
        maze_matrix[i] = read_line(stream);
    
    return maze_matrix;
}

/**
 * @brief Conta quantos caracteres do tipo dado (pessoa, caminho ou caminho percorrido)
 * estão presentes no labirinto.
 * 
 * @param maze_matrix matriz que representa o labirinto;
 * @param n_rows número de linhas na matriz do labirinto;
 * @param n_columns número de colunas na matriz do labirinto;
 * @param type caractere que define o tipo pessoa, caminho ou caminho percorrido;
 * @return número de ocorrências do caractere type na matriz do labirinto (int). 
 */
int count_char(char **maze_matrix, int n_rows, int n_columns, char type)
{
    int count = 0;

    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_columns; j++)
        {
            if (maze_matrix[i][j] == type)
                count++;
        }
    }
    
    return count;
}

/**
 * @brief Função recursiva que percorre o labirinto, buscando a saída através da seguinte
 * ordem de busca: cima, direita, baixo, esquerda.
 * 
 * @param maze_matrix matriz que representa o labirinto;
 * @param n_rows número de linhas na matriz do labirinto;
 * @param n_columns número de colunas na matriz do labirinto;
 * @param i coordenada i (linha) da posição atual no labirinto;
 * @param j coordenada j (coluna) da posição atual no labirinto;
 * @param escaped flag que indica se a saída do labirinto já foi encontrada.
 */
void escape_maze(char **maze_matrix, int n_rows, int n_columns, int i, int j, int *escaped)
{
    maze_matrix[i][j] = VISITED;

    if (i == 0 || i == n_rows - 1 || j == 0 || j == n_columns - 1)
    {
        *escaped = TRUE;
        return;
    }

    if (maze_matrix[i - 1][j] == PATH && *escaped == FALSE)
        escape_maze(maze_matrix, n_rows, n_columns, i - 1, j, escaped);
    if (maze_matrix[i][j + 1] == PATH && *escaped == FALSE)
        escape_maze(maze_matrix, n_rows, n_columns, i, j + 1, escaped);
    if (maze_matrix[i + 1][j] == PATH && *escaped == FALSE)
        escape_maze(maze_matrix, n_rows, n_columns, i + 1, j, escaped);
    if (maze_matrix[i][j - 1] == PATH && *escaped == FALSE)
        escape_maze(maze_matrix, n_rows, n_columns, i, j - 1, escaped);
}