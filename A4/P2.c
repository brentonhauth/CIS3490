/**
 * @brief CIS*3490 - A4 - (P2) "stable marriage problem code"
 * @author Brenton Hauth (1229237)
 * @date March 14, 2023
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE     (256)
#define MAXSIZE     (4)     /* table size doesn't extend past 4, so can statically set it */

/**
 * Fill out a table with the values read from the file
 * @param file the file to read the incoming data from
 * @param mat where to enter the values
 * @param size the dimensions of the matrix/data being retrieved
 */
int fill_table(FILE* file, int mat[MAXSIZE][MAXSIZE], const int size) {
    int a, b, c, d;
    for (int i = 0; i < size; ++i) {
        int j = 0;
        // Quick way to retrieve either 3 or 4 numbers,
        //      doesn't need to be too complex.
        if (size == 3) {
            if (fscanf(file, "%d %d %d", &a, &b, &c) > 0) {
                mat[i][j++] = a;
                mat[i][j++] = b;
                mat[i][j] = c;
            }
        } else if (size == 4) {
            if (fscanf(file, "%d %d %d %d", &a, &b, &c, &d) > 0) {
                mat[i][j++] = a;
                mat[i][j++] = b;
                mat[i][j++] = c;
                mat[i][j] = d;
            }
        } else {
            printf("** Invalid size: %d\n", size);
            return -1;
        }
    }
    return 0;
}

/**
 * Read data from a user input file, then fill the tables with it.
 * @param men the table of men's preferences to fill
 * @param women the table of women's preferences to fill
 */
int read_data(int men[MAXSIZE][MAXSIZE], int women[MAXSIZE][MAXSIZE]) {
    char buf[BUFSIZE];
    printf("Enter a file: ");
    scanf("%s", buf);

    if (!strcmp(buf, ".1")) { // for quick debugging
        strcpy(buf, "data_A4_Q2_1.txt");
    } else if (!strcmp(buf, ".2")) {
        strcpy(buf, "data_A4_Q2_2.txt");
    }

    FILE* file = fopen(buf, "r");
    if (file == NULL) {
        printf("** Coulld not open file. Goodbye.\n");
        return -1; // handle error
    }
    int size = -1;
    if (fgets(buf, BUFSIZE, file) != NULL) {
        size = atoi(buf); // convert size to int
    }

    if (size != 3 && size != 4) { // for added safety
        printf("** Invalid size!\n");
        fclose(file);
        return -1;
    }

    // fill both tables
    fill_table(file, men, size);

    // fgets(buf, BUFSIZE, file); // for safety...?

    fill_table(file, women, size);

    // close file and send back size
    fclose(file);
    return size;
}


/**
 * Gets a woman's preference on a man (or their ranking).
 * @param table the table to lookup from
 * @param size the size of the dimentions in the table
 * @param w the woman currently looking at
 * @param m the man to find the woman's preference of
 */
int get_pref(int table[MAXSIZE][MAXSIZE], const int size, int w, int m) {
    for (int j = 0; j < size; ++j) {
        if (table[w][j] == m) {
            // Checks if the woman's preference matched the current man
            return (j + 1); // returns the ranking
        }
    }
    return size + 1; // in case of bug, play it safe
}

/**
 * Performs the stable marriage problem
 * @param marriage where to write the output matrix
 * @param men the preferences of the men
 * @param women the preferences of the women
 * @param size the size of the matrices
 */
void stable_marriage(
    int marriage[MAXSIZE][MAXSIZE],
    int men[MAXSIZE][MAXSIZE],
    int women[MAXSIZE][MAXSIZE], const int size) {
    //
    // Easier to keep track of both sides rather than
    // constantly iterating over the table.
    int man_partner[MAXSIZE]; // keeps track of a man's current partner
    int woman_partner[MAXSIZE]; // keeps track of a woman's current parter
    for (int i = 0; i < size; ++i) {
        // mark all values as 0
        for (int j = 0; j < size; ++j) {
            marriage[i][j] = 0;
        }
        man_partner[i] = 0;
        woman_partner[i] = 0;
    }


    int free_men = size; // current count of the free men

    while (free_men > 0) {
        for (int m = 0; m < size; ++m) {
            if (man_partner[m]) {
                // Man is taken, continue to next man
                continue;
            }

            // iters through the current man, getting their preferences in order
            for (int j = 0; j < size; ++j) {
                int w = men[m][j] - 1; // man's j'th preference
                // Propose to woman
                if (!woman_partner[w]) { // Woman doesn't have a partner
                    man_partner[m] = (w + 1); // set his new wife
                    woman_partner[w] = (m + 1); // set her new husband
                    --free_men; // 1 less free man
                    break; // move onto the next man

                    // Check if the woman prefers the new man over their current partner.
                } else if (get_pref(women, size, w, m + 1) < get_pref(women, size, w, woman_partner[w])) {
                    // new person is a better match than current
                    man_partner[m] = (w + 1); // set his new wife
                    man_partner[woman_partner[w] - 1] = 0; // make previous husband single
                    woman_partner[w] = (m + 1); // set her new husband
                    // free men count doesn't change
                    break; // move onto the next man
                }
                // otherwise, man was rejected and should continue to their next choice.
            }
        }
    }

    for (int i = 0; i < size; ++i) {
        // Fill out the marriage table
        marriage[i][man_partner[i] - 1] = 1;
    }
}


int main() {

    // Create tables for men, women and the marriage
    int men[MAXSIZE][MAXSIZE];
    int women[MAXSIZE][MAXSIZE];
    int marriage[MAXSIZE][MAXSIZE];

    // grab data
    int size = read_data(men, women);

    // do algorithm
    stable_marriage(marriage, men, women, size);

    // Print out the matrix
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", marriage[i][j]);
        }
        printf("\n");
    }
    return 0;
}
