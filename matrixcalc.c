#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <ctype.h>
#include <signal.h>

int** dizi;
void createMatrix();
void sumOfMatrix();
void signal_handler();
int number; // kullanicidan alinan sayi

pthread_t thread_matrix_creator;
pthread_t thread_matrix_sum;

pthread_mutex_t lockCreate;
pthread_mutex_t lockSum;


int main() {
	
	pthread_mutex_init(&lockSum, NULL);
	pthread_mutex_init(&lockCreate, NULL);
	pthread_mutex_lock(&lockSum);
	void* status;
		
	while(1){
		signal(SIGHUP, signal_handler);
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, signal_handler);
		signal(SIGKILL, signal_handler);
		signal(SIGTERM, signal_handler);
		signal(SIGTSTP, signal_handler);
			
		printf("Bir sayi giriniz: ");
		scanf("%d",&number);
		if (number <= 0) {
			pthread_detach(thread_matrix_creator);
			pthread_detach(thread_matrix_sum);
			return 0;
		}
	
		pthread_create(&thread_matrix_creator, NULL, createMatrix, NULL);
		pthread_create(&thread_matrix_sum, NULL, sumOfMatrix, NULL);
		
	    	pthread_join(thread_matrix_sum, &status);
	    	pthread_join(thread_matrix_creator, &status);
	 }
	
	return 0;
}
void signal_handler()
{
	pthread_detach(thread_matrix_creator);
	pthread_detach(thread_matrix_sum);
 	exit(0);
}

void createMatrix(){
    /*20x20 ve elemanlarinin max degeri 100
     olan bir matris olusturur.*/
     
    for(int k = 0; k < number; k++) {
    	    pthread_mutex_lock(&lockCreate);
	    srand(time(0));
	    dizi = malloc(20 * sizeof (int*)); //satirlar icin yer ayrildi

	    for(int i = 0; i < 20; i++){
		*(dizi + i) = malloc(20 * sizeof (int)); //sutunlar icin yer ayrildi
	    }

	    for(int i = 0; i < 20; i++){
		for (int j = 0; j < 20; j++){
		    *(*(dizi + i) + j) = rand() % 100; //olusturdugumuz matrisin elemanlarina deger atandi
		}
	    }
	    // matrisi ekrana bas
	    printf("\n");
	    for (int i = 0; i < 20; i++) {
		 for(int j = 0; j < 20; j++){
		     printf("%d	",  *(*(dizi + i) + j));
		 }
		 printf("\n");
	    }
	    pthread_mutex_unlock(&lockSum);
    }
}

void sumOfMatrix(){

	for(int k = 0; k < number; k++) {	
	        pthread_mutex_lock(&lockSum);
		int sum = 0;
		printf("\n");
	    	for (int i = 0; i < 20; i++) {
	    		for(int j = 0; j < 20; j++){
		     		sum +=  *(*(dizi + i) + j);
		 	}
	    	}
	    	printf("sum: %d\n",sum);
	    	pthread_mutex_unlock(&lockCreate);
	}
}
