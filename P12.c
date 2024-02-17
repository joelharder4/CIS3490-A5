/* Assignment 5
 * Name: Joel Harder
 * ID:   1191485
 * Date: April 10, 2023
 * Subset-Sum Problem using Backtracking
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define INPUT_LEN 512
#define SET_LEN 1024

typedef struct node {
    int val;
    struct node* left;
    struct node* right;
} node;

void getFileName(char* buffer);
int readFile(char* fileName, int* set);
int getSum();
node* newNode(int value);
int backtrackSumSubset(int* set, int length, int target, int depth, node* branch, int* deadends);
int comp_int(const void *a, const void *b);

int main(void) {
    printf("Backtracking program for subset sum problem\n");
    node* root = newNode(0);
    int deadends = 0;
    int* fullSet = malloc(sizeof(int) * SET_LEN);
    char* fileName = malloc(sizeof(char) * INPUT_LEN);
    getFileName(fileName);

    int length = readFile(fileName, fullSet);
    if (length < 0) {
        return 1;
    }

    int sum = getSum();

    struct timespec begin, end; 
    timespec_get(&begin, TIME_UTC);
    
    qsort(fullSet, length, sizeof(int), comp_int);
    int count = backtrackSumSubset(fullSet, length, sum, 0, root, &deadends);

    timespec_get(&end, TIME_UTC);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("Number of dead ends: %d\n", deadends);
    printf("Number of subsets where their sum is %d: %d\n", sum, count);
    printf("This algorithm took %.2f ms\n", elapsed*1000);

    return 0;
}



int comp_int(const void *a, const void *b) {
    int* n1 = (int*)a;
    int* n2 = (int*)b;
    if (*n1 > *n2) {
        return 1;
    } else if (*n1 == *n2) {
        return 0;
    } else {
        return -1;
    }
}



node* newNode(int value) {
    node* new = malloc( sizeof(node) );
    new->val = value;
    new->left = NULL;
    new->right = NULL;
    return new;
}


// returns number of subsets where sum = target
int backtrackSumSubset(int* set, int length, int target, int depth, node* branch, int* deadends) {
    int num = set[depth];
    int count = 0;

    if (branch->val + num == target) {
        return 1;
    }

    // dead end reached
    if (branch->val + num > target || depth >= length-1) {
        (*deadends)++;
        return 0;
    }

    branch->left = newNode( branch->val + num ); // branch with num
    branch->right = newNode( branch->val ); // branch without num

    count += backtrackSumSubset(set, length, target, depth+1, branch->left, deadends);
    count += backtrackSumSubset(set, length, target, depth+1, branch->right, deadends);
    return count;
}





void getFileName(char* buffer) {
    write(STDOUT_FILENO, "Enter a file name: ", sizeof(char)*20);
    
    int length = read(STDIN_FILENO, buffer, INPUT_LEN);
    
    // if the buffer did not run out of room
    if (length < INPUT_LEN && length > 1) {
        buffer[length-1] = '\0';
    } else {
        write(STDOUT_FILENO, "File name is invalid. Please try again.\n\n", sizeof(char)*42);
        getFileName(buffer);
    }
}



int getSum() {
    int sum = 0;
    printf("Enter the target sum: ");
    scanf("%d", &sum);
    return sum;
}



int readFile(char* fileName, int* set) {
    int n = 0;
    FILE* fd = fopen(fileName, "r");

    if (fd == NULL) {
        fprintf(stderr, "could not open file\n");
        return -1;
    }

    while (!feof(fd)) {
        fscanf(fd, "%d ", &(set[n]));
        n++;
    }

    fclose(fd);
    return n;
}

