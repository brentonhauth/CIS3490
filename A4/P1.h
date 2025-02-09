#ifndef _P1_H_
/**
 * @brief CIS*3490 - A4 - General code used in both Q1.1 & Q1.2.
 * @author Brenton Hauth (1229237)
 * @date March 14, 2023
 */
#define _P1_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define TEST_FILE       "data_A4_Q1.txt"
#define WORD_COUNT      (2045)
#define BUFSIZE         (256)
#define IS_SPACE(c)     ((c) == '\n' || (c) == '\x20' || (c) == '\t')


/**
 * Keeps a word and it's probability together,
 * easier for program to run.
 */
typedef struct SearchWord {
    const char* word;
    double prob;
} SearchWord;

/**
 * For representing a node in the tree
 */
typedef struct TreeNode {
    SearchWord* data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

/**
 * Creates a node in memory for a tree
 * @param data the search word to insert into the tree
 * @returns a new tree node
 */
TreeNode* create_tree_node(SearchWord* data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * Frees a tree and all it's children from memory
 * @param tree the tree to free from memory
 */
void freetree(TreeNode* tree) {
    if (tree != NULL) {
        freetree(tree->left);
        freetree(tree->right);
        free(tree);
    }
}

/**
 * For debugging, prints the tree, nothing special
 * @param tree the tree to print
 * @param layer the current level on the tree (helps add spaces)
 */
void print_tree(TreeNode* tree, int layer) {
    if (tree != NULL) {
        char buf[BUFSIZE];
        int i = 0;
        for (; i < layer; ++i) {
            buf[i] = '\x20';
        }
        buf[i] = '\0';
        printf("%s%s[%d] \"%s\" (%.5lf)\n", buf, buf, layer, tree->data->word, tree->data->prob);
        print_tree(tree->left, layer + 1);
        print_tree(tree->right, layer + 1);
    }
}

/**
 * Swaps data in a word list at 2 indexes
 * @param data the set of words
 * @param i the first index to swap
 * @param j the second index to swap
 */
void swap_probs(SearchWord* data, int i, int j) {
    double p = data[i].prob;
    const char* s = data[i].word;
    data[i].prob = data[j].prob;
    data[i].word = data[j].word;
    data[j].prob = p;
    data[j].word = s;
}

/**
 * Compares 2 words in alphabetical order. Needed for qsort.
 * @param a the first word
 * @param b the second word
 */
int wordcmp(const void* a, const void* b) {
    SearchWord* p = ((SearchWord*)a);
    SearchWord* q = ((SearchWord*)b);
    return strcmp(p->word, q->word);
}

/**
 * Creates a list of the search words and the
 * probability they appear in the list.
 * @param data the list of words
 * @param count the number of words
 * @param size where to write the size of the list
 * @returns The list of unique words with their probabilities sorted in alphabetical order
 */
SearchWord* create_prob_list(char** data, const int count, int* size) {
    // set all values to 0 for safety
    SearchWord* probs = (SearchWord*)calloc(count, sizeof(SearchWord));
    int s = 0; // keep track of the things
    double P = 1.0 / (double)count; // Probability for 1 word
    for (int i = 0; i < count; ++i) { // iters through the words
        char did_set = 0;
        for (int j = 0; j < s; ++j) { // iters through the current words added
            if (!strcmp(probs[j].word, data[i])) { // word is already in list
                did_set = 1; // mark as set
                probs[j].prob += P;  // increase probability
                break;
            }
        }
        if (!did_set) {
            // Wasn't in list so add it to the list
            probs[s].word = data[i];
            probs[s++].prob = P;
        }
    }

    if (s < count) {
        // Resize if it's less than the count
        probs = (SearchWord*)realloc(probs, sizeof(SearchWord) * s);
    }

    // Sort them in alphabetical order
    qsort(probs, s, sizeof(SearchWord), wordcmp);

    // Return values out
    *size = s;
    return probs;
}

/**
 * Read the data of words from the file
 * @param size where to write the # of words
 * @returns the list of words to search
 */
char** read_data(int* size) {
    char** data = (char**)malloc(sizeof(char*) * WORD_COUNT); // malloc'd word count
    
    FILE* file = fopen(TEST_FILE, "r"); // open the file
    char buf[BUFSIZE];

    int s = 0; // keep track of size
    while (fgets(buf, BUFSIZE, file) != NULL) {
        int len = strlen(buf);
        for (int start = 0; start < len; ++start) {
            if (IS_SPACE(buf[start])) {
                // continue to the next character
                continue;
            }
            for (int end = (start + 1); end <= len; ++end) {
                if (end == len || IS_SPACE(buf[end])) {
                    // Create space for string in memory
                    int slen = end - start;
                    char* str = (char*)malloc(sizeof(char) * (slen + 1));
                    // copy the data from the buffer to memory
                    strncpy(str, buf + start, slen);
                    str[slen + 1] = '\0'; // set last character to null terminator
                    data[s++] = str; // add to array
                    start = end; // reset the start
                    break; // continue to next word
                }
            }
        }
    }
    
    // close the file and return values
    fclose(file);
    *size = s;
    return data;
}


/**
 * Recursively search the word in the tree, and display a message going down
 * @param tree the tree to search
 * @param w the word to search
 */
void search_bst(const TreeNode* tree, const char* w) {
    if (tree == NULL) {
        // Was unable to locate the word
        printf("Not found.\n");
        return;
    }

    int c = strcmp(w, tree->data->word); // do comparison
    printf("Compared with \"%s\" (%.3lf), ", tree->data->word, tree->data->prob);

    if (c < 0) { // word is left of current
        printf("go left subtree\n");
        search_bst(tree->left, w);
    } else if (c > 0) { // word is right of current
        printf("go right subtree\n");
        search_bst(tree->right, w);
    } else { // word is current
        printf("found!\n");
    }
}


#endif // !_P1_H_
