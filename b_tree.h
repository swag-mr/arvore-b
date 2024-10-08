#ifndef B_TREE_H
#define B_TREE_H

typedef struct no {
    int n;
    int folha;
    int *chaves;
    char **filhos;
    char *filename;
} NO;

char* gerarNomeArquivo();
char* gerarNomeArquivoArvore();
NO* criarNo(int T, int folha, char *nomeArquivo);
void liberarNo(NO *no);
NO* lerNo(char *nome_arquivo, int T);
void lerArvore(const char *nome_arquivo, char **nome_raiz, int *T);
void gravarArvore(const char *nome_arquivo, char *nome_raiz, int T);
void gravarNo(NO *no, int T);
void listarArquivosArvore();
int verificarNomeArquivo(char *nome);
NO* buscaInsercao(NO *no, int chave, int T);
NO* busca(NO *no, int chave, int T, int *posicao);
void splitChild(NO *pai, int indice, int T);
void inserirNaoCheio(NO *no, int chave, int T);
int inserir(NO **raiz, int chave, int T);
void imprimirArvoreB(NO* no, int nivel, int T);
int primeiroMenu();
void exibirMenu();

void apagarArquivo(char *filename);
void funde(NO *no, int indice, int T);
void emprestaAnterior(NO *no, int indice, int T);
void emprestaSeguinte(NO *no, int indice, int T);
void preenche(NO *no, int indice, int T);
void removerChaveNaoFolha(NO *no, int indice, int T);
void removerChaveFolha(NO *no, int indice, int T);
int remover(NO *no, int chave, int T);
int removerCLRS(NO **raiz, int chave, int T);

void testeScript(NO *no, int T);

#endif
