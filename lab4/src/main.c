#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//////////////////////////////////////////

#define DIVISION_BY_ZERO 1
#define SUCCES 2;
#define MAX_SIZE 2005
#define MAX_INPUT_SIZE 1001

//////////////////////////////////////////

typedef struct struct_node Node;
typedef struct struct_stack Stack;

//////////////////////////////////////////
struct struct_node
{
    int value;
    Node *next;
};

struct struct_stack
{
    Node *head;
};

//////////////////////////////////////////

bool inputInfixForm(char *infixForm);

Node *createNode(int value);

Stack *createStack();
void push(Stack *stack, int value);
int pop(Stack *stack);
int isEmpty(Stack *stack);
void destroyStack(Stack *stack);
char getElement(Stack *stack);

int priority(char operationSign);
bool isOperation(char currentSymbol);
void digit(int *in, int *out, unsigned int currentIndex, char deck[], char infixForm[], char currentSymbol);
bool operation(unsigned int currentIndex, char deck[], char infixForm[], Stack *stack, char currentSymbol);
bool clearStack(Stack* stack, char* deck);

void openBracket(int *in, Stack *stack, char currentSymbol);
bool closeBracket(int *in, int *out, Stack *stack, char deck[]);

bool makePolishNotation(char infixForm[], char deck[]);

void plus(Stack* stack);
void minus(Stack* stack);
void multiplication(Stack* stack);
int division(Stack* stack);
void pushDigit(char currentSymbol, int* currentIndex, char* polishNotation, Stack* stack);
int count(char s[]);

//////////////////////////////////////////

int main(void)
{
    char infixForm[MAX_INPUT_SIZE];

    if (!inputInfixForm(infixForm))
    {
        return EXIT_SUCCESS;
    }

    char deck[MAX_SIZE] = "";

    if (!makePolishNotation(infixForm, deck))
    {
        printf("syntax error");
        return EXIT_SUCCESS;
    }

    if (count(deck) == DIVISION_BY_ZERO)
    {
        printf("division by zero");
    }

    return EXIT_SUCCESS;
}

// ERRORS ////////////////////////////////

bool checkLength(int len)
{
    if (len == 0)
    {
        printf("syntax error");
        return false;
    }

    return true;
}

//////////////////////////////////////////

bool inputInfixForm(char *infixForm)
{
    int c;
    int len = 0;

    while ((c = getchar()) != '\n')
    {
        infixForm[len++] = c;
    }

    if (!checkLength(len))
    {
        return false;
    }

    infixForm[len] = '\0';

    return true;
}

// STACK //////////////////////////////////

Node *createNode(int value)
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->value = value;
    node->next = NULL;

    return node;
}

Stack *createStack()
{
    Stack *stack = malloc(sizeof(Stack));
    stack->head = NULL;

    return stack;
}

void push(Stack *stack, int value)
{
    Node *node = createNode(value);
    node->next = stack->head;
    stack->head = node;
}

int pop(Stack *stack)
{
    if (stack->head)
    {
        Node *node = stack->head;
        int value = node->value;
        stack->head = node->next;

        free(node);

        return value;
    }

    return -1;
}

int isEmpty(Stack *stack)
{
    return stack->head == NULL;
}

void destroyStack(Stack *stack)
{
    while (isEmpty(stack) != 1)
    {
        pop(stack);
    }
    free(stack);
}

char getElement(Stack *stack)
{
    char value = pop(stack);
    push(stack, value);
    return value;
}

// GET POLISH NOTATION //////////////////

bool isOperation(char c)
{
    if (c == '*' || c == '/' || c == '+' || c == '-')
    {
        return true;
    }

    return false;
}

int priority(char operationSign)
{
    if ( operationSign == '+' ||  operationSign == '-')
    {
        return 1;
    }

    else if (operationSign == '/' ||  operationSign == '*')
    {
        return 2;
    }

    else
    {
        return 0;
    }
}

bool operation(unsigned int currentIndex, char deck[], char infixForm[], Stack *stack, char currentSymbol)
{
    if (((currentIndex + 1) < strlen(infixForm) && isOperation(infixForm[currentIndex + 1])) || currentIndex + 1 >= strlen(infixForm))
    {
        destroyStack(stack);
        return false;
    }

    while (priority(getElement(stack)) >= priority(currentSymbol))
    {
        deck[strlen(deck)] = pop(stack);
        deck[strlen(deck)] = ' ';
    }

    push(stack, currentSymbol);

    return true;
}

