#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>


#define N_THREADS 8
pthread_t thr_counter[N_THREADS];

pthread_mutex_t even_mutex;
pthread_mutex_t odd_mutex;
unsigned long n_odd = 0;
unsigned long n_even = 0;

int a[N_THREADS];

void *counter(void *arg) {
	int n;
	int *c = arg;
	double d; 
	  
	for (int k=0; k<*c; k++) {
		if(n_odd >= 18000 || n_even >= 18000)
			pthread_exit(NULL);
		n = rand()%100;
		for (int j=0; j<n; j++)
			d = sqrt((double)j);
		if (n%2 == 0){
			pthread_mutex_lock(&even_mutex);
			n_even++;
			pthread_mutex_unlock(&even_mutex);
		}
		else{
			pthread_mutex_lock(&odd_mutex);
			n_odd++;
			pthread_mutex_unlock(&odd_mutex);
		}	
	}
	
	pthread_exit(NULL);
}

int main() {
	int cnt, k;
	srand(time(NULL));
	
	
	for (k=0; k<N_THREADS; k++) {
		a[k] = (k+1)*1000;
		pthread_create(&thr_counter[k], NULL, counter, &a[k]);
	}
	
	for (k=0; k<N_THREADS; k++) {
		pthread_join(thr_counter[k], NULL);
	}
	
	printf("Broj parnih:   \t %lu\n", n_even);
	printf("Broj neparnih: \t %lu\n", n_odd);
	printf("Ukupno:        \t %lu\n", n_odd+n_even);
	return 0;
}
