#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#define TOL 1e-15
#define NTHREADS 4

/* Variaveis globais */
pthread_mutex_t x_mutex;
int qntTrapezios = 4;
double integral = 0, integral_nova = 0;
double h = 0;

int j = 0;
int a = 2, b = 4;

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
    int inicio, fim;
} tArgs;

double f(double x)
{
    return x * x;
    //    double aux = pow(10,7);
    //  double aux2 = pow(x,3);
    // return ((-2*x) + ((3*aux2)/aux) );
}

double trapezoidal_seq(double f(double x), double a, double b, int n)
{
    double x, h, soma_local = 0, integral;
    int i;
    h = fabs(b - a) / n;
    for (i = 1; i < n; i++)
    {
        x = a + i * h;
        soma_local = soma_local + f(x);
    }
    integral = (h / 2) * (f(a) + f(b) + 2 * soma_local);
    return integral;
}

void *tarefa(void *arg)
{
    tArgs *args = (tArgs *)arg;
    int i, intervalo;
    int id = args->id;
    double somaLocal = 0;
    double x;
    intervalo = qntTrapezios / NTHREADS;
    args->inicio = id * intervalo;
    args->fim = args->inicio + intervalo;
    if (qntTrapezios % NTHREADS != 0)
    {
        if (id == (NTHREADS - 1))
            args->fim += (qntTrapezios % NTHREADS);
    }
    h = fabs(b - a) / qntTrapezios;
    for (int i = args->inicio; i < args->fim; i++)
    {
        x = a + i * h;
        somaLocal += f(x);
    }
    pthread_mutex_lock(&x_mutex);
    integral_nova += (2 * somaLocal);
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

int main()
{
    int i;
    double inicio, fim, delta, diferenca;
    pthread_mutex_init(&x_mutex, NULL);
    tArgs *args;
    pthread_t threads[NTHREADS];
    args = (tArgs *)malloc(sizeof(tArgs) * NTHREADS);
    /* Cria as threads */
    GET_TIME(inicio);
    do
    {
        integral = integral_nova;
        for (int i = 0; i < NTHREADS; i++)
        {
            (args + i)->id = i;
            (args + i)->inicio = 0;
            (args + i)->fim = 0;
            if (pthread_create(&threads[i], NULL, tarefa, (void *)(args + i)))
            {
                fprintf(stderr, "ERRO--pthread_create\n");
                return 3;
            }
        }
        /* Espera todas as threads completarem */
        for (int i = 0; i < NTHREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }
        integral_nova += f(a) + f(b);
        integral_nova *= (h / 2);
        qntTrapezios = qntTrapezios * 1.5;
        diferenca = fabs(integral_nova - integral);
        j++;
    } while (diferenca >= TOL && j < 30);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de processamento concorrente: %lf", delta);
    printf("\nO resultado da aproximação concorrente é: %lf\n\n", integral_nova);
    double res_conc = integral_nova;
    // Forma  Sequencial:
    GET_TIME(inicio);
    integral_nova = trapezoidal_seq(f, a, b, i);
    do
    {
        integral = integral_nova;
        i++;
        integral_nova = trapezoidal_seq(f, a, b, i);
    } while (fabs(integral_nova - integral) >= TOL);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de processamento sequencial: %lf", delta);
    /*Print the answer */
    printf("\nO resultado da aproximação sequencial é: %lf", integral_nova);
    // Compara os resultados
    diferenca = fabs(res_conc - integral_nova);
    diferenca = (diferenca * 100) / integral_nova; // calculando o erro relativo
    printf("\n\nA diferença relativa entre os dois metodos é de %.10f%%\n", diferenca);

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
}