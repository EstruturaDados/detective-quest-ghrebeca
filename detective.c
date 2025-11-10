// DETECTIVE QUEST - NÍVEL AVENTUREIRO
// Tema 4 - Árvores Binárias e Árvores Binárias de Busca (BST)
// Desenvolvido por Rebeca Vieira Maia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 50

// ======================== ESTRUTURAS ========================

// Estrutura que representa cada sala da mansão
typedef struct Sala {
    char nome[max];
    char pista[max];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura que representa um nó da árvore de pistas (BST)
typedef struct Pista {
    char pista[max];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// ======================== FUNÇÕES ========================
// Cria dinamicamente uma nova sala
Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Insere uma nova pista na BST
Pista* inserirPista(Pista *raiz, char *pista) {
    if (raiz == NULL) {
        Pista *nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->pista, pista);
        nova->esquerda = NULL;
        nova->direita = NULL;
        return nova;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

// Exibe pistas em ordem alfabética (in-ordem)
void exibirPistas(Pista *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf(" - %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Permite o jogador explorar a mansão
void explorarSalas(Sala *atual, Pista **raizPista) {
    char opcao;
    char caminho[500] = ""; // guarda o percurso do jogador

   while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Se a sala tem pista, coleta
        if (strcmp(atual->pista, " ") != 0) {
            printf("Você encontrou uma pista: %s!\n", atual->pista);
            *raizPista = inserirPista(*raizPista, atual->pista);
        }

        // Verifica se é um nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nFim da exploração! Você chegou ao último cômodo.\n");
            printf("\nSeu percurso: %sFIM\n", caminho);
            return;
        }

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
                printf("\nSeu percurso até agora: %sSAÍDA\n", caminho);
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

        strcat(caminho, atual->nome);
        strcat(caminho, " -> ");
    }
}

// ======================== MAIN ========================
int main() {
    // Cria o mapa fixo da mansão (árvore binária)
    Sala *raiz = criarSala("Hall de Entrada", " ");
    raiz->esquerda = criarSala("Sala de Estar", "Chave Dourada");
    raiz->direita = criarSala("Biblioteca", "Estante Retangular");

    raiz->esquerda->esquerda = criarSala("Quarto", " ");
    raiz->esquerda->direita = criarSala("Closet", "Porta de Vidro");
    raiz->direita->esquerda = criarSala("Quarto de Hóspedes", "Cama Beliche");
    raiz->direita->direita = criarSala("Cozinha", "Armário Escondido");

    raiz->esquerda->esquerda->esquerda = criarSala("Sala de Televisão", " ");
    raiz->esquerda->esquerda->direita = criarSala("Lavanderia", "Vassoura Azul");
    raiz->esquerda->direita->esquerda = criarSala("Jardim", "Portal Secreto");
    raiz->direita->direita->esquerda = criarSala("Piscina", "Borda Infinita");
    raiz->direita->esquerda->direita = criarSala("Sauna", "Fumaça");

    printf("===== DETECTIVE QUEST - MANSÃO INTERATIVA =====\n");

    Pista *raizPista = NULL;
    explorarSalas(raiz, &raizPista);

    if (raizPista) {
        printf("\n===== PISTAS COLETADAS (em ordem alfabética) =====\n");
        exibirPistas(raizPista);
    } else {
        printf("\nNenhuma pista foi coletada!\n");
    }

    printf("\nParabéns, detetive! Você coletou todas as pistas e desvendou o mistério da mansão!\n");
    return 0;
}
