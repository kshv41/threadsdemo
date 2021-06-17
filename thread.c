#include <stdio.h>
#include <stdlib.h>
#include <phtread.h>

#define ARR_SIZE 30 
#define NUM_THREADS 4

typedef struct Array {
	int length;
	int * data; 
}Array;

typedef struct inst_t {
	Array * array1;
	Array *array2;
	Array * res;
	int start;
	int finish;
}inst_t;

Array * createArray(int numElements) {
	Array * array = malloc(sizeof(Array));
	array->length = numElements;
	array ->data = (int *) malloc( sizeof(int) * numElements);

	for(int i =0; i< numElements ; i++)
		array->data[i] = rand() % 100;

	return array;
}

void* runner(void * args) {
	inst_t instructions = (inst_t *) args;
	for(int i =instructions->start; i< instructions->finish; i++)
		instructions->res->data[i] = instructions->array1->data[i] 
		+ instructions->array2->data[i]  
	
	pthread_exit(0);


}


Array* addArrays(Array * array1, Array *array2){
	Array * result = (Array *) malloc(sizeof(Array));
	result->length = ARR_SIZE;
	result->data = (int*) malloc(sizeof(int) * ARR_SIZE);
	//
	//
	inst_t myInsturctions[NUM_THREADS];
	for(int i =0; i< NUM_THREADS; i++){
		myInsturctions[i] = *(inst_t*) malloc(sizeof(inst_t));
		myInsturctions[i].array1 = array1;
		myInsturctions[i].array2 = array2;
		myInsturctions[i].res = result;

		myInsturctions[i].start = ARR_SIZE/NUM_THREADS *i;
		myInsturctions[i].finish = myInsturctions[i].start + (ARR_SIZE/NUM_THREADS); 

	}



	pthread_t myThread[NUM_THREADS];
	for(int i =0; i< NUM_THREADS; i++)
		pthread_create(&myThread[i], NULL, runner, &myInsturctions[i]);
	
	for(int i =0; i< NUM_THREADS; i++)
		pthread_join(myThread[i], NULL);
		
	return result;

}


void printArray(Array * arr) {
	for(int i =0; i< ARR_SIZE; i++) 
		printf("%3d", arr->data[i]);

	printf("\n");

}

void destroyArray(Array ** array){
	free(*array->data);
	free(*array);
	*array = NULL;

}

int main(void) {
	Array *array1= createArray(ARR_SIZE);
	Array *array2= createArray(ARR_SIZE);

	Array *result = addArrays(array1, array2);

	printArray(array1);
	printArray(array2);
	printArray(result);

	destroyArray(&array1);
	destroyArray(&array2);
	destroyArray(&result);
}