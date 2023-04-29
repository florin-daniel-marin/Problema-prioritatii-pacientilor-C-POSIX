#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define THREAD_NUM 5

//semafoare si mutex-uri:
sem_t sG, sM, sR;
sem_t emptySeats, occupiedSeats, monitor;
sem_t barrierM, barrierR;
pthread_mutex_t mutexQueue, mutexCabinet;

int isG = 0; 
int isM = 0; //val conditie 0: exista; 1: nu exista pacient
int isBarrier = 0;
int count = 0;


void* addQueue (void* addQueue){
	for(int i = 0; i < 15; i++){		
		//random number 1,2,3:	
		int priority = rand() % 3 + 1;
		
		sem_wait(&emptySeats);
		sem_post(&occupiedSeats);
		
		pthread_mutex_lock(&mutexQueue);
		//add pacient Grav: G
			if (priority == 3){
				sem_post(&sG);
				// Pacienti
        		printf("<---------------------------Pacient 'G' a intrat in sala de asteptare\n");
			}

		//add pacient Mediu: M
			if (priority == 2){
				sem_post(&sM);
				// Pacienti
        		printf("<---------------------------Pacient 'M' a intrat in sala de asteptare\n");		
			}

		//add pacient Redus: R
			if (priority == 1){
				sem_post(&sR);
				// Pacienti
        		printf("<---------------------------Pacient 'R' a intrat in sala de asteptare\n");	
			}
		pthread_mutex_unlock(&mutexQueue);	
		
		float timp_adaugare = rand() /(float)RAND_MAX * 6 + 0.5;
		sleep(timp_adaugare);
	}
}

void* G (void* args){
	while (1){
		isG = 0;																			//niciun G in coada
		
		sem_wait(&sG);																		//exista G, intra in cabinet
		
		isG = 1;																			//G in cabinet
		
		sleep(0.1);																		//timpul de parcurs pana in cabinet
		
		pthread_mutex_lock(&mutexCabinet);
		//---shared resource---
			printf("\nnr.%d Pacientul G a intrat in cabinet!\n", count);
			float timp_consultare = rand() /(float)RAND_MAX * 4.5 + 4.5;
			sleep ( timp_consultare);
			printf("Pacientul 'G' a iesit din cabinet dupa %.2f min\n", timp_consultare*10);
		pthread_mutex_unlock(&mutexCabinet);

		sleep(0.1);																		//timpul de realizare cabinet gol
		
		sem_post(&emptySeats);
		sem_post(&monitor);
	}
}

void* M (void* args){
	while (1){
		//sem_wait(&barrierM);															//Nu este G, M poate intra
		isM = 0;																		//niciun M in coada
		isBarrier = 0;																	//bariera este deschisa pentru R pt ca M nu a intrat in cabinet
		
		sem_wait(&sM);																	//Exista M in coada, asteapta sa intre in cabinet
		sem_wait(&barrierM);															//Nu este G, M poate intra
		isM = 1;																		//M in cabinet
		isBarrier = 1;																	//bariera inchisa pentru R
		//printf("test->bariera inchisa pentru R!\n");

		sleep(0.1);																		//timpul de parcurs pana in cabinet
		
		pthread_mutex_lock(&mutexCabinet);
		//---shared resource---
			printf("\nnr.%d Pacientul M a intrat in cabinet!\n", count);
			float timp_consultare = rand() /(float)RAND_MAX * 3 + 3;
			sleep ( timp_consultare);
			printf("Pacientul 'M' a iesit din cabinet dupa %.2f min\n", timp_consultare*10);
		pthread_mutex_unlock(&mutexCabinet);

		sleep(0.1);																		//timpul de realizare cabinet gol
		sem_post(&emptySeats);
		sem_post(&monitor);
		
	}
}

void* R (void* args){
	while (1){
		sem_wait(&sR);																	//Exista R in coada, asteapta sa intre in cabinet
		sleep(0.5);																		//timpul de asteptare raspuns monitorizare pacienti
		sem_wait(&barrierR);															//Nu este nici M, nici G, R poate intra

		sleep(0.1);																		//timpul de parcurs pana in cabinet
		
		pthread_mutex_lock(&mutexCabinet);
		//---shared resource---	
			printf("\nnr.%d Pacientul R a intrat in cabinet!\n", count);
			float timp_consultare = rand() /(float)RAND_MAX * 2 + 1;
			sleep (timp_consultare);
			printf("Pacientul 'R' a iesit din cabinet dupa %.2f min\n", timp_consultare*10);
		pthread_mutex_unlock(&mutexCabinet);

		sleep(0.1);																		//timpul de realizare cabinet gol 
		
		sem_post(&emptySeats);
		sem_post(&monitor);
		
	}
}

void* monitorizare (void* args){
	while (1){
		sem_wait(&monitor);																	//usa cabinetului s-a deschis, se cauta pacientul
		isBarrier = 0;																		//M nu a intrat, R poate intra
		sleep(0.15);																		//asteapta sa se gaseasca G in coada
		
		pthread_mutex_lock(&mutexQueue);	
		if (isG == 0){																		//nu e G si niciun G in asteptare
			sem_post(&barrierM);															//M poate intra, G nu este
		//printf("test->bariera M ridicata!\n");
		}
		pthread_mutex_unlock(&mutexQueue);
		
		sleep (0.15);																		//asteapta sa se gaseasca M in coada
		
		pthread_mutex_lock(&mutexQueue);
		if (isG == 0 && isM == 0 && isBarrier == 0){																		//nu e G, nici M si nici M in asteptare
			sem_post(&barrierR);															//R poate intra, G si M nu sunt
			//printf("test->bariera R ridicata!\n");
		}
		pthread_mutex_unlock(&mutexQueue);
		//exit conditions:
		count++;
		if (count == 10){
			sleep(12);
			exit(0);
		}
	}
}


void* (*functie [])(void*) = {addQueue, G, M, R, monitorizare};

int main(int argc, char* argv[]){
	int i;
	srand(time(NULL));
	pthread_t th[THREAD_NUM];

    sem_init(&sG, 0, 0);
    sem_init(&sM, 0, 0);
    sem_init(&sR, 0, 0);
	sem_init(&occupiedSeats, 0, 0);
    sem_init(&emptySeats, 0, 10);
	sem_init(&monitor, 0, 1);
	sem_init(&barrierM, 0, 0);
	sem_init(&barrierR, 0, 0);

    pthread_mutex_init(&mutexQueue, NULL);
	pthread_mutex_init(&mutexCabinet, NULL);

	for (i = 0; i < THREAD_NUM; i++){
        if (pthread_create(&th[i], NULL,  (void*)(*(functie+i)), NULL) != 0){
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++){
        if (pthread_join(th[i], NULL) != 0){
            perror("Failed to join thread");
        }
    }

    sem_destroy(&sG);
    sem_destroy(&sM);
    sem_destroy(&sR);
	sem_destroy(&occupiedSeats);
    sem_destroy(&emptySeats);
    sem_destroy(&monitor);
	sem_destroy(&barrierM);
	sem_destroy(&barrierR);
    pthread_mutex_destroy(&mutexQueue);
	pthread_mutex_destroy(&mutexCabinet);
    pthread_exit(NULL);
}
