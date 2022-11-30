#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>


#define N_THREADS 8





typedef struct options{
	unsigned long* n_odd;
	unsigned long* n_even;
	int counter;
}options;

void *counter(void *arg) {
	options* opcije = (options*)arg;
	int n;
	double d; 
	for (int k=0; k<opcije->counter; k++) {
		n = rand()%100;
		for (int j=0; j<n; j++)
			d = sqrt((double)j);
		if (n%2 == 0){			
			*(opcije->n_even)+=1;			
		}
		else{			
			*(opcije->n_odd)+=1;
		}	
	}
	
	pthread_exit(NULL);
}

int main() {
	int cnt, k;
	pthread_t thr_counter[N_THREADS];
	srand(time(NULL));
	
	unsigned long n_odd=0;
	unsigned long n_even=0;
	
	options* opcije[N_THREADS];
	
	for (k=0; k<N_THREADS; k++) {
		opcije[k] = malloc(sizeof(options));
		opcije[k]->n_odd=malloc(sizeof(unsigned long));
		opcije[k]->n_even=malloc(sizeof(unsigned long));
		opcije[k]->counter=(k+1)*1000;
		pthread_create(&thr_counter[k], NULL, counter, opcije[k]);
	}
	
	for (k=0; k<N_THREADS; k++) {
		pthread_join(thr_counter[k], NULL);
		n_odd+=*(opcije[k]->n_odd);
		n_even+=*(opcije[k]->n_even);
		free(opcije[k]->n_odd);
		free(opcije[k]->n_even);
		free(opcije[k]);
	}
	
	
	printf("Broj parnih:   \t %lu\n", n_even);
	printf("Broj neparnih: \t %lu\n", n_odd);
	printf("Ukupno:        \t %lu\n", n_odd+n_even);
	
	
	return 0;
}

