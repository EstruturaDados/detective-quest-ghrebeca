// DETECTIVE QUEST - NÍVEL NOVATO
// Tema 4 - Árvores Binárias
// Desenvolvido por Rebeca Vieira Maia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ======================== ESTRUTURA ========================
// Estrutura que representa cada sala da mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ======================== FUNÇÕES ========================
// Cria dinamicamente uma nova sala
Sala* criarSala(char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Permite o jogador explorar a mansão
void explorarSalas(Sala *atual) {
    char opcao;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Verifica se é um nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim da exploração! Você chegou ao último cômodo.\n");
            return;
        }

        // Mostra opções de caminho disponíveis
        printf("Escolha um caminho:\n");
        if (atual->esquerda) printf(" (e) Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita) printf(" (d) Ir para a direita (%s)\n", atual->direita->nome);
        printf(" (s) Sair do jogo\n> ");
        scanf(" %c", &opcao);

        switch (opcao) {
            case 'e':
                if (atual->esquerda)
                    atual = atual->esquerda;
                else
                    printf("Não há sala à esquerda!\n");
                break;
            case 'd':
                if (atual->direita)
                    atual = atual->direita;
                else
                    printf("Não há sala à direita!\n");
                break;
            case 's':
                printf("\nVocê decidiu sair da mansão.\n");
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// ======================== MAIN ========================
int main() {
    // Cria o mapa fixo da mansão (árvore binária)
    Sala *raiz = criarSala("Hall de Entrada");
    raiz->esquerda = criarSala("Sala de Estar");
    raiz->direita = criarSala("Biblioteca");

    raiz->esquerda->esquerda = criarSala("Quarto");
    raiz->esquerda->direita = criarSala("Closet");
    raiz->direita->esquerda = criarSala("Quarto de Hóspedes");
    raiz->direita->direita = criarSala("Cozinha");

    raiz->esquerda->esquerda->esquerda = criarSala("Sala de Televisão");
    raiz->esquerda->esquerda->direita = criarSala("Lavanderia");
    raiz->esquerda->direita->esquerda = criarSala("Jardim");
    raiz->direita->direita->esquerda = criarSala("Piscina");
    raiz->direita->esquerda->direita = criarSala("Sauna");

    printf("===== DETECTIVE QUEST - MANSÃO INTERATIVA =====\n");
    explorarSalas(raiz);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
