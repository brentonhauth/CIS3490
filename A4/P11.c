/**
 * @brief CIS*3490 - A4 - (P1.1) "binary search tree via optimal BST"
 * @author Brenton Hauth (1229237)
 * @date March 14, 2023
 */
#include "P1.h"


/**
 * Utilized the optimal binary search tree algorithm for creating 2 tables
 * used to make a tree.
 * @param data the list of words to construct the tables from
 * @param n the # of elements in data, and will determine the dimensions of C & R
 * @param _C where to save the comps table
 * @param _R where to save the roots table
 */
void optimal_bst(const SearchWord* data, const int n, double** _C, int** _R) {
    /// NOTE: Since the psuedo code implements the table as a matrix of [1..n+1][0..n],
    ///       it took a little bit of reworking to write it as a [0..n][0..n].

    const int m = n + 1; // the dimensions of the comp & root tables
    // Can access [i,j] from a 1D array by doing [i * size + j]
    double* C = (double*)calloc(m * m, sizeof(double));
    int* R = (int*)calloc(m * m, sizeof(int));
    // use calloc, set all places to 0 for ultra extra safety

    // Fill 1st & 2nd diagonals
    for (int i = 0; i < m; ++i) {
        R[i * m + i] = 0; // Set main diagonals to 0
        C[i * m + i] = 0;
        if (i < n) { // check to not overflow data
            // Fills the first diagonal
            C[i * m + (i + 1)] = data[i].prob; // Fills the first diagonal
            R[i * m + (i + 1)] = (i + 1); // Set thing in root table
        }
    }
    C[n * m + n] = 0; // set last spot to 0

    // iterate through remaining diagonals. Start on 3rd diagonal
    // since the first 2 were already filled.
    for (int d = 2; d < m; ++d) {
        int i = 0; // Start on first row
        for (int j = d; j < m; ++j) { // go through the columns, until the end
            double minval = INT_MAX; // Initialize minimum value to very large value
            int kmin = 0; // set minimum k to 0 for safety
            for (int k = (i + 1); k <= j; ++k) { // iterate from next value of i to j
                double val = C[i * m + (k - 1)] + C[k * m + j]; // adding [i][k-1] + [k(+1)][j]
                if (val < minval) {
                    // Reset minimum values
                    minval = val;
                    kmin = k;
                }
            }
            R[i * m + j] = kmin; // set to the k minimum
            double sum = 0;
            for (int s = i; s < j; ++s) {
                sum += data[s].prob; // sum all probabilities from i to j-1
            }
            C[i * m + j] = sum + minval; // set C[i,j] to sum & min value
            ++i; // Move to next row, as j moves to next column
        }
    }

    // Return both tables
    *_C = C;
    *_R = R;
}


/**
 * Constructs an optimal binary search tree for a list of words and 2 tables (R & C).
 * Recursively creates the sub trees by going either left (for left subtree) or down
 * (for right subtree) from the current position. Start with (a,b)=(0,n) for tree root.
 * @param R a (n+1)^2 sized 2D array (disguised as 1D) for storing the root values
 * @param C a (n+1)^2 sized 2D array (disguised as 1D) for storing the avg # of comps
 * @param data the word data that R & C are constructed from, and to create the tree from
 * @param n the dimension tracker for R, C & data
 * @param a the row to start contructing the tree from
 * @param b the column to start contructing the tree from
 */
TreeNode* create_subtree(int* R, double* C, SearchWord* data, const int n, const int a, const int b) {
    const int m = n + 1; // dimensions of R & C
    int i = R[a * m + b]; // grab the index from R, for data
    if (i <= 0) {
        // base case to check if it reaches past the diagonals
        return NULL;
    }

    int i_1 = i - 1; // since data is 0 indexed, but R contains values indexed from 1

    SearchWord* dat = &(data[i_1]);
    TreeNode* node = create_tree_node(dat);
    
    // set the new probability, so that it will be displayed with the word
    dat->prob = C[a * m + b];

    // Create left & right subtrees
    node->left = create_subtree(R, C, data, n, a, i_1); // stay in current row but go left
    node->right = create_subtree(R, C, data, n, i, b); // stay in column but go down
    
    return node; // return structure
}


/**
 * Wrapper for create_subtree(...), uses the optimal_bst(...) function,
 * to create 2 tables, then creates a tree based on those 2 tables.
 * @param data the list of words to create the tree from
 * @param n the number of elements in data
 * @returns a tree based on the data
 */
TreeNode* create_tree(SearchWord* data, const int n) {
    double* C = NULL; // Where to store C & R
    int* R = NULL;
    optimal_bst(data, n, &C, &R); // Create tables

    // start creating the tree with R & C at position [0,n] in the tables
    TreeNode* node = create_subtree(R, C, data, n, 0, n);

    // free up the memory used for R & C
    free(C);
    free(R);

    return node; // return the whole tree
}


int main() {
    // Setup
    int size, psize;
    char** data = read_data(&size);

    SearchWord* words = create_prob_list(data, size, &psize);
    TreeNode* tree = create_tree(words, psize);

    // Search
    char input[BUFSIZE];
    printf("Enter a key: ");
    scanf("%s", input);
    search_bst(tree, input);


    // Cleanup
    freetree(tree);
    for (int i = 0; i < size; ++i) {
        free(data[i]); // free each word in memory
    }
    free(words);
    free(data);
    return 0;
}