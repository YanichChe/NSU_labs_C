#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#define SIZE 10

//////////////////////////////////////////

int len;

int findMaxInd(int array[]);
int findBiggerInd(int array[], int number);

void change(int* array, int ind_first, int ind_second);
void reversePart(int* array, int ind);

bool isCorrect(int array[]);
int inputArray(int* array);
void makePermutations(int startIndex, int* array);

// MAIN //////////////////////////////////

int main(void)
 {
	int startIndex;
	int array[SIZE];

	if (inputArray(array) == 0 || scanf("%d", &startIndex) != 1) 
		return EXIT_SUCCESS;

	if (isCorrect(array) == false) 
		printf("%s", "bad input");

	makePermutations(startIndex, array);

	return EXIT_SUCCESS;
}

//////////////////////////////////////////

void makePermutations(int startIndex, int* array)
{
	int maxInd = findMaxInd(array);

	while (maxInd != -1 && startIndex != 0) {

		int number = array[maxInd];
		int change_ind = findBiggerInd(array, number);

		change(array, maxInd, change_ind);
		reversePart(array, maxInd);

		for (int i = 0; i < len; i++) {
			printf("%d", array[i]);
		}

		printf("\n");
		maxInd = findMaxInd(array);
		--startIndex;
		
	}

}

//////////////////////////////////////////

int inputArray(int* array) 
{
	len = 0;
	char c;

	while ((c = getchar()) != '\n') {

		if (len > 9) {
			printf("%s", "bad input");
			return 0;
		}

		array[len++] = c - '0';

		if (len > 10 || !(c >= '0' && c <= '9')) {
			printf("%s", "bad input");
			return 0;
		}

	}
	return 1;
}

//////////////////////////////////////////

bool isCorrect(int array[]) 
{
	int sample[SIZE] = { 0 };

	for (int i = 0; i < len; i++) {

		sample[array[i]] += 1;
		if (sample[array[i]] > 1) 
			return false;
	}

	return true;
}

//////////////////////////////////////////

int findMaxInd(int array[]) 
{
	for (int i = len - 2; i >= 0; i--) {
		if (array[i] < array[i + 1]) {
			return i;
		}
	}

	return -1;
}

int findBiggerInd(int array[], int number) 
{
	for (int i = len - 1; i >= 0; i--) {
		if (array[i] > number) {
			return i;
		}
	}

	return -1;
}

void change(int* array, int ind_first, int ind_second) 
{
	int buff = array[ind_first];
	array[ind_first] = array[ind_second];
	array[ind_second] = buff;
}

void reversePart(int* array, int ind) 
{
	int n = len - 1;
	int i = ind + 1;

	for (; i < n; i++, n--) {
		int buff = array[i];
		array[i] = array[n];
		array[n] = buff;
	}

}
