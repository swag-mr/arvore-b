#ifndef B_TREE_H
#define B_TREE_H

typedef struct no {
    int n;
    int folha;
    int *chaves;
    char **filhos;
} NO;



#include "b_tree.c"
#endif
