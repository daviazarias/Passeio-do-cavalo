#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "passeio.h"

// Implementar o trabalho aqui

bool cavaloRetrocedendo = false;

void empilha(Pilha *pilha, int lin, int col){
    //Operação de empilhar.
    pilha->topo++;
    pilha->avancosPossiveis[pilha->topo] = col;
    pilha->topo++;
    pilha->avancosPossiveis[pilha->topo] = lin;
    return;
}

void proxCasa(Pilha *pilha, Casa *novaCasa){
    //Operação de desempilhar.
    novaCasa->pos_lin = pilha->avancosPossiveis[pilha->topo];
    pilha->topo--;
    novaCasa->pos_col = pilha->avancosPossiveis[pilha->topo];
    pilha->topo--;
    return;
}

void tentaEmpilhar(Pilha *pilha, bool **tabuleiro, int lin, int col, int max_lin, int max_col){
    //Só é possível empilhar as coordenadas caso elas estejam dentro dos limites do tabuleiro
    //e a casa associada a elas ainda não tenha sido acessada durante o trajeto atual.
    if(lin >= 0 && lin < max_lin && col >= 0 && col < max_col && !tabuleiro[lin][col]){
        empilha(pilha, lin, col);
    }
    return;
}

Pilha *constroiPilha(bool **tabuleiro, Casa *casa, int max_lin, int max_col){
    //Monta a pilha a partir de cálculos com as coordenadas atuais.
    //A pilha consiste de posições do tabuleiro aonde o cavalo pode ir em seguida.
    //Os cálculos são feitos baseados no movimento em "L" do cavalo no tabuleiro.
    Pilha *pilha = malloc(sizeof(Pilha));
    pilha->topo = -1;
    tentaEmpilhar(pilha, tabuleiro, casa->pos_lin + 2, casa->pos_col + 1, max_lin, max_col);
    tentaEmpilhar(pilha, tabuleiro, casa->pos_lin + 2, casa->pos_col - 1, max_lin, max_col);
    tentaEmpilhar(pilha, tabuleiro, casa->pos_lin - 2, casa->pos_col + 1, max_lin, max_col);
    tentaEmpilhar(pilha, tabuleiro, casa->pos_lin - 2, casa->pos_col - 1, max_lin, max_col);
    tentaEmpilhar(pilha, tabuleiro, casa->pos_lin + 1, casa->pos_col + 2, max_lin, max_col);
    tentaEmpilhar(pilha, tabuleiro, casa->pos_lin + 1, casa->pos_col - 2, max_lin, max_col);
    tentaEmpilhar(pilha, tabuleiro, casa->pos_lin - 1, casa->pos_col + 2, max_lin, max_col);
    tentaEmpilhar(pilha, tabuleiro, casa->pos_lin - 1, casa->pos_col - 2, max_lin, max_col);
    return pilha;
}

Casa *cavaloAvanca(bool **tabuleiro, Casa* casaAtual, int m, int n){
    //Monta a nova casa a partir das informações da casa de origem (antes do salto).
    //Casa de origem NULL indica que o cavalo deve ser posicionado na casa inferior esquerda.
    Casa *novaCasa = malloc(sizeof(Casa));

    novaCasa->casaAnterior = casaAtual;

    if(casaAtual == NULL){
        novaCasa->pos_lin = m - 1;
        novaCasa->pos_col = 0;
    } else {
        //Coordenadas novas advêm da pilha da casa de origem.
        proxCasa(casaAtual->casasSeguintes, novaCasa);
    }

    //Calcula novos movimentos possíveis a partir da nova posição (após o salto) e
    //empilha suas coordenadas.
    novaCasa->casasSeguintes = constroiPilha(tabuleiro, novaCasa, m, n);
    tabuleiro[novaCasa->pos_lin][novaCasa->pos_col] = true;
    cavaloRetrocedendo = false;

    return novaCasa;
}

