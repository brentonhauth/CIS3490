#include "P1.h"

/**
 * Counts the number of inversions present
 * in the data through mergesort
 * @param data the data count inversions
 * @param size the size of the data
 */
uint_t count_inversions(uint_t* data, const int size) {
    if (size <= 1) { // ensures defualt case
        return 0;
    }
    uint_t inv = 0;
    int m = size >> 1; // divide by 2 and round down
    int lsize = m, rsize = size - m; // Set left & right partition sizes
    
    // Create left side and recursively count left size
    uint_t* left = malloc(sizeof(uint_t) * lsize); 
    memcpy(left, data, sizeof(uint_t) * lsize);
    inv += count_inversions(left, lsize); // Add all from left side

    // Create right side and recursively count right size
    uint_t* right = malloc(sizeof(uint_t) * rsize);
    memcpy(right, data + m, sizeof(uint_t) * rsize);
    inv += count_inversions(right, rsize); // Add all from right side

    int i = 0, j = 0, k = 0; // left, right, data
    inv += lsize * rsize; // theoretical maximum number of inversions
    // int f = 0, g = 0, w = 0;
    while (i < lsize && j < rsize) { // run until either left or right is copied
        if (left[i] < right[j]) { // left is smaller
            inv -= (rsize - j); // take off the number that would be greater
            // "If I am smaller than you, than so are my friends"
            //      ("except the ones before me")
            data[k++] = left[i++];
        } else {
            data[k++] = right[j++];
        }
    }
    if (j < rsize) {
        // Right still has more elements
        memcpy(data + k, right + j, (rsize - j) * sizeof(uint_t));

    } else {
        // Left still has more elements
        memcpy(data + k, left + i, (lsize - i) * sizeof(uint_t));
    }
    free(left);
    free(right);
    return inv;
}



int main() {
    uint_t* data;
    int size = read_file(&data);

    if (size < 0) {
        // End program
        return -1;
    }

    timestamp_t ts;
    time_start(&ts);
    uint_t inv = count_inversions(data, size);
    time_elapsed(&ts);
    printf("Counted inversions: %u\n", inv);
    free(data);
    return 0;
}
