#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>


#define N_THREADS 8





typedef struct options{
	pthread_mutex_t* even_mutex;
	pthread_mutex_t* odd_mutex;
	unsigned long* n_odd;
	unsigned long* n_even;
	int counter;
}options;

void *counter(void *arg) {
	options* opcije = (options*)arg;
	int n;
	double d; 
	for (int k=0; k<opcije->counter; k++) {
		if(*(opcije->n_odd) >= 18000 || *(opcije->n_even) >= 18000)
			pthread_exit(NULL);
		n = rand()%100;
		for (int j=0; j<n; j++)
			d = sqrt((double)j);
		if (n%2 == 0){
			pthread_mutex_lock(opcije->even_mutex);
			*(opcije->n_even)+=1;
			pthread_mutex_unlock(opcije->even_mutex);
		}
		else{
			pthread_mutex_lock(opcije->odd_mutex);
			*(opcije->n_odd)+=1;
			pthread_mutex_unlock(opcije->odd_mutex);
		}	
	}
	
	pthread_exit(NULL);
}

int main() {
	int cnt, k;
	pthread_t thr_counter[N_THREADS];
	srand(time(NULL));
	
	pthread_mutex_t* even_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_t* odd_mutex = malloc(sizeof(pthread_mutex_t));
	unsigned long* n_odd = malloc(sizeof(unsigned long));
	unsigned long* n_even = malloc(sizeof(unsigned long));
	
	options* opcije[N_THREADS];
	
	for (k=0; k<N_THREADS; k++) {
		opcije[k] = malloc(sizeof(options));
		opcije[k]->even_mutex=even_mutex;
		opcije[k]->odd_mutex=odd_mutex;
		opcije[k]->n_odd=n_odd;
		opcije[k]->n_even=n_even;
		opcije[k]->counter=(k+1)*1000;
		pthread_create(&thr_counter[k], NULL, counter, opcije[k]);
	}
	
	for (k=0; k<N_THREADS; k++) {
		pthread_join(thr_counter[k], NULL);
		free(opcije[k]);
	}
	
	
	printf("Broj parnih:   \t %lu\n", *n_even);
	printf("Broj neparnih: \t %lu\n", *n_odd);
	printf("Ukupno:        \t %lu\n", *n_odd+*n_even);
	
	free(n_odd);
	free(n_even);
	free(even_mutex);
	free(odd_mutex);
	return 0;
}