Casa *cavaloRetrocede(bool **tabuleiro, Casa *casaAtual){
    //Recupera a casa anterior, desfaz o último avanço e libera a memória
    //previamente alocada para a casa (e sua pilha associada).
    Casa *novaCasa = casaAtual->casaAnterior;
    tabuleiro[casaAtual->pos_lin][casaAtual->pos_col] = false;
    cavaloRetrocedendo = true;
    free(casaAtual->casasSeguintes);
    free(casaAtual);
    return novaCasa;
}

bool avancoEhPossivel(Casa *casaAtual){
    //Verifica se ainda há coordenadas na pilha para onde o cavalo possa avançar.
    return (casaAtual->casasSeguintes->topo != - 1);
}

bool verifica(bool **tabuleiro, Casa *casaAtual, int lin, int col){
    //Varre o tabuleiro para verificar se ele todo foi percorrido
    //durante o trajeto atual. Caso a última ação do cavalo tenha sido um retorno,
    //não há necessidade de verificar o tabuleiro, pois a resposta é trivial.
    if(cavaloRetrocedendo){
        return false;
    }

    for(int i = 0; i < lin; i++){
        for(int j = 0; j < col; j++){
            if(!tabuleiro[i][j])
                return false;
        }
    }
    return true;
}

bool caminhoFechado(Casa *casaAtual, int m, int n){
    //Verifica se o cavalo pode retornar à casa inicial a partir de sua posição atual.
    return ((casaAtual->pos_lin == m - 2 && casaAtual->pos_col == 2) || (casaAtual->pos_lin == m - 3 && casaAtual->pos_col == 1));
}

void computa_passeios(bool **tabuleiro, int m, int n) {
    int fechados = 0;
    int abertos = 0;

    Casa *posicaoCavalo = cavaloAvanca(tabuleiro, NULL, m, n);

    while(posicaoCavalo != NULL)
    {
        if(avancoEhPossivel(posicaoCavalo))
        {
            posicaoCavalo = cavaloAvanca(tabuleiro, posicaoCavalo, m, n);
                   
        } else {

            if(verifica(tabuleiro, posicaoCavalo, m, n))
            {
                if(caminhoFechado(posicaoCavalo, m, n)){
                    fechados++;
                } else abertos++;
                //printf("Abertos: %d. Fechados: %d\n", abertos, fechados);
            }

            posicaoCavalo = cavaloRetrocede(tabuleiro, posicaoCavalo);
        }
    } 
    
    printf("%d\n%d\n", fechados, abertos);
    return;
}

int main(int argc, char* argv[]) {

    int n, m;
    int instancia_num = -1;
    instancia_num = atoi(argv[1]);
    if (instancia_num <= 0 || instancia_num > 20) {
        printf("Para executar o código, digite ./passeio x\nonde x é um número entre 1 e 20 que simboliza a instância utilizada\n");
        exit(0);
    }
    
    // Tabuleiro do jogo
    bool **tabuleiro = ler_instancia(instancia_num, &n, &m);

    computa_passeios(tabuleiro, n, m);

    return 0;
}

bool **ler_instancia(int instancia_num, int *n, int *m) {
    int i;
    
    // Montando o caminho para a instancia
    char *arquivo_instancia = "./instancias/";
    asprintf(&arquivo_instancia, "%s%d", arquivo_instancia, instancia_num);
    
    // Ponteiro para a instância
    FILE* file;
 
    // Abrindo a instância em modo leitura
    file = fopen(arquivo_instancia, "r");

    if (NULL == file) {
        printf("Arquivo de instância não encontrado.\n");
        exit(0);
    }

    // Lendo o arquivo da instância
    fscanf (file, "%d", n);
    fscanf (file, "%d", m);

    // Alocando o tabuleiro dinamicamente
    // Usando calloc ao invés de malloc para inicializar todo o tabuleiro com zeros
    bool** tabuleiro = (bool**)calloc(*n, sizeof(bool*));
    for (i = 0; i < *n; i++) {
        tabuleiro[i] = (bool*)calloc(*m, sizeof(bool));
    }

    fclose(file);

    return tabuleiro;
}