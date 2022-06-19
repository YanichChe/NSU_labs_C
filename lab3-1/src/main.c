#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

//////////////////////////////////////////

int *inputArray(int len);
void printOutput(int len, int *array);

void quickSort(int *array, int leftBorder, int rightBorder);
void swap(int *array, int i, int j);

// MAIN //////////////////////////////////

int main(void)
{
	int len;

	if (scanf("%d", &len) != 1 || len == 0)
	{
		return EXIT_SUCCESS;
	}

	int *array = inputArray(len);

	quickSort(array, 0, len - 1);

	printOutput(len, array);

	free(array);

	return EXIT_SUCCESS;
}

// OUTPUT /////////////////////////////////

void printOutput(int len, int *array)
{
	for (int i = 0; i < len; i++)
	{
		printf("%d ", array[i]);
	}
}

// INPUT //////////////////////////////////

int *inputArray(int len)
{
	int *array = (int *)malloc(len * sizeof(int));

	for (int i = 0; i < len; i++)
	{
		if (scanf("%d", &array[i]) != 1)
		{
			return NULL;
		}
	}

	return array;
}

// QUICK SORT /////////////////////////////

void swap(int *array, int i, int j)
{
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}

void quickSort(int *array, int leftBorder, int rightBorder)
{
	int i = leftBorder;
	int j = rightBorder;

	int pivot = array[(leftBorder + rightBorder) / 2];
	do
	{
		while (array[i] < pivot)
		{
			i++;
		}

		while (array[j] > pivot)
		{
			j--;
		}

		if (i <= j)
		{

			if (array[i] > array[j])
			{
				swap(array, i, j);
			}

			i++;
			j--;
		}
	} while (i <= j);

	if (leftBorder < j)
	{
		quickSort(array, leftBorder, j);
	}

	if (i < rightBorder)
	{
		quickSort(array, i, rightBorder);
	}
}
