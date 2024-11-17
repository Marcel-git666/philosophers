#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void *arg) {
	int index = *(int*) arg;
	int sum = 0;
	for (int i = 0; i < 5; i++) {
		sum += primes[i +  index];
	}
	printf("Sum of primes from thread: %d\n", sum);
	*(int *) arg = sum;
	return (arg);
}

int main(int argc, char* argv[]) {
	pthread_t th[2];

	for (int i = 0; i < 2; i++) {
		int *a = malloc(sizeof(int));
		*a = i * 5;
		if (pthread_create(th+i, NULL, &routine, a) != 0) {
		return (1);
		}
	}
	int global_sum = 0;
	for (int i = 0; i < 2; i++) {
		int* r;
		if (pthread_join(th[i], (void **) &r) != 0) {
				return 2;
		}
		printf("Result from thread: %d is: %d\n", i, *r);
		global_sum += *r;
		free(r);
	}
	printf("Global sum is: %d\n", global_sum);
	return 0;
}
