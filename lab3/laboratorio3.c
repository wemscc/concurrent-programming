#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
/*Por algum motivo incluir a "timer.h" não está funcionando, então colei o código aqui*/
#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H
#include <time.h>
#define BILLION 1000000000L
#define GET_TIME(now)                                    \
    {                                                    \
        struct timespec time;                            \
        clock_gettime(CLOCK_MONOTONIC, &time);           \
        now = time.tv_sec + time.tv_nsec / 1000000000.0; \
    }
#endif

typedef struct
{
    int id;
    int linhaA, colunasB, linhaB;
    int inicio, fim;
} tArgs;
int nthreads;     // numero de threads
float *A, *B, *C; // vetor de entrada com dimensao dim
void *tarefa(void *arg)
{
    tArgs *args = (tArgs *)arg;
    long int id = args->id;
    double somaLocal = 0; // variavel local da soma de elementos
    int intervalo;
    intervalo = args->linhaA/nthreads; // numero de linhas que cada thread tem que calcular
    args->inicio = id * intervalo;
    args->fim = args->inicio + intervalo;
    if (args->linhaA%nthreads!=0){
        if (id == (nthreads-1))
            args->fim += (args->linhaA%nthreads);
    }
    for (int i = args->inicio; i < args->fim; i++)
    {
        for (int j = 0; j < args->colunasB; j++)
        {
            somaLocal = 0;
            for (int k = 0; k < args->linhaB; k++)
            {
                somaLocal += A[i * args->linhaB + k] * B[k * args->colunasB + j];
            }
            C[i * args->colunasB + j] = somaLocal;
        }
    }
    pthread_exit(NULL);
}

float *criaMatriz(char nome[], int *linhas, int *colunas)
{
    float *matriz;          // matriz auxiliar
    long long int tam;      // qtde de elementos na matriz
    FILE *descritorArquivo; // descritor do arquivo de entrada
    size_t ret;             // retorno da funcao de leitura no arquivo de entrada
    // abre o arquivo para leitura binaria
    descritorArquivo = fopen(nome, "rb");
    if (!descritorArquivo)
    {
        fprintf(stderr, "Erro de abertura do arquivo\n");
    }
    // le as dimensoes da matriz
    ret = fread(linhas, sizeof(int), 1, descritorArquivo);
    if (!ret)
    {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
    }
    ret = fread(colunas, sizeof(int), 1, descritorArquivo);
    if (!ret)
    {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
    }
    // *linhas = linha;
    //  *colunas = coluna;
    tam = *linhas * *colunas; // calcula a qtde de elementos da matriz

    // aloca memoria para a matriz
    matriz = (float *)malloc(sizeof(float) * tam);
    if (!matriz)
    {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
    }
    // carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matriz, sizeof(float), tam, descritorArquivo);
    if (ret < tam)
    {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
    }
    // finaliza o uso das variaveis
    fclose(descritorArquivo);
    return matriz;
}
void gera_Arquivo(float *matriz, int linhas, int colunas, char nome[])
{
    FILE *descritorArquivo;     // descritor do arquivo de saida
    size_t ret;                 // retorno da funcao de escrita no arquivo de saida
    int tam = linhas * colunas; // numero de elementos na matriz
    descritorArquivo = fopen(nome, "wb");
    if (!descritorArquivo)
    {
        fprintf(stderr, "Erro de abertura do arquivo\n");
    }
    // escreve numero de linhas e de colunas
    ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
    ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
    // escreve os elementos da matriz
    ret = fwrite(matriz, sizeof(float), tam, descritorArquivo);
    if (ret < tam)
    {
        fprintf(stderr, "Erro de escrita no  arquivo\n");
    }
    // finaliza o uso das variaveis
    fclose(descritorArquivo);
}

int main(int argc, char *argv[])
{
    double inicio, fim, delta;
    pthread_t *tid; // identificadores das threads no sistema
    tArgs *args;    // identificadores locais das threads e dimensao
    int i;
    int linhasA, colunasA, linhasB, colunasB; // dimensoes da matriz
    if (argc < 5)
    { // recebe os argumentos de entrada
        fprintf(stderr, "Digite: %s <arquivo entrada> <arquivo entrada> <arquivo saida> <numero de threads>\n", argv[0]);
        return 1;
    }
    GET_TIME(inicio);

    A = criaMatriz(argv[1], &linhasA, &colunasA);
    B = criaMatriz(argv[2], &linhasB, &colunasB);
    nthreads = atoi(argv[4]);
    if (colunasA != linhasB)
    {
        printf("Erro. Condicao de existência, colunas de a = linhas de b, não satisfeita.");
        return 1;
    }
    else
    {
        if (nthreads > linhasA)
            nthreads = linhasA;
        tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
        if (tid == NULL)
        {
            fprintf(stderr, "ERRO--malloc\n");
            return 2;
        }
        args = (tArgs *)malloc(sizeof(tArgs) * nthreads);
        C = (float *)malloc(sizeof(float) * linhasA * colunasB);
        GET_TIME(fim);
        delta = fim - inicio;
        printf("Tempo inicializacao: %lf\n", delta);
        GET_TIME(inicio);
        for (i = 0; i < nthreads; i++)
        {
            (args + i)->id = i;
            (args + i)->linhaA = linhasA;
            (args + i)->linhaB = linhasB;
            (args + i)->colunasB = colunasB;
            if (pthread_create(tid + i, NULL, tarefa, (void *)(args + i)))
            {
                fprintf(stderr, "ERRO--pthread_create\n");
                return 3;
            }
        }
        
    }

    // aguardar o termino das threads
    for (long int i = 0; i < nthreads; i++)
    {
        if (pthread_join(*(tid + i), NULL))
        {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    gera_Arquivo(C, linhasA, colunasB, argv[3]);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo processamento: %lf\n", delta);
    GET_TIME(inicio);
    free(args);
    free(tid);
    free(A);
    free(B);
    free(C);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo processamento: %lf\n", delta);
    return 0;
}
