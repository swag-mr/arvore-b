#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "b_tree.h"

char* gerarNomeArquivo(){
    char *nome = (char*) malloc(30 * sizeof(char));
    const char *caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    strcpy(nome, "arquivos/");
    for(int i = 9; i < 29; i++){
        nome[i] = caracteres[rand() % 62]; // Pois sao 61 caracteres
    }
    nome[29] = '\0'; // Adiciona o \0 (fim da string)
    return nome;
}

NO* criarNo(int T, int folha){
    NO *no = (NO*) malloc(sizeof(NO));

    no->n = 0;
    no->chaves = (int*) malloc((2 * T - 1) * sizeof(int));
    no->filhos = (char**) malloc((2 * T) * sizeof(char*));
    no->folha = folha;

    return no;
}

void liberarNo(NO *no){
    // Libera o espaço alocado para as chaves
    free(no->chaves);

    if(!no->folha){
        for(int i=0; i <= no->n; i++){
            // Se nao for folha, libera o espaço alocado para cada filho
            free(no->filhos[i]);
        }
    }

    // Libera o espaço alocado para o array de filhos
    free(no->filhos);
    // Libera o no
    free(no);
}

NO* lerNo(const char *nome_arquivo, int T){
    FILE *arquivo = fopen(nome_arquivo, "rb");
    
    if(arquivo == NULL){
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    // Verifica se o nó do arquivo é folha
    int folha;
    fread(&folha, sizeof(int), 1, arquivo);

    // Inicializa o nó com a informação da folha obtida
    NO *no = criarNo(T, folha);

    // Le a quantidade de chaves no nó
    fread(&no->n, sizeof(int), 1, arquivo);

    // Le o array de chaves no arquivo
    fread(no->chaves, sizeof(int), no->n, arquivo);

    if(!no->folha){
        for(int i=0; i <= no->n; i++){
            // Aloca todos os filhos para ler o nome deles
            no->filhos[i] = (char*) malloc(30 * sizeof(char));
            // Le o nome de cada nó filho
            fread(no->filhos[i], sizeof(char), 30, arquivo);
        }
    }

    fclose(arquivo);
    return no;
}

void gravarNo(const char *nome_arquivo, NO *no, int T){
    FILE *arquivo = fopen(nome_arquivo, "wb");

    if(arquivo == NULL){
        perror("Erro ao abrir o arquivo para gravação");
        return;
    }

    // Grava se é folha ou não
    fwrite(&no->folha, sizeof(int), 1, arquivo);

    // Grava o número de chaves do nó
    fwrite(&no->n, sizeof(int), 1, arquivo);

    // Grava as chaves
    fwrite(no->chaves, sizeof(int), no->n, arquivo);

    // Se não for uma folha, grava os nomes dos arquivos dos filhos
    if(!no->folha){
        for (int i = 0; i <= no->n; i++) {
            fwrite(no->filhos[i], sizeof(char), 30, arquivo); // Assume que o tamanho máximo do nome do arquivo é 30
        }
    }

    fclose(arquivo);
}

NO* busca(NO *no, int chave, int T, int *posicao, char **nomeBuscado, char *nomeNoAtual){ 
    int i = 0;
    while(i < no->n && chave > no->chaves[i]){
        i++;
    }

    if(i < no->n && chave == no->chaves[i]){
        strcpy(*nomeBuscado, nomeNoAtual);
        *posicao = i;
        return no;
    }

    if(no->folha){
        return NULL;
    }else{
        return busca(lerNo(no->filhos[i], T), chave, T, posicao, nomeBuscado, no->filhos[i]);
    }
}

void splitChild(NO *pai, int indice, int T, char *nomeArquivoPai){
    NO *filho = lerNo(pai->filhos[indice], T);
    NO *novoNo = criarNo(T, filho->folha);

    novoNo->n = T - 1;

    // Move as chaves e filhos para o novo nó
    for(int i=0; i < T - 1; i++){
        novoNo->chaves[i] = filho->chaves[i+T];
    }

    if(!filho->folha){
        for(int i=0; i < T; i++){
            novoNo->filhos[i] = strdup(filho->filhos[i+T]);
        }
    }

    filho->n = T - 1;

    // Andar os filhos do pai para inserir o novoNo
    for(int i=pai->n; i > indice; i--){
        pai->filhos[i+1] = pai->filhos[i];
    }
    
    char *nomeArquivoNovoNo = gerarNomeArquivo();

    pai->filhos[indice+1] = nomeArquivoNovoNo;

    // Andar as chaves do pai para inserir a mediana das chaves do filho
    for(int i=pai->n-1; i >= indice; i--){
        pai->chaves[i+1] = pai->chaves[i];
    }

    pai->chaves[indice] = filho->chaves[T-1];
    pai->n++;

    gravarNo(nomeArquivoPai, pai, T);
    gravarNo(nomeArquivoNovoNo, novoNo, T);
    gravarNo(pai->filhos[indice], filho, T);

    // Verificar se liberar o nome
}

void inserirNaoCheio(NO *no, int chave, int T, char *nomeArquivoNo){
    int i = no->n - 1;

    if(no->folha){
        // Insere a chave na posicao correta
        while(i >= 0 && chave < no->chaves[i]){
            no->chaves[i+1] = no->chaves[i];
            i--;
        }
        no->chaves[i+1] = chave;
        no->n++;

        // Grava o nó alterado de volta no arquivo
        gravarNo(nomeArquivoNo, no, T);
    }else{
        // Acha o filho para inserir a chave
        while(i >= 0 && chave < no->chaves[i]){
            i--;
        }
        i++;

        // Carrega o filho do arquivo
        NO *filho = lerNo(no->filhos[i], T);

        inserirNaoCheio(filho, chave, T, no->filhos[i]);

        if(filho->n == 2 * T - 1){
            splitChild(no, i, T, nomeArquivoNo);
        }

        // Libera a memória do filho
        liberarNo(filho);
    }
}

void inserir(NO **raiz, int chave, int T, char **nomeArquivoRaizAtual){
    NO *no = *raiz;

    if(no == NULL){
        // Cria uma raiz
        *raiz = criarNo(T, 1);
        (*raiz)->chaves[0] = chave;
        (*raiz)->n = 1;

        // Grava a nova raiz em um arquivo
        char *nomeArquivoRaiz = gerarNomeArquivo();
        gravarNo(nomeArquivoRaiz, *raiz, T);

        *nomeArquivoRaizAtual = nomeArquivoRaiz;
    }else{
        if(no->n == 2 * T - 2){
            // Tive que colocar 2 * T - 2, pois antes ele enchia o array, e não splitava
            inserirNaoCheio(*raiz, chave, T, *nomeArquivoRaizAtual);

            // Split se a raiz estiver cheia
            NO *novaRaiz = criarNo(T, 0);

            novaRaiz->filhos[0] = *nomeArquivoRaizAtual;

            // Atualiza a raiz
            *raiz = novaRaiz;

            // Grava a nova raiz em um arquivo
            char *nomeArquivoNovaRaiz = gerarNomeArquivo();

            splitChild(novaRaiz, 0, T, nomeArquivoNovaRaiz);

            /* // Insere na nova raiz que foi splitada */
            /* inserirNaoCheio(*raiz, chave, T, nomeArquivoNovaRaiz); */

            *nomeArquivoRaizAtual = nomeArquivoNovaRaiz;
        }else{
            inserirNaoCheio(*raiz, chave, T, *nomeArquivoRaizAtual);
            // Nao precisa retornar o nome do arquivo atual da raiz, pois nao alterou ele
        }
    }
}

// Função de impressão da Árvore B
void imprimirArvoreB(NO* no, int nivel, int T) {
    if (no != NULL) {
        int i;
        for (i = 0; i < no->n; i++) {
            if (!no->folha) {
                NO *filho = lerNo(no->filhos[i], T);
                imprimirArvoreB(filho, nivel + 1, T);
            }
            for (int j = 0; j < nivel; j++) {
                printf("    ");  // Indentação para mostrar a profundidade
            }
            printf("%d\n", no->chaves[i]);
        }
        if (!no->folha) {
            NO *filho = lerNo(no->filhos[i], T);
            imprimirArvoreB(filho, nivel + 1, T);
        }
    }
}

void exibirMenu(){
    printf("\n=====================================\n");
    printf("|        MENU - ÁRVORE B            |\n");
    printf("=====================================\n");
    printf("| 1. Inserir na árvore              |\n");
    printf("| 2. Buscar na árvore               |\n");
    printf("| 3. Imprimir a árvore              |\n");
    printf("| 4. Sair                           |\n");
    printf("=====================================\n");
    printf("Digite sua escolha: ");
}
