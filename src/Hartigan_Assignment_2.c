// COURSE: CS390
// AUTHOR: Matt Hartigan
// DATE: 12-May-2019


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Define macro constants
#define MAX_STATES 25    // max number of states allowed
#define MAX_STATE_SIZE 10    // max size of a given state
#define MAX_FILENAME_LENGTH 25    // max filename length


// Define user-defined data structures
typedef struct state {
	char symbol[MAX_STATE_SIZE];    // string to hold state symbol
} STATE;

typedef struct transition {
	STATE *startingState;    // holds starting state for a transition
	STATE *transitionFunction;    // holds symbol for transition function
	STATE *endingState;    // holds ending state for a transition
	struct transition *next;    // points to next node in linked list
} TRANSITION_NODE;

typedef struct automaton {
	STATE *stateArrayPtr;    // points to array of STATE data structures
	STATE *initialStatePtr;    // points to initial state of automaton
	STATE *finalStateArrayPtr;    // points to array of STATE data structures
	TRANSITION_NODE *transitionListPtr;    // points to linked list of transition functions
} AUTOMATON;


// Define function prototypes
int main(void);
char* findTransitionFunction(char* str1, char* str2, AUTOMATON* targetDFA);  
void createDFA(char* filename);
AUTOMATON* loadDFA(char* filename);
void executeDFA(AUTOMATON* targetDFA);


// Define functions
// Function findTransitionFunction - Takes head node of linked list and two parameters, searches
// through linked list until a match is found and returns the match. 
char* findTransitionFunction(char* str1, char* str2, AUTOMATON* targetDFA) {
	char* returnStr;
	returnStr = NULL;    // initialize to null
	TRANSITION_NODE *checkNode = targetDFA->transitionListPtr;    // point to list head
	
	while(checkNode != NULL) {
		if ( (strcmp(str1, checkNode->startingState->symbol) == 0) && (strcmp(str2, checkNode->transitionFunction->symbol) == 0)) {
			returnStr = malloc(sizeof(MAX_STATE_SIZE));
			strcpy(returnStr, checkNode->endingState->symbol);
			return returnStr;
		}
		
		checkNode = checkNode->next;	// move to next node
	}
	
	return returnStr;
}

