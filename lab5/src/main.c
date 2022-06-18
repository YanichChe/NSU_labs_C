#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>

int last_bit;
FILE* file_output;
FILE* input_file;
char output[]= "00000000";

enum
{
	RIGHT = 1,
	LEFT = 0,
	SIZE_BYTE = 8,
	VALUES_CHAR = 256
};

typedef struct st_node Node;
struct st_node {
	unsigned char symbol;
	int value;
	char* code;
	Node* parent;
	Node* left_child;
	Node* right_child;
};

int is_leaf(Node* node);
Node* create_node(Node* parent, unsigned char symbol, int value);
Node* create_tree(Node* nodes[], int size);
int get_value(Node* node);
Node* create_tree(Node* nodes[], int size);

void swap(Node* nodes[], int i, int j);
void quick_sort(Node* nodes[], int first, int last);
void reverse(char* c);

void make_codes(Node* node, char** codes, int n);
void fill_sent(int* n, char ch);
void encode_tree(Node* node, char** codes, int count, int* n);

void from_bites_to_char(char* bites);
void from_char_to_bites(int symbol, char* bites);

int read_bite(int* i, char buffer[]);
unsigned char read_byte(int* ind, char buffer[]);
Node* read_tree(Node* head, int* i, char buffer[], int* n);

void free_tree(Node* head);
void create_frequency_table(int frequency_table[]);
void fill_frequency_table(int frequency_table[], int* size, int* count, int* last_char);
void create_nodes(Node** nodes, int size, int frequency_table[]);
void encode_text(int n, char** codes);
void free_encode(Node* node, char** codes, Node** nodes);

void decode_text(Node* head, char buffer[], int count, int i);

void encode() {
	int size = 0;
	int count = 0;
	int last_char = 0;
	int frequency_table[VALUES_CHAR];

	if ((file_output = fopen("output.txt", "wb")) == NULL)
	{
		printf("ERROR");
	}

	create_frequency_table(frequency_table);
	fill_frequency_table(frequency_table, &size, &count, &last_char);

	if (size == 0) {
		printf("%d", count);
		fclose(file_output);
		fclose(input_file);
		return;
	}
	if (size == 1) {
		printf("%d", -1);
		fprintf(file_output, "%d\n", count);
		fprintf(file_output, "%d", last_char);
		fclose(file_output);
		fclose(input_file);
		return;
	}

	Node** nodes = (Node**)malloc(size * sizeof(Node*));
	create_nodes(nodes, size, frequency_table);
	Node* node5 = create_tree(nodes, size);

	char** codes;
	codes = (char**)malloc(VALUES_CHAR * sizeof(char*));
	for (int i = 0; i < VALUES_CHAR; i++) {
		codes[i] = NULL;
	}

	make_codes(node5, codes, size + 1);
	int n = -1; // count index of array

	printf("%d", count);

	encode_tree(node5, codes, size, &n);
	fseek(input_file, 0, SEEK_SET);
	encode_text(n, codes);
	fclose(file_output);
	fclose(input_file);
	free_encode(node5, codes, nodes);
}

void decode() {
	int i = 9;
	char buffer[VALUES_CHAR + 1];
	for (int i = 0; i < 9; i++) {
		buffer[i] = '0';
	}
	int count = 0;
	int size = 0;

	if (fscanf(input_file, "%d", &count) != 1) {
		return;
	}
	if (count == 0) {
		return;
	}

	if ((file_output = fopen("output.txt", "rb")) == NULL)
	{
		printf("ERROR");
	}

	if (count == -1) {
		int number;
		int ch;
		if (fscanf(file_output, "%d\n%d", &number, &ch) != 2) {
			return;
		}
		for (int i = 0; i < number; i++) {
			printf("%c", ch);
		}
		return;
	}
	Node* head = read_tree(NULL, &i, buffer, &size);
	decode_text(head, buffer, count, i);
	free_tree(head);
}

///////////////////////////////////////////////////////////////////////////////////////////
int main() {
	setlocale(LC_ALL, "Rus");
	if ((input_file = fopen("in.txt", "r")) == NULL)
	{
		printf("ERROR");
		return EXIT_SUCCESS;
	}
	int ch = fgetc(input_file);
	if (ch == 'c') {
		encode();
	}
	else {
		decode();
	}
	return EXIT_SUCCESS;
}

////////////////////////  SECONDARY FUNCTIONS  //////////////////////////////////////////

