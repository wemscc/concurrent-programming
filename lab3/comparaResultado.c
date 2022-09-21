/* Programa que le dois arquivos binarios, com cada um contendo o resultado da multiplicação de duas matrizes:
 * um calculado de forma sequencial e outro de forma concorrente
 * após isso, calcula a diferença de cada valor em cada posição das matrizes afim de ver se essa diferença é maior do que nossa tolerância.
 * se for, nossa multiplicação concorrente deu errado.
 * se não, a corretude está provada (a corretude da primeira versão foi checada no lab de semana passada)
 * Entrada: nome dos arquivo de entrada
 * Saida:
 * */

#include <stdio.h>
#include <stdlib.h>
#define TOL 1e-5
int main(int argc, char *argv[])
{
    float *matrizSEQ, *matrizCONC;       // matriz que será carregada do arquivo
    int linhas, colunas,checa = 0;    // dimensoes da matriz
    long long int tam;      // qtde de elementos na matriz
    FILE *descritorArquivo; // descritor do arquivo de entrada
    size_t ret;             // retorno da funcao de leitura no arquivo de entrada

    // recebe os argumentos de entrada
    if (argc < 3)
    {
        fprintf(stderr, "Digite: %s <arquivo entrada>\n", argv[0]);
        return 1;
    }

    // abre o arquivo para leitura binaria
    descritorArquivo = fopen(argv[1], "rb");
    if (!descritorArquivo)
    {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    // le as dimensoes da matriz
    ret = fread(&linhas, sizeof(int), 1, descritorArquivo);
    if (!ret)
    {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    ret = fread(&colunas, sizeof(int), 1, descritorArquivo);
    if (!ret)
    {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    tam = linhas * colunas; // calcula a qtde de elementos da matriz

    // aloca memoria para a matriz
    matrizSEQ = (float *)malloc(sizeof(float) * tam);
    if (!matrizSEQ)
    {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        return 3;
    }

    // carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matrizSEQ, sizeof(float), tam, descritorArquivo);
    if (ret < tam)
    {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        return 4;
    }

    // abre o arquivo para leitura binaria
    descritorArquivo = fopen(argv[2], "rb");
    if (!descritorArquivo)
    {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    // le as dimensoes da matriz
    ret = fread(&linhas, sizeof(int), 1, descritorArquivo);
    if (!ret)
    {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    ret = fread(&colunas, sizeof(int), 1, descritorArquivo);
    if (!ret)
    {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }

    // aloca memoria para a matriz
    matrizCONC = (float *)malloc(sizeof(float) * tam);
    if (!matrizCONC)
    {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        return 3;
    }

    // carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matrizCONC, sizeof(float), tam, descritorArquivo);
    if (ret < tam)
    {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        return 4;
    }

    // imprime a matriz na saida padrao
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++){
            if (abs(matrizSEQ[i*colunas+j] - matrizCONC[i*colunas+j] ))
            checa = 1;
            i = linhas;
            break;
        }
    }

    // finaliza o uso das variaveis
    if (!checa)
        printf("Resultado respeita a tolerancia.\n");
    else
        printf("Resultado nao respeita a tolerancia\n");
    fclose(descritorArquivo);
    free(matrizSEQ);
    free(matrizCONC);
    return 0;
}
