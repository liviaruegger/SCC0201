/**
 * @author Ana Lívia Ruegger Saldanha (N.USP 8586691)
 * @brief  SCC0201 — Introdução à Ciência de Computação II
 *         Trabalho 01: Campo Minado
 *         
 * @date   2021-09-17
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define VAZIO '.'
#define MINA  '*'

#define IMPRIMIR_TABULEIRO 1
#define IMPRIMIR_COM_DICAS 2
#define ACAO_DO_USUARIO    3

char *read_line(FILE *stream);
char **read_matrix(FILE *stream, int *n_rows);

void adicionar_dicas(char **tabuleiro, int n_linhas);
void alterar_vizinhos(char **tabuleiro, int n_linhas, int i, int j);
void somar_um(char **tabuleiro, int n_linhas, int i, int j);

void processar_acao(char **tabuleiro, int n_linhas, int i, int j);
void imprimir_dica(char **tabuleiro, int n_linhas, int i_dica, int j_dica);
void revelar_espacos_vazios(char **tab, char **novo_tab, int n_linhas, int i, int j);
char **inicializar_nova_matriz(char **tabuleiro, int n_linhas);

void imprimir_tabuleiro(char **tabuleiro, int n_linhas);
void desalocar_memoria_tabuleiro(char **tabuleiro, int n_linhas);

int main()
{
    // 1. Leitura
    int comando;
    scanf("%d\n", &comando);

    char *nome_do_arquivo = read_line(stdin);

    // 2. Inicialização do tabuleiro
    FILE *arquivo_tabuleiro = fopen(nome_do_arquivo, "r");

    int n_linhas;
    char **tabuleiro = read_matrix(arquivo_tabuleiro, &n_linhas);

    fclose(arquivo_tabuleiro);
    free(nome_do_arquivo);

    // 3. Ação do usuário
    switch (comando)
    {
        case IMPRIMIR_TABULEIRO:
            imprimir_tabuleiro(tabuleiro, n_linhas);
            break;

        case IMPRIMIR_COM_DICAS:
            adicionar_dicas(tabuleiro, n_linhas);
            imprimir_tabuleiro(tabuleiro, n_linhas);
            break;
        
        case ACAO_DO_USUARIO: ;         // Obs: o ponto e vírgula permite inserir uma
            int i, j;                   // declaração de variável logo após o switch.
            scanf("%d %d\n", &i, &j);
            adicionar_dicas(tabuleiro, n_linhas);
            processar_acao(tabuleiro, n_linhas, i, j);
            break;
    }

    desalocar_memoria_tabuleiro(tabuleiro, n_linhas);

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
 * @brief Lê uma matriz do arquivo dado até encontrar o fim do arquivo (EOF).
 * 
 * @param stream entrada (arquivo);
 * @param n_rows endereço onde será armazenado o número de linhas lidas;
 * @return ponteiro para a matriz lida (char**). 
 */
char **read_matrix(FILE *stream, int *n_rows)
{
    char **matrix  = NULL; 
    char  *new_row = NULL;
    
    bool is_eof = false;
    int  line_count = 0;

    while (is_eof == false)
    {
        char c = fgetc(stream);
        if (c != EOF)
        {
            ungetc(c, stream);
            
            matrix = (char **) realloc(matrix, (line_count + 1) * sizeof (char *));

            new_row = read_line(stream);
            matrix[line_count++] = new_row;    
        }
        else
        {
            is_eof = true;
        }
    }

    *n_rows = line_count;
    
    return matrix;
}

/**
 * @brief Processa a matriz do tabuleiro, adicionando as dicas (contagem de minas na
 * vizinhança de cada célula). A função procura as minas presentes no tabuleiro e,
 * para cada mina encontrada, chama a função alterar_vizinhos.
 * 
 * @param tabuleiro ponteiro para a matriz do tabuleiro;
 * @param n_linhas número de linhas no tabuleiro.
 */
void adicionar_dicas(char **tabuleiro, int n_linhas)
{
    int n_colunas = strlen(tabuleiro[0]);

    for (int i = 0; i < n_linhas; i++)
    {
        for (int j = 0; j < n_colunas; j++)
        {
            if (tabuleiro[i][j] == MINA)
                alterar_vizinhos(tabuleiro, n_linhas, i, j);
        }
    }
}

