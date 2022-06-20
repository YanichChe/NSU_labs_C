#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////
typedef struct Node
{
	int value;
	int height;

	struct Node *left;
	struct Node *right;

} Node;

typedef struct Tree
{
	Node *head;
	Node **nodes;
	int len;

} Tree;

//////////////////////////////////////////

Node *createNode(int value, Tree *tree);
Tree *createTree(int len);

void input_tree(Tree *tree);

int getHeight(Node *node);
int bfactor(Node *parent);
void fixHeight(Node *parent);

Node *rightRotate(Node *parent, Tree *tree);
Node *leftRotate(Node *parent, Tree *tree);
Node *balance(Node *parent, Tree *tree);
Node *insert(Node *parent, int data, Tree *tree);

// MAIN //////////////////////////////////

int main(void)
{
	int len;

	if (scanf("%d", &len) != 1)
		return EXIT_SUCCESS;

	if (len == 0)
	{
		printf("%d", 0);
		return EXIT_SUCCESS;
	}

	Tree *tree = createTree(len);
	input_tree(tree);

	printf("%d\n", getHeight(tree->head));

	return EXIT_SUCCESS;
}

//////////////////////////////////////////

Node *createNode(int value, Tree *tree)
{
	Node *node = tree->nodes[tree->len];

	node->value = value;
	node->right = NULL;
	node->left = NULL;
	node->height = 1;

	tree->len++;
	return node;
}

Tree *createTree(int len)
{
	Tree *tree = malloc(sizeof(Tree));

	Node **nodes = malloc(sizeof(Node *) * len);
	for (int i = 0; i < len; i++)
	{
		nodes[i] = malloc(sizeof(Node) * len);
	}

	tree->head = NULL;
	tree->nodes = nodes;
	tree->len = 0;

	return tree;
}

// INPUT /////////////////////////////////

void input_tree(Tree *tree)
{
	while (1)
	{

		int obj;

		if (scanf("%d", &obj) != 1)
			return;

		if (tree->len == 0)
		{
			Node *root = createNode(obj, tree);
			tree->head = root;
		}
		else
			tree->head = insert(tree->head, obj, tree);
	}

	fixHeight(tree->head);
}

// AVL ///////////////////////////////////

int getHeight(Node *node)
{
	if (node)
		return node->height;
	return 0;
}

int bfactor(Node *parent)
{
	return (getHeight(parent->right) - getHeight(parent->left));
}

void fixHeight(Node *parent)
{
	int lHeight = 0;
	int rHeight = 0;

	if (parent == NULL)
		return;

	if (parent->left)
		lHeight = parent->left->height;

	if (parent->right)
		rHeight = parent->right->height;

	if (rHeight > lHeight)
		parent->height = rHeight + 1;
	else
		parent->height = lHeight + 1;
}

Node *rightRotate(Node *parent, Tree *tree)
{
	Node *newParent = parent->left;
	tree->head = newParent;

	parent->left = newParent->right;
	newParent->right = parent;

	fixHeight(parent);
	fixHeight(newParent);

	return newParent;
}

Node *leftRotate(Node *parent, Tree *tree)
{
	Node *newParent = parent->right;
	tree->head = newParent;

	parent->right = newParent->left;
	newParent->left = parent;

	fixHeight(parent);
	fixHeight(newParent);

	return newParent;
}

Node *balance(Node *parent, Tree *tree)
{
	fixHeight(parent);

	if (bfactor(parent) == 2)
	{

		if (bfactor(parent->right) < 0)
			parent->right = rightRotate(parent->right, tree);

		return leftRotate(parent, tree);
	}

	if (bfactor(parent) == -2)
	{
		if (bfactor(parent->left) > 0)
			parent->left = leftRotate(parent->left, tree);

		return rightRotate(parent, tree);
	}

	return parent;
}

Node *insert(Node *parent, int data, Tree *tree)
{
	if (parent == NULL)
		parent = createNode(data, tree);

	else
	{

		if (data < parent->value)
			parent->left = insert(parent->left, data, tree);

		else
			parent->right = insert(parent->right, data, tree);
	}

	return balance(parent, tree);
}
