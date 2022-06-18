#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#define SIZE 10

int len;
int find_max_ind(int array[]);
int find_ind_bigger(int array[], int number);
void change(int* array, int ind_first, int ind_second);
void reverse_part(int* array, int ind);
int is_correct(int array[]);
int make_array(int* array);

int main(void) {
	int k;
	int array[SIZE];
	if (make_array(array) == 0) {
		return EXIT_SUCCESS;
	}
	if (scanf("%d", &k) != 1) {
		return EXIT_SUCCESS;
	}
	if (is_correct(array) == 0) {
		printf("%s", "bad input");
	}
	int max_ind = find_max_ind(array);
	while (max_ind != -1 && k != 0) {
		int number = array[max_ind];
		int change_ind = find_ind_bigger(array, number);
		change(array, max_ind, change_ind);
		reverse_part(array, max_ind);
		for (int i = 0; i < len; i++) {
			printf("%d", array[i]);
		}
		printf("\n");
		max_ind = find_max_ind(array);
		--k;
		
	}
	return EXIT_SUCCESS;
}
int make_array(int* array) {
	len = 0;
	char c;
	int i = 0;
	while ((c = getchar()) != '\n') {
		if (i > 9) {
			printf("%s", "bad input");
			return 0;
		}
		array[i] = c - '0';
		++i;
		++len;
		if (len > 10 || !(c >= '0' && c <= '9')) {
			printf("%s", "bad input");
			return 0;
		}
	}
	return 1;
}
int is_correct(int array[]) {
	int sample[10] = { 0 };
	for (int i = 0; i < len; i++) {
		sample[array[i]] += 1;
		if (sample[array[i]] > 1) {
			return 0;
		}
	}
	return 1;
}
int find_max_ind(int array[]) {
	for (int i = len - 2; i >= 0; i--) {
		if (array[i] < array[i + 1]) {
			return i;
		}
	}
	return -1;
}

int find_ind_bigger(int array[], int number) {
	for (int i = len - 1; i >= 0; i--) {
		if (array[i] > number) {
			return i;
		}
	}
	return -1;
}

void change(int* array, int ind_first, int ind_second) {
	int buff = array[ind_first];
	array[ind_first] = array[ind_second];
	array[ind_second] = buff;
}

void reverse_part(int* array, int ind) {
	int n = len - 1;
	int i = ind + 1;
	for (; i < n; i++, n--) {
		int buff = array[i];
		array[i] = array[n];
		array[n] = buff;
	}
}
