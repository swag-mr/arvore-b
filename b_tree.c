#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
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

char* gerarNomeArquivoArvore(){
    char *nome = (char*) malloc(30 * sizeof(char));
    const char *caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    strcpy(nome, "trees/");
    for(int i = 6; i < 26; i++){
        nome[i] = caracteres[rand() % 62]; // Pois sao 61 caracteres
    }
    nome[26] = '\0'; // Adiciona o \0 (fim da string)
    return nome;
}

NO* criarNo(int T, int folha, char *nomeArquivo){
    NO *no = (NO*) malloc(sizeof(NO));

    no->n = 0;
    no->chaves = (int*) malloc((2 * T - 1) * sizeof(int));
    no->filhos = (char**) malloc((2 * T) * sizeof(char*));
    no->filename = (char*) malloc(30 * sizeof(char));
    strcpy(no->filename, nomeArquivo);
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

    // Libera o espaço alocado para o filename
    free(no->filename);

    // Libera o espaço alocado para o array de filhos
    free(no->filhos);
    // Libera o no
    free(no);
}

NO* lerNo(char *nome_arquivo, int T){
    FILE *arquivo = fopen(nome_arquivo, "rb");
    
    if(arquivo == NULL){
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    // Verifica se o nó do arquivo é folha
    int folha;
    fread(&folha, sizeof(int), 1, arquivo);

    // Inicializa o nó com a informação da folha obtida
    NO *no = criarNo(T, folha, nome_arquivo);

    // Le a quantidade de chaves no nó
    fread(&no->n, sizeof(int), 1, arquivo);

    // Le o array de chaves no arquivo
    fread(no->chaves, sizeof(int), no->n, arquivo);
    
    // Le o filename
    fread(no->filename, sizeof(char), 30, arquivo);

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

void gravarNo(NO *no, int T){
    FILE *arquivo = fopen(no->filename, "wb");

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

    // Grava o filename
    fwrite(no->filename, sizeof(char), 30, arquivo);

    // Se não for uma folha, grava os nomes dos arquivos dos filhos
    if(!no->folha){
        for (int i = 0; i <= no->n; i++) {
            fwrite(no->filhos[i], sizeof(char), 30, arquivo); // Assume que o tamanho máximo do nome do arquivo é 30
        }
    }

    fclose(arquivo);
}

void lerArvore(const char *nome_arquivo, char **nome_raiz, int *T){
    FILE *arquivo = fopen(nome_arquivo, "rb");

    if(arquivo == NULL){
        perror("Erro ao abrir o arquivo");
        return;
    }

    fread(T, sizeof(int), 1, arquivo);

    fread(*nome_raiz, sizeof(char), 30, arquivo);

    fclose(arquivo);
}

void gravarArvore(const char *nome_arquivo, char *nome_raiz, int T){
    FILE *arquivo = fopen(nome_arquivo, "wb");

    if(arquivo == NULL){
        perror("Erro ao abrir o arquivo");
        return;
    }

    fwrite(&T, sizeof(int), 1, arquivo);

    fwrite(nome_raiz, sizeof(char), 30, arquivo);

    fclose(arquivo);
}

void listarArquivosArvore(){
    DIR *d;

    struct dirent *dir;
    d = opendir("./trees/");
    if(d){
        while((dir = readdir(d)) != NULL){
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
}

int verificarNomeArquivo(char *nome){
	DIR *d;
	struct dirent *dir;
	d = opendir("./trees/");
	int cont = 0;
	if(d){
		while((dir = readdir(d)) != NULL){
			if(strcmp(dir->d_name, nome) == 0){
				cont++;
				break;
			}
		}
		closedir(d);
	}

	return cont;
}

NO* buscaInsercao(NO *no, int chave, int T){ 
    int inicio = 0;
    int fim = no->n - 1;

    while(inicio <= fim){
        int meio = (inicio + fim) / 2;

        if(chave == no->chaves[meio]){
            return no;
        }else if(chave > no->chaves[meio]){
            inicio = meio + 1;
        }else{
            fim = meio - 1;
        }
    }

    if(no->folha){
        return NULL;
    }else{
        return buscaInsercao(lerNo(no->filhos[inicio], T), chave, T);
    }
}

NO* busca(NO *no, int chave, int T, int *posicao){ 
    int inicio = 0;
    int fim = no->n - 1;

    while(inicio <= fim){
        int meio = (inicio + fim) / 2;

        if(chave == no->chaves[meio]){
            *posicao = meio;
            return no;
        }else if(chave > no->chaves[meio]){
            inicio = meio + 1;
        }else{
            fim = meio - 1;
        }
    }

    if(no->folha){
        return NULL;
    }else{
        return busca(lerNo(no->filhos[inicio], T), chave, T, posicao);
    }
}

void splitChild(NO *pai, int indice, int T){
    char *nomeArquivoNovoNo = gerarNomeArquivo();

    NO *filho = lerNo(pai->filhos[indice], T);
    NO *novoNo = criarNo(T, filho->folha, nomeArquivoNovoNo);

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
    

    pai->filhos[indice+1] = nomeArquivoNovoNo;

    // Andar as chaves do pai para inserir a mediana das chaves do filho
    for(int i=pai->n-1; i >= indice; i--){
        pai->chaves[i+1] = pai->chaves[i];
    }

    pai->chaves[indice] = filho->chaves[T-1];
    pai->n++;

    gravarNo(pai, T);
    gravarNo(novoNo, T);
    gravarNo(filho, T);

    // Verificar se liberar o nome
}

void inserirNaoCheio(NO *no, int chave, int T){
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
        gravarNo(no, T);
    }else{
        // Acha o filho para inserir a chave
        while(i >= 0 && chave < no->chaves[i]){
            i--;
        }
        i++;

        // Carrega o filho do arquivo
        NO *filho = lerNo(no->filhos[i], T);

        inserirNaoCheio(filho, chave, T);

        if(filho->n == 2 * T - 1){
            splitChild(no, i, T);
        }

        // Libera a memória do filho
        liberarNo(filho);
    }
}

int inserir(NO **raiz, int chave, int T){
    NO *no = *raiz;

    if(no == NULL){
        // Cria uma raiz
        char *nomeArquivoRaiz = gerarNomeArquivo();

        *raiz = criarNo(T, 1, nomeArquivoRaiz);
        (*raiz)->chaves[0] = chave;
        (*raiz)->n = 1;

        // Grava a nova raiz em um arquivo
        gravarNo(*raiz, T);
    }else{
        NO *buscado = buscaInsercao(no, chave, T);
        if(buscado != NULL){
            // Não inseriu
            return 0;
        }
        
        inserirNaoCheio(*raiz, chave, T);

        if(no->n == 2 * T - 1){
            // Split se a raiz estiver cheia
            char *nomeArquivoNovaRaiz = gerarNomeArquivo();

            NO *novaRaiz = criarNo(T, 0, nomeArquivoNovaRaiz);

            novaRaiz->filhos[0] = (*raiz)->filename;

            // Atualiza a raiz
            *raiz = novaRaiz;

            // Grava a nova raiz em um arquivo
            splitChild(novaRaiz, 0, T);
        }
    }
    return 1;
}

void apagarArquivo(char *filename){
    printf("apagou %s", filename);
    remove(filename);
}

void removerChaveNaoFolha(NO *no, int indice, int T){
    int k = no->chaves[indice];

    // Caso a: o filho predecessor tem T chaves
    NO *predecessor = lerNo(no->filhos[indice], T);
    if(predecessor->n >= T){
        // Acha o maior elemento da subarvore da esquerda para trocar pela chave
        NO *atual = lerNo(no->filhos[indice], T);
        while(!atual->folha){
            atual = lerNo(atual->filhos[atual->n], T);
        }
        
        int pred = atual->chaves[atual->n-1];
        // Copia o valor do maior da subarvore da esquerda no nó atual
        no->chaves[indice] = pred;

        // Grava o no
        gravarNo(no, T);

        // Remove o valor copiado recursivamente
        remover(predecessor, pred, T);
        /* liberarNo(atual); */
    }

    // Caso b: o filho sucessor tem T chaves
    NO *sucessor = lerNo(no->filhos[indice+1], T);
    if(sucessor->n >= T){
        // Acha o menor elemento da subarvore da direita para trocar pela chave
        NO *atual = lerNo(no->filhos[indice+1], T);
        while(!atual->folha){
            atual = lerNo(atual->filhos[0], T);
        }

        int suc = atual->chaves[0];
        // Copia o valor de menor da subarvore da direita no nó atual
        no->chaves[indice] = suc;
        
        // Grava o no
        gravarNo(no, T);

        // Remove o valor copiado recursivamente
        remover(sucessor, suc, T);
        /* liberarNo(atual); */
    }

    // Caso c: sucessor e predecessor com T - 1 chaves
    if((sucessor->n == T - 1) && (predecessor->n == T - 1)){
        // Adiciona a chave k a ser removida no predecessor para no final remove-la recursivamente
        predecessor->chaves[T - 1] = k;

        // Copia as chaves do sucessor no predecessor
        for(int i=0; i < sucessor->n; ++i){
            predecessor->chaves[i + T] = sucessor->chaves[i];
        }

        // Se o predecessor nao for folha, copia os filhos do sucessor nele
        if(!predecessor->folha){
            for(int i=0; i < sucessor->n; ++i){
                strcpy(predecessor->filhos[i + T], sucessor->filhos[i]); 
            }
        }

        // Reorganiza as chaves no nó
        for(int i=indice+1; i < no->n; ++i){
            no->chaves[i - 1] = no->chaves[i];
        }

        // Reorganiza os filhos no nó
        for(int i=indice+2; i <= no->n; ++i){
            strcpy(no->filhos[i-1], no->filhos[i]);
        }

        // A quantidade de filhos agora é 2T - 1 (T - 1 + T - 1 + 1)
        predecessor->n += sucessor->n + 1;
        no->n--;

        // Grava o nó
        gravarNo(no, T);

        // Remove recursivamente a chave k do predecessor
        remover(predecessor, k, T);

        // Apaga o arquivo do sucessor
        apagarArquivo(sucessor->filename);
    } 

    /* liberarNo(predecessor); */
    /* liberarNo(sucessor); */
}

void removerChaveFolha(NO *no, int indice, int T){
    // Move as chaves
    for(int i=indice+1; i < no->n; i++){
        no->chaves[i-1] = no->chaves[i];
    }

    no->n--;

    gravarNo(no, T);
}

int remover(NO *no, int chave, int T){
    // Busca a chave no nó atual
    int indice = 0;

    while(indice < no->n && chave != no->chaves[indice]){
        indice++;
    }

    if(indice < no->n && chave == no->chaves[indice]){
        // Achou a chave no nó atual
        if(no->folha){
            // Caso 1 : remover do nó folha
            removerChaveFolha(no, indice, T);
        }else{
            // Remover do nó interno
            removerChaveNaoFolha(no, indice, T);
        }
        // Removeu
        return 1;
    }else{
        if(no->folha){
            // Não achou a chave na árvore
            return 0;
        }

        
    }
    return 1;
}

int removerCLRS(NO **raiz, int chave, int T){
    int removeu = remover(*raiz, chave, T);

    if((*raiz)->n == 0){
        NO *temp = *raiz;
        *raiz = lerNo((*raiz)->filhos[0], T);
        gravarNo(*raiz, T);

        apagarArquivo(temp->filename);
        liberarNo(temp);
    }

    return removeu;
}

// Função de impressão da Árvore B
void imprimirArvoreB(NO* no, int nivel, int T) {
    if (no != NULL) {
        int i;
        if (!no->folha) {
            NO *filho = lerNo(no->filhos[no->n], T);
            imprimirArvoreB(filho, nivel + 1, T);
        }
        for (i = no->n - 1; i >= 0; i--) {
            for (int j = 0; j < nivel; j++) {
                printf("    ");  // Indentação para mostrar a profundidade
            }
            printf("%d\n", no->chaves[i]);

            if (!no->folha) {
                NO *filho = lerNo(no->filhos[i], T);
                imprimirArvoreB(filho, nivel + 1, T);
            }
        }
    }
}

int primeiroMenu(){
    int opt;
    while(1){
        printf("\n=====================================\n");
        printf("|        MENU - ÁRVORE B            |\n");
        printf("=====================================\n");
        printf("| 1. Criar árvore                   |\n");
        printf("| 2. Ler árvore                     |\n");
        printf("| 3. Sair                           |\n");
        printf("=====================================\n");
        printf("Digite sua escolha: ");
        scanf("%d", &opt);
        
        if(3 >= opt && opt >= 1){
            return opt;
        }else{
            printf("\nDigite uma opção válida\n");
            printf("\nPressione Enter para continuar...");
            getchar(); // Para capturar o Enter após escolher a opção
            getchar(); // Para aguardar o usuário pressionar Enter
            system("clear");
        }
    }
}

void exibirMenu(){
    printf("\n=====================================\n");
    printf("|        MENU - ÁRVORE B            |\n");
    printf("=====================================\n");
    printf("| 1. Inserir na árvore              |\n");
    printf("| 2. Remover na árvore              |\n");
    printf("| 3. Buscar na árvore               |\n");
    printf("| 4. Imprimir a árvore              |\n");
    printf("| 5. Sair                           |\n");
    printf("=====================================\n");
    printf("Digite sua escolha: ");
}
