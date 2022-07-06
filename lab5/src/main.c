#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>

//////////////////////////////////////////

#define CHAR_SIZE 256
#define BYTE_SIZE 8
#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

// STRUCTURES ////////////////////////////

typedef struct st_node Node;
typedef struct st_frequency_table FrequenceTable;
typedef struct st_byte Byte;

//////////////////////////////////////////

struct st_node
{
	unsigned char symbol;
	int frequency;

	Node *left_child;
	Node *right_child;
};

struct st_frequency_table
{
	int *symbols;
	int number_symbols;
	int size;
};

struct st_byte
{
	char bites[9];
	int len;
};

// ENCODER ///////////////////////////////

FrequenceTable *create_frequency_table(FILE *input_file);
Node *create_node(unsigned char symbol, int frequency);

int comparator(const void *a, const void *b);

Node **create_nodes_array(FrequenceTable *frequence_table);
Node *create_tree(FrequenceTable *frequence_table);

unsigned char **generate_code_table();
unsigned char **create_code_table(Node *head);

bool is_leaf(Node *node);
void encode_symbol(unsigned char **code_table, unsigned char *code, Node *node, int len);
void encode_tree(Node *node, FILE *output_file);

Byte *create_byte();
void from_bites_to_char(char bites[], FILE *output_file);
void clear_byte(Byte *byte);
void sent_char(unsigned char ch, Byte *byte, FILE *output_file);

void encode_text(FILE *input, unsigned char **code_table, FILE *output_file);

// DECODER ///////////////////////////////

int get_number_symbols(FILE *input_file);

Node *decode_tree(FILE *input_file);

void from_char_to_bites(unsigned int symbol, Byte *byte);
unsigned char get_char(Byte *byte, FILE *input_file);

FILE *decode_text(FILE *input_file, int number_symbols, Node *head);

// FREE //////////////////////////////////

void delete_frequency_table(FrequenceTable *frequency_table);
void delete_code_table(unsigned char **code_table);
void delete_tree(Node *head);
void delete_byte(Byte *byte);

// ENCODE ////////////////////////////////

void encode(FILE *input_file)
{
	FILE *output_file;
	if ((output_file = fopen(OUTPUT_FILE, "wb")) == NULL)
	{
		printf("ERROR");
		return;
	}

	FILE *tree_file;
	if ((tree_file = fopen("tree.txt", "wb")) == NULL)
	{
		printf("ERROR");
		fclose(output_file);
		return;
	}

	FrequenceTable *frequency_table = create_frequency_table(input_file);

	if (frequency_table->size == 0)
	{
		delete_frequency_table(frequency_table);
		fprintf(tree_file, "%d\n", 0);
		
		fclose(input_file);
		fclose(output_file);
		fclose(tree_file);

		return;
	}

	Node *head = create_tree(frequency_table);

	unsigned char **code_table = create_code_table(head);

	fprintf(tree_file, "%d\n", frequency_table->number_symbols);
	encode_tree(head, tree_file);

	fseek(input_file, 0, SEEK_SET);
	encode_text(input_file, code_table, output_file);

	fclose(input_file);
	fclose(output_file);
	fclose(tree_file);

	delete_frequency_table(frequency_table);
	delete_code_table(code_table);
	delete_tree(head);
}

// DECODE ////////////////////////////////

void decode(FILE *input_file)
{
	FILE* tree_file;
	if ((tree_file = fopen("tree.txt", "rb")) == NULL)
	{
		printf("ERROR");
	}

	int number_symbols = get_number_symbols(tree_file);

	if (number_symbols == 0)
	{
		fclose(tree_file);
		return;
	}

	Node *head = decode_tree(tree_file);

	FILE *output_file = decode_text(input_file, number_symbols, head);

	fclose(input_file);
	fclose(output_file);
	fclose(tree_file);

	delete_tree(head);
}

// MAIN //////////////////////////////////

int main()
{
	FILE *input_file;
	setlocale(LC_ALL, "Rus");

	if ((input_file = fopen(INPUT_FILE, "rb")) == NULL)
	{
		printf("ERROR");
		return EXIT_SUCCESS;
	}

	int ch = fgetc(input_file);

	if (ch == 'c')
	{
		encode(input_file);
	}

	else
	{
		decode(input_file);
	}

	return EXIT_SUCCESS;
}

