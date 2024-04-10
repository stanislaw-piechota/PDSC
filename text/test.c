#include "rand_malloc.h"
#include <stdio.h>
#include <stdlib.h>

void test_malloc();
void test_realloc();

int main()
{
	test_malloc();
	printf("-------------------------------------\n");
	test_realloc();
}

void test_malloc()
{
	printf("Calling malloc multiple times:\n");

	for (int i = 0; i < 10; ++i) {
		int* array = malloc(5 * sizeof(int));
		if (array != NULL) {
			printf("Good allocation\n");
			free(array);
		}
		else {
			printf("Bad allocation, null pointer returned\n");
		}
	}
}

void test_realloc()
{
	printf("Calling realloc multiple times:\n");

	int* array = NULL;
	for (int i = 0; i < 10; ++i) {
		int* new_array = realloc(array, (i + 1) * sizeof(int));
		if (new_array != NULL) {
			printf("Good allocation\n");
			array = new_array;
		}
		else {
			printf("Bad allocation, null pointer returned\n");
		}
	}
	free(array);
}
