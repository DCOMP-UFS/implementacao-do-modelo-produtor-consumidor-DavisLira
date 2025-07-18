Com base no exemplo pth_pool.c, implemente o modelo de comunicação entre threads Produtor/Consumidor usando uma fila intermediária controlada por variáveis de condição. Enquanto um grupo de 3 threads produzirá relógios vetoriais que serão colocados na fila, outro grupo de 3 threads consumirá esses relógios da fila e os imprimirá na saída padrão. Para verificar o uso correto das variáveis de condição na sua tarefa de sincronização entre produtores e consumidores, crie dois cenários de teste:
Cenário onde a fila ficará cheia: Threads produtoras produzem relógios na fila em uma taxa maior que as threads consumidoras. Por exemplo, em quanto cada thread produtora produz 1 relógio na fila a cada segundo, cada thread consome 1 relógio da fila a cada dois segundos. 
Cenário onde a fila ficará vazia: Threads produtoras produzem relógios na fila em uma taxa menor que as threads consumidoras. Por exemplo, em quanto cada thread produtora produz 1 relógio na fila a cada 2 segundos, cada thread consome 1 relógio da fila a cada 1 segundo.


Para rodar:

gcc -o fila_cheia fila_cheia.c -lpthread
./fila_cheia
