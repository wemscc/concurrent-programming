# concurrent-programming
ICP361 - UFRJ /22.2

Wemerson Silva C. da Costa

# Multiplicação de matrizes 500 X 500

- 1ª Iteração:

	- Tempo inicializacao: 0.001184
	- Tempo processamento: 0.434967
	- Tempo finalizacao: 0.000153
- 2ª Iteração:

	- Tempo inicializacao: 0.000801
	- Tempo processamento: 0.437170
	- Tempo finalizacao: 0.000174
- 3ª Iteração:

	- Tempo inicializacao: 0.001089
	- Tempo processamento: 0.433052
	- Tempo finalizacao: 0.000177
## Média: 

	Tempo inicialização: 0.001024667
	Tempo processamento: 0.435063
	Tempo finalização: 0.000168

# Multiplicação de matrizes 1000 X 1000 


- 1ª Iteração:

	- Tempo inicializacao: 0.004203
	- Tempo processamento: 3.549578
	- Tempo finalizacao: 0.000636
  
- 2ª Iteração:

	- Tempo inicializacao: 0.004483
	- Tempo processamento: 3.556310
	- Tempo finalizacao: 0.000736
  
- 3ª Iteração:

	- Tempo inicializacao: 0.004005
	- Tempo processamento: 3.485972
	- Tempo finalizacao: 0.000623
## Média: 

	Tempo inicialização: 0.004230333
	Tempo processamento:  3.53062
	Tempo finalização: 0.000665

# Multiplicação de matrizes 2000 X 2000
- 1ª Iteração:

	- Tempo inicializacao: 0.012456
	- Tempo processamento: 31.895294
	- Tempo finalizacao: 0.002637
- 2ª Iteração:

	- Tempo inicializacao: 0.014405
	- Tempo processamento: 31.730372
	- Tempo finalizacao: 0.002720
- 3ª Iteração:

	- Tempo inicializacao: 0.015184
	- Tempo processamento: 31.942050
	- Tempo finalizacao: 0.002595
## Média: 

	Tempo inicialização: 0.014015
	Tempo processamento: 31.855905333
	Tempo finalização: 0.002650667


# Média das três Matrizes:


 ## Média 500X500: 

	Tempo inicialização: 0.001024667
	Tempo processamento: 0.435063
	Tempo finalização: 0.000168
 ## Média 1000X1000: 

	Tempo inicialização: 0.004230333
	Tempo processamento:  3.53062
	Tempo finalização: 0.000665
## Média 2000x2000: 

	Tempo inicialização: 0.014015
	Tempo processamento: 31.855905333
	Tempo finalização: 0.002650667

# Ganho de desempenho estimado para as operações:
$T_s$ : Tempo total de execução do problema sequencial;

$t_s$ : tempo da parte sequencial do programa que não será dividida entre threads

$t_p$ : tempo da parte sequencial do programa que será dividida entre n threads

$N$   : numero de threads



$S = \dfrac{T_s}{t_s+\dfrac{t_p}{N}}$


  ## 500X500, utilizando:
  
    -2 Threads: S = 1.98793...
    -4 Threads: S = 3.94138...
    -8 Threads: S = 7.74836...
    
  ## 1000X1000, utilizando:
  
    -2 Threads: S = 1.99723...
    -4 Threads: S = 3.98345...
    -8 Threads: S = 7.92321...
  
 ## 2000X2000, utilizando:
  
    -2 Threads: S = 1.99895...
    -4 Threads: S = 3.99373...
    -8 Threads: S = 7.97082...