//////////////////////////////////////////

FrequenceTable *create_frequency_table(FILE *input_file)
{
	FrequenceTable *frequency_table = malloc(sizeof(FrequenceTable));

	int *symbols = calloc(CHAR_SIZE, sizeof(int));

	int size = 0;
	int number_symbols = 0;

	int input_char = fgetc(input_file);
	while (input_char != EOF)
	{
		if (symbols[input_char] == 0)
		{
			size += 1;
		}

		number_symbols += 1;
		symbols[input_char] += 1;

		input_char = fgetc(input_file);
	}

	frequency_table->symbols = symbols;
	frequency_table->number_symbols = number_symbols;
	frequency_table->size = size;

	return frequency_table;
}

Node *create_node(unsigned char symbol, int frequency)
{
	Node *new_node = malloc(sizeof(Node));

	new_node->symbol = symbol;
	new_node->frequency = frequency;
	new_node->left_child = new_node->right_child = NULL;

	return new_node;
}

int comparator(const void *a, const void *b)
{
	Node **nodeA = (Node **)a;
	Node **nodeB = (Node **)b;

	return ((*nodeB)->frequency - (*nodeA)->frequency);
}

Node **create_nodes_array(FrequenceTable *frequence_table)
{
	Node **nodes_array = malloc(frequence_table->size * sizeof(Node *));
	int index = 0;

	for (int i = 0; i < CHAR_SIZE; i++)
	{
		if (frequence_table->symbols[i] != 0)
		{
			Node *node = create_node(i, frequence_table->symbols[i]);
			nodes_array[index++] = node;
		}
	}

	qsort(nodes_array, frequence_table->size, sizeof(Node *), comparator);

	return nodes_array;
}

Node *create_tree(FrequenceTable *frequence_table)
{
	Node **nodes_array = create_nodes_array(frequence_table);
	int size = frequence_table->size;

	if (size == 1)
	{
		Node *new_node = create_node(0, 0);
		Node *node1 = nodes_array[0];
		Node *node2 = create_node(node1->symbol, 0);

		new_node->left_child = node1;
		new_node->right_child = node2;

		free(nodes_array);
		return new_node;
	}

	while (nodes_array[1] != NULL)
	{
		Node *new_node = create_node(0, nodes_array[size - 1]->frequency + nodes_array[size - 2]->frequency);

		Node *node1 = nodes_array[size - 1];
		Node *node2 = nodes_array[size - 2];

		new_node->left_child = node1;
		new_node->right_child = node2;

		nodes_array[size - 1] = NULL;
		size--;
		nodes_array[size - 1] = new_node;

		qsort(nodes_array, size, sizeof(Node *), comparator);
	}

	Node* head = nodes_array[0];

	free(nodes_array);
	return head;
}

unsigned char **generate_code_table()
{
	unsigned char **code_table = malloc(CHAR_SIZE * sizeof(char *));

	for (int i = 0; i < CHAR_SIZE; i++)
	{
		code_table[i] = malloc(CHAR_SIZE * sizeof(char));
		code_table[i][0] = '\0';
	}

	return code_table;
}

bool is_leaf(Node *node)
{
	if (node->left_child == NULL)
	{
		return true;
	}

	return false;
}

void encode_symbol(unsigned char **code_table, unsigned char *code, Node *node, int len)
{
	if (is_leaf(node))
	{
		code[len] = '\0';
		strcpy((char *)code_table[node->symbol], (char *)code);
	}

	else
	{
		code[len] = '0';
		encode_symbol(code_table, code, node->left_child, len + 1);

		code[len] = '1';
		encode_symbol(code_table, code, node->right_child, len + 1);
	}
}

unsigned char **create_code_table(Node *head)
{
	unsigned char **code_table = generate_code_table();

	unsigned char *code = malloc(CHAR_SIZE * sizeof(char));

	encode_symbol(code_table, code, head, 0);

	free(code);
	return code_table;
}

void encode_tree(Node *node, FILE *output_file)
{
	if (is_leaf(node) == true)
	{
		fputc('1', output_file);
		fputc(node->symbol, output_file);
	}

	else
	{
		fputc('0', output_file);

		encode_tree(node->left_child, output_file);
		encode_tree(node->right_child, output_file);
	}
}

