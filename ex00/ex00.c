#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define UNINITIALISED '$'

typedef struct number
{
    int value;
    int count;
}   number_t;

bool check_if_counted(int value, number_t *numbers, int numbers_counted);
int  count_occurrences(int value, int *sequence, int sequence_size);

int main()
{
    int *sequence = NULL;
    int  sequence_size = 0;
    char temp_scan = UNINITIALISED;

    // Receber a sequência de inteiros da entrada padrão.
    do
    {
        sequence = (int *) realloc(sequence, (sequence_size + 1) * sizeof(int));
        
        scanf("%d", &sequence[sequence_size]);
        sequence_size++;
        
        temp_scan = getc(stdin);

    } while (temp_scan != EOF);

    // Array que guardará cada valor que aparece na sequência e seu número de ocorrências.
    number_t *numbers = NULL;
    int numbers_counted = 0;
    
    // Contar as ocorrências de cada valor presente pelo menos uma vez na sequência.
    for (int i = 0; i < sequence_size; i++) 
    {
        bool counted = check_if_counted(sequence[i], numbers, numbers_counted);

        if (counted == false)
        {
            numbers = (number_t *) realloc(numbers, (numbers_counted + 1) * sizeof(number_t));
            
            numbers[numbers_counted].value = sequence[i];
            numbers[numbers_counted].count = count_occurrences(sequence[i], sequence, sequence_size);
            numbers_counted++;
        }    
    }
    
    // Saída: 
    for (int i = 0; i < numbers_counted; i++)
        printf("%d (%d)\n", numbers[i].value, numbers[i].count);
    
    // Limpar memória utilizada na heap.
    free(sequence);
    free(numbers);

    return 0;
}

/**
 * @brief Checa se as ocorrências do inteiro dado já foram contadas.
 * 
 * @param value inteiro presente na sequência;
 * @param numbers array de números que já foram contados;
 * @param numbers_counted tamanho do array de números contados;
 * @return true (já foi contado) ou false (não foi contado).
 */
bool check_if_counted(int value, number_t *numbers, int numbers_counted)
{  
    for (int i = 0; i < numbers_counted; i++)
    {
        if (value == numbers[i].value)
            return true;
    }
    
    return false;
}

/**
 * @brief Conta quantas vezes o inteiro dado aparece na sequência.
 * 
 * @param value inteiro presente na sequência;
 * @param sequence sequência recebida (array de inteiros);
 * @param sequence_size tamanho da sequência;
 * @return quantidade de ocorrências do valor na sequência (inteiro).
 */
int count_occurrences(int value, int *sequence, int sequence_size)
{
    int count = 0;
    for (int i = 0; i < sequence_size; i++)
    {
        if (sequence[i] == value)
            count++;
    }   

    return count;
}