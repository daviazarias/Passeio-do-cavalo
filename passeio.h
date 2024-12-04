#include <stdbool.h>

#ifndef _H_PASSEIO
#define _H_PASSEIO

typedef struct {
    int avancosPossiveis[14];
    int topo;
} Pilha;

typedef struct Casa {
    struct Casa *casaAnterior;
    int pos_lin;
    int pos_col;
    Pilha *casasSeguintes;
} Casa;

bool **ler_instancia(int instancia_num, int *n, int *m);
void computa_passeios(bool **tabuleiro, int m, int n);

void empilha(Pilha *pilha, int lin, int col);
void proxCasa(Pilha *pilha, Casa *novaCasa);
void tentaEmpilhar(Pilha *pilha, bool **tabuleiro, int lin, int col, int max_lin, int max_col);
Pilha *constroiPilha(bool **tabuleiro, Casa *casa, int max_lin, int max_col);
Casa *cavaloAvanca(bool **tabuleiro, Casa* casaAtual, int m, int n);
Casa *cavaloRetrocede(bool **tabuleiro, Casa *casaAtual);
bool avancoEhPossivel(Casa *casaAtual);
bool verifica(bool **tabuleiro, Casa *casaAtual, int lin, int col);
bool caminhoFechado(Casa *casaAtual, int m, int n);

#endif // _H_PASSEIO