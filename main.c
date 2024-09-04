#include "b_tree.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>

int main(){
    srand(time(NULL));
    setlocale(LC_ALL, "pt_BR.UTF-8");

    NO *raiz = NULL;
    char *nomeArquivoRaiz = (char*) malloc(21 * sizeof(char));

    int opcao, chave, T, pos;
    NO *buscado;

    printf("Digite o T da arvore: ");
    scanf("%d", &T);

    while(1) {
        exibirMenu();
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite a chave a ser inserida: ");
                scanf("%d", &chave);
                inserir(&raiz, chave, T, &nomeArquivoRaiz);
                printf("\nChave %d inserida com sucesso.\n", chave);
                break;

            case 2:
                printf("Digite a chave a ser buscada: ");
                scanf("%d", &chave);
                buscado = busca(raiz, chave, T, &pos);
                if(buscado != NULL) {
                    printf("\nChave %d encontrada na posição %d.\n", buscado->chaves[pos], pos);
                } else {
                    printf("\nChave %d não encontrada.\n", chave);
                }
                break;

            case 3:
                printf("\nEstrutura da Árvore B:\n");
                imprimirArvoreB(raiz, 0, T);
                printf("\n");
                break;

            case 4:
                printf("\nSaindo...\n");
                free(nomeArquivoRaiz);
                return 0;

            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
        printf("Pressione Enter para continuar...");
        getchar(); // Para capturar o Enter após escolher a opção
        getchar(); // Para aguardar o usuário pressionar Enter
        system("clear");
    }

    return 0;
}
