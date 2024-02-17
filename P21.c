/* Assignment 5
 * Name: Joel Harder
 * ID:   1191485
 * Date: April 10, 2023
 * Brute force Person-Job Assignment
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

void getFileName(char* buffer);
int** readFile(char* fileName, int* count);
int factorial(int num);
void generatePermutations(int** permutations, int* nums, int start, int end, int* count);
int** BFAssignment(int** data, int nPeople);

int main(void) {
    printf("Brute force program for person-job assignment problem\n");
    char* fileName = malloc(sizeof(char) * INPUT_LEN);
    getFileName(fileName);
    
    int nPeople;
    int** data = readFile(fileName, &nPeople);
    if (nPeople < 0) {
        return 1;
    }

    printf("Number of possible assignments: %d\n", factorial(nPeople));

    struct timespec begin, end; 
    timespec_get(&begin, TIME_UTC);

    BFAssignment(data, nPeople);

    timespec_get(&end, TIME_UTC);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("This algorithm took %.2f seconds\n", elapsed);

    return 0;
}



int factorial(int num) {
    int fact = num;
    for (int i = 1; i < num; i++) {
        fact *= i;
    }
    return fact;
}





void generatePermutations(int** permutations, int* nums, int start, int end, int* count) {
    if (start == end) {
        for (int i = 0; i <= end; i++) {
            permutations[*count][i] = nums[i];
        }
        (*count)++;
        return;
    }

    for (int i = start; i <= end; i++) {
        int temp = nums[start];
        nums[start] = nums[i];
        nums[i] = temp;

        generatePermutations(permutations, nums, start + 1, end, count);

        temp = nums[start];
        nums[start] = nums[i];
        nums[i] = temp;
    }
}



int** BFAssignment(int** data, int nPeople) {
    int numPermutations = factorial(nPeople);
    int** permutations = malloc(numPermutations * sizeof(int*));
    int* nums = malloc(nPeople * sizeof(int));
    int count = 0;
    
    for (int i = 0; i < numPermutations; i++) {
        permutations[i] = malloc(nPeople * sizeof(int));
    }

    for (int i = 0; i < nPeople; i++) {
        nums[i] = i;
    }
    
    generatePermutations(permutations, nums, 0, nPeople - 1, &count);
    
    int max = INT_MIN;
    int* assignment = malloc(nPeople * sizeof(int));

    for (int i = 0; i < numPermutations; i++) {
        int cost = 0;
        for (int j = 0; j < nPeople; j++) {
            cost += data[j][permutations[i][j]];
        }
        if (cost > max) {
            max = cost;
            for (int j = 0; j < nPeople; j++) {
                assignment[j] = permutations[i][j] + 1;
            }
        }
    }

    printf("The total value: %d\n", max);
    printf("The person-job assignment selected:\n");

    for (int i = 0; i < nPeople; i++) {
        printf("%d ", assignment[i]);
    }
    printf("\n");

    for (int i = 0; i < numPermutations; i++) {
        free(permutations[i]);
    }
    free(permutations);
    free(assignment);
    free(nums);
    return data;
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
