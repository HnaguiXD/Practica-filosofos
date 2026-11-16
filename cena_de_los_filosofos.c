#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_FILOSOFOS 5
#define COMIDAS 3

sem_t tenedores[NUM_FILOSOFOS];

void* filosofo(void* num) {
    int id = *(int*)num;
    int izq = id;
    int der = (id + 1) % NUM_FILOSOFOS;

    for (int i = 1; i <= COMIDAS; i++) {
        printf("Filósofo %d está pensando...\n", id);
        sleep(rand() % 2 + 1);

        printf("Filósofo %d tiene hambre.\n", id);
        if (id % 2 == 0) {
            sem_wait(&tenedores[der]);
            printf("Filósofo %d tomó el tenedor derecho (%d).\n", id, der);
            sem_wait(&tenedores[izq]);
            printf("Filósofo %d tomó el tenedor izquierdo (%d).\n", id, izq);
        } else {
            sem_wait(&tenedores[izq]);
            printf("Filósofo %d tomó el tenedor izquierdo (%d).\n", id, izq);
            sem_wait(&tenedores[der]);
            printf("Filósofo %d tomó el tenedor derecho (%d).\n", id, der);
        }

        printf(">>> Filósofo %d está COMIENDO (comida %d/%d) <<<\n", id, i, COMIDAS);
        sleep(rand() % 2 + 1);

        sem_post(&tenedores[izq]);
        sem_post(&tenedores[der]);
        printf("Filósofo %d terminó de comer y soltó los tenedores (%d y %d).\n", id, izq, der);
    }

    printf("*** Filósofo %d ha terminado todas sus comidas y se retira de la mesa. ***\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    srand(time(NULL));

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        if (sem_init(&tenedores[i], 0, 1) != 0) {
            perror("Error al inicializar semáforo");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        if (pthread_create(&hilos[i], NULL, filosofo, &ids[i]) != 0) {
            perror("Error al crear el hilo");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        sem_destroy(&tenedores[i]);
    }

    printf("\nLa cena ha finalizado sin interbloqueos.\n");
    return 0;
}