/**
 * @brief Recebe as coordenadas de uma célula onde há uma mina e altera o conteúdo
 * dos seus vizinhos (chamando a função somar_um para cada um desses vizinhos).
 * 
 * @param tabuleiro ponteiro para a matriz do tabuleiro;
 * @param n_linhas número de linhas no tabuleiro;
 * @param i coordenada i da célula onde se encontra a mina;
 * @param j coordenada j da célula onde se encontra a mina.
 */
void alterar_vizinhos(char **tabuleiro, int n_linhas, int i, int j)
{
    somar_um(tabuleiro, n_linhas, i - 1, j - 1);
    somar_um(tabuleiro, n_linhas, i - 1, j    );
    somar_um(tabuleiro, n_linhas, i - 1, j + 1);
    somar_um(tabuleiro, n_linhas, i    , j - 1);
    somar_um(tabuleiro, n_linhas, i    , j + 1);
    somar_um(tabuleiro, n_linhas, i + 1, j - 1);
    somar_um(tabuleiro, n_linhas, i + 1, j    );
    somar_um(tabuleiro, n_linhas, i + 1, j + 1);
}

/**
 * @brief Recebe as coordenadas de uma célula vizinha de uma mina e insere/altera
 * a dica que deverá ser exibida na impressão do tabuleiro.
 * 
 * @param tabuleiro ponteiro para a matriz do tabuleiro;
 * @param n_linhas número de linhas no tabuleiro;
 * @param i coordenada i da célula que deverá ser alterada;
 * @param j coordenada j da célula que deverá ser alterada.
 */
void somar_um(char **tabuleiro, int n_linhas, int i, int j)
{
    int n_colunas = strlen(tabuleiro[0]);

    if (i >= 0 && i < n_linhas && j >= 0 && j < n_colunas)
    {
        // Três casos possíveis:
        //   - vazio  -> troca para 1
        //   - número -> soma 1
        //   - MINA  -> não altera
        if (tabuleiro[i][j] == VAZIO)
            tabuleiro[i][j] = '1';
        else if (tabuleiro[i][j] >= '1' && tabuleiro[i][j] < '8')
            tabuleiro[i][j]++;
    }
}

/**
 * @brief Processa a ação indicada pelo usuário de acordo com o conteúdo da célula cujas
 * coordenadas também foram digitadas pelo usuário, chamando a devida função.
 * 
 * @param tabuleiro ponteiro para a matriz do tabuleiro;
 * @param n_linhas número de linhas no tabuleiro;
 * @param i coordenada i digitada pelo usuário;
 * @param j coordenada j digitada pelo usuário.
 */
void processar_acao(char **tabuleiro, int n_linhas, int i, int j)
{
    if (tabuleiro[i][j] == MINA)
    {
        imprimir_tabuleiro(tabuleiro, n_linhas);
    }
    else if (tabuleiro[i][j] >= '1' && tabuleiro[i][j] <= '8')
    {
        imprimir_dica(tabuleiro, n_linhas, i, j);
    }
    else if (tabuleiro[i][j] == VAZIO)
    {
        char **novo_tabuleiro = inicializar_nova_matriz(tabuleiro, n_linhas);
        revelar_espacos_vazios(tabuleiro, novo_tabuleiro, n_linhas, i, j);
        imprimir_tabuleiro(novo_tabuleiro, n_linhas);
        desalocar_memoria_tabuleiro(novo_tabuleiro, n_linhas);
    }
}

/**
 * @brief Imprime o tabuleiro revelando a dica contida na célula indicada; para todas
 * as outras células, imprime um 'X'.
 * 
 * @param tabuleiro ponteiro para a matriz do tabuleiro;
 * @param n_linhas número de linhas no tabuleiro;
 * @param i_dica coordenada i da célula contendo a dica;
 * @param j_dica coordenada j da célula contendo a dica.
 */
