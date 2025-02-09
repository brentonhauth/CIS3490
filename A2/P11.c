#include "P1.h"

/**
 * Counts the inversions in a dataset via bruteforce
 * @param data the data to look through to find inversions
 * @param size the size of the dataset to look through
 */
uint_t count_inversions(const uint_t* data, const uint_t size) {
    uint_t inv = 0;
    for (int i = 0; i < (size - 1); ++i) {
        for (int j = (i + 1); j < size; ++j) {
            if (data[i] > data[j]) { // Check
                ++inv;
            }
        }
    }
    return inv;
}

int main() {
    uint_t* data;
    int size = read_file(&data);

    if (size < 0) {
        // End program
        return -1;
    }

    // uint_t raw[10] = {1, 9, 6, 7, 3, 7, 21, 5, 10, 2};

    timestamp_t ts;
    time_start(&ts); // start timer
    uint_t inv = count_inversions(data, size);
    time_elapsed(&ts); // end timer and print

    printf("Inversions counted: %u\n", inv);
    free(data);
    return 0;
}
