/**
 * @brief CIS*3490 - A5 - Subset Sum Problem via Backtracking
 * @author Brenton Hauth (1229237)
 * @date March 29, 2023
 */
#include "P1.h"


/**
 * Recursively navigate the subsets like branches, by either moving right (and
 * excluding the i'th element) or moving left (and including the i'th element)
 * to count how many subsets sum to `d`.
 * @param data the (sorted) data set
 * @param n the number of elements in data
 * @param d the number to check the sum of a subset against
 * @param i the index of the current node
 * @param s the sum of the previous entries
 * @param dead to count the dead branches
 * @returns the number of subsets that sum to `d`
 */
uint_t backtrack_subset_sum(
    const uint_t* data, const int n, const uint_t d,
    const int i, const uint_t s, uint_t* dead) {
    // First check if the node is a match, before including/excluding the i'th element
    if (s == d) {
        // We have a match, checking lower branches
        // would be useless since they're all sorted
        return 1;
    } else if (s > d || i >= n) {
        // If the sum is greater than `d`, or if `i` is out
        // of range, then it is considered a dead branch.
        ++(*dead); // increment dead branch count
        return 0;
    }

    // Otherwise, we can navigate the "branches" of the tree, 
    // and sum the total found for each branch.
    
            // Exclude i'th element and move right
    return backtrack_subset_sum(data, n, d, (i + 1), s, dead)
            
            // Include i'th element and move left
        + backtrack_subset_sum(data, n, d, (i + 1), s + data[i], dead);
}


/**
 * Determines the number of subsets that sum to a given value.
 * Wrapper for backtrack_subset_sum(...), first sorts the elements,
 * then starts at the root node in the tree (sum of 0, indexed at 0).
 * @param data the elements to create subsets from
 * @param n the number of elements in data
 * @param d the value to check the sum of subsets to
 * @param dead where to record the number of dead branches
 * @returns the number of subsets that sum to `d`.
 */
uint_t subset_sum(uint_t* data, const int n, const uint_t d, uint_t* dead) {
    qsort(data, n, sizeof(uint_t), uintcmp); // sort the elements for easier navigation
    *dead = 0; // reset to 0 dead branches.
    // Recursively navigate the tree, while starting with a sum of 0 at index 0
    return backtrack_subset_sum(data, n, d, 0, 0, dead);
}



int main() {
    printf("[Backtracking program for subset sum problem]\n");
    const int n = NUMCOUNT;
    uint_t d, subsets, dead;
    double ms;

    // setup
    uint_t* data = read_file(n, &d);

    // Start program
    timestamp_t ts;
    time_start(&ts);
    subsets = subset_sum(data, n, d, &dead);
    ms = time_elapsed(&ts);

    // Display output
    printf("Number of dead ends: %u\n", dead);
    printf("Number of subsets: %u\n", subsets);
    printf("\nExecution time = %d ms\n", (int)ms);

    // Cleanup
    free(data);
    return 0;
}