void openBracket(int *in, Stack *stack, char currentSymbol)
{
    *in = 1;
    push(stack, currentSymbol);
}

bool closeBracket(int *in, int *out, Stack *stack, char deck[])
{
    if (*in == 1 && *out == 0)
    {
        destroyStack(stack);
        return false;
    }

    *in = 0;
    *out = 0;

    while (getElement(stack) != '(')
    {
        if (getElement(stack) == -1)
        {
            destroyStack(stack);
            return false;
        }
        deck[strlen(deck)] = pop(stack);
        deck[strlen(deck)] = ' ';
    }

    pop(stack);

    return true;
}

void digit(int *in, int *out, unsigned int currentIndex, char deck[], char infixForm[], char currentSymbol)
{
    if (*in == 1)
    {
        *out = 1;
    }
    deck[strlen(deck)] = currentSymbol;
    if (currentIndex + 1 < strlen(infixForm) && (infixForm[currentIndex + 1] < '0' || infixForm[currentIndex + 1] > '9'))
    {
        deck[strlen(deck)] = ' ';
    }
    else if (currentIndex + 1 == strlen(infixForm))
    {
        deck[strlen(deck)] = ' ';
    }
}

bool clearStack(Stack* stack, char* deck)
{
    char element;

    while ((element = pop(stack)) != -1)
    {
        if (element == '(')
        {
            destroyStack(stack);
            return false;
        }
        deck[strlen(deck)] = element;
        deck[strlen(deck)] = ' ';
    }

    deck[strlen(deck)] = '=';

    return true;
}

bool makePolishNotation(char infixForm[], char deck[])
{
    Stack *stack = createStack();

    int in_bracket = 0;
    int out_bracket = 0;

    for (unsigned int currentIndex = 0; currentIndex < strlen(infixForm); currentIndex++)
    {
        char currentSymbol = infixForm[currentIndex];

        if (isOperation(currentSymbol))
        {
            if (!operation(currentIndex, deck, infixForm, stack, currentSymbol))
            {
                return false;
            }
        }

        else if (currentSymbol == '(')
        {
            openBracket(&in_bracket, stack, currentSymbol);
        }

        else if (currentSymbol == ')')
        {
            if (!closeBracket(&in_bracket, &out_bracket, stack, deck))
            {
                return false;
            }
        }

        else if (isdigit(infixForm[currentIndex]))
        {
            digit(&in_bracket, &out_bracket, currentIndex, deck, infixForm, currentSymbol);
        }

        else
        {
            destroyStack(stack);
            return false;
        }
    }

    if (!clearStack(stack, deck)){
        return false;
    }

    return true;
}

// GET RESULT ////////////////////////////

void plus(Stack *stack)
{
    int res = pop(stack) + pop(stack);
    push(stack, res);
}

void minus(Stack *stack)
{
    int op1 = pop(stack);
    int op2 = pop(stack);
    int res = op2 - op1;
    push(stack, res);
}

void multiplication(Stack *stack)
{
    int res = pop(stack) * pop(stack);
    push(stack, res);
}

int division(Stack *stack)
{
    int op2 = pop(stack);

    if (op2 == 0)
    {
        destroyStack(stack);
        return DIVISION_BY_ZERO;
    }

    int res = pop(stack) / op2;

    push(stack, res);

    return SUCCES;
}

void pushDigit(char currentSymbol, int* currentIndex, char* polishNotation, Stack* stack){
    int res = currentSymbol - '0';

    while (*currentIndex + 1 < strlen(polishNotation) - 1 && polishNotation[(*currentIndex + 1)] != ' ')
    {
        res = res * 10 + (polishNotation[++(*currentIndex)] - '0');
    }

    push(stack, res);
}

int count(char polishNotation[])
{
    int res, op1, op2;

    Stack *stack = createStack();

    for (unsigned int currentIndex = 0; currentIndex < strlen(polishNotation); currentIndex++)
    {
        char currentSymbol = polishNotation[currentIndex];

        switch (currentSymbol)
        {

        case ' ':
            break;

        case '=':
            printf("%d", pop(stack));
            destroyStack(stack);
            return SUCCES;

        case '+':
            plus(stack);
            break;

        case '-':
            minus(stack);
            break;

        case '*':
            multiplication(stack);
            break;

        case '/':

            if (division(stack) == DIVISION_BY_ZERO){
                return DIVISION_BY_ZERO;
            }

            break;

        default:
            pushDigit(currentSymbol, &currentIndex, polishNotation, stack);
            break;
        }
    }
    return SUCCES;
}
