#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int value;
	int height;
	struct Node* left;
	struct Node* right;
}Node;
int i = 0;
Node* head;
Node* createNode(int value, Node nodes[]) {
	Node* node = &nodes[i++];
	node->value = value;
	node->right = NULL;
	node->left = NULL;
	node->height = 1;
	return node;
}
int height(Node* node) {
	if (node) {
		return node->height;
	}
	return 0;
}
int bfactor(Node* parent) {
	return (height(parent->right) - height(parent->left));
}
void fixHeight(Node* parent) {
	int lHeight = 0;
	int rHeight = 0;
	if (parent == NULL) { return; }
	if (parent->left) {
		lHeight = parent->left->height;
	}
	if (parent->right) {
		rHeight = parent->right->height;
	}
	if (rHeight > lHeight) {
		parent->height = rHeight + 1;
	}
	else {
		parent->height = lHeight + 1;
	}

}
Node* right_rotate(Node* parent) {
	Node* newParent = parent->left;
	head = newParent;
	parent->left = newParent->right;
	newParent->right = parent;
	fixHeight(parent);
	fixHeight(newParent);
	return newParent;
}
Node* left_rotate(Node* parent) {
	Node* newParent = parent->right;
	head = newParent;
	parent->right = newParent->left;
	newParent->left = parent;
	fixHeight(parent);
	fixHeight(newParent);
	return newParent;
}
Node* balance(Node* parent) {
	fixHeight(parent);
	if (bfactor(parent) == 2) {
		if (bfactor(parent->right) < 0) {
			parent->right = right_rotate(parent->right);
		}
		return left_rotate(parent);
	}
	if (bfactor(parent) == -2) {
		if (bfactor(parent->left) > 0) {
			parent->left = left_rotate(parent->left);
		}
		return right_rotate(parent);
	}
	return parent;
}
Node* insert(Node* parent, int data, Node nodes[]) {
	if (parent == NULL) {
		parent = createNode(data, nodes);
	}
	else {
		if (data < parent->value) {
			parent->left = insert(parent->left, data, nodes);
		}
		else {
			parent->right = insert(parent->right, data, nodes);
		}
	}
	return balance(parent);
}
int main(void) {
	Node* root = NULL;
	int len;
	Node* nodes = NULL;
	if (scanf("%d", &len) != 1) { return EXIT_SUCCESS; }
	if (len == 0) { printf("%d", 0); }
	else {
		nodes = (Node*)malloc(sizeof(Node) * len);
		for (int i = 0; i < len; i++) {
			int obj;
			if (scanf("%d", &obj) != 1) { return EXIT_SUCCESS;; }
			if (i == 0) {
				root = createNode(obj, nodes);
				head = root;
			}
			else { head = insert(head, obj, nodes); }
		}
		fixHeight(head);
		printf("%d\n", height(head));
		free(nodes);
	}
	return EXIT_SUCCESS;
}
