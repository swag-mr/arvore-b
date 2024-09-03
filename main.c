#include "b_tree.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
    srand(time(NULL));

    NO *raiz = NULL;
    char *nomeArquivoRaiz = (char*) malloc(21 * sizeof(char));

    inserir(&raiz, 5, 3, &nomeArquivoRaiz);
    inserir(&raiz, 2, 3, &nomeArquivoRaiz);
    inserir(&raiz, -3, 3, &nomeArquivoRaiz);
    inserir(&raiz, 7, 3, &nomeArquivoRaiz);
    inserir(&raiz, -5, 3, &nomeArquivoRaiz);
    inserir(&raiz, -6, 3, &nomeArquivoRaiz);
    inserir(&raiz, -7, 3, &nomeArquivoRaiz);
    inserir(&raiz, -8, 3, &nomeArquivoRaiz);
    inserir(&raiz, -9, 3, &nomeArquivoRaiz);

    printf("\n%s\n", nomeArquivoRaiz);

    printf("imprimir arvore:\n\n");
    imprimir(raiz, 3);
    printf("\n");

    return 0;
}
