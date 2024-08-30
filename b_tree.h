#ifndef B_TREE_H
#define B_TREE_H

typedef struct no {
    int n;
    int folha;
    int *chaves;
    char **filhos;
} NO;

NO* criarNo(int T, int folha);
void liberarNo(NO *no);
NO* lerNo(const char *nome_arquivo, int T);
void gravarNo(const char *nome_arquivo, NO *no, int T);
NO* busca(NO *no, int chave, int T);

#include "b_tree.c"
#endif
