#include "b_tree.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int main(){
    srand(time(NULL));
    setlocale(LC_ALL, "pt_BR.UTF-8");

    NO *raiz = NULL;
    char *nomeArquivoRaiz = (char*) malloc(30 * sizeof(char));
    char *nomeArquivoArvore = (char*) malloc(27 * sizeof(char));
    char *auxiliarArvore = (char*) malloc(27 * sizeof(char));

    strcpy(nomeArquivoArvore, "trees/");

    int opcao, chave, pos, inseriu, opt;
    int T = 0;
    NO *buscado;
    char *nomeBuscado = (char*) malloc(30 * sizeof(char));

    opt = primeiroMenu();

    switch(opt){
        case 1:
            // Criar arvore
            printf("Pressione Enter para continuar...");
            getchar(); // Para capturar o Enter após escolher a opção
            getchar(); // Para aguardar o usuário pressionar Enter
            system("clear");
            while(T < 2){
                printf("Digite o T da arvore: ");
                scanf("%d", &T);
                if(T < 2){
                    printf("\nT inválido!\n");
                    printf("\nPressione Enter para continuar...");
                    getchar();
                    getchar();
                    system("clear");
                }
            }
            nomeArquivoArvore = gerarNomeArquivoArvore();
            break;
        case 2:
            // Ler arvore
            while(1){
                getchar();
                system("clear");

                printf("Arquivos Disponíveis:\n\n");
                listarArquivosArvore();
                printf("\nDigite o nome do arquivo para a árvore: ");
                fgets(auxiliarArvore, 27, stdin);

                int i=0;
                while(auxiliarArvore[i] != '\n'){
                    i++;
                }
                auxiliarArvore[i] = '\0';

                int nomeExistente = verificarNomeArquivo(auxiliarArvore);
                strcat(nomeArquivoArvore, auxiliarArvore);

                if(nomeExistente){
                    lerArvore(nomeArquivoArvore, &nomeArquivoRaiz, &T);
                    raiz = lerNo(nomeArquivoRaiz, T);
                    break;
                }else{
                    printf("Digite um arquivo valido!\n");
                }
            }
            break;
        default:
            printf("\nSaindo...\n");
            return 0;
    }

    while(1) {
        exibirMenu();
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite a chave a ser inserida: ");
                scanf("%d", &chave);
                inseriu = inserir(&raiz, chave, T, &nomeArquivoRaiz);
                if(inseriu){
                    printf("\nChave %d inserida com sucesso.\n", chave);
                }else{
                    printf("\nChave %d já existe na árvore.\n", chave);
                }
                break;

            case 2:
                printf("Digite a chave a ser buscada: ");
                scanf("%d", &chave);
                buscado = busca(raiz, chave, T, &pos, &nomeBuscado, nomeArquivoRaiz);
                if(buscado != NULL) {
                    printf("\nChave %d encontrada na posição %d do nó %s.\n", buscado->chaves[pos], pos+1, nomeBuscado);
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
                gravarArvore(nomeArquivoArvore, nomeArquivoRaiz, T); 
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
