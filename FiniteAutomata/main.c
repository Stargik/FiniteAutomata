#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_INPUT_FILE_NAME "input.txt"
#define DEFAULT_OUTPUT_FILE_NAME "output.txt"
#define DEFAULT_TRANSITION_COUNT 100
#define W1 "ab"
#define W2 "cabb"
#define WORD_MAX_SIZE 30

struct Transition
{
    int currentState;
    char symbol;
    int resultState;
};

struct Automata
{
    int alphabetCount;
    int statesCount;
    int finalStatesCount;
    int transitionsCount;
    int startState;
    int* finalStates;
    struct Transition* transitions;
};

void readFromFile(FILE *fp, struct Automata *automata){
    fscanf(fp, "%d\n", &(automata->alphabetCount));
    fscanf(fp, "%d\n", &(automata->statesCount));
    fscanf(fp, "%d\n", &(automata->startState));
    fscanf(fp, "%d", &(automata->finalStatesCount));
    int *finalStates = malloc(sizeof(int) * automata->finalStatesCount);
    for (int i = 0; i < automata->finalStatesCount; i++) {
        fscanf(fp, "%d", &finalStates[i]);
    }
    automata->finalStates = finalStates;
    int transitionsCount = DEFAULT_TRANSITION_COUNT;
    struct Transition *transitions = malloc(sizeof(struct Transition) * transitionsCount);
    int res;
    for (int i = 0; i < transitionsCount; i++) {
        res = fscanf(fp, "%d %s %d", &(transitions[i].currentState), &(transitions[i].symbol), &(transitions[i].resultState));
        if (res != 3){
            transitionsCount = i;
            break;
        }
    }
    automata->transitions = transitions;
    automata->transitionsCount = transitionsCount;
}

int runAutomata(struct Automata *automata, char* word){
    int index = 0;
    int isTransitionExist = 0;
    
    struct Transition *transition = malloc(sizeof(struct Transition));
    for (int i = 0; i < automata->transitionsCount; i++) {
        if (automata->startState == automata->transitions[i].currentState && automata->transitions[i].symbol == word[index]){
            *transition = automata->transitions[i];
            printf("(%d, %s, %d)\n", transition->currentState, &(transition->symbol), transition->resultState);
            isTransitionExist = 1;
            index++;
            break;
        }
    }
    
    if (!isTransitionExist){
        return 0;
    }
    
    while (word[index] != '\0') {
        isTransitionExist = 0;
        for (int i = 0; i < automata->transitionsCount; i++) {
            if (automata->transitions[i].symbol == word[index] && automata->transitions[i].currentState == transition->resultState){
                *transition = automata->transitions[i];
                index++;
                isTransitionExist = 1;
                printf("(%d, %s, %d)\n", transition->currentState, &(transition->symbol), transition->resultState);
                break;
            }
        }
        if (!isTransitionExist){
            printf("%s", "The next transition is not exist.\n");
            return 0;
        }
    }
    
    int isFinalState = 0;
    for (int i = 0; i < automata->finalStatesCount; i++) {
        if (transition->resultState == automata->finalStates[i]){
            isFinalState = 1;
            break;
        }
    }
    return isFinalState;
}

int main(int argc, const char * argv[]) {
    char * filename = DEFAULT_INPUT_FILE_NAME;
    FILE *fp = fopen(filename, "r");

    struct Automata *automata = malloc(sizeof(struct Automata));
    readFromFile(fp, automata);
    
    printf("%d\n", (automata->alphabetCount));
    printf("%d\n", (automata->statesCount));
    printf("%d\n", (automata->startState));
    printf("%d", (automata->finalStatesCount));
    for (int i = 0; i < automata->finalStatesCount; i++) {
        printf(" %d", automata->finalStates[i]);
    }
    printf("\n");
    for (int i = 0; i < automata->transitionsCount; i++) {
        printf("%d %s %d\n", (automata->transitions[i].currentState), &(automata->transitions[i].symbol), (automata->transitions[i].resultState));
    }
    printf("%d\n", (automata->transitionsCount));
    
    
    char w[WORD_MAX_SIZE + strlen(W1) + strlen(W2)];
    char w0[WORD_MAX_SIZE] = "";
    scanf("%s", w0);
    strcat(w, W1);
    strcat(w, w0);
    strcat(w, W2);
    printf("%s\n", w);
    
    if(runAutomata(automata, w)){
        printf("%s", "Yes\n");
    }else{
        printf("%s", "No\n");
    }
}
