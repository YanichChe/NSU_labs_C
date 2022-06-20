#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

//////////////////////////////////////////

#define MAX_NUMBER_VERTEX 5000

//////////////////////////////////////////

bool checkErrors(int numberVertices, int numberEdges);
bool checkEdge(int start, int end, int numberVertices, long long int distance);
bool checkSpanningTree(int* parent, int numberVertices);

int **createMatrix(int numberVertices);
long long int *createMinDistance(int numberVertices);
bool inputMatrix(int numberEdges, int numberVertices, int **matrix);

void changeMinDistance(int minIndex, int **matrix,
					   long long int *minDistance, int numberVertices, int *parent, int *visited);

void setValue(long long int *minElement, int *minIndex);
int *primAlgoritnm(long long int *minDistance, int **matrix, int index, int numberVertices);

void printMST(int numberVertices, int* parent);

void freeAll(long long int *minDistance, int **matrix, int numberVertices);

// MAIN //////////////////////////////////

int main()
{
	int numberVertices;
	int numberEdges;

	if (scanf("%d\n%d", &numberVertices, &numberEdges) != 2)
	{
		return EXIT_SUCCESS;
	}

	if (!checkErrors(numberVertices, numberEdges))
	{
		return EXIT_SUCCESS;
	}

	long long int *minDistance = createMinDistance(numberVertices);
	int **matrix = createMatrix(numberVertices);

	if (!inputMatrix(numberEdges, numberVertices, matrix))
	{
		freeAll(minDistance, matrix, numberVertices);
		return EXIT_SUCCESS;
	}

	int *parent = primAlgoritnm(minDistance, matrix, 1, numberVertices);

	if (!checkSpanningTree(parent,numberVertices))
	{
		freeAll(minDistance, matrix, numberVertices);
		free(parent);
		return EXIT_SUCCESS;
	}

	printMST(numberVertices, parent);

	freeAll(minDistance, matrix, numberVertices);
	free(parent);
	
	return EXIT_SUCCESS;
}

// ERRORS ////////////////////////////////////////

bool checkErrors(int numberVertices, int numberEdges)
{
	if (numberVertices == 0)
	{
		printf("no spanning tree");
		return false;
	}

	if (numberVertices < 0 || numberVertices > MAX_NUMBER_VERTEX)
	{
		printf("bad number of vertices");
		return false;
	}
	if (numberEdges < 0 || numberEdges > (numberVertices) * (numberVertices + 1) / 2)
	{
		printf("bad number of edges");
		return false;
	}
	return true;
}

bool checkEdge(int start, int end, int numberVertices, long long int distance)
{
	if (start < 1 || start > numberVertices || end < 1 || end > numberVertices)
	{
		printf("bad vertex");
		return true;
	}
	if (distance < 0 || distance > INT_MAX)
	{
		printf("bad length");
		return true;
	}
	return false;
}

bool checkSpanningTree(int* parent, int numberVertices)
{
	int noSpanningTree = 0;

	for (int i = 0; i < numberVertices; i++)
	{
		if (parent[i] == 0)
		{
			noSpanningTree++;
		}
	}

	if (noSpanningTree != 1)
	{
		printf("no spanning tree");
		return false;
	}

	return true;
}
//////////////////////////////////////////

int **createMatrix(int numberVertices)
{
	int **matrix = (int **)calloc(numberVertices, sizeof(int *));
	for (int i = 0; i < numberVertices; i++)
	{
		matrix[i] = (int *)calloc(numberVertices, sizeof(int));
	}
	return matrix;
}

long long int *createMinDistance(int numberVertices)
{
	long long int *minDistance = (long long int *)malloc(numberVertices * sizeof(long long int));
	for (int i = 0; i < numberVertices; i++)
	{
		minDistance[i] = LLONG_MAX;
	}
	return minDistance;
}

bool inputMatrix(int numberEdges, int numberVertices, int **matrix)
{
	for (int i = 0; i < numberEdges; i++)
	{
		int start, end;
		long long distance;

		if (scanf("%d %d %lld", &start, &end, &distance) != 3)
		{
			printf("bad number of lines");
			return false;
		}
		if (checkEdge(start, end, numberVertices, distance))
		{
			return false;
		}
		matrix[start - 1][end - 1] = (int)distance;
		matrix[end - 1][start - 1] = (int)distance;
	}

	return true;
}

//////////////////////////////////////////

void changeMinDistance(int minIndex, int **matrix,
					   long long int *minDistance, int numberVertices, int *parent, int *visited)
{
	for (int i = 0; i < numberVertices; i++)
	{
		if (visited[i] == 0 && matrix[minIndex][i] > 0)
		{
			long long int tmp = matrix[minIndex][i];
			if (tmp < minDistance[i])
			{
				minDistance[i] = tmp;
				parent[i] = minIndex + 1;
			}
		}
	}
}

//////////////////////////////////////////

void setValue(long long int *minElement, int *minIndex)
{
	*minElement = LLONG_MAX;
	*minIndex = INT_MAX;
}

int *primAlgoritnm(long long int *minDistance, int **matrix, int index, int numberVertices)
{
	int *visited = (int *)calloc(numberVertices, sizeof(int));
	int *parent = (int *)calloc(numberVertices, sizeof(int));

	minDistance[index - 1] = 0;
	int minIndex = 0;
	long long int minElement = 0;

	while (minIndex != INT_MAX)
	{
		setValue(&minElement, &minIndex);
		for (int i = 0; i < numberVertices; i++)
		{
			if (visited[i] == 0 && minDistance[i] < minElement)
			{
				minElement = minDistance[i];
				minIndex = i;
			}
		}
		if (minIndex != INT_MAX)
		{
			changeMinDistance(minIndex, matrix, minDistance, numberVertices, parent, visited);
			visited[minIndex] = 1;
		}
	}
	free(visited);
	return parent;
}

//////////////////////////////////////////

void printMST(int numberVertices, int* parent)
{
	for (int i = 0; i < numberVertices; i++)
	{
		if (parent[i] != 0)
		{
			printf("%d %d\n", i + 1, parent[i]);
		}
	}
}

//////////////////////////////////////////

void freeAll(long long int *minDistance, int **matrix, int numberVertices)
{
	free(minDistance);
	for (int i = 0; i < numberVertices; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}
