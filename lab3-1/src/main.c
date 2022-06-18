#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
void quick_sort(int* array, int left_border, int right_border);
void swap(int* array, int i, int j);

int main(void) {
	int len;
	if (scanf("%d", &len) != 1) {
		return 0;
	}
	int *array;
	if (len == 0) {return EXIT_SUCCESS;}
	array = (int*)malloc(len*sizeof(int));
	for (int i = 0;i < len;i++) {
		if (scanf("%d", &array[i]) != 1) {
			return 0;
		}
	}
	quick_sort(array, 0, len - 1);
	for (int i = 0; i < len;i++) {
		printf("%d ", array[i]);
	}
	free(array);
	return EXIT_SUCCESS;
}
void quick_sort(int* array, int left_border, int right_border) {
	int i = left_border;
	int j = right_border;

	int pivot = array[(left_border + right_border) / 2];
	do {
		while (array[i] < pivot) { i++; }
		while (array[j] > pivot) { j--; }
		if (i <= j) {
			if (array[i] > array[j]) {swap(array, i, j);}
			i++;
			j--;
		}
	} while (i <= j);
	if (left_border < j) {
		quick_sort(array, left_border, j);
	}
	if (i < right_border) {
		quick_sort(array, i, right_border);
	}
}
void swap(int *array, int i, int j) {
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}
