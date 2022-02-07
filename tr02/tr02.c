/**
 * @author Ana Lívia Ruegger Saldanha (N.USP 8586691)
 * @brief  SCC0201 — Introdução à Ciência de Computação II
 *         Trabalho 02: Compressão de Áudio
 *         
 * @date   2021-10-21
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>

char *read_line(FILE *stream);
unsigned char *read_wav_data(char *fname, int *length);

double complex *DFT(unsigned char *audio, int length);
void inverse_DFT(double complex *coef, unsigned char *data, int length);

void compress(double complex *coef, int length, int n_valid_samples);
void adapted_mergesort(double complex *coef, int *posit_vector, int start, int end);
void merge(double complex *coef, int *posit, int start, int center, int end);


int main()
{
    // Leitura:
    char *original_file_name = read_line(stdin);

    int n_valid_samples;
    scanf("%d", &n_valid_samples);

    int length;
    unsigned char *data = read_wav_data(original_file_name, &length);


    // Compressão:
    double complex *coef = DFT(data, length);

    int counter = 0;
    for (int i = 0; i < length; i++)
    {
        if (creal(coef[i]) <= 0.0 && cimag(coef[i]) <= 0.0)
            counter++;
    }
    printf("%d\r\n", counter);
    
    compress(coef, length, n_valid_samples);
    inverse_DFT(coef, data, length);
    

    // Criar novo arquivo:
    char *new_file_name = (char *) malloc((12 + strlen(original_file_name)) * sizeof(char));
    strcpy(new_file_name, "compressed_");
    strcat(new_file_name, original_file_name);
    
    unsigned char *header_data = malloc(sizeof(unsigned char) * 44);

    FILE *original_file = fopen(original_file_name, "rb");
    fread(header_data, sizeof(unsigned char), 44, original_file);
    fclose(original_file);    

    FILE *new_file = fopen(new_file_name, "wb");
    fwrite(header_data, sizeof(unsigned char), 44, new_file);
    free(header_data);
    
    fwrite(data, sizeof(unsigned char), length, new_file);
    fclose(new_file);    


    free(original_file_name);
    free(new_file_name);
    free(data);

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
 * @brief Lê um arquivo do formato WAVE (.wav) como um arquivo binário, armazenando seus
 * dados em um vetor de unsigned char.
 * 
 * @param fname nome do arquivo que será lido;
 * @param length endereço onde será armazenado o tamanho do vetor lido;
 * @return vetor com os dados do arquivo .wav (unsigned char *).
 */
unsigned char *read_wav_data(char *fname, int *length)
{
    FILE *fp = fopen(fname, "rb");
    int data_size;

    // Faz a leitura do cabeçalho (4 bytes).
    fseek(fp, 40, SEEK_SET);
    fread(&data_size, sizeof(int), 1, fp);

    *length = data_size;
    printf("%d\r\n", data_size);

    unsigned char *data = malloc(sizeof(unsigned char) * (data_size));

    for (int i = 0; i < data_size; i++)
        fread(&data[i], sizeof(unsigned char), 1, fp);

    fclose(fp);

    return data;
}


/**
 * @brief Transformada Discreta de Fourier (DFT).
 * 
 * @param audio vetor de unsigned char contendo os dados do arquivo .wav;
 * @param length tamanho do vetor;
 * @return vetor de coeficientes obtidos pela DFT (double complex *). 
 */
double complex *DFT(unsigned char *audio, int length)
{
    double complex *coef = (double complex *) calloc(length, sizeof(double complex));

    for (int k = 0; k < length; k++)
    {
        for (int n = 0; n < length; n++)
            coef[k] += audio[n] * cexp((-2.0 * M_PI * (((k + 1) * n * 1.0) / (length * 1.0))) * _Complex_I);
    }

    return coef;
}

/**
 * @brief Inversa da Transformada Discreta de Fourier.
 * 
 * @param coef vetor de coeficientes (double complex) obtidos pela DFT;
 * @param data vetor de unsigned char contendo os dados do áudio;
 * @param length tamanho do vetor.
 */
