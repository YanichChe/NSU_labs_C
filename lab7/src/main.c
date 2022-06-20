#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

//////////////////////////////////////////

bool inputNumberVertices(int* numberVertices);
bool inputMatrix(int numberVertices, int numberEdges, char** matrix);
char** createMatrix(int numberVertices);

bool checkNumberLines(int* numberEdges);
bool checkNumberEdges(int numberEdges, int numberVertices);
bool checkVertex(int start, int finish, int numberVertices);

bool DFS(int numberVertices, int* visited, char** matrix, int* output);
int topologicSort(int index, int visited[], int numberVertices, char** matrix, int output[], int flag, int* indexOutput);
void printReverse(int numberVertices, int output[]);

//////////////////////////////////////////

void freeArrays(char** matrix, int* visited, int* output, int numberVertices) 
{
	for (int i = 0; i < numberVertices; i++) {
		free(matrix[i]);
	}
	free(matrix);
	free(visited);
	free(output);
}

// MAIN //////////////////////////////////

int main() 
{
	int numberVertices = 0;

	if (!inputNumberVertices(&numberVertices)){
		return EXIT_SUCCESS;
	}

	int* output = (int*)malloc(sizeof(int) * numberVertices);
	char** matrix = createMatrix(numberVertices);
	int* visited = calloc(numberVertices, sizeof(int));

	int numberEdges = 0;

	if (!checkNumberLines(&numberEdges) || !checkNumberEdges(numberEdges, numberVertices)
		|| !inputMatrix(numberVertices, numberEdges, matrix)) {

		freeArrays(matrix, visited, output, numberVertices);
		return EXIT_SUCCESS;

	}

	DFS(numberVertices, visited, matrix, output);
	printReverse(numberVertices, output);

	freeArrays(matrix, visited, output, numberVertices);

	return EXIT_SUCCESS;
}

// CHECK ERRORS /////////////////////////

bool checkNumberLines(int* numberEdges)
{
	if (scanf("%d", numberEdges) != 1) {
		printf("bad number of lines");
		return false;
	}

	return true;
}

bool checkNumberEdges(int numberEdges, int numberVertices)
{
	if (numberEdges < 0 || numberEdges > numberVertices * (numberVertices + 1) / 2) {
		printf("bad number of edges");
		return false;
	}

	return true;
}

bool checkVertex(int start, int finish, int numberVertices)
{
	if (start <= 0 || finish <= 0 || start > numberVertices || finish > numberVertices) {
		printf("bad vertex");
		return false;
	}

	return true;
}

//////////////////////////////////////////

char** createMatrix(int numberVertices)
{
	char** matrix = (char**)malloc(sizeof(char*) * numberVertices);

	int len = (numberVertices - numberVertices % 8 + 8) / 8;
	for (int i = 0; i < numberVertices; i++) {
		matrix[i] = (char*)malloc(sizeof(char) * len);
		for (int j = 0; j < len; ++j) {
			matrix[i][j] = 0;
		}
	}

	return matrix;
}

// TOPOLOGIC SORT /////////////////////////

bool DFS(int numberVertices, int* visited, char** matrix, int* output)
{
	int indexOutput = 0;

	for (int i = 0; i < numberVertices; i++) {
		if (visited[i] == 0) {
			if (!topologicSort(i, visited, numberVertices, matrix, output, i, &indexOutput)) {
				printf("impossible to sort");
				return false;
			}
		}
	}

	return true;
}

int topologicSort(int index, int visited[], int numberVertices, char** matrix, int output[], int flag, int* indexOutput) 
{
	visited[index] = 1;

	for (int i = numberVertices - 1; i >= 0; i--) {
		int el = matrix[index][i / 8];
		if ((el & (1 << (7 - i % 8))) != 0 && (visited[i] == 0))
			if (!topologicSort(i, visited, numberVertices, matrix, output, flag, indexOutput)) {
				return false;
			}
		if ((el & (1 << (7 - i % 8))) != 0 && (visited[i] == 1) && (i == flag))
			return false;
	}

	output[(*indexOutput)++] = index + 1;
	return true;
}

void printReverse(int numberVertices, int output[]) 
{
	for (int i = numberVertices - 1; i >= 0; i--) {
		printf("%d ", output[i]);
	}
}

// INPUT /////////////////////////////////

bool inputNumberVertices(int* numberVertices)
{
	if (scanf("%d", numberVertices) != 1) {
		printf("bad number of lines");
		return false;
	}
	if (*numberVertices < 0 || *numberVertices > 2000) {
		printf("bad number of vertices");
		return false;
	}

	return true;
}

bool inputMatrix(int numberVertices, int numberEdges, char** matrix) 
{
	for (int i = 0; i < numberEdges; i++) {
		int start, finish;
		if (scanf("%d %d", &start, &finish) != 2) {
			printf("bad number of lines");
			return false;
		}
		if (!checkVertex(start, finish, numberVertices)) return false;
		if (start == finish) {
			printf("impossible to sort");
			return false;
		}
		int el = matrix[start - 1][(finish - 1) / 8];
		matrix[start - 1][(finish - 1) / 8] = el | (1 << (7 - ((finish - 1) % 8)));
	}
	return true;
}