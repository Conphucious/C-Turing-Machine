#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	char data;
	struct Node *right;
	struct Node *left;
} Node;

typedef struct Instruction {
	int startState;
	int ascii;
	char write;
	char move;
	int endState;
} Instruction;

void insert(Node **headNode, char data);
void push(Node** headNode, char data);
void insertAfterNode(Node* previousNode, char data);
void displayLinkedList(Node *node);
char** loadFile();
void loadInstructions(char *tape, Instruction **instructions);

int currentState = 0;
int numOfStates = 0;
int numOfLines = 0;
int endStateCond = -1;

int main() {
	// load file
	char **tape = loadFile();
	int numOfStates = atoi(tape[1]);
	printf("Writing tape...\n");

	// load initial tape
	Node *headNode = NULL;
	for (int i = 0; i < strlen(tape[0]) - 1; i++)
		insert(&headNode, tape[0][i]);
	printf("Initial tape contents: ");
	displayLinkedList(headNode);

	// create instructions
	Instruction **instructions = malloc((sizeof(Instruction*) * 255) + 1);
	for (int i = 0; i < numOfStates; i++)
		instructions[i] = malloc((sizeof(Instruction) * numOfStates) + 1);

	// load instructions
	for (int i = 4; i < numOfLines; i++)
		loadInstructions(tape[i], instructions);

	// rwHead and process instructions on it
	Node* currentNode = headNode;

	endStateCond = (numOfStates - 1);
	while (currentState != endStateCond) {

		int ascii = currentNode -> data;

		currentNode -> data = instructions[currentState][ascii].write;
		char dir = instructions[currentState][ascii].move;
		printf("[%d][%d] and (%c %c %d)\n", currentState, ascii, currentNode -> data, dir, instructions[currentState][ascii].endState);
		if (currentState != instructions[currentState][ascii].endState)
			printf("STATE CHANGE FROM [%d] to [%d]\n", currentState, instructions[currentState][ascii].endState);
		displayLinkedList(headNode);

		int oldCurrent = currentState;
		currentState = instructions[currentState][ascii].endState;

		if (dir == 'L') {
			if (currentNode -> left == NULL) {
				push(&headNode, 'B');
				currentNode = currentNode -> left;
			} else
				currentNode = currentNode -> left;
		} else if (dir == 'R') {
			if (currentNode -> right == NULL) {
				insert(&currentNode, 'B');
				currentNode = currentNode -> right;
			} else
				currentNode = currentNode -> right;
		}
	}
	printf("Final tape contents: ");
	displayLinkedList(headNode);
	return 0;
}

char** loadFile() {
	printf("\n\n\nJ I M M Y ' S       T U R I N G      M A C H I N E\n");
	printf("--------------------------------------------------\n");
	printf("Enter tape location: > ");
	char *fileName = malloc(sizeof(char) * 150);
	fgets(fileName, 150, stdin);
	fileName[strlen(fileName) - 1] = '\0';

	FILE *file;
	file = fopen(fileName, "r");
	if (file == NULL) {
		printf("File could not be read!\n\n");
		return NULL;
	}
	char line[100];
	char contents[257][257];
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 100; j++)
		contents[i][j] = '\0';
	}
	int i = 0;
	while (fgets(line, sizeof(line), file))
	strcpy(contents[i++], line);
	numOfLines = i;

	// convert to char** to pass
	char **tape = malloc(sizeof(char*) * i);
	for (int j = 0; j < i; j++)
	tape[j] = malloc(sizeof(char) * i);

	for (int e = 0; e < i; e++) {
		for (int r = 0; r < i; r++)
		tape[e][r] = contents[e][r];
	}
	fclose(file);
	return tape;
}
void loadInstructions(char *tape, Instruction **instructions) {
	int state = atoi(&tape[0]);
	int ascii = tape[2];
	instructions[state][ascii].startState = atoi(&tape[0]);
	instructions[state][ascii].ascii = tape[2];
	instructions[state][ascii].write = tape[4];
	instructions[state][ascii].move = tape[6];
	instructions[state][ascii].endState = atoi(&tape[8]);
	// printf("[%d][%d] - (%c %c %d)\n", instructions[state][ascii].startState, instructions[state][ascii].ascii, instructions[state][ascii].write, instructions[state][ascii].move, instructions[state][ascii].endState);
}

void push(Node** headNode, char data) {	// adds to front
	Node* node = malloc(sizeof(Node));
	node -> data = data;

	node -> right = *headNode;
	node -> left = NULL;

	if (*headNode != NULL)
		(*headNode) -> left = node;

	(*headNode) = node;
}
void insertAfterNode(Node* previousNode, char data) {
	if (previousNode == NULL) {
		printf("UNABLE TO INSERT AFTER A NULL NODE AT TAIL\n");
		return;
	}

	Node* node = malloc(sizeof(Node));
	node -> data = data;
	node -> right = previousNode -> right;
	previousNode -> right = node;
	node -> left = previousNode;

	if (node -> right != NULL)
		node -> right -> left = node;
}
void insert(Node** headNode, char data) {
	Node* node = malloc(sizeof(Node));
	Node* lastNode = *headNode;
	node -> data = data;
	node -> right = NULL;

	if (*headNode == NULL) {
		node -> left = NULL;
		*headNode = node;
		return;
	}

	while (lastNode -> right != NULL)
		lastNode = lastNode -> right;
		lastNode -> right = node;
		node -> left = lastNode;
}
void displayLinkedList(Node* node) {
    Node* tail;
    while (node != NULL) {
        printf("%c", node->data);
        tail = node;
        node = node->right;
    }
	printf("\n");
}
