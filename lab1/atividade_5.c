#include <stdio.h>
#include <pthread.h>
#define NTHREADS 2 
#define TAM 10000 
float vetor[TAM];

void * tarefa (void * arg){
    int posicao = * ((float *) arg); // variável para armazenar a posição do vetor que estamos olhando agora
    vetor[posicao-1] *= 1.1; // aplica a operação que queremos
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[NTHREADS]; //identificadores das threads no sistema
    int i,j,final,inicio,intervalo; //variaveis auxiliares
    float verifica; //variavel auxiliar
    int contador = 0; // variavel lógica auxiliar
    intervalo = TAM/NTHREADS; // o número de posições do vetor que cada thread deve calcular de maneira que cada thread calcule a mesma quantidade
    for (i=0;i<TAM;i++) 
        vetor[i] = i + 1; // preenche o vetor 
    for (i=0;i<NTHREADS;i++){ 
        inicio = i * intervalo; // calcula a posição inicial do intervalo de posições que a thread i deve calcular
        final = (i+1) * intervalo; // calcula a posicao final do intervalo de posições que a thread i deve calcular
        for (j=inicio;j<final;j++){ // a thread i calcula todas as posicoes nesse intervalo (explicado acima)
            if (pthread_create(&tid[i], NULL, tarefa, (void *)&vetor[j]))  // cria as threads
                    printf ("Erro - pthread_create\n");
        }
    }
    for (i=0;i<NTHREADS;i++){
        if (pthread_join(tid[i], NULL)) // garante que a mensagem final vai ser executada depois que todas as threads terminarem o cálculo
            printf ("Erro - pthread_create\n");
    }
    for (i=0;i<TAM;i++){ 
        verifica = (i+1)*1.1; // essa variável calcula a lei de formação que aplicamos durante o programa no nosso vetor, pegando os mesmos números iniciais e aplicando a mesma operação
        if (vetor[i] != verifica){ 
            contador = 0; // se um número for diferente do resultado esperado, damos como FALSE o resultado da nossa operação lógica
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
