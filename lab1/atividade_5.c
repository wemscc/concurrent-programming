#include <stdio.h>
#include <pthread.h>
#define NTHREADS 2 
#define TAM 20 
float vetor[TAM];
int intervalo = TAM/NTHREADS; // o número de posições do vetor que cada thread deve calcular de maneira que cada thread calcule a mesma quantidade
void * tarefa (void * arg){
    int i =  *((int *) arg);
    int inicio = i * intervalo;  // calcula a posição inicial do intervalo de posições que a thread i deve calcular
    int final = (i+1) * intervalo; // calcula a posicao final do intervalo de posições que a thread i deve calcular
    int j; //variavel de iteração
    for (j=inicio;j<final;j++){ // a thread calcula em todas as posicoes do vetor nesse intervalo a função que queremos (intervalo explicado acima)
        vetor[j] *= 1.1; // aumenta em 10% na posição original do vetor
    } 
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[NTHREADS]; //identificadores das threads no sistema
    int ident[NTHREADS]; //identificador local da thread
    int i; //variável auxiliar
    float verifica; //variavel auxiliar
    int contador = 0; // variavel lógica auxiliar
    for (i=0;i<TAM;i++) 
        vetor[i] = i + 1; // preenche o vetor 
    for (i=0;i<NTHREADS;i++){ 
        ident[i] = i;
        if (pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i]))  // cria as threads
            printf ("Erro - pthread_create\n");
    }
    for (i=0;i<NTHREADS;i++){
        if (pthread_join(tid[i], NULL)) // garante que a mensagem final vai ser executada depois que todas as threads terminarem o cálculo
            printf ("Erro - pthread_create\n");
    }
    for (i=0;i<TAM;i++){ 
        verifica = (i+1)*1.1; // essa variável calcula a lei de formação que aplicamos durante o programa no nosso vetor, pegando os mesmos números iniciais e aplicando a mesma operação
        if (vetor[i] != verifica){ // se um número for diferente do resultado esperado, damos como FALSE o resultado da nossa operação lógica
            break; // e saímos do for
        }
        contador = 1; // se o programa chegou até aqui, significa que ele calculou todos os números corretamente, isto é, nunca deu break, e portanto o resultado da nossa operação lógica é TRUE
    }
    if (contador) // só entra aqui se o que veio do for anterior é TRUE, isto é, todos os valores estão corretos
        printf("Todos os elementos foram calculados corretamente.\n"); 
    else        //  entra aqui mesmo que apenas um valor do vetor tenha dado errado.
        printf("Erro! Valores preenchidos erroneamente.\n");
    return 0;
}