void from_bites_to_char(char* bites) {
	int output_char = 0;
	for (int i = 0; i < 8; i++) {
		if (bites[i] == '1') {
			output_char += pow(2, 7 - i);
		}
	}
	fputc(output_char, file_output);
}

int is_leaf(Node* node) {
	if (node->left_child == NULL && node->right_child == NULL) {
		return true;
	}
	return false;
}

Node* create_node(Node* parent, unsigned char symbol, int value) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->code = (char*)malloc(2 * sizeof(char));
	new_node->code[0] = '\0';
	new_node->value = value;
	new_node->parent = parent;
	new_node->symbol = symbol;
	new_node->left_child = NULL;
	new_node->right_child = NULL;
	return new_node;
}

/*get the symbol frequency value*/
int get_value(Node* node) {
	if (node != NULL) {
		return node->value;
	}
	return 0;
}

void swap(Node* nodes[], int i, int j) {
	Node* tmp = nodes[i];
	nodes[i] = nodes[j];
	nodes[j] = tmp;
}

/*sort for add node*/
void quick_sort(Node* nodes[], int first, int last) {
	int i = first,
		j = last,
		x = get_value(nodes[(first + last) / 2]);
	do {
		while (get_value(nodes[i]) > x) i++;
		while (get_value(nodes[j]) < x) j--;
		if (i <= j) {
			if (get_value(nodes[i]) < get_value(nodes[j])) {
				swap(nodes, i, j);
			};
			i++;
			j--;
		}
	} while (i <= j);
	if (i < last) {
		quick_sort(nodes, i, last);
	}
	if (first < j) {
		quick_sort(nodes, first, j);
	}
}

/*get numbers of nodes which dont point to NULL*/
int get_real_size(Node* nodes[], int size) {
	int n = 0;
	for (int i = 0; i < size; i++) {
		if (nodes[i] != NULL) {
			n += 1;
		}
	}
	return n;
}

/*create Huffman tree*/
Node* create_tree(Node* nodes[], int size) {
	while (get_real_size(nodes, size) != 1) {
		Node* new_node = create_node(NULL, 0, get_value(nodes[size - 1]) + get_value(nodes[size - 2]));
		nodes[size - 1]->parent = new_node;
		nodes[size - 2]->parent = new_node;
		Node* node1 = nodes[size - 1];
		Node* node2 = nodes[size - 2];
		new_node->left_child = node1;
		new_node->right_child = node2;
		nodes[size - 1] = NULL;
		size--;
		nodes[size - 1] = new_node;
		quick_sort(nodes, 0, size - 1);
	}

	return nodes[0];
}

/*create code table */
void make_codes(Node* node, char** codes, int n) {
	if (is_leaf(node) == true) {
		unsigned char symbol = node->symbol;
		char* code = (char*)malloc(n * sizeof(char));
		int i = 0;
		while (node->parent != NULL) {
			if (node->parent->left_child == node) {
				code[i++] = '0';
			}
			else {
				code[i++] = '1';
			}
			node = node->parent;
		}
		code[i] = '\0';
		reverse(code);
		codes[symbol] = code;
	}
	else {
		if (node->left_child != NULL) {
			make_codes(node->left_child, codes, n);
		}
		if (node->right_child != NULL) {
			make_codes(node->right_child, codes, n);
		}
	}
}

/*add extra zeroes to byte*/
void fill_sent(int* n, char ch) {
	if (*n == 8) {
		from_bites_to_char(output);
		return;
	}
	*n += 1;
	output[*n] = ch;
	if (*n == 7) {
		from_bites_to_char(output);
		*n = -1;
		for (int i = 0; i < 8; i++) {
			output[i] = '0';
		}
		output[8] = '\0';
	}
}

void reverse(char* c) {
	char* new_char = (char*)malloc((strlen(c) + 1) * sizeof(char));
	for (unsigned int i = 0; i < strlen(c); i++) {
		new_char[i] = c[strlen(c) - i - 1];
	}
	new_char[strlen(c)] = '\0';
	c[0] = '\0';
	strncpy(c, new_char, strlen(new_char));
	free(new_char);
}

/*encode Huffman tree*/
void encode_tree(Node* node, char** codes, int count, int* n) {
	if (is_leaf(node) == true) {
		char bites[] = "00000000";
		fill_sent(n, '1');
		from_char_to_bites(node->symbol, bites);
		for (int i = 0; i < 8; i++) {
			fill_sent(n, bites[i]);
		}
	}
	else {
		fill_sent(n, '0');
		if (node->left_child != NULL) {
			encode_tree(node->left_child, codes, count, n);
		}
		if (node->right_child != NULL) {
			encode_tree(node->right_child, codes, count, n);
		}
	}
}

