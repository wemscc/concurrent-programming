/*
Programa: pc.c
Descricao: implementa  o problema dos produtores/consumidores usando variaveis de condicao da biblioteca Pthread
Autor: Silvana Rossetto
*/

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5 // tamanho do buffer
#define P 3 // qtde de threads produtoras
#define C 4 // qtde de threads consumidoras

// variaveis do problema
int Buffer[N];                                       // espaco de dados compartilhados
int count = N, out = 0;                              // variaveis de estado
sem_t mutexCons, mutexProd, condicaoProd, condicaoCons; // variáveis que garantem: exclusao mutua, condicao para retirar e condição para inserir, respectivamente.

// inicializa o buffer
void IniciaBuffer(int n)
{
    int i;
    for (i = 0; i < n; i++)
        Buffer[i] = 0;
    sem_post(&condicaoProd);
}

// imprime o buffer
void ImprimeBuffer(int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", Buffer[i]);
    printf("\n");
}

// insere um elemento no Buffer ou bloqueia a thread caso o Buffer esteja cheio
void Insere(int item, int id)
{
    printf("P[%d] quer inserir\n", id);
    // fica nesse wait até a variável out ser 0, isto é, todos os itens já terem sido retirados do buffer
    sem_wait(&condicaoProd);
    // exclusao mutua entre os produtores
    sem_wait(&mutexProd);
    for (int i = 0; i < N; i++)
    {
        Buffer[i] = i + 1; // preenche o buffer completamente * nao precisa mais da variável in já que preenchemos tudo de uma vez, e apenas quando está vazio
    }
    printf("P[%d] inseriu\n", id);
    ImprimeBuffer(N); 
    for (int i =0; i < N; i++)
        sem_post(&condicaoCons);    // sinaliza todos os slots cheios
    sem_post(&mutexProd);  // mutex só aqui no final pra garantir que consumidores não consumam enquanto nao terminamos de preencher o buffer
   
}

// retira um elemento no Buffer ou bloqueia a thread caso o Buffer esteja vazio
int Retira(int id)
{
    int item;
    printf("C[%d] quer consumir\n", id);
    // aguarda o buffer ficar cheio
    sem_wait(&condicaoCons);
    // exclusao mutua entre consumidores
    sem_wait(&mutexCons);
    item = Buffer[out];
    Buffer[out] = 0;
    out = (out + 1) % N;
    printf("C[%d] consumiu %d\n", id, item);
    ImprimeBuffer(N);
    if (out == 0)     // se entra aqui, significa que ja consumimos tudo 
    {
        sem_post(&condicaoProd); // manda encher o buffer de novo
    }
    sem_post(&mutexCons); // fim da exclusao mutua
    return item; //retorna o produto
}

// thread produtora
void *produtor(void *arg)
{
    int *id = (int *)arg;
    printf("Sou a thread produtora %d\n", *id);
    while (1)
    {
        // produzindo o item
        Insere(*id, *id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

// thread consumidora
void *consumidor(void *arg)
{
    int *id = (int *)arg;
    int item;
    printf("Sou a thread consumidora %d\n", *id);
    while (1)
    {
        item = Retira(*id);
        sleep(1); // faz o processamento do item
    }
    free(arg);
    pthread_exit(NULL);
}

// funcao principal
int main(void)
{
    // variaveis auxiliares
    int i;

    // identificadores das threads
    pthread_t tid[P + C];
    int *id[P + C];

    // aloca espaco para os IDs das threads
    for (i = 0; i < P + C; i++)
    {
        id[i] = malloc(sizeof(int));
        if (id[i] == NULL)
            exit(-1);
        *id[i] = i + 1;
    }

    // inicializa os semaforos
    sem_init(&mutexCons, 0, 1);
    sem_init(&mutexProd, 0, 1);
    sem_init(&condicaoProd, 0, 0);
    sem_init(&condicaoCons, 0, 0);

    // inicializa o Buffer
    IniciaBuffer(N); // permite que o primeiro produtor possa inserir
    ImprimeBuffer(N);
    // cria as threads produtoras
    for (i = 0; i < P; i++)
    {
        if (pthread_create(&tid[i], NULL, produtor, (void *)id[i]))
            exit(-1);
    }

    // cria as threads consumidoras
    for (i = 0; i < C; i++)
    {
        if (pthread_create(&tid[i + P], NULL, consumidor, (void *)id[i + P]))
            exit(-1);
    }

    pthread_exit(NULL);
    return 1;
}
