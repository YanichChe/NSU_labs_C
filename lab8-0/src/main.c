#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

typedef struct _edge Edge;
struct _edge {
	int first;
	int second;
	int weight;
};

typedef struct _graph Graph;
struct _graph {
	int vertices_number;
	int edges_number;
	Edge* edges;
};

typedef struct _set Set;
struct _set {
	int parent;
	int prior;
};

int find_ind(int vertex, Set parents[]);
void unite(int first, int second, Set parents[]);
Graph* create_graph(int vertices_number, int edges_number);
int comparator(const void* first, const void* second);
void fill_sets(Set set[], int vertices_number);
int Kruskal(Graph* graph, Edge* output);

int main() {
	int number_vertices;
	int number_edges;
	if (scanf("%d", &number_vertices) != 1) {
		return EXIT_SUCCESS;
	}
	if (scanf("%d", &number_edges) != 1) {
		return EXIT_SUCCESS;
	}
	if (number_vertices == 0) {
		printf("no spanning tree\n");
		return EXIT_SUCCESS;
	}
	if (number_vertices < 0 || number_vertices > 5000) {
		printf("bad number of vertices");
		return EXIT_SUCCESS;
	}
	if (number_edges < 0 || number_edges >(number_vertices) * (number_vertices + 1) / 2) {
		printf("bad number of edges");
	}
	Graph* graph = create_graph(number_vertices, number_edges);
	Edge* output = (Edge*)malloc(number_vertices * sizeof(Edge));
	for (int i = 0; i < number_edges; i++) {
		int start;
		int end;
		int weight;
		if (scanf("%d %d %d", &start, &end, &weight) != 3) {
			printf("bad number of lines");
			free(graph->edges);
			free(graph);
			free(output);
			return EXIT_SUCCESS;
		}
		if (start < 1 || start > number_vertices || end < 1 || end > number_vertices) {
			printf("bad vertex");
			free(graph->edges);
			free(graph);
			free(output);
			return EXIT_SUCCESS;
		}
		if (weight < 0 || weight >INT_MAX) {
			printf("bad length");
			free(graph->edges);
			free(graph);
			free(output);
			return EXIT_SUCCESS;
		}
		graph->edges[i].first = start - 1;
		graph->edges[i].second = end - 1;
		graph->edges[i].weight = weight;
	}

	if (Kruskal(graph, output) == 0) {
		for (int i = 0; i < number_vertices - 1; i++) {
			printf("%d %d\n", output[i].first + 1, output[i].second + 1);
		}
	}
	free(graph->edges);
	free(graph);
	free(output);
	return EXIT_SUCCESS;
}

int find_ind(int vertex, Set parents[]) {
	if (parents[vertex].parent != vertex)
		parents[vertex].parent = find_ind(parents[vertex].parent, parents);
	return parents[vertex].parent;
}

void unite(int first, int second, Set parents[]) {
	int first_parent = find_ind(first, parents);
	int second_parent = find_ind(second, parents);

	if (parents[first_parent].prior > parents[second_parent].prior) {
		parents[second_parent].parent = first_parent;
		return;
	}

	else if (parents[first_parent].prior < parents[second_parent].prior) {
		parents[first_parent].parent = second_parent;
		return;
	}
	parents[second_parent].parent = first_parent;
	parents[first_parent].prior++;
}

Graph* create_graph(int vertices_number, int edges_number) {
	Graph* new_graph = (Graph*)malloc(sizeof(Graph));
	new_graph->vertices_number = vertices_number;
	new_graph->edges_number = edges_number;
	new_graph->edges = (Edge*)malloc(edges_number * sizeof(Edge));
	return new_graph;
}

int comparator(const void* first, const void* second) {
	Edge* el1 = (Edge*)first;
	Edge* el2 = (Edge*)second;
	return el1->weight > el2->weight;
}

void fill_sets(Set set[], int vertices_number) {
	for (int i = 0; i < vertices_number; i++) {
		set[i].parent = i;
		set[i].prior = 0;
	}
}

int Kruskal(Graph* graph, Edge* output) {
	qsort(graph->edges, graph->edges_number, sizeof(Edge), comparator);
	Set* sets = (Set*)malloc(graph->vertices_number * sizeof(Set));
	fill_sets(sets, graph->vertices_number);
	int n = 0;
	for (int i = 0; n < graph->vertices_number - 1; i++) {
		if (i >= graph->edges_number) {
			printf("no spanning tree\n");
			free(sets);
			return 1;
		}
		Edge edge = graph->edges[i];
		int first = find_ind(edge.first, sets);
		int second = find_ind(edge.second, sets);
		if (first != second) {
			output[n] = edge;
			n += 1;
			unite(first, second, sets);
		}
	}
	free(sets);
	return 0;
}
