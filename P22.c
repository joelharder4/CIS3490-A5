/* Assignment 5
 * Name: Joel Harder
 * ID:   1191485
 * Date: April 10, 2023
 * Branch and bound Person-Job Assignment
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <limits.h>

#define INPUT_LEN 512
#define MAX_PEOPLE 1024

typedef struct node {
    int upBound;
    int depth;
    int person;
    int* path;
    struct node** children;
    struct node* parent;
} node;



void getFileName(char* buffer);
int** readFile(char* fileName, int* count);
int factorial(int num);
void generateChildren(int** data, node* parent, int maxDepth);
int getUpperBound(int** data, node* leaf, int maxDepth);
void printNode(node* toPrint);
void findMaxSolution(int** data, int nPeople);



int main(void) {
    printf("Branch and bound program for the person-job assignment problem\n");
    char* fileName = malloc(sizeof(char) * INPUT_LEN);
    getFileName(fileName);
    
    int nPeople;
    int** data = readFile(fileName, &nPeople);
    if (nPeople < 0) {
        return 1;
    }

    struct timespec begin, end; 
    timespec_get(&begin, TIME_UTC);

    findMaxSolution(data, nPeople);

    timespec_get(&end, TIME_UTC);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("This algorithm took %.2f ms\n", elapsed*1000);

    return 0;
}


int isNumIn(int num, int* arr, int len) {
    for (int i = 0; i < len; i++) {
        if (num == arr[i]) {
            return 1;
        }
    }

    return 0;
}



int getUpperBound(int** data, node* leaf, int maxDepth) {
    node* cur = leaf;
    int* path = malloc(sizeof(int) * maxDepth+1);
    int upperBound = 0;
    int num, max;
    int k = 0;

    // get all previous tree
    while (cur->parent != NULL) {
        num = data[cur->depth][cur->person];
        upperBound += num;
        path[k] = cur->person;
        cur = cur->parent;
        
        k++;
    }

    for (int i = leaf->depth+1; i <= maxDepth; i++) {
        max = INT_MIN;

        // find the maximum value in row i
        for (int j = 0; j <= maxDepth; j++) {
            if ( isNumIn(j, path, k) == 0 ) {
                num = data[i][j];
                if (num > max) {
                    max = num;
                }
            }
        }

        upperBound += max;
    }
    free(path);
    return upperBound;
}



void generateChildren(int** data, node* parent, int maxDepth) {
    parent->children = malloc(sizeof(node*) * (maxDepth+1));
    node* child = NULL;
    node* cur = parent;
    int* path = malloc(sizeof(int) * (maxDepth+1));
    int k = 0;

    while (cur->parent != NULL) {
        path[k] = cur->person;
        k++;
        cur = cur->parent;
    }
    

    for (int i = 0; i <= maxDepth; i++) {
        if (isNumIn(i, path, k) == 1) {
            parent->children[i] = NULL;
            continue;
        }
        parent->children[i] = malloc(sizeof(node));
        child = parent->children[i];
        child->depth = parent->depth + 1;
        child->person = i;
        child->children = NULL;
        child->parent = parent;
        child->upBound = getUpperBound(data, child, maxDepth);
    }
}


void printNode(node* toPrint) {
    if (toPrint == NULL) {
        printf("node:\n  NULL\n");
    } else {
        printf("node:\n  depth:%d\n  person:%d\n  upBound:%d\n\n", toPrint->depth, toPrint->person, toPrint->upBound);
    }
}


// depth-first search to find leaf with highest upper bound
void findMaxLeaf(int** data, node* cur, node** maxLeaf, int maxDepth, int* maxUpBound) {
    // if its a leaf node
    if (cur->children == NULL) {
        if (cur->upBound > *maxUpBound) {
            *maxUpBound = cur->upBound;
            *maxLeaf = cur;
        }
    } else {
        
        for (int i = 0; i <= maxDepth; i++) {
            if (cur->children[i] != NULL) {
                findMaxLeaf(data, cur->children[i], maxLeaf, maxDepth, maxUpBound);
            }
        }
    }
}



void findMaxSolution(int** data, int nPeople) {
    int maxDepth = nPeople-1;
    node* root = malloc(sizeof(node));
    root->parent = NULL;
    root->depth = -1;
    root->person = -1;
    root->upBound = getUpperBound(data, root, maxDepth);
    generateChildren(data, root, maxDepth);

    node* maxLeaf = NULL;
    node* prevMax = root;
    int maxUpBound;
    int curDepth = 0;

    while (curDepth <= maxDepth) {
        maxUpBound = INT_MIN;
        findMaxLeaf(data, prevMax, &maxLeaf, maxDepth, &maxUpBound);

        printf("Maximum upper bound: %d\n", maxLeaf->upBound);

        if (maxLeaf != NULL) {
            generateChildren(data, maxLeaf, maxDepth);
            
            if (curDepth < maxLeaf->depth+1) {
                curDepth = maxLeaf->depth+1;
            }
        }
        prevMax = maxLeaf;
    }

    // get the path of the maxLeaf
    node* cur = maxLeaf;
    int* path = malloc(sizeof(int) * (maxDepth+1));
    int* byJob = malloc(sizeof(int) * (maxDepth+1));
    int k = 0;

    // get the path of the final node
    while (cur->parent != NULL) {
        path[k] = cur->person;
        k++;
        cur = cur->parent;
    }

    for (int i = k-1; i >= 0; i--) {
        byJob[path[i]] = nPeople - i;
    }
    
    printf("the person-job assignment selected:\n ");
    for (int i = 0; i < k; i++) {
        printf("%d ", byJob[i]);
    }
    printf("\nMax total value: %d\n", maxLeaf->upBound);
}





int factorial(int num) {
    int fact = num;
    for (int i = 1; i < num; i++) {
        fact *= i;
    }
    return fact;
}




void getFileName(char* buffer) {
    write(STDOUT_FILENO, "Enter the file name: ", sizeof(char)*22);
    
    int length = read(STDIN_FILENO, buffer, INPUT_LEN);
    
    // if the buffer did not run out of room
    if (length < INPUT_LEN && length > 1) {
        buffer[length-1] = '\0';
    } else {
        write(STDOUT_FILENO, "File name is invalid. Please try again.\n\n", sizeof(char)*42);
        getFileName(buffer);
    }
}





int** readFile(char* fileName, int* count) {
    int i = 0;
    int j = 0;
    FILE* fd = fopen(fileName, "r");

    if (fd == NULL) {
        fprintf(stderr, "could not open file\n");
        *count = -1;
        return NULL;
    }

    int** data = malloc(sizeof(int*) * MAX_PEOPLE);
    while (!feof(fd)) {

        data[i] = malloc(sizeof(int) * MAX_PEOPLE);
        j = 0;

        do {
            fscanf(fd, "%d", &(data[i][j]));
            j++;
        } while(fgetc(fd) != 10 && !feof(fd));

        i++;
    }

    fclose(fd);
    *count = i-1;
    return data;
}
