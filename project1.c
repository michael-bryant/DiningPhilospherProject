#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int state[N];
int phil[N] = {0, 1, 2, 3, 4};

sem_t mutex;
sem_t S[N];

void test(int phnum){
	if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
		state[phnum] = EATING;
		sleep(2);
		printf("Philosopher %d takes sticks %d and %d\n", phnum +1, LEFT +1, phnum + 1);
		printf("Philosopher %d is eating\n", phnum+1);
		sem_post(&S[phnum]);
		}
	}

void take_stick(int phnum){
	sem_wait(&mutex);
	//declare hunger
	state[phnum] = HUNGRY;
	printf("Philosopher %d is hungry\n", phnum + 1);
	//eat if sticks available
	test(phnum);
	sem_post(&mutex);
	//if no sticks available, wait to be signalled
	sem_wait(&S[phnum]);
	sleep(1);
	}
void put_stick(int phnum){
	sem_wait(&mutex);
	//declare change to thinking state
	state[phnum] = THINKING;

	printf("Philosopher %d dropping sticks %d and %d\n", phnum+1, LEFT+1, phnum+1);
	printf("Philosopher %d is thinking\n", phnum+1);

	test(LEFT);
	test(RIGHT);

	sem_post(&mutex);
	}

void* philosopher(void* num){
	while(1){
		int* i = num;
		sleep(1);
		take_stick(*i);
		sleep(0);
		put_stick(*i);
		}
	}

int main(){
	int i;
	pthread_t thread_id[N];
	//initialize semaphores
	sem_init(&mutex, 0, 1);

	for(i = 0; i < N; i++){
		sem_init(&S[i], 0, 0);
	}
	for(i = 0; i < N; i++){
		pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
		printf("Philosopher %d is thinking\n", i+1);
	}
	for(i = 0; i < N; i++){
		pthread_join(thread_id[i], NULL);
	}
}