// Function createDFA - Creates a DFA and saves it in an output file based on user input.
void createDFA(char* filename) {
	FILE *outputFileObject;

	// Open DFA output file
	outputFileObject = fopen(filename, "w");
	if (outputFileObject == NULL) {
		printf("ERROR: Unable to create output file.  Exiting now.");
		exit(1);
	}
	else {
		printf("Successfully created DFA output file.\n");
		
		STATE userInput;    // temp STATE to hold user input
		STATE *inputStateArray[MAX_STATES];    // array of states
		STATE inputInitialState;    // intial state
		STATE *inputFinalStateArray[MAX_STATES];    // array of final states
		int i;
		for (i = 0; i < MAX_STATES; i++) {    // initialize array values to null
			inputStateArray[i] = NULL;
			inputFinalStateArray[i] = NULL;
		}	
		
		// Prompt user for to enter all states
		printf("Note that all input should be single chars.\n");
		printf("Enter states one-by-one.  Type 'Done' into prompt to move on: ");
		fflush(stdin);
		scanf("%s", userInput.symbol);
		
		int j = 0;
		STATE *statePtr;    // used to place state elements into array
		statePtr = malloc(sizeof(STATE));
		while ( (strcmp(userInput.symbol, "Done")  != 0) && (j < MAX_STATES ) ) {
			inputStateArray[j] = statePtr;
			strcpy(statePtr->symbol, userInput.symbol);
			fprintf(outputFileObject, "%s ", inputStateArray[j]->symbol);
			j++;
			statePtr = malloc(sizeof(STATE));
			printf("Enter next state: ");
			fflush(stdin);
			scanf("%s", userInput.symbol);
		}
		fprintf(outputFileObject, "END\n");
		printf("Done inputting states.\n");
	
		// Prompt user for initial state
		printf("Enter initial state: ");
		fflush(stdin);
		scanf("%s", inputInitialState.symbol);
		fprintf(outputFileObject, "%s ", inputInitialState.symbol);
		fprintf(outputFileObject, "END\n");
		printf("Done inputting initial state.\n");
		
		// Prompt user for final states
		printf("Enter final states one-by-one.  Type 'Done' into prompt to move on: ");
		fflush(stdin);
		scanf("%s", userInput.symbol);
		
		int k = 0;
		STATE *finalStatePtr;    // used to place final state elements into array
		finalStatePtr = malloc(sizeof(STATE));
		while ( (strcmp(userInput.symbol, "Done")  != 0) && (k < MAX_STATES ) ) {
			inputFinalStateArray[k] = finalStatePtr;
			strcpy(finalStatePtr->symbol, userInput.symbol);
			fprintf(outputFileObject, "%s ", inputFinalStateArray[k]->symbol);
			k++;
			finalStatePtr = malloc(sizeof(STATE));
			printf("Enter next final state: ");
			fflush(stdin);
			scanf("%s", userInput.symbol);
		}
		fprintf(outputFileObject, "END\n");
		printf("Done inputting final state(s).\n");


		// Prompt user for transition functions, generate nodes, store in a linked list
		printf("Follow the subsequent prompts to enter transition functions one-by-one.\n");
		fflush(stdin);
		
		int m = 0;
		STATE *transitionPtr;    // used to place values into fields of TRANSITION_NODE
		transitionPtr = malloc(sizeof(STATE));
		TRANSITION_NODE *transitionList = malloc(sizeof(TRANSITION_NODE));    // head node for linked list
		TRANSITION_NODE *inputTransitionNode = malloc(sizeof(TRANSITION_NODE));	
		
		printf("Type any key to enter a transition function.  Type 'Done' to stop: ");
		fflush(stdin);
		scanf("%s", userInput.symbol);
		
		while ( strcmp(userInput.symbol, "Done")  != 0 ) {
			
			// Starting state
			printf("Transition %d - Enter starting state: ", m);
			fflush(stdin);
			scanf("%s", userInput.symbol);		
			inputTransitionNode->startingState = transitionPtr;    
			strcpy(transitionPtr->symbol, userInput.symbol);
			transitionPtr = malloc(sizeof(STATE));			
			
			// Transition function
			printf("Transition %d - Enter transition function: ", m);
			fflush(stdin);
			scanf("%s", userInput.symbol);		
			inputTransitionNode->transitionFunction = transitionPtr;
			strcpy(transitionPtr->symbol, userInput.symbol);
			transitionPtr = malloc(sizeof(STATE));
			
			// Ending state
			printf("Transition %d - Enter ending state: ", m);
			fflush(stdin);
			scanf("%s", userInput.symbol);		
			inputTransitionNode->endingState = transitionPtr;
			strcpy(transitionPtr->symbol, userInput.symbol);
			transitionPtr = malloc(sizeof(STATE));
			
			fprintf(outputFileObject, "%s/%s/%s ", inputTransitionNode->startingState->symbol, inputTransitionNode->transitionFunction->symbol, inputTransitionNode->endingState->symbol);
			
			// Add node to linked list
			if (m == 0) {    // empty list
				inputTransitionNode->next = NULL;
				transitionList = inputTransitionNode;    // point list head to first node
				inputTransitionNode = malloc(sizeof(TRANSITION_NODE));    // point to fresh block of memory
			}
			else {    // non-empty list
				inputTransitionNode->next = transitionList;    // copy previous list head to 'next' of new head node
				transitionList = inputTransitionNode;    // point list head to new first node
				inputTransitionNode = malloc(sizeof(TRANSITION_NODE));    // point to fresh block of memory
			}
			
			m++;

			printf("Type any key to enter another transition function.  Type 'Done' to stop: ");
			fflush(stdin);
			scanf("%s", userInput.symbol);
		}
		fprintf(outputFileObject, "END\n");
		printf("Done inputting transition function(s).\n");
	}

	fclose(outputFileObject);
}

