#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>

//////////////////////////////////////////

typedef struct _edge Edge;
typedef struct _graph Graph;
typedef struct _set Set;

//////////////////////////////////////////
struct _edge
{
	int first;
	int second;
	int weight;
};

struct _graph
{
	int numberVertices;
	int numberEdges;
	Edge *edges;
};

struct _set
{
	int parent;
	int prior;
};

//////////////////////////////////////////

Graph *createGraph(int numberVertices, int numberEdges);
bool inputGraph(Graph *graph);

bool checkNumbers(int numberVertices, int numberEdges);
bool checkNumberLines(int *start, int *end, long long int *weight);
bool checkLenght(long long weight);
bool checkVertex(int start, int end, long long weight, int numberVertices);

int findIndex(int vertex, Set parents[]);
void unite(int first, int second, Set parents[]);
int comparator(const void *first, const void *second);
void fillSets(Set set[], int numberVertices);

bool Kruskal(Graph *graph, Edge *output);

void printOutput(int numberVertices, Edge *output);

void freeAll(Graph *graph, Edge *output);

// MAIN //////////////////////////////////

int main()
{
	int numberVertices;
	int numberEdges;

	if (scanf("%d", &numberVertices) != 1 || scanf("%d", &numberEdges) != 1)
	{
		return EXIT_SUCCESS;
	}

	if (!checkNumbers(numberVertices, numberEdges))
	{
		return EXIT_SUCCESS;
	}

	Graph *graph = createGraph(numberVertices, numberEdges);
	Edge *output = (Edge *)malloc(numberVertices * sizeof(Edge));

	if (!inputGraph(graph))
	{
		freeAll(graph, output);
		return EXIT_SUCCESS;
	}

	if (Kruskal(graph, output))
	{
		printOutput(numberVertices, output);
	}

	freeAll(graph, output);
	return EXIT_SUCCESS;
}

// INPUT GRAPH ///////////////////////////

bool inputGraph(Graph *graph)
{
	for (int i = 0; i < graph->numberEdges; i++)
	{
		int start;
		int end;
		long long int weight;

		if (!checkNumberLines(&start, &end, &weight) || !checkVertex(start, end, weight, graph->numberVertices))
		{
			return false;
		}

		graph->edges[i].first = start - 1;
		graph->edges[i].second = end - 1;
		graph->edges[i].weight = (int)weight;
	}

	return true;
}

// ERRORS ////////////////////////////////

bool checkNumbers(int numberVertices, int numberEdges)
{
	if (numberVertices == 0)
	{
		printf("no spanning tree\n");
		return false;
	}

	if (numberVertices < 0 || numberVertices > 5000)
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

bool checkNumberLines(int *start, int *end, long long int *weight)
{
	if (scanf("%d %d %lld", start, end, weight) != 3)
	{
		printf("bad number of lines");
		return false;
	}

	return true;
}

bool checkLenght(long long weight)
{
	if (weight < 0 || weight > INT_MAX)
	{
		printf("bad length");
		return false;
	}

	return true;
}

bool checkVertex(int start, int end, long long weight, int numberVertices)
{
	if (start < 1 || start > numberVertices || end < 1 || end > numberVertices)
	{
		printf("bad vertex");
		return false;
	}

	if (!checkLenght(weight))
		return false;

	return true;
}

//////////////////////////////////////////

int findIndex(int vertex, Set parents[])
{
	if (parents[vertex].parent != vertex)
		parents[vertex].parent = findIndex(parents[vertex].parent, parents);
	return parents[vertex].parent;
}

void unite(int first, int second, Set parents[])
{
	int first_parent = findIndex(first, parents);
	int second_parent = findIndex(second, parents);

	if (parents[first_parent].prior > parents[second_parent].prior)
	{
		parents[second_parent].parent = first_parent;
		return;
	}

	else if (parents[first_parent].prior < parents[second_parent].prior)
	{
		parents[first_parent].parent = second_parent;
		return;
	}

	parents[second_parent].parent = first_parent;
	parents[first_parent].prior++;
}

//////////////////////////////////////////

Graph *createGraph(int numberVertices, int numberEdges)
{
	Graph *new_graph = (Graph *)malloc(sizeof(Graph));

	new_graph->numberVertices = numberVertices;
	new_graph->numberEdges = numberEdges;
	new_graph->edges = (Edge *)malloc(numberEdges * sizeof(Edge));

	return new_graph;
}

//////////////////////////////////////////

int comparator(const void *first, const void *second)
{
	Edge *el1 = (Edge *)first;
	Edge *el2 = (Edge *)second;
	return el1->weight > el2->weight;
}

void fillSets(Set set[], int numberVertices)
{
	for (int i = 0; i < numberVertices; i++)
	{
		set[i].parent = i;
		set[i].prior = 0;
	}
}

bool Kruskal(Graph *graph, Edge *output)
{
	qsort(graph->edges, graph->numberEdges, sizeof(Edge), comparator);

	Set *sets = (Set *)malloc(graph->numberVertices * sizeof(Set));
	fillSets(sets, graph->numberVertices);

	int indexOutput = 0;
	for (int i = 0; indexOutput < graph->numberVertices - 1; i++)
	{

		if (i >= graph->numberEdges)
		{
			printf("no spanning tree\n");
			free(sets);
			return false;
		}

		Edge edge = graph->edges[i];

		int first = findIndex(edge.first, sets);
		int second = findIndex(edge.second, sets);

		if (first != second)
		{
			output[indexOutput++] = edge;
			unite(first, second, sets);
		}
	}

	free(sets);
	return true;
}

// OUTPUT ////////////////////////////////

void printOutput(int numberVertices, Edge *output)
{
	for (int i = 0; i < numberVertices - 1; i++)
	{
		printf("%d %d\n", output[i].first + 1, output[i].second + 1);
	}
}

//////////////////////////////////////////

void freeAll(Graph *graph, Edge *output)
{
	free(graph->edges);
	free(graph);
	free(output);
}
