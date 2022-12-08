
Abaixo seguirá o log (ou parte dele) de uma execução do programa
```
0 0 0 0 0  				// Esse print serve apenas para mostrar o buffer inicializado, e nessa hora, liberamos o primeiro produtor
Sou a thread produtora 1
P[1] quer inserir
P[1] inseriu
1 2 3 4 5 				// A primeira produtora consegue preencher o buffer
Sou a thread produtora 2
P[2] quer inserir 			// Nao consegue inserir, pois o buffer está cheio
Sou a thread produtora 3
P[3] quer inserir 			// Nao consegue inserir porque o buffer está cheio
Sou a thread consumidora 4		//Primeira thread consumidora aparece
C[4] quer consumir
C[4] consumiu 1				// Consumidor consome tranquilamente, pois o buffer está quase cheio.
0 2 3 4 5 
Sou a thread consumidora 5
C[5] quer consumir
C[5] consumiu 2				 //  Consumidor consome tranquilamente, pois o buffer ainda tem itens.
0 0 3 4 5 
Sou a thread consumidora 6
C[6] quer consumir
C[6] consumiu 3
0 0 0 4 5 Sou a thread consumidora 7     // Consumidor consome tranquilamente, pois o buffer ainda tem itens.
C[7] quer consumir

C[7] consumiu 4
0 0 0 0 5 				 // Consumidor consome tranquilamente, pois o buffer ainda tem itens.
P[1] quer inserir
C[4] quer consumir
C[4] consumiu 5
0 0 0 0 0 				 // Consumidor consome o último item do buffer, liberando um produtor.
P[2] inseriu				
1 2 3 4 5 				// Como o buffer acabou de ficar vazio, o produtor que estava esperando vai lá e preenche o buffer
C[5] quer consumir
C[5] consumiu 1
0 2 3 4 5  				// Consumidor consome tranquilamente, pois o buffer está cheio
C[6] quer consumir
C[6] consumiu 2
0 0 3 4 5 				// Consumidor consome tranquilamente, pois o buffer ainda tem itens.
C[7] quer consumir
C[7] consumiu 3
0 0 0 4 5 				// Consumidor consome tranquilamente, pois o buffer ainda tem itens
C[4] quer consumir
C[4] consumiu 4
0 0 0 0 5 P[2] quer inserir 		// Produtor não consegue inserir, pois ainda há um item no buffer

C[5] quer consumir
C[5] consumiu 5
0 0 0 0 0 				// Consumidor consome o último item do buffer, liberando um produtor
P[3] inseriu		
1 2 3 4 5 				// Embora o P[2] estivesse querendo inserir, o P[3] passou na frente dele e preencheu o buffer

```

Portanto, podemos verificar a corretude do programa:
	- Os produtores preenchem o buffer completamente quando é sua vez de inserir
	- Os consumidores consomem apenas 1 item por vez, e na ordem que os mesmos foram colocados
	- Os produtores que tentam colocar coisa no buffer que não está vazio tem que esperar
	- Os consumidores não tiram os itens enquanto os produtores não terminaram de pegar (isso no caso foi mais uma escolha de implementação do que necessariamente um sinal de corretude)	

