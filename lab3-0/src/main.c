#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

//////////////////////////////////////////

void swap(int *array, int i, int j);
void heap_sort(int *array, int len);
void sift(int *array, int len, int root);

int *input_array(int len);
void print_output(int len, int *array);

// MAIN //////////////////////////////////

int main(void)
{
	int len;

	if (scanf("%d", &len) != 1)
		return 0;

	if (len == 0)
		return EXIT_SUCCESS;

	int *array = input_array(len);

	heap_sort(array, len);

	print_output(len, array);

	free(array);

	return EXIT_SUCCESS;
}

// INPUT //////////////////////////////////

int *input_array(int len)
{
	int *array = (int *)malloc(len * sizeof(int));

	for (int i = 0; i < len; i++){
		if (scanf("%d", &array[i]) != 1){
			return NULL;
		}
	}

	return array;
}
// OUTPUT ////////////////////////////////

void print_output(int len, int *array)
{
	for (int i = 0; i < len; i++){
		printf("%d ", array[i]);
	}
}

// HEAP SORT /////////////////////////////

void sift(int *array, int len, int root)
{
	int largest = root;
	int left = 2 * root + 1;
	int right = 2 * root + 2;

	if (left < len && array[left] > array[largest])
		largest = left;

	if (right < len && array[right] > array[largest])
		largest = right;

	if (largest != root)
	{
		swap(array, largest, root);
		sift(array, len, largest);
	}
}

void heap_sort(int *array, int len)
{
	for (int i = len / 2 - 1; i >= 0; i--){
		sift(array, len, i);
	}

	for (int i = len - 1; i >= 0; i--){
		swap(array, 0, i);
		sift(array, i, 0);
	}
}

void swap(int *array, int i, int j)
{
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}
