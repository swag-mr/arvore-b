#include <stdio.h>
#include <stdlib.h>
#include "b_tree.h"

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
            no->filhos[i] = (char*) malloc(100 * sizeof(char));
            // Le o nome de cada nó filho
            fread(no->filhos[i], sizeof(char), 100, arquivo);
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
            fwrite(no->filhos[i], sizeof(char), 100, arquivo); // Assume que o tamanho máximo do nome do arquivo é 100
        }
    }

    fclose(arquivo);
}

NO* busca(NO *no, int chave, int T){
    // Busco a posição da chave no nó
    int i = 0;
    while(i < no->n && chave > no->chaves[i]){
        i++;
    }

    if(i < no->n && chave == no->chaves[i]){
        // A chave foi encontrada no nó atual
        return no;
    }

    if(no->folha){
        // Não encontramos a chave no nó atual, e não existem nós abaixos
        // logo, a chave não está presente
        return NULL;
    }
    
    // Le o filho e continua a busca
    NO *filho = lerNo(no->filhos[i], T);
    if(filho == NULL){
        // Se a leitura falhou, retorna NULL;
        return NULL;
    }

    NO *resultado = busca(filho, chave, T);
    liberarNo(filho);
    
    return resultado;
}