void inverse_DFT(double complex *coef, unsigned char *data, int length)
{
    for (int n = 0; n < length; n++)
    {
        double complex temp_data = 0;
        for (int k = 0; k < length; k++)
            temp_data += coef[k] * cexp((2.0 * M_PI * (((k + 1) * n * 1.0) / (length * 1.0))) * _Complex_I);
        
        temp_data /= (length * 1.0);
        data[n] = (int)creal(temp_data);
    }

    free(coef);
}


/**
 * @brief Modifica um vetor de coeficientes obtidos pela DFT, substituindo por zero
 * os valores que serão descartados na compressão.
 * 
 * @param coef vetor de coeficientes obtidos pela DFT;
 * @param length tamanho do vetor;
 * @param n_valid_samples número de samples que serão mantidos no áudio.
 */
void compress(double complex *coef, int length, int n_valid_samples)
{
    // Vetor de posições: em vez de ordenar o vetor em si, ordenaremos suas respectivas
    // posições em um vetor auxiliar, comparando sempre os valores no vetor original.
    int *posit_vector = (int *) malloc(length * sizeof(int));
    for (int i = 0; i < length; i++)
        posit_vector[i] = i;

    adapted_mergesort(coef, posit_vector, 0, length - 1);

    // Saída:
    for (int i = 0; i < n_valid_samples; i++)
    {
        printf("%d", (int)cabs(coef[posit_vector[i]]));
        if (i < n_valid_samples - 1) printf("\r\n");
    }

    // Zerar os coeficientes que estão após T (n_valid_samples) no vetor original.
    for (int i = n_valid_samples; i < length; i++)
        coef[posit_vector[i]] = 0;

    free(posit_vector);
}

/**
 * @brief Merge Sort adaptado que trabalha com dois vetores: um deles contém apenas as
 * posições dos valores que estamos ordenando e será modificado pela função; o outro não
 * será modificado, mas os seus valores serão utilizados na comparação. 
 * 
 * @param coef vetor de coeficientes obtidos pela DFT;
 * @param posit_vector vetor contendo as posições dos valores sendo ordenados;
 * @param start início do trecho que será ordenado;
 * @param end fim do trecho que será ordenado.
 */
void adapted_mergesort(double complex *coef, int *posit_vector, int start, int end)
{
    if (end <= start)
        return;

    int center = (int)((start + end) / 2.0);
    adapted_mergesort(coef, posit_vector, start, center);
    adapted_mergesort(coef, posit_vector, center + 1, end);

    merge(coef, posit_vector, start, center, end);
}

/**
 * @brief Intercala, de forma ordenada, os valores das duas metades de um vetor. Esta
 * função faz parte do Merge Sort adaptado, trabalhando com dois vetores: um vetor de
 * posições (será alterado) e o vetor de coeficientes (mantém original).
 * 
 * @param coef vetor de coeficientes obtidos pela DFT;
 * @param posit vetor contendo as posições dos valores sendo ordenados;
 * @param start início do vetor;
 * @param center centro do vetor;
 * @param end fim do vetor.
 */
void merge(double complex *coef, int *posit, int start, int center, int end)
{
    int *aux = (int *) calloc((end - start) + 1, sizeof(int));

    int i = start;
    int j = center + 1;
    int k = 0;

    while (i <= center && j <= end)
    {
        if (cabs(coef[posit[i]]) >= cabs(coef[posit[j]]))
        {
            aux[k] = posit[i];
            i++;
        }
        else
        {
            aux[k] = posit[j];
            j++;
        }
        k++;
    }

    while (i <= center)
    {
        aux[k] = posit[i];
        i++;
        k++;
    }

    while (j <= end)
    {
        aux[k] = posit[j];
        j++;
        k++;
    }

    for (i = start, k = 0; i <= end; i++, k++)
        posit[i] = aux[k];
    
    free(aux);
}