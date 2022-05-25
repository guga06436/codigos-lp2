------------ INSTRUÇÕES DE COMPILAÇÃO E EXECUÇÃO ---------------

O programa deverá receber 3 parâmetros na sua execução: 

- se usará processos [COMANDO: -p] ou threads [COMANDO: -t]; 
- quantas lebres serão criadas para a corrida [COMANDO: NUMERO INTEIRO];
- e um valor inteiro, que representa o tamanho da pista da corrida
  em metros [COMANDO: NUMERO INTEIRO].

Exemplo de execução 1:
  gcc -o ex ex.c -lpthread 
  ./ex -p 5 2

Exemplo de execução 2:
  gcc -o ex ex.c -lpthread
  ./ex -t 5 2

--------------- OBSERVAÇÕES IMPRTANTES --------------------

O programa utiliza de uma função genérica chamada corrida, sendo usada 
tanto para threads quanto para processos.

Essa função recebe como parâmetro o indice da lebre e a distancia maxima
que a lebre salta, esse ultimo que é um numero gerado aleatoriamente entre
30 e 60.

Observa-se que para encerrar o programa utilizou-se a função exit() em threads e
a função kill() em processos. No entanto, como as outras threads/processos estão
sendo executadas em um for, não foi possível encerrar a execução no mesmo instante
que uma das lebres houvesse ganhado.

Para manutenção e aperfeiçoamento do código recomenda-se contornar essa situação
após obtiver os conhecimentos necessários, provavelmente cobrados na segunda
prova.