/*from char to bites[]*/
void from_char_to_bites(int symbol, char* bites) {
	int i = 7;
	while (symbol != 0) {
		if (symbol % 2 == 0) {
			bites[i] = '0';
			symbol = symbol / 2;
		}
		else {
			bites[i] = '1';
			symbol = symbol / 2;
		}
		i--;
	}

	bites[8] = '\0';
}


int read_bite(int* i, char buffer[]) {
	char ch = 'a';
	if (*i < 8) {
		ch = buffer[(*i)++];
	}
	if (*i >= 8) {
		int b = 0;
		b = fgetc(file_output);
		for (int j = 0; j < 8; j++) {
			buffer[j] = '0';
		}
		from_char_to_bites(b, buffer);

		if (*i == 9) {
			*i = 0;
			ch = buffer[(*i)++];
		}
		else {
			*i = 0;
		}
	}
	if (ch == '1') {
		return 1;
	}
	return 0;
}

/*read symbol*/
unsigned char read_byte(int* ind, char buffer[]) {
	int symbol = 0;
	for (int i = 7; i >= 0; i--) {
		int ch = read_bite(ind, buffer);
		if (ch == 1) {
			symbol += pow(2, i);
		}
	}
	return symbol;
}

/*decode tree*/
Node* read_tree(Node* head, int* i, char buffer[], int* n) {
	if (read_bite(i, buffer) == 1) {
		Node* new_node = create_node(head, read_byte(i, buffer), 0);
		*n += 1;
		return new_node;
	}
	else {
		Node* new_node = create_node(head, 0, 0);
		Node* left_child = read_tree(new_node, i, buffer, n);
		Node* right_child = read_tree(new_node, i, buffer, n);
		new_node->left_child = left_child;
		new_node->right_child = right_child;
		return new_node;
	}
}

void free_tree(Node* head) {
	if (head != NULL) {
		free_tree(head->left_child);
		free_tree(head->right_child);
		free(head->code);
		free(head);
	}
}

void create_frequency_table(int frequency_table[]) {
	for (int i = 0; i < VALUES_CHAR; i++) {
		frequency_table[i] = 0;
	}
}

void fill_frequency_table(int frequency_table[], int* size, int* count, int* last_char) {
	int input_char = fgetc(input_file);
	while (input_char != EOF) {
		if (frequency_table[input_char] == 0) {
			*size += 1;
		}
		*count += 1;
		frequency_table[input_char] += 1;
		*last_char = input_char;
		input_char = fgetc(input_file);
	}
}

void create_nodes(Node** nodes, int size, int frequency_table[]) {
	Node* node;
	int ind = 0;
	for (int i = 0; i < VALUES_CHAR; i++) {
		if (frequency_table[i] != 0) {
			node = create_node(NULL, i, frequency_table[i]);
			nodes[ind++] = node;
		}
	}
	quick_sort(nodes, 0, size - 1);
}

void encode_text(int n, char** codes) {
	int input_char = fgetc(input_file);
	while ((input_char = fgetc(input_file)) != EOF) {
		for (unsigned int j = 0; j < strlen(codes[input_char]); j++) {
			fill_sent(&n, codes[input_char][j]);
		}
	}
	for (int i = n + 1; i < 8;i++) {
		output[i] = '0';
	}
	if (n != -1) {
		n = 8;
		fill_sent(&n, '0');
	}
}

void free_encode(Node* node, char** codes, Node** nodes) {
	free_tree(node);
	for (int i = 0; i < VALUES_CHAR; i++) {
		if (codes[i] != NULL) {
			free(codes[i]);
		}
	}
	/*for (int i = 0; i < size; i++) {
		free(nodes[i]);
	}*/
	free(nodes);
	free(codes);
}

void decode_text(Node* head, char buffer[], int count, int i) {
	int bit;
	Node* tek_node = head;
	while ((bit = read_bite(&i, buffer)) != -1 && count != 0) {
		if (bit == 0) {
			if (tek_node->left_child != NULL) {
				tek_node = tek_node->left_child;
			}
			else {
				printf("%c", tek_node->symbol);
				count--;
				tek_node = head->left_child;
			}
		}
		else {
			if (tek_node->right_child != NULL) {
				tek_node = tek_node->right_child;
			}
			else {
				printf("%c", tek_node->symbol);
				count--;
				tek_node = head->right_child;
			}
		}
	}
}
