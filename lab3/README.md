# concurrent-programming
ICP361 - UFRJ /22.2

Wemerson Silva C. da Costa

# Multiplicação de matrizes de forma concorrente - 500 X 500
## Média:
  - 2 Threads:
      - Tempo inicialização: 0.001105667
      - Tempo processamento: 0.173718333
      - Tempo finalização:   0.000207

   - 4 Threads:
      - Tempo inicialização: 0.001139333
      - Tempo processamento: 0.088670667
      - Tempo finalização:   0.000117667
	
  - 8 Threads:
    - Tempo inicialização: 0.001066333
    - Tempo processamento: 0.074045667
    - Tempo finalização:   0.000115667

# Multiplicação de matrizes de forma concorrente - 1000 X 1000
## Média:
 - 2 Threads:
    - Tempo inicialização: 0.008572667
    - Tempo processamento: 1.354392
    - Tempo finalização:   0.000763667
  
 - 4 Threads:
    - Tempo inicialização: 0.003667667
    - Tempo processamento: 0.686892667
    - Tempo finalização:   0.000668667
	
  - 8 Threads:
    - Tempo inicialização: 0.003266333
    - Tempo processamento: 0.511642333
    - Tempo finalização:   0.000730667
  
# Multiplicação de matrizes de forma concorrente - 2000 X 2000
## Média:
 - 2 Threads:
    - Tempo inicialização: 0.028825333
    - Tempo processamento: 13.046820667
    - Tempo finalização:   0.002722667

 - 4 Threads:
    - Tempo inicialização: 0.012098
    - Tempo processamento: 6.300909
    - Tempo finalização:   0.002777333
	
 - 8 Threads:
    - Tempo inicialização: 0.011771333
    - Tempo processamento: 4.624220333
    - Tempo finalização:   0.002513

## Corretude
A corretude dos resultados foi checada utilizando o programa comparaResultado.c, que lê dois arquivos binários que contém duas matrizes:
a primeira é a que calculamos no lab passado, cuja corretude já foi demonstrada, e a segunda é a que estamos calculando nesse lab. 

Após isso, ele checa cada posiçao e vê se em algum momento o módulo da diferença da mesma posição nas duas matrizes ultrapassa a nossa tolerância: $e⁻5$. Se sim, ele interrompe o programa e retorna que a tolerância não foi respeitada, o que nos indica que a forma concorrente do nosso programa foi calculada de forma errônea. 
Senão, ele retorna que a tolerância foi respeitada, o que nos indica que a matriz gerada pela forma concorrente foi calculada corretamente.

## Desempenho
Utilizando a Lei de Amdahl para medirmos o ganho de desempenho:

$$S = \dfrac{T_{seq}}{T_{conc}}$$

Ganho de desempenho que tivemos X Ganho que estimamos no último lab:
  - 500X500, utilizando:
    - 2 Threads: 2.49274 X 1.98793
    - 4 Threads: 4.85118 X 3.94138
    - 8 Threads: 5.79913 X 7.74836
    
  - 1000X1000, utilizando:
    - 2 Threads: 2.59095 X 1.99723
    - 4 Threads: 5.11169 X 3.98345
    - 8 Threads: 6.85237 X 7.92321

  - 2000X2000, utilizando:
    - 2 Threads: 2.43704 X 1.99895
    - 4 Threads: 5.04649 X 3.99373
    - 8 Threads: 6.87130 X 7.97082

Portanto, temos como resultado que usando poucas threads, o resultado obtido superou nossas estimativas, mas aconteceu o contrário quando usamos o número máximo de threads que definimos (8).


## Configurações da máquina: 

OS: Pop!_OS 22.04 LTS

Processador: AMD® Ryzen 5 3600 6-core processor × 12 

GPU: Geforce GTX 1050 Ti

Memória RAM: 2 * 8GB 3000 Mhz

Unidade de Armazenamento: SSD CRUCIAL 480 GB

