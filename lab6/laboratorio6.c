#include <stdio.h>
#include <pthread.h>

#define X 1 // thread que só escreve
#define Y 2 // thread que só lê
#define Z 3 // thread que lê e escreve

int base = 0;           // variável do problema
int qnt_leitores = 0;   // quantidade de leitores
int qnt_escritores = 0; // quantidade de escritores

// variaveis para sincronização
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

// entrada leitura
void InicLeit(int id)
{
    pthread_mutex_lock(&mutex);
    printf("L[%d] quer ler\n", id);
    while (qnt_escritores > 0)
    {
        printf("L[%d] bloqueou\n", id);
        pthread_cond_wait(&cond_leit, &mutex);
        printf("L[%d] desbloqueou\n", id);
    }
    qnt_leitores++;
    pthread_mutex_unlock(&mutex);
}

// saida leitura
void FimLeit(int id)
{
    pthread_mutex_lock(&mutex);
    printf("L[%d] terminou de ler\n", id);
    qnt_leitores--;
    if (qnt_leitores == 0)
        pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}

// entrada escrita
void InicEscr(int id)
{
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);
    while ((qnt_leitores > 0) || (qnt_escritores > 0))
    {
        printf("E[%d] bloqueou\n", id);
        pthread_cond_wait(&cond_escr, &mutex);
        printf("E[%d] desbloqueou\n", id);
    }
    qnt_escritores++;
    pthread_mutex_unlock(&mutex);
}

// saida escrita
void FimEscr(int id)
{
    pthread_mutex_lock(&mutex);
    printf("E[%d] terminou de escrever\n", id);
    qnt_escritores--;
    pthread_cond_signal(&cond_escr);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

// thread1
void *T1(void *arg)
{
    int *id = (int *)arg;
    while (1)
    {
        InicEscr(*id);
        printf("Escritor[%d] está incrementando a variável em uma unidade.\n", *id);
        base++;
        FimEscr(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

// thread2
void *T2(void *arg)
{
    int *id = (int *)arg;

    while (1)
    {
        InicLeit(*id);
        printf("Leitor[%d] está lendo\n", *id);
        if (base % 2 == 0)
            printf("O numero %d é par \n", base);
        else
            printf("O numero %d é impar\n", base);
        FimLeit(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

// thread3
void *T3(void *arg)
{
    int *id = (int *)arg;
    int boba1, boba2;
    while (1)
    {
        InicLeit(*id);
        printf("LeitorEscritor[%d] está lendo a variavel\n", *id);
        printf("LeitorEscritor[%d], base = %d\n", *id, base);
        FimLeit(*id);
        boba1 = 100;
        boba2 = 10000;
        while (boba1 < boba2)
            boba1++; // processamento bobo
        InicEscr(*id);
        printf("LeitorEscritor[%d] está escrevendo o valor do seu identificador na variável base\n", *id);
        base = *id;
        FimEscr(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[X + Y + Z];
    int id[X + Y + Z];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_escr, NULL);
    // cria as threads que so escrevem
    for (int i = 0; i < X; i++)
    {
        id[i] = i + 1;
        if (pthread_create(&tid[i], NULL, T1, (void *)&id[i]))
            exit(-1);
    }

    // cria as threads que so leem
    for (int i = 0; i < Y; i++)
    {
        id[i + X] = i + 1;
        if (pthread_create(&tid[i + X], NULL, T2, (void *)&id[i + X]))
            exit(-1);
    }

    // cria as threads que so leem e escrevem
    for (int i = 0; i < Z; i++)
    {
        id[i + X + Y] = i + 1;
        if (pthread_create(&tid[i + X + Y], NULL, T3, (void *)&id[i + X + Y]))
            exit(-1);
    }

    pthread_exit(NULL);
    return 0;
}