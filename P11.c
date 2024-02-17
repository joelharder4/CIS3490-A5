/* Assignment 5
 * Name: Joel Harder
 * ID:   1191485
 * Date: April 10, 2023
 * Brute force Subset-Sum Problem
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define INPUT_LEN 512
#define SET_LEN 1024

void getFileName(char* buffer);
int readFile(char* fileName, int* set);
int getSum();
int BFSumSubset(int* set, int length, int target);

int main(void) {
    printf("Brute force program for subset sum problem\n");
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

    int count = BFSumSubset(fullSet, length, sum);

    timespec_get(&end, TIME_UTC);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
    
    printf("Number of all the subsets: %.0f\n", pow(2, length));
    printf("Number of subsets where their sum is %d: %d\n", sum, count);
    printf("This algorithm took %.2f ms\n", elapsed*1000);

    return 0;
}



int BFSumSubset(int* set, int length, int target) {
    int subsetNum = pow(2, length);
    int count = 0;
    int current, sum;

    while (subsetNum > 0) {
        current = subsetNum;
        sum = 0;
        for(int i = 0; i < length; i++) {
            if (current & 1) {
                sum += set[i];
            }
            current = current >> 1;
        }
        if (sum == target) {
            count++;
        }
        subsetNum--;
    }

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

