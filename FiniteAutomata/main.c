#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DEFAULT_INPUT_FILE_NAME "input.txt"
#define DEFAULT_OUTPUT_FILE_NAME "output.txt"
#define DEFAULT_TRANSITION_COUNT 100
#define W1 "a"
#define W2 "b"
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

int runAutomata(struct Automata *automata, char* word, int productiveStates[DEFAULT_TRANSITION_COUNT]){
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
        if (productiveStates[transition->resultState]){
            isFinalState = 1;
            break;
        }
    }
    return isFinalState;
}

void findReachableStates(int statesCount, int transitionsCount, struct Transition transitions[], int startState, int reachableStates[DEFAULT_TRANSITION_COUNT]) {
    int queue[DEFAULT_TRANSITION_COUNT];
    int front = 0, back = 0;
    queue[back++] = startState;
    reachableStates[startState] = 1;
    while (front < back) {
        int currentCtate = queue[front++];

        for (int i = 0; i < transitionsCount; i++) {
            if (transitions[i].currentState == currentCtate && !reachableStates[transitions[i].resultState]) {
                reachableStates[transitions[i].resultState] = 1;
                queue[back++] = transitions[i].resultState;
            }
        }
    }
}

void findProductiveStates(int statesCount, int transitionsCount, struct Transition transitions[], int finalStates[], int finalStatesCount, int productiveStates[DEFAULT_TRANSITION_COUNT]) {
    
    for (int i = 0; i < finalStatesCount; i++) {
        productiveStates[finalStates[i]] = 1;
    }
    
    int isUpdated = 1;
    while (isUpdated) {
        isUpdated = 0;
        
        for (int i = 0; i < transitionsCount; i++) {
            int currentState = transitions[i].currentState;
            int resultState = transitions[i].resultState;
            
            if (productiveStates[resultState] && !productiveStates[currentState]) {
                productiveStates[currentState] = 1;
                isUpdated = 1;
            }
        }
    }
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
    int reachableStates[DEFAULT_TRANSITION_COUNT] = {0};
    findReachableStates(automata->statesCount, automata->transitionsCount, automata->transitions, automata->startState, reachableStates);
    int productiveStates[DEFAULT_TRANSITION_COUNT] = {0};
    findProductiveStates(automata->statesCount, automata->transitionsCount, automata->transitions, automata->finalStates, automata->finalStatesCount, productiveStates);
    
    
    
    char w[WORD_MAX_SIZE] = "";
    scanf("%s", w);
    printf("%s\n", w);
    int res = 0;
    for (int i = 0; i < automata->statesCount; i++) {
        if(reachableStates[i]){
            automata->startState = i;
            if(runAutomata(automata, w, productiveStates)){
                res = 1;
                break;
            }
        }
    }
    if(res){
        printf("%s", "----------------\n");
        printf("%s", "Yes\n");
    }else{
        printf("%s", "No\n");
    }
    

}
