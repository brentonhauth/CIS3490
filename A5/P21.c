/**
 * @brief CIS*3490 - A5 - job assigment brute force program
 * @author Brenton Hauth (1229237)
 * @date March 29, 2023
 */
#include "P2.h"

/**
 * Gets the next permutation in lexicographic order. Based on
 * psuedo code found on p.146 in the Textbook.
 * @param p the current permutation, should contain values from 1 to n
 * @param n the number of elements in p
 * @returns TRUE if there was a next permutation, FALSE if not.
 */
bool_t lex_permute(uint_t* p, const int n) {
    // Find largest value of I, s.t. a[i] < a[i+1]
    int I = -1;
    for (int i = 0; i < (n - 1); ++i) {
        if (p[i] < p[i + 1]) {
            I = i;
        }
    }
    if (I == -1) {
        // I was not set, so it is in reverse order
        return FALSE;
    }

    // Find the largest value of J, s.t. a[I] < a[J] for j in (I+1)..n
    int J = -1;
    uint_t tmp = UINT_MAX;
    for (int j = (I + 1); j < n; ++j) {
        if (p[j] < tmp && p[I] < p[j]) {
            tmp = p[j];
            J = j;
        }
    }
    if (J == -1) { // for safety
        return FALSE;
    }

    // swap the values of p[I] and p[J]
    tmp = p[I];
    p[I] = p[J];
    p[J] = tmp;

    // reverse the elements from I+1 to n.
    int l = (I + 1), r = n;
    while (l < r) {
        tmp = p[l];
        p[l++] = p[--r];
        p[r] = tmp;
    }
    return TRUE;
}


/**
 * Does the job assignment problem through a brute force method.
 * @param data the job matrix (n x n)
 * @param n the dimension of the matrix
 * @param _maxcost where to write the maximum cost
 * @returns the list of jobs assigned to each person
 */
uint_t* job_assignment(uint_t* data, const int n, uint_t* _maxcost, uint_t* _count) {
    uint_t* permute = (uint_t*)malloc(sizeof(uint_t) * n);
    uint_t* jobs = (uint_t*)malloc(sizeof(uint_t) * n);

    // set starting permutation, and default answer/cost
    uint_t count = 1, maxcost = 0;
    for (int i = 0; i < n; ++i) {
        permute[i] = (i + 1);
        jobs[i] = (i + 1);
        maxcost += data[i * n + i];
    }

    // Move to next permute, will stop when at last
    while (lex_permute(permute, n)) {
        uint_t cost = 0;
        for (int j = 0; j < n; ++j) {
            // get the person assigned to the jth job, and get their cost for it
            const int i = (permute[j] - 1);
            cost += data[i * n + j];
        }

        if (cost > maxcost) {
            // reset the values of the max cost
            maxcost = cost;
            memcpy(jobs, permute, sizeof(uint_t) * n);
        }
        ++count; // will eventually count to n!, but good to have for reference
    }

    free(permute); // done with object

    // Return values
    *_count = count;
    *_maxcost = maxcost;
    return jobs;
}



int main() {
    const int n = MATSIZE;
    timestamp_t ts;
    double ms;
    uint_t maxcost, count;

    uint_t* data = read_data(n);

    // for (int i = 0; i < n; ++i) {
    //     for (int j = 0; j < n; ++j) {
    //         printf("%d ", data[i * n + j]);
    //     }
    //     printf("\n");
    // }

    printf("[Brute force program for assignment problem]\n");

    time_start(&ts);
    uint_t* jobs = job_assignment(data, n, &maxcost, &count);
    ms = time_elapsed(&ts);

    printf("The number of all the possible assignments: %u\n", count);
    printf("The person-job assignment selected:\n");
    for (int i = 0; i < n; ++i) {
        printf(" %u", jobs[i]);
    }
    printf("\nThe total value: %u\n", maxcost);
    printf("Execution time = %d ms\n", (int)ms);

    // Cleanup
    free(jobs);
    free(data);
    return 0;
}
