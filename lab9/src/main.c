#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

//////////////////////////////////////////

#define MAX_NUMBER_VERTEX 5000

//////////////////////////////////////////

typedef struct _graph Graph;

//////////////////////////////////////////

struct _graph
{
	int numberVertices;
	int numberEdges;
	long long int **matrix;
};

//////////////////////////////////////////

bool checkErrors(int numberVertices, int numberEdges);
bool checkEdge(int start, int end, int numberVertices, long long distance);
bool checkVertex(int index, int numberVertices);
bool checkLength(int distance);
bool checkOverflow(int index, int numberVertices, long long int *minDistance);

long long int **createMatrix(int numberVertices);
long long int *createMinDistance(int numberVertices);

bool inputMatrix(int numberEdges, int numberVertices, long long int **matrix);
Graph *createGraph(int numberVertices, int numberEdges);

void changeMinDistance(long long int minElement, int minIndex, long long int *minDistance, int *parent, Graph* graph);
void setValue(long long int *minElement, int *minIndex);
int *fillMinDistance(long long int *minDistance, int index, Graph* graph);
void Dijkstra(int numberVertices, int finishIndex, int startIndex, long long int *minDistance, int *parent);

void printMinDistance(long long int *minDistance, int numberVertices, int index);
void printWay(int outputIndex, int *way);

void freeMatrix(long long int **matrix, int numberVertices);
void destroyGraph(Graph* graph);

// MAIN //////////////////////////////////

int main()
{
	int numberVertices;
	int numberEdges;
	int startIndex, finishIndex;
	Graph* graph;

	if (scanf("%d\n%d %d\n%d", &numberVertices, &startIndex, &finishIndex, &numberEdges) != 4)
	{
		return EXIT_SUCCESS;
	}
	if (!checkEdge(startIndex, finishIndex, numberVertices, 0))
	{
		return EXIT_SUCCESS;
	}
	if (!checkErrors(numberVertices, numberEdges))
	{
		return EXIT_SUCCESS;
	}

	if ((graph = createGraph(numberVertices, numberEdges)) == NULL){
		return EXIT_SUCCESS;
	}

	long long int *minDistance = createMinDistance(graph->numberVertices);
	int *parent = fillMinDistance(minDistance, startIndex, graph);

	printMinDistance(minDistance, numberVertices, startIndex - 1);
	Dijkstra(numberVertices, finishIndex, startIndex, minDistance, parent);

	destroyGraph(graph);
	free(minDistance);
	free(parent);
	
	return EXIT_SUCCESS;
}

// ERRORS ////////////////////////////////

bool checkVertex(int index, int numberVertices)
{
	if (index<1 || index>numberVertices){
		printf("bad vertex");
		return false;
	}

	return true;
}

bool checkLength(int distance)
{
	if (distance < 0 || distance > INT_MAX)
	{
		printf("bad length");
		return false;
	}

	return true;
}
bool checkErrors(int numberVertices, int numberEdges)
{
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
	if (!checkVertex(start, numberVertices) || !checkVertex(end, numberVertices))
	{
		return false;
	}
	if (distance < 0 || distance > INT_MAX)
	{
		printf("bad length");
		return false;
	}
	return true;
}

bool checkOverflow(int index, int numberVertices, long long int *minDistance)
{
	if (minDistance[index] <= INT_MAX)
	{
		return false;
	}

	int count = 0;

	for (int i = 0; i < numberVertices; i++)
	{

		if (i != index && minDistance[i] != LLONG_MAX && minDistance[i] >= INT_MAX)
		{
			count += 1;
			if (count == 2)
				return true;
		}
	}

	return false;
}

//////////////////////////////////////////

