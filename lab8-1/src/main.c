#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#define MAX_NUMBER_VERTEX  5000

bool check_errors(int vertices_number, int edges_number)
{
	if (vertices_number < 0 || vertices_number > MAX_NUMBER_VERTEX) {
		printf("bad number of vertices");
		return true;
	}
	if (edges_number < 0 || edges_number >(vertices_number) * (vertices_number + 1) / 2) {
		printf("bad number of edges");
		return true;
	}
	return false;
}

bool check_edge(int start, int end, int vertices_number, int distance)
{
	if (start < 1 || start > vertices_number || end < 1 || end > vertices_number) {
		printf("bad vertex");
		return true;
	}
	if (distance < 0 || distance >INT_MAX) {
		printf("bad length");
		return true;
	}
	return false;
}

int** create_matrix(int vertices_number)
{
	int** matrix = (int**)calloc(vertices_number, sizeof(int*));
	for (int i = 0; i < vertices_number; i++) {
		matrix[i] = (int*)calloc(vertices_number, sizeof(int));
	}
	return matrix;
}

long long int* create_min_distance(int vertices_number)
{
	long long int* min_distance = (long long int*)malloc(vertices_number * sizeof(long long int));
	for (int i = 0; i < vertices_number; i++) {
		min_distance[i] = LLONG_MAX;
	}
	return min_distance;
}

void change_min_distance(int min_index, int** matrix, 
	long long int* min_distance, int vertices_number, int* parent, int* visited) 
{
	for (int i = 0; i < vertices_number; i++) {
		if (visited[i] == 0) {
			if (matrix[min_index][i] > 0) {
				long long int tmp = matrix[min_index][i];
				if (tmp < min_distance[i]) {
					min_distance[i] = tmp;
					parent[i] = min_index + 1;
				}
			}
		}
	}
}

void set_value(long long int* min_element, int* min_index) {
	*min_element = LLONG_MAX;
	*min_index = INT_MAX;
}

int* fill_min_distance(long long int* min_distance, int** matrix, int index, int vertices_number)
{
	int* visited = (int*)calloc(vertices_number, sizeof(int));
	int* parent = (int*)calloc(vertices_number, sizeof(int));

	min_distance[index - 1] = 0;
	int min_index = 0;
	long long int min_element = 0;

	while (min_index != INT_MAX) {
		set_value(&min_element, &min_index);
		for (int i = 0; i < vertices_number; i++) {
			if (visited[i] == 0 && min_distance[i] < min_element) {
				min_element = min_distance[i];
				min_index = i;
			}
		}
		if (min_index != INT_MAX) {
			change_min_distance(min_index, matrix, min_distance, vertices_number, parent, visited);
			visited[min_index] = 1;
		}
	}
	free(visited);
	return parent;
}



void free_arrays(long long int* min_distance, int** matrix, int vertices_number) {
	free(min_distance);
	for (int i = 0; i < vertices_number; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

int main()
{
	int vertices_number;
	int edges_number;

	if (scanf("%d\n%d", &vertices_number, &edges_number) != 2) {
		return EXIT_SUCCESS;
	}

	if (vertices_number == 0) {
		printf("no spanning tree");
		return EXIT_SUCCESS;
	}

	if (check_errors(vertices_number, edges_number)) {
		return EXIT_SUCCESS;
	}

	long long int* min_distance = create_min_distance(vertices_number);
	int** matrix = create_matrix(vertices_number);

	for (int i = 0; i < edges_number; i++) {
		int start, end, distance;
		if (scanf("%d %d %d", &start, &end, &distance) != 3) {
			printf("bad number of lines");
			free_arrays(min_distance, matrix, vertices_number);
			return EXIT_SUCCESS;
		}
		if (check_edge(start, end, vertices_number, distance)) {
			free_arrays(min_distance, matrix, vertices_number);
			return EXIT_SUCCESS;
		}
		matrix[start - 1][end - 1] = distance;
		matrix[end - 1][start - 1] = distance;
	}

	int* parent = fill_min_distance(min_distance, matrix, 1, vertices_number);
	int no_spanning = 0;

	for (int i = 0; i < vertices_number; i++) {
		if (parent[i] == 0) {
			no_spanning++;
		}
	}
	if (no_spanning != 1) {
		printf("no spanning tree");
		free_arrays(min_distance, matrix, vertices_number);
		free(parent);
		return EXIT_SUCCESS;
	}
	for (int i = 0; i < vertices_number; i++) {
		if (parent[i] != 0) {
			printf("%d %d\n", i + 1, parent[i]);
		}
	}
	free_arrays(min_distance, matrix, vertices_number);
	free(parent);
	return EXIT_SUCCESS;
}


