#ifndef B_TREE_H
#define B_TREE_H

typedef struct no {
    int n;
    int folha;
    int *chaves;
    char **filhos;
} NO;

char* gerarNomeArquivo();
NO* criarNo(int T, int folha);
void liberarNo(NO *no);
NO* lerNo(const char *nome_arquivo, int T);
void gravarNo(const char *nome_arquivo, NO *no, int T);
NO* busca(NO *no, int chave, int T, int *posicao);
void splitChild(NO *pai, int indice, int T, char *nomeArquivoPai);
void inserirNaoCheio(NO *no, int chave, int T, char *nomeArquivoNo);
void inserir(NO **raiz, int chave, int T, char **nomeArquivoRaizAtual);
void imprimirArvoreB(NO* no, int nivel, int T);
void exibirMenu();

#endif
