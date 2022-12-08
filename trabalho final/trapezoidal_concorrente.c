#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#define TOL 1e-8

/* Variaveis globais */
pthread_mutex_t x_mutex;
int qntTrapezios = 8;                       // chute inicial de trapézios
double integral = 0, integral_auxiliar = 0; // integra
double h = 0;                               // largura do trapézio
int NTHREADS;                               //numero de threads
int max_iter = 35;                          // numero de iterações máximo
int a = 0, b = 1000000;                     // limites de integração

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
    int id;          // id da thread
    int inicio, fim; // variáveis que guardam as posições iniciais e finais de cada thread
} tArgs;

double f(double x) // funcao a ser integrada
{
    return x * x;
}
double trapezoidal_seq(double f(double x), double a, double b, int n) // implementação sequencial do problema
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
    double somaLocal = 0;                // variavel acumuladora da função
    double x;                            // variável que será utilizada para calcular valor da função no ponto
    intervalo = qntTrapezios / NTHREADS; // tamanho do intervalo de cada thread
    pthread_mutex_lock(&x_mutex);
    h = fabs(b - a) / qntTrapezios; // vai diminuindo a largura do trapézio conforme vamos aumentando o número dos mesmos, com exclusão mutua já que a variável é global
    pthread_mutex_unlock(&x_mutex);
    args->inicio = id * intervalo;        // atribui a posição inicial de onde a thread deve começar a calcular
    args->fim = args->inicio + intervalo; // atribui a posição final de onde a thread deve começar a calcular
    if (qntTrapezios % NTHREADS != 0)
    {
        if (id == (NTHREADS - 1))
            args->fim += (qntTrapezios % NTHREADS); // coloca para a última thread calcular caso tenha resto na divisão de tarefas
    }

    for (int i = args->inicio; i < args->fim; i++)
    {
        x = a + i * h;     //   atribui o ponto x dentro do intervalo
        somaLocal += f(x); // calcula a função no ponto x e poe no acumulador
    }
    pthread_mutex_lock(&x_mutex);
    integral_auxiliar += (2 * somaLocal); // vai atualizando o valor da integral conforme vamos calculando os pontos, com exclusão mutua já que a variável é global
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i;
    double inicio, fim, delta, diferenca;
    pthread_mutex_init(&x_mutex, NULL);
    NTHREADS = atoi(argv[1]);
    tArgs *args;
    pthread_t threads[NTHREADS];
    args = (tArgs *)malloc(sizeof(tArgs) * NTHREADS);
    /* Cria as threads */
    GET_TIME(inicio);
    do
    {
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
        integral_auxiliar += f(a) + f(b);                // calcula o resto da expressao agora que todos os pontos foram calculados
        integral_auxiliar = integral_auxiliar * (h / 2); // calcula o resto da expressao agora que todos os pontos foram calculados
        diferenca = fabs(integral_auxiliar - integral);  // calcula a diferenca da integral que acabamos de calcular com a que já tinhamos calculado
        diferenca = diferenca / integral_auxiliar;
        integral = integral_auxiliar;                    // agora a nova integral vira a velha
        integral_auxiliar = 0;                           // e criamos uma nova integral
        qntTrapezios = qntTrapezios * 1.5;               // vamos aumentando o numero de trapézios para aumentar a precisão
        max_iter--;                                      // condição de parada caso entremos num cenário que esteja demorando muito
    } while (diferenca > TOL && max_iter > 0);           // condição de parada: atingimos o limite de tolerancia ou acabaram nossas iterações disponíveis.
    GET_TIME(fim);
    delta = fim - inicio;
    /*Mostra o resultado concorrente */
    printf("\nTempo de processamento concorrente: %lf", delta);
    printf("\nO resultado da aproximação concorrente é: %lf\n\n", integral);
    printf ("\n Qnt de trapezios: %d \n",qntTrapezios);
    double res_conc = integral;
    // Forma  Sequencial:
    GET_TIME(inicio);
    i = 0;
    integral_auxiliar = trapezoidal_seq(f, a, b, i);
    do
    {
        integral = integral_auxiliar;
        i++;
        integral_auxiliar = trapezoidal_seq(f, a, b, i);
    } while (fabs(integral_auxiliar - integral) >= TOL); // condição de parada: atingimos o limite de tolerancia
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de processamento sequencial: %lf", delta);
    /*Mostra o resultado sequencial */
    printf("\nO resultado da aproximação sequencial é: %lf ", integral_auxiliar);
    printf ("\n Qnt de trapezios: %d \n",i);
    // Compara os resultados
    diferenca = fabs(res_conc - integral_auxiliar);
    diferenca = diferenca / res_conc; // calculando o erro relativo
    printf("\n\nA diferença relativa entre os dois metodos é de %.10f%%\n", diferenca * 100);
    if (diferenca > TOL)
        printf("\nFalhou no teste de corretude.");
    else
        printf("\nCorretude provada!");
    /* Desaloca variaveis e termina */
    GET_TIME(inicio);
    pthread_mutex_destroy(&x_mutex);
    free(args);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de finalização: %lf", delta);

    return 0;
}
