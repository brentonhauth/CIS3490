/**
 * @brief CIS*3490 - A5 - assignment problem using branch and bound program 
 * @author Brenton Hauth (1229237)
 * @date March 29, 2023
 */
#include "P2.h"


/**
 * Checks if a value `x` is in a list of integer `data`
 * @param x the value to check
 * @param data the array of integerse
 * @param n the number of elements in data
 */
bool_t includes(const int x, const int* data, const int n) {
    for (int i = 0; i < n; ++i) {
        if (x == data[i]) {
            return TRUE; // Found element, return true
        }
    }
    return FALSE; // could not find element
}

/**
 * Find the maximum value for a persons job
 * @param data the n x n matrix containing the jobs and people
 * @param row the row index of the person
 * @param n the dimensions of the data matrix
 * @param exclude the list of integers to exclude when checking for max
 * @param xn the number of elements in `exclude`
 */
int max_row_val(uint_t* data, const int row, const int n, int* exclude, const int xn) {
    int k = -1;
    uint_t max = 0;
    for (int j = 0; j < n; ++j) {
        uint_t dat = data[row * n + j];
        if (!includes(j, exclude, xn) && dat > max) {
            max = dat;
            k = j;
        }
    }
    return k;
}

/**
 * recursively check the branches and find the best one.
 * @param assign the job assignments
 * @param asgn_cost the assignment cost
 * @param lvl the level of the current person to assign a job to
 * @param data the n x n matrix of jobs and people
 * @param n the dimensions of data, size of assign
 */
void job_assignment_branch(int* ub, int* assign, uint_t* asgn_cost, const int lvl, uint_t* data, const int n) {
    if (lvl == n) {
        return; // stop when we've gone through everyone
    }

    if (lvl > 0) {
        // copy first part of assignment to upperbound
        memcpy(ub, assign, sizeof(int) * lvl);
    }

    uint_t newub = 0; // storing the new upper bound cost

    int A = 0; // for storing the assignment

    for (int a = 0; a < n; ++a) {
        if (includes(a, assign, lvl)) {
            // if the current assignment value has already been
            // assigned, then there is no point in checking.
            continue;
        }

        ub[lvl] = a; // assign for potential
        // Keep track of new potential cost,
        uint_t cost = (*asgn_cost) + data[lvl * n + a];

        // Start at next level and gooooooooo
        for (int i = (lvl + 1); i < n; ++i) {
            // get the index of the max row value
            const int j = max_row_val(data, i, n, ub, lvl + 1);
            if (j >= 0) { // (safety)
                // add to potential list, and add value to cost
                ub[i] = j;
                cost += data[i * n + j];
            }
        }
        if (cost > newub) {
            newub = cost; // assign new upper bound
            A = a; // assign the current level value of the new maximum
        }
    }

    assign[lvl] = A; // Add a new assignment
    *asgn_cost += data[lvl * n + A]; // add to the assignment cost

    printf("Maximum upper bound: %u\n", newub); // print new upper bound

    // Call proper branch
    job_assignment_branch(ub, assign, asgn_cost, lvl + 1, data, n);
}

/**
 * Perform job assignment problem using branch and bound technique.
 * Wrapper for job_assignment_branch(...).
 * @param data n x n matrix of jobs and people
 * @param n the dimensions of the matrix
 * @param out_cost write the cost
 */
int* job_assignment(uint_t* data, const int n, uint_t* out_cost) {
    // For keeping track of the upper bound
    int* ub = (int*)malloc(sizeof(int) * n);

    // Keeps track of assigned jobs
    int* assign = (int*)malloc(sizeof(int) * n);


    // call first branch
    job_assignment_branch(ub, assign, out_cost, 0, data, n);


    // Reorder the list so that index corresponds to the job,
    // and the value corresponds to the person
    for (int i = 0; i < n; ++i) {
        int k = assign[i];
        ub[k] = (i + 1);
    }

    free(assign); // free up unused element
    return ub;
}


int main() {
    printf("[Branch and bound program for assignment problem]\n");
    const int n = MATSIZE;
    timestamp_t ts;
    uint_t out_cost = 0;


    uint_t* data = read_data(n);
    time_start(&ts);
    int* jobs = job_assignment(data, n, &out_cost);
    double ms = time_elapsed(&ts);

    printf("The person-job assignment selected:\n");
    for (int i = 0; i < n; ++i) {
        printf(" %u", jobs[i]);
    }

    printf("\nMax total value: %u\n", out_cost);
    printf("Execution time = %d\n", (int)ms);

    free(jobs);
    free(data);
    return 0;
}
