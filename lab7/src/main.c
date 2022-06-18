#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#define SUCCESS 0;
#define ERROR 1;

int topologic_sort(int ind, int visited[], int n, char** matrix, int output[], int flag, int* k);
int fill_matrix(int n, int m, char** matrix);
void print_reverse(int n, int output[]);

void free_arrays(char** matrix, int* visited, int* output, int n) {
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);
	free(visited);
	free(output);
}
int main() {
	int n = 0;
	char** matrix;
	int* visited;
	int* output;
	if (scanf("%d", &n) != 1) {
		printf("bad number of lines");
		return EXIT_SUCCESS;
	}
	if (n < 0 || n > 2000) {
		printf("bad number of vertices");
		return EXIT_SUCCESS;
	}

	output = (int*)malloc(sizeof(int) * n);
	matrix = (char**)malloc(sizeof(char*) * n);
	int z = (n - n % 8 + 8) / 8;
	for (int i = 0; i < n; i++) {
		matrix[i] = (char*)malloc(sizeof(char) * z);
		for (int j = 0; j < z; ++j) {
			matrix[i][j] = 0;
		}
	}

	visited = (int*)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		visited[i] = 0;
	}
	int m = 0;

	if (scanf("%d", &m) != 1) {
		printf("bad number of lines");
		free_arrays(matrix, visited, output, n);
		return EXIT_SUCCESS;
	}

	if (m < 0 || m > n * (n + 1) / 2) {
		printf("bad number of edges");
		free_arrays(matrix, visited, output, n);
		return EXIT_SUCCESS;
	}

	if (fill_matrix(n, m, matrix) == 1) {
		free_arrays(matrix, visited, output, n);
		return EXIT_SUCCESS;
	}
	int k = 0;
	for (int i = 0; i < n; i++) {
		if (visited[i] == 0) {
			if (topologic_sort(i, visited, n, matrix, output, i, &k) == 1) {
				printf("impossible to sort");
				free_arrays(matrix, visited, output, n);
				return EXIT_SUCCESS;
			}
		}
	}
	print_reverse(n, output);
	free_arrays(matrix, visited, output, n);
	return EXIT_SUCCESS;
}
int topologic_sort(int ind, int visited[], int n, char** matrix, int output[], int flag, int* k) {
	visited[ind] = 1;
	for (int i = n - 1; i >= 0; i--) {
		int el = matrix[ind][i / 8];
		if ((el & (1 << (7 - i % 8))) != 0 && (visited[i] == 0))
			if (topologic_sort(i, visited, n, matrix, output, flag, k) == 1) {
				return ERROR;
			}
		if ((el & (1 << (7 - i % 8))) != 0 && (visited[i] == 1) && (i == flag))
			return ERROR;
	}
	output[(*k)++] = ind + 1;
	return SUCCESS;
}

int fill_matrix(int n, int m, char** matrix) {
	for (int i = 0; i < m; i++) {
		int a, b;
		if (scanf("%d %d", &a, &b) != 2) {
			printf("bad number of lines");
			return ERROR;
		}
		if (a <= 0 || b <= 0 || a > n || b > n) {
			printf("bad vertex");
			return ERROR;
		}
		if (a == b) {
			printf("impossible to sort");
			return ERROR;
		}
		int el = matrix[a - 1][(b - 1) / 8];
		matrix[a - 1][(b - 1) / 8] = el | (1 << (7 - ((b - 1) % 8)));
	}
	return SUCCESS;
}

void print_reverse(int n, int output[]) {
	for (int i = n - 1; i >= 0; i--) {
		printf("%d ", output[i]);
	}
}
