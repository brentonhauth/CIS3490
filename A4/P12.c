/**
 * @brief CIS*3490 - A4 - "binary search tree using greedy technique"
 * @author Brenton Hauth (1229237)
 * @date March 14, 2023
 */
#include "P1.h"


/**
 * Recursively creates a tree using a greedy technique. Places the word with the
 * highest probability at the root, then places remaining words either
 * left or right depending how their order alphabetically.
 * @param data the set of words to create the tree from
 * @param size the number of elements in data
 * @returns a binary search tree crafted from the set of words
 */
TreeNode* create_tree(SearchWord* data, const int size) {
    if (size <= 0) {
        // Safety in case size is 0 (or less by accident)
        return NULL;
    } else if (size == 1) {
        // Wrap first word in tree node
        return create_tree_node(data);
    }

    // Places the word with the highest probability at index 0
    for (int i = 1; i < size; ++i) {
        if (data[i].prob > data[0].prob) {
            // Swap if found
            swap_probs(data, 0, i);
        }
    }

    // Create a node with root
    TreeNode* node = create_tree_node(data);

    int l = 1, r = size;
    while (l < r) {
        // compares alphabetically, places "<" words in left, else right
        if (strcmp(data[l].word, data[0].word) < 0) {
            ++l;
        } else {
            swap_probs(data, l, --r);
        }
    };
    if (l - 1 > 0) {
        // Creates a left subtree
        node->left = create_tree(data + 1, l - 1);
    }
    if (size - r > 0) {
        // Creates a right subtree
        node->right = create_tree(data + r, size - r);
    }
    return node; // returns current tree
}



int main() {
    // Setup
    int size, psize;
    char** data = read_data(&size);

    SearchWord* words = create_prob_list(data, size, &psize);

    // create the tree
    TreeNode* tree = create_tree(words, psize);

    // print_tree(tree, 0);

    printf("Enter a key: ");
    char input[BUFSIZE];
    scanf("%s", input);
    // Search
    search_bst(tree, input);


    // Free everything
    freetree(tree);
    for (int i = 0; i < size; ++i) {
        free(data[i]);
    }
    free(data);
    free(words);
    return 0;
}