Byte *create_byte()
{
	Byte *byte = malloc(sizeof(*byte));

	for (int i = 0; i < 8; i++)
	{
		byte->bites[i] = '0';
	}

	byte->len = 0;

	return byte;
}

void from_bites_to_char(char bites[], FILE *output_file)
{
	int output_char = 0;

	for (int i = 0; i < 8; i++)
	{
		if (bites[i] == '1')
		{
			output_char += pow(2, 7 - i);
		}
	}

	fputc(output_char, output_file);
}

void clear_byte(Byte *byte)
{
	for (int i = 0; i < 8; i++)
	{
		byte->bites[i] = '0';
	}
	
	byte->len = 0;
}

void sent_char(unsigned char ch, Byte *byte, FILE *output_file)
{
	byte->bites[(byte->len)++] = ch;

	if (byte->len == BYTE_SIZE)
	{
		from_bites_to_char(byte->bites, output_file);
		clear_byte(byte);
	}
}

void encode_text(FILE *input, unsigned char **code_table, FILE *output_file)
{
	Byte *byte = create_byte();

	int input_char = fgetc(input);

	while ((input_char = fgetc(input)) != EOF)
	{
		for (unsigned int j = 0; j < strlen((char *)code_table[input_char]); j++)
		{
			sent_char(code_table[input_char][j], byte, output_file);
		}
	}

	if (byte->len != 0)
	{
		from_bites_to_char(byte->bites, output_file);
	}

	delete_byte(byte);

}

//////////////////////////////////////////

int get_number_symbols(FILE *input_file)
{
	int number_symbols = 0;

	if (fscanf(input_file, "%d\n", &number_symbols) != 1)
	{
		return -1;
	}

	return number_symbols;
}

Node *decode_tree(FILE *input_file)
{
	if (fgetc(input_file) == '1')
	{
		Node *new_node = create_node(fgetc(input_file), 0);
		return new_node;
	}

	else
	{
		Node *new_node = create_node(0, 0);

		Node *left_child = decode_tree(input_file);
		Node *right_child = decode_tree(input_file);

		new_node->left_child = left_child;
		new_node->right_child = right_child;

		return new_node;
	}
}

void from_char_to_bites(unsigned int symbol, Byte *byte)
{
	int i = BYTE_SIZE - 1;

	while (symbol != 0)
	{
		if (symbol % 2 == 0)
			byte->bites[i--] = '0';

		else
			byte->bites[i--] = '1';

		symbol = symbol / 2;
	}

	for (int j = 0; j <= i; j++)
	{
		byte->bites[j] = '0';
	}

	byte->bites[BYTE_SIZE] = '\0';
}

unsigned char get_char(Byte *byte, FILE *input_file)
{
	if (byte->len == BYTE_SIZE)
	{
		from_char_to_bites(fgetc(input_file), byte);
		byte->len = 0;
	}

	unsigned char ch = byte->bites[(byte->len)++];

	return ch;
}

FILE *decode_text(FILE *input_file, int number_symbols, Node *head)
{
	FILE *output_file;
	if ((output_file = fopen(OUTPUT_FILE, "wb")) == NULL)
	{
		printf("ERROR");
		return NULL;
	}

	Byte *byte = create_byte();
	byte->len = BYTE_SIZE;

	Node *node = head;

	while (number_symbols != 0)
	{
		unsigned char ch = get_char(byte, input_file);

		if (ch == '0')
		{
			node = node->left_child;
		}

		else
		{
			node = node->right_child;
		}

		if (is_leaf(node))
		{
			fputc(node->symbol, output_file);
			node = head;
			number_symbols--;
		}
	}

	delete_byte(byte);
	return output_file;
}

// FREE //////////////////////////////////

void delete_frequency_table(FrequenceTable *frequency_table)
{
	free(frequency_table->symbols);
	free(frequency_table);
}

void delete_code_table(unsigned char **code_table)
{
	for (int i = 0; i < CHAR_SIZE; i++)
	{
		free(code_table[i]);
	}

	free(code_table);
}

void delete_tree(Node *head)
{
	if (head != NULL && !is_leaf(head))
	{
		delete_tree(head->left_child);
		delete_tree(head->right_child);
	}

	free(head);
}

void delete_byte(Byte *byte)
{
	free(byte);
}
