// DETECTIVE QUEST - NÍVEL MESTRE
// Árvores Binárias, BST e Tabela Hash
// Desenvolvido por Rebeca Vieira Maia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10
#define MAX_STR 100

// ===============================
// Structs
// ===============================

// Nó da árvore de salas
typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// Nó da BST de pistas coletadas
typedef struct Pista {
    char pista[MAX_STR];
    struct Pista *esq;
    struct Pista *dir;
} Pista;

// Nó da lista da hash (pista → suspeito)
typedef struct NoHash {
    char pista[MAX_STR];
    char suspeito[50];
    struct NoHash *prox;
} NoHash;

// Tabela hash
typedef struct {
    NoHash *itens[TAM_HASH];
} HashTable;


// ===============================
// Funções auxiliares
// ===============================

// Função hash simples
int hash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

// ===============================
// Criação das estruturas
// ===============================

// Cria sala
Sala* criarSala(char *nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

// Insere pista na BST
Pista* inserirPista(Pista *raiz, char *pista) {
    if (raiz == NULL) {
        Pista *novo = (Pista*)malloc(sizeof(Pista));
        strcpy(novo->pista, pista);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

// Insere pista → suspeito na hash
void inserirNaHash(HashTable *tabela, char *pista, char *suspeito) {
    int indice = hash(pista);

    NoHash *novo = (NoHash*)malloc(sizeof(NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela->itens[indice];
    tabela->itens[indice] = novo;
}

// Busca suspeito pela pista
char* encontrarSuspeito(HashTable *tabela, const char *pista) {
    int indice = hash(pista);
    NoHash *atual = tabela->itens[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// Exibe BST em ordem
void exibirPistasInOrder(Pista *raiz) {
    if (raiz == NULL) return;
    exibirPistasInOrder(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistasInOrder(raiz->dir);
}


// Conta quantas pistas levam ao suspeito
int contarPistasDoSuspeito(Pista *n, HashTable *tabela, const char *suspeito) {
    if (n == NULL) return 0;

    int c = 0;
    char *s = encontrarSuspeito(tabela, n->pista);

    if (s != NULL && strcmp(s, suspeito) == 0)
        c = 1;

    return c
        + contarPistasDoSuspeito(n->esq, tabela, suspeito)
        + contarPistasDoSuspeito(n->dir, tabela, suspeito);
}


// ===============================
// Verificação final
// ===============================

// Confere se suspeito tem ao menos 2 pistas
void verificarSuspeitoFinal(Pista *raiz, HashTable *tabela, const char *suspeito) {
    if (raiz == NULL) {
        printf("\nNenhuma pista coletada.\n");
        return;
    }

    int contador = contarPistasDoSuspeito(raiz, tabela, suspeito);

    if (contador >= 2)
        printf("\nAcusação confirmada! %s é culpado(a) (%d pistas).\n", suspeito, contador);
    else
        printf("\nAcusação inválida. Apenas %d pista(s).\n", contador);
}

// ===============================
// Exploração da mansão
// ===============================

// Devolve pista da sala
char* pistaPorSala(const char *nomeSala) {
    if (strcmp(nomeSala, "Biblioteca") == 0) return "Luvas rasgadas";
    if (strcmp(nomeSala, "Cozinha") == 0) return "Faca suja";
    if (strcmp(nomeSala, "Sala de Estar") == 0) return "Pegadas de lama";
    if (strcmp(nomeSala, "Escritorio") == 0) return "Bilhete ameaçador";
    if (strcmp(nomeSala, "Jardim") == 0) return "Flor arrancada";
    return NULL;
}

// Visita salas e coleta pistas
void explorarSalas(Sala *atual, Pista **pistas, HashTable *tabela) {
    if (atual == NULL) return;

    printf("\nVocê está na sala: %s\n", atual->nome);

    char *pista = pistaPorSala(atual->nome);
    if (pista != NULL) {
        printf("Você encontrou: %s\n", pista);
        *pistas = inserirPista(*pistas, pista);
    }

    char opcao;
    printf("\n(e) esquerda | (d) direita | (s) sair\n> ");
    scanf(" %c", &opcao);

    if (opcao == 'e') explorarSalas(atual->esq, pistas, tabela);
    else if (opcao == 'd') explorarSalas(atual->dir, pistas, tabela);
    else if (opcao == 's') return;
    else {
        printf("Opção inválida.\n");
        explorarSalas(atual, pistas, tabela);
    }
}


// ===============================
// main
// ===============================
int main() {
    // Mapa da mansão
    Sala *inicio = criarSala("Sala de Estar");
    inicio->esq = criarSala("Biblioteca");
    inicio->dir = criarSala("Cozinha");
    inicio->esq->esq = criarSala("Escritorio");
    inicio->esq->dir = criarSala("Jardim");

    // Tabela hash
    HashTable tabela;
    for (int i = 0; i < TAM_HASH; i++) tabela.itens[i] = NULL;

    // Associações de pistas
    inserirNaHash(&tabela, "Luvas rasgadas", "Jardineiro");
    inserirNaHash(&tabela, "Pegadas de lama", "Mordomo");
    inserirNaHash(&tabela, "Faca suja", "Bibliotecária");
    inserirNaHash(&tabela, "Bilhete ameaçador", "Jardineiro");
    inserirNaHash(&tabela, "Flor arrancada", "Sra. White");

    // Exploração
    Pista *pistasColetadas = NULL;
    printf("Bem-vindo à Mansão Enigma!\n");
    explorarSalas(inicio, &pistasColetadas, &tabela);

    // Pistas coletadas
    printf("\n===== PISTAS COLETADAS =====\n");
    if (pistasColetadas == NULL)
        printf("Nenhuma pista.\n");
    else
        exibirPistasInOrder(pistasColetadas);

    // Acusação
    char suspeito[50];
    printf("\nSuspeitos:\nBibliotecária\nMordomo\nSra. White\nJardineiro\n");
    printf("Digite quem deseja acusar:\n> ");
    scanf(" %[^\n]", suspeito);

    verificarSuspeitoFinal(pistasColetadas, &tabela, suspeito);

    printf("\nFim da investigação.\n");
    return 0;
}