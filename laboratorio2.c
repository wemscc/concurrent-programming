#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Por algum motivo incluir a "timer.h" não está funcionando, então colei o código aqui*/
#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H
#include <time.h>
#define BILLION 1000000000L
#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}
#endif

float *A, *B, *C;
float *criaMatriz(char nome[],int *linhas, int *colunas){
    float *matriz; //matriz auxiliar
    long long int tam; //qtde de elementos na matriz
    FILE * descritorArquivo; //descritor do arquivo de entrada
    size_t ret; //retorno da funcao de leitura no arquivo de entrada
    //abre o arquivo para leitura binaria
    descritorArquivo = fopen(nome, "rb");
    if(!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
    }
    //le as dimensoes da matriz
    ret = fread(linhas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
    }
    ret = fread(colunas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
    }
    // *linhas = linha;
    //  *colunas = coluna;
    tam = *linhas * *colunas; //calcula a qtde de elementos da matriz

    //aloca memoria para a matriz
    matriz = (float*) malloc(sizeof(float) * tam);
    if(!matriz) {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
    }
    //carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matriz, sizeof(float), tam, descritorArquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
    }
    //finaliza o uso das variaveis
    fclose(descritorArquivo);
    return matriz;

}
void gera_Arquivo(float *matriz, int linhas, int colunas, char nomeA[], char nomeB[]){
    FILE * descritorArquivo; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida
    int tam = linhas * colunas; // numero de elementos na matriz
    char nome[999]; // variável para armarzenar o nome do arquivo que será gerado
    strcpy(nome,nomeA);
    strcat(nome,"X");
    strcat(nome,nomeB); // o arquivo será gerado com o nome arquivo_AXarquivo_B

    descritorArquivo = fopen(nome, "wb");
    if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
    }
    //escreve numero de linhas e de colunas
    ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
    ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
    //escreve os elementos da matriz
    ret = fwrite(matriz, sizeof(float), tam, descritorArquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de escrita no  arquivo\n");
    }
    //finaliza o uso das variaveis
    fclose(descritorArquivo);
}

int main(int argc, char* argv[]){
    double inicio,fim, delta;
    GET_TIME(inicio);

    int i,j,k;
    int linhasA,colunasA,linhasB, colunasB; //dimensoes da matriz
    if(argc < 3) { //recebe os argumentos de entrada
      fprintf(stderr, "Digite: %s <arquivo entrada> <arquivo entrada>\n", argv[0]);
      return 1;
    }
    A = criaMatriz(argv[1],&linhasA,&colunasA);
    B = criaMatriz(argv[2],&linhasB,&colunasB);
    if (colunasA != linhasB){
        printf("Erro. Condicao de existência, colunas de a = linhas de b, não satisfeita.");
        return 1;
    } 
    else{
        GET_TIME(fim);
        delta = fim-inicio;
        printf("Tempo inicializacao: %lf\n",delta);
        GET_TIME(inicio);

        C = (float*) malloc(sizeof(float) * linhasA * colunasB);
        for (i=0;i<linhasA;i++){
            for(j=0;j<colunasB;j++){
                 C[i*linhasA+j] = 0; //inicializa a nova matriz nova para evitarmos erros com o += ali embaixo
                for (k=0;k<linhasB;k++)
                    C[i*linhasA+j] += A[i*linhasA+k] * B[k*colunasB+j];
            }
        }
        gera_Arquivo(C, linhasA, colunasB,argv[1],argv[2]);   
     
        GET_TIME(fim);
        delta = fim-inicio;
        printf("Tempo processamento: %lf\n",delta);
    }   
    
    GET_TIME(inicio);
    free(A);
    free(B);
    free(C);
    GET_TIME(fim);
    delta = fim-inicio;
    printf("Tempo finalizacao: %lf\n\n",delta);
    return 0;
}