// Function loadDFA - Reads a DFA from input file and stores in an
// automaton data structure for later use.
AUTOMATON* loadDFA(char* filename) {
	char inputFilename[MAX_FILENAME_LENGTH];
	FILE *inputFileObject;
	
	// Process according to whether or not filename parameter is defined
	if (strcmp(filename, "BLANK") == 0) {
		// Prompt user for input filename
		printf("Please provide the filename of the saved DFA (must be stored in same directory as Hartigan_Assignment_2.c): ");
		fflush(stdin);
		scanf("%s", &inputFilename);
	}
	else {
		strcpy(inputFilename, filename);
	}
	
	// Open file, read and store contents
	inputFileObject = fopen(inputFilename, "r");
	if (inputFileObject == NULL) {
		printf("ERROR: Input file not found.  Exiting now.");
		exit(1);
	}
	else {
		printf("Successfully opened input file.\n");
		
		AUTOMATON savedDFA;    // create automaton instance to store data read from file
		savedDFA.stateArrayPtr = malloc(sizeof(STATE) * MAX_STATES);
		savedDFA.initialStatePtr = malloc(sizeof(STATE));
		savedDFA.finalStateArrayPtr = malloc(sizeof(STATE) * MAX_STATES);
		
		int i, j;
		char *inputString;

		// Read states
		j = 0;
		fscanf(inputFileObject, "%s", inputString);
		
		while (strcmp(inputString, "END") != 0) {
			strcpy(savedDFA.stateArrayPtr[j].symbol, inputString);
			j++;
			inputString = malloc(sizeof(STATE));
			fscanf(inputFileObject, "%s", inputString);
		}
		
		// Read initial state
		j = 0;
		fscanf(inputFileObject, "%s", inputString);
		while (strcmp(inputString, "END") != 0) {
			strcpy(savedDFA.initialStatePtr->symbol, inputString);
			j++;
			inputString = malloc(sizeof(STATE));
			fscanf(inputFileObject, "%s", inputString);
		}
		
		// Read final states
		j = 0;
		fscanf(inputFileObject, "%s", inputString);
		while (strcmp(inputString, "END") != 0) {
			strcpy(savedDFA.finalStateArrayPtr[j].symbol, inputString);
			j++;
			inputString = malloc(sizeof(STATE));
			fscanf(inputFileObject, "%s", inputString);
		}
		
		// Read transition functions
		i  = 0;
		j = 0;
		TRANSITION_NODE *readNode = malloc(sizeof(TRANSITION_NODE));
		TRANSITION_NODE *readList = malloc(sizeof(TRANSITION_NODE));    // head node for linked list read from file

		fscanf(inputFileObject, "%s", inputString);    // get first transition function
		while (strcmp(inputString, "END") != 0) {
			// parse input string and assign values to the appropriate TRANSITION_NODE fields
			char *parser = strtok(inputString, "/");
			while (parser != NULL) {
				if (i == 0) {    // starting state
					readNode->startingState = malloc(sizeof(STATE));
					strcpy(readNode->startingState->symbol, parser);
					parser = strtok(NULL, "/");
					i++;
				}
				else if (i == 1) {    // transition function
					readNode->transitionFunction = malloc(sizeof(STATE));
					strcpy(readNode->transitionFunction->symbol, parser);
					parser = strtok(NULL, "/");
					i++;
				}
				else {    // ending state
					readNode->endingState = malloc(sizeof(STATE));
					strcpy(readNode->endingState->symbol, parser);
					parser = strtok(NULL, "/");
				}	
			}
			
			// readNode now has the correct states... add it to the linked list
			if (j == 0) {    // empty list
				readNode->next = NULL;
				readList = readNode;    // point list head to first node
				readNode = malloc(sizeof(TRANSITION_NODE));    // point to fresh block of memory
			}
			else {    // non-empty list
				readNode->next = readList;    // copy previous list head to 'next' of new head node
				readList = readNode;    // point list head to new first node
				readNode = malloc(sizeof(TRANSITION_NODE));    // point to fresh block of memory
			}
			
			j++;			
			
			// get next transition function and repeat process
			i = 0;
			inputString = malloc(sizeof(STATE));
			fscanf(inputFileObject, "%s", inputString);
		}
		
		// Add transition function linked list to automaton
		savedDFA.transitionListPtr = readList;
		
		// Close input file and return automaton pointer
		fclose(inputFileObject);
		printf("Successfully loaded DFA.\n");
		return &savedDFA;
	}
}