long long int **createMatrix(int numberVertices)
{
	long long int **matrix = calloc(numberVertices, sizeof(long long int *));
	for (int i = 0; i < numberVertices; i++)
	{
		matrix[i] = calloc(numberVertices, sizeof(long long int));
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

bool inputMatrix(int numberEdges, int numberVertices, long long int **matrix)
{
	for (int i = 0; i < numberEdges; i++)
	{
		int start, end;
		long long int distance;
		if (scanf("%d %d %lld", &start, &end, &distance) != 3)
		{
			printf("bad number of lines");
			freeMatrix(matrix, numberVertices);
			return false;
		}
		if (!checkEdge(start, end, numberVertices, distance))
		{
			freeMatrix(matrix, numberVertices);
			return false;
		}

		matrix[start - 1][end - 1] = distance;
		matrix[end - 1][start - 1] = distance;
	}

	return true;
}

Graph *createGraph(int numberVertices, int numberEdges)
{
	long long int **matrix = createMatrix(numberVertices);
	if (!inputMatrix(numberEdges, numberVertices, matrix))
	{
		return NULL;
	}

	Graph *graph = malloc(sizeof(Graph));
	graph->numberEdges = numberEdges;
	graph->numberVertices = numberVertices;
	graph->matrix = matrix;

	return graph;
}

//////////////////////////////////////////

void changeMinDistance(long long int minElement, int minIndex, long long int *minDistance, int *parent, Graph* graph)
{
	for (int i = 0; i < graph->numberVertices; i++)
	{
		if (graph->matrix[minIndex][i] > 0)
		{
			long long int tmp = minElement + graph->matrix[minIndex][i];
			if (tmp < minDistance[i])
			{
				minDistance[i] = tmp;
				parent[i] = minIndex + 1;
			}
		}
	}
}

void setValue(long long int *minElement, int *minIndex)
{
	*minElement = LLONG_MAX;
	*minIndex = INT_MAX;
}

int *fillMinDistance(long long int *minDistance, int index, Graph* graph)
{
	int *visited = calloc(graph->numberVertices, sizeof(int));
	int *parent = calloc(graph->numberVertices, sizeof(int));

	minDistance[index - 1] = 0;
	int minIndex = 0;
	long long int minElement = 0;

	while (minIndex != INT_MAX)
	{
		setValue(&minElement, &minIndex);
		for (int i = 0; i < graph->numberVertices; i++)
		{
			if (visited[i] == 0 && minDistance[i] < minElement)
			{
				minElement = minDistance[i];
				minIndex = i;
			}
		}
		if (minIndex != INT_MAX)
		{
			changeMinDistance(minElement, minIndex, minDistance, parent, graph);
			visited[minIndex] = 1;
		}
	}
	free(visited);
	return parent;
}

void Dijkstra(int numberVertices, int finishIndex, int startIndex, long long int *minDistance, int *parent)
{
	int *way = (int *)malloc(numberVertices * sizeof(numberVertices));
	int currentIndex = finishIndex - 1;
	int outputIndex = 0;
	way[outputIndex++] = finishIndex;

	if (checkOverflow(finishIndex - 1, numberVertices, minDistance))
	{
		printf("\noverflow");
		free(way);
		return;
	}
	if (minDistance[finishIndex - 1] == LLONG_MAX)
	{
		printf("\nno path");
		free(way);
		return;
	}

	while (currentIndex != startIndex - 1)
	{
		currentIndex = parent[currentIndex] - 1;
		way[outputIndex++] = currentIndex + 1;
	}
	printWay(outputIndex, way);
	free(way);
}

// OUTPUT ////////////////////////////////

void printMinDistance(long long int *minDistance, int numberVertices, int index)
{
	for (int i = 0; i < numberVertices; i++)
	{
		if (minDistance[i] == LLONG_MAX)
		{
			printf("oo ");
		}
		else if (i == index)
		{
			printf("0 ");
		}
		else if (minDistance[i] > INT_MAX)
		{
			printf("INT_MAX+ ");
		}
		else
		{
			printf("%lld ", minDistance[i]);
		}
	}
}

void printWay(int outputIndex, int *way)
{
	printf("\n");
	for (int i = 0; i < outputIndex; i++)
	{
		printf("%d ", way[i]);
	}
}

//////////////////////////////////////////

void freeMatrix(long long int **matrix, int numberVertices)
{
	for (int i = 0; i < numberVertices; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

void destroyGraph(Graph* graph){
	freeMatrix(graph->matrix, graph->numberVertices);
	free(graph);
}
