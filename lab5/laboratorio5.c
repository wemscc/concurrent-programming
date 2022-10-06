#include <stdio.h>
#include <pthread.h>
#define NTHREADS 5

int estado = 0;
pthread_mutex_t x_mutex;
pthread_cond_t thread_4_cond;
pthread_cond_t thread_1_cond;
void *A(void *arg)
{
    pthread_mutex_lock(&x_mutex);
    if (estado < 3)
    {
        pthread_cond_wait(&thread_1_cond, &x_mutex);
        printf("Volte sempre!\n");
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}
void *B(void *arg)
{
    char *string = (char*) arg;
    pthread_mutex_lock(&x_mutex);
    if (estado == 0)
    {
        pthread_cond_wait(&thread_4_cond, &x_mutex);
        printf("%s\n", string);
        estado++;
    }
    if (estado == 3)
        pthread_cond_signal(&thread_1_cond);
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

void *C(void *arg)
{
    printf("Seja bem-vindo!\n");
    pthread_mutex_lock(&x_mutex);
    estado++;
    pthread_cond_broadcast(&thread_4_cond);
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

int main(void)
{
    int i;
    pthread_t threads[NTHREADS];
    /* Inicilaiza o mutex (lock de exclusao mutua) e as variaveis de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&thread_4_cond, NULL);
    pthread_cond_init(&thread_1_cond, NULL);
    /* Cria as threads */
    pthread_create(&threads[0], NULL, A, NULL);
    pthread_create(&threads[1], NULL, B, (void *)"Sente-se por favor");
    pthread_create(&threads[2], NULL, B, (void *)"Fique a vontade");
    pthread_create(&threads[3], NULL, C, NULL);
    for (i = 0; i < NTHREADS - 1; i++) //espera todas as threads terminarem
    {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&thread_1_cond);
    pthread_cond_destroy(&thread_4_cond);
    return 0;
}