// Function executeDFA - Uses the DFA stored in the AUTOMATON parameter to 
// run through an input test string and determine whether or not it is acceptable
// based on the rulse of the DFA.
void executeDFA(AUTOMATON* targetDFA) {
	char testString[MAX_FILENAME_LENGTH];    // holds user input
	char str1[MAX_FILENAME_LENGTH];
	char str2[MAX_FILENAME_LENGTH];
	char temp[2];
	char *returnStrPtr;
	int i;
	int isValid = 1;
	
	// Prompt user for string to test
	printf("Please type in the string you would like to test.  Press return when finished.\n");
	fflush(stdin);
	scanf("%s", testString);
	printf("The input test string is: %s\n", testString);
	
	// Get first char, match to initial state
	i = 0;
	temp[0] = testString[i];
	printf("The first char is: %c\n", temp[0]);
	temp[1] = '\0';
	strcpy(str1,temp);
	strcpy(str2,targetDFA->initialStatePtr->symbol);
	if (strcmp(str1,str2) != 0) {    // initial state does not match
		printf("Initial state does not match.  The test has failed!\n");
		exit(1);
	}
	else {    // current state matches, prepare for next check
		i++;
		temp[0] = testString[i];
		temp[1] = '\0';
		strcpy(str2,temp);    // get second char from test string to check
	}
	
	// Loop through remaining chars, matching to available transition functions
	while (testString[i] != '\0') {
		
		returnStrPtr = findTransitionFunction(str1, str2, targetDFA);  
		
		if (returnStrPtr != NULL) {    // match found
			strcpy(str1,returnStrPtr);    // update state
			i++;
			temp[0] = testString[i];
			temp[1] = '\0';
			strcpy(str2,temp);    // get next check case
		}
		else {    // no match
			printf("No transition function found.  The test has failed!\n");
			exit(1);
		}
	}
	
	// Check for final state match
	i = 0;
	printf("The final state calculated is: %s\n", str1);
	while ( i < MAX_STATES) {
		if (strcmp(targetDFA->finalStateArrayPtr[i].symbol, str1) == 0) {
			printf("Final state matches. The test string is accepted!\n");
			return;
		}
		
		i++;
	}
	
	printf("Final state does not match.  The test has failed!\n");    // no final state match found
}

// Function main - Main function for program.
int main() {
	int userResponse = 0;
	char outputFilename[MAX_FILENAME_LENGTH];	
	AUTOMATON *inputDFA;
	inputDFA = malloc(sizeof(AUTOMATON));
	
	// Greet user and prompt for session type
	printf("Welcome to Assignment 2.\n");
	printf("Would you like to create a new DFA (type: 1), or use a saved one (type: 2)?\n");
	fflush(stdin);
	scanf("%i", &userResponse);
	
	// Execute session type
	// Create new DFA
	if (userResponse == 1) {
		printf("You chose to create a new DFA.\n");
		
		// Prompt user for DFA output filename
		printf("Please provide the output filename to save the DFA to: ");
		fflush(stdin);
		scanf("%s", &outputFilename);
		createDFA(outputFilename);
		inputDFA = loadDFA(outputFilename);
		executeDFA(inputDFA);	
	}
	// Open existing DFA
	else if (userResponse == 2) {
		printf("You chose to use a saved DFA.\n");
		strcpy(outputFilename, "BLANK");	
		inputDFA = loadDFA(outputFilename);
		executeDFA(inputDFA);

	}
	// Invalid session
	else {
		printf("ERROR: Invalid input.  Exiting now.");
		exit(1);
	}
	
	return 0;
}