void imprimir_dica(char **tabuleiro, int n_linhas, int i_dica, int j_dica)
{
    int n_colunas = strlen(tabuleiro[0]);

    for (int i = 0; i < n_linhas; i++)
    {
        for (int j = 0; j < n_colunas; j++)
        {
            if (i == i_dica && j == j_dica)
                printf("%c", tabuleiro[i][j]);
            else
                printf("X");
        }
        printf("\n");
    }
}

/**
 * @brief Função recursiva que, a partir do tabuleiro original e das coordenadas dadas,
 * vai copiando para uma segunda matriz (a princício preenchida com 'X') o conteúdo das
 * células que devem ser reveladas (células vizinhas contendo '.' ou uma dica).
 * 
 * @param tab ponteiro para o tabuleiro original;
 * @param novo_tab ponteiro para o novo tabuleiro com as células que foram reveladas;
 * @param n_linhas número de linhas de ambos os tabuleiros;
 * @param i coordenada i da célula que será processada;
 * @param j coordenada j da célula que será processada.
 */
void revelar_espacos_vazios(char **tab, char **novo_tab, int n_linhas, int i, int j)
{
    // Checa se a célula está dentro da matriz.
    if (i < 0 || i >= n_linhas)
        return;
    int n_colunas = strlen(tab[0]);
    if (j < 0 || j >= n_colunas)
        return;

    // Se a célula for uma dica, ela é revelada e o processo é encerrado.
    if (tab[i][j] >= '1' && tab[i][j] <= '8')
    {
        novo_tab[i][j] = tab[i][j];
        return;
    }

    // Se a célula for vazia, ela é revelada e a função é chamada para os vizinhos.
    if (tab[i][j] == VAZIO && novo_tab[i][j] == 'X')
    {
        novo_tab[i][j] = tab[i][j];

        revelar_espacos_vazios(tab, novo_tab, n_linhas, i - 1, j - 1);
        revelar_espacos_vazios(tab, novo_tab, n_linhas, i - 1, j    );
        revelar_espacos_vazios(tab, novo_tab, n_linhas, i - 1, j + 1);
        revelar_espacos_vazios(tab, novo_tab, n_linhas, i    , j - 1);
        revelar_espacos_vazios(tab, novo_tab, n_linhas, i    , j + 1);
        revelar_espacos_vazios(tab, novo_tab, n_linhas, i + 1, j - 1);
        revelar_espacos_vazios(tab, novo_tab, n_linhas, i + 1, j    );
        revelar_espacos_vazios(tab, novo_tab, n_linhas, i + 1, j + 1);        
    }
}

/**
 * @brief Cria e preenche com 'X' uma matriz com as mesmas dimensões do tabuleiro.
 * 
 * @param tabuleiro ponteiro para a matriz do tabuleiro;
 * @param n_linhas número de linhas no tabuleiro;
 * @return ponteiro para a matriz criada (char**). 
 */
char **inicializar_nova_matriz(char **tabuleiro, int n_linhas)
{
    int n_colunas = strlen(tabuleiro[0]);
    
    char **matriz_copia = (char **) malloc(n_linhas * sizeof(char *));

    for (int i = 0; i < n_linhas; i++)
    {
        char *linha_nova = (char *) malloc((n_colunas + 1) * sizeof(char));
        
        for (int j = 0; j < n_colunas; j++)
            linha_nova[j] = 'X';
        linha_nova[n_colunas] = '\0';

        matriz_copia[i] = linha_nova;
    }

    return matriz_copia;
}

/**
 * @brief Imprime, na saída padrão, a matriz de caracteres que representa o tabuleiro.
 * 
 * @param tabuleiro ponteiro para a matriz do tabuleiro;
 * @param n_linhas número de linhas no tabuleiro.
 */
void imprimir_tabuleiro(char **tabuleiro, int n_linhas)
{
    for (int i = 0; i < n_linhas; i++)
        printf("%s\n", tabuleiro[i]);
}

/**
 * @brief Desaloca a memória ocupada pelo tabuleiro.
 * 
 * @param tabuleiro ponteiro para a matriz do tabuleiro;
 * @param n_linhas número de linhas no tabuleiro.
 */
void desalocar_memoria_tabuleiro(char **tabuleiro, int n_linhas)
{
    for (int i = 0; i < n_linhas; i++)
        free(tabuleiro[i]);
    free(tabuleiro);
}