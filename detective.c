// DETECTIVE QUEST - NÍVEL MESTRE
// Tema 4 - Árvores Binárias, BST e Tabela Hash
// Desenvolvido por Rebeca Vieira Maia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10
#define MAX_STR 100

// ===============================
// Structs principais
// ===============================

// Estrutura de um cômodo (nó da árvore binária da mansão)
typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// Estrutura da árvore BST de pistas coletadas
typedef struct Pista {
    char pista[MAX_STR];
    struct Pista *esq;
    struct Pista *dir;
} Pista;

// Nó da tabela hash (pista -> suspeito)
typedef struct NoHash {
    char pista[MAX_STR];
    char suspeito[50];
    struct NoHash *prox;
} NoHash;

// Estrutura da tabela hash
typedef struct {
    NoHash *itens[TAM_HASH];
} HashTable;

// ===============================
// Funções auxiliares
// ===============================

// Função hash simples: soma os códigos dos caracteres e aplica módulo
int hash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

// ===============================
// Criação e manipulação das estruturas
// ===============================

/*
 * criarSala()
 * Cria dinamicamente um cômodo (nó da árvore binária) com o nome informado.
 * Retorna ponteiro para o novo nó.
 */
Sala* criarSala(char *nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));

    strcpy(nova->nome, nome);   // <-- CORRETO AGORA
    nova->esq = NULL;
    nova->dir = NULL;

    return nova;
}

/*
 * inserirPista()
 * Insere uma nova pista na BST de forma ordenada (alfabeticamente).
 * Retorna a nova raiz da subárvore.
 */
Pista* inserirPista(Pista *raiz, char *pista) {
    if (raiz == NULL) {
        Pista *novo = (Pista*)malloc(sizeof(Pista));

        strcpy(novo->pista, pista);  // copia a string, já insere '\0'
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

/*
 * inserirNaHash()
 * Insere uma relação pista -> suspeito na tabela hash.
 * Usa encadeamento (lista ligada) para tratar colisões.
 */
void inserirNaHash(HashTable *tabela, char *pista, char *suspeito) {
    int indice = hash(pista);

    NoHash *novo = (NoHash*)malloc(sizeof(NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela->itens[indice];
    tabela->itens[indice] = novo;
}

/*
 * encontrarSuspeito()
 * Consulta a tabela hash para saber qual suspeito está associado a uma pista.
 * Retorna ponteiro para a string do suspeito (dentro do nó da tabela) ou NULL se não encontrado.
 */
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

/*
 * exibirPistasInOrder()
 * Percorre a BST em ordem (in-order) e exibe as pistas coletadas em ordem alfabética.
 */
void exibirPistasInOrder(Pista *raiz) {
    if (raiz == NULL) return;
    exibirPistasInOrder(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistasInOrder(raiz->dir);
}

/*
 * verificarSuspeitoFinal()
 * Percorre a BST de pistas coletadas, conta quantas pistas apontam para o suspeito acusado
 * (usando a tabela hash para mapear pista -> suspeito) e decide se a acusação é válida.
 * Requisito: pelo menos 2 pistas apontando para o mesmo suspeito para confirmar acusação.
 */
void verificarSuspeitoFinal(Pista *raiz, HashTable *tabela, const char *suspeito) {
    if (raiz == NULL) {
        printf("\nNenhuma pista coletada — não é possível acusar.\n");
        return;
    }

    // Contagem recursiva simples
    int contador = 0;

    // função interna recursiva
    // conta quantas pistas na BST apontam para 'suspeito'
    int contar(Pista *n) {
        if (n == NULL) return 0;
        int c = 0;
        char *s = encontrarSuspeito(tabela, n->pista);
        if (s != NULL && strcmp(s, suspeito) == 0) c = 1;
        c += contar(n->esq);
        c += contar(n->dir);
        return c;
    }

    contador = contar(raiz);

    if (contador >= 2)
        printf("\nAcusação confirmada! %s é o(a) culpado(a) com base nas evidências (%d pistas).\n", suspeito, contador);
    else
        printf("\nAcusação inválida. %s não tem pistas suficientes (%d pista(s)).\n", suspeito, contador);
}

// ===============================
// Exploração da mansão
// ===============================

/*
 * pistaPorSala()
 * Retorna a pista associada ao nome da sala.
 * Retorna NULL se a sala não tiver pista.
 */
char* pistaPorSala(const char *nomeSala) {
    if (strcmp(nomeSala, "Biblioteca") == 0)
        return "Luvas rasgadas";
    if (strcmp(nomeSala, "Cozinha") == 0)
        return "Faca suja";
    if (strcmp(nomeSala, "Sala de Estar") == 0)
        return "Pegadas de lama";
    if (strcmp(nomeSala, "Escritorio") == 0)
        return "Bilhete ameaçador";
    if (strcmp(nomeSala, "Jardim") == 0)
        return "Flor arrancada";
    return NULL;
}

/*
 * explorarSalas()
 * Navega recursivamente pela árvore da mansão:
 * - Exibe a sala atual,
 * - Coleta a pista (se houver) e insere na BST de pistas coletadas,
 * - Pergunta ao jogador para ir à esquerda, direita ou sair.
 */
void explorarSalas(Sala *atual, Pista **pistas, HashTable *tabela) {
    if (atual == NULL) return;

    printf("\nVocê está na sala: %s\n", atual->nome);

    char *pista = pistaPorSala(atual->nome);
    if (pista != NULL) {
        printf("Você encontrou uma pista: %s\n", pista);
        *pistas = inserirPista(*pistas, pista);
    }

    char opcao;
    printf("\nEscolha: (e) esquerda | (d) direita | (s) sair\n> ");
    scanf(" %c", &opcao);

    if (opcao == 'e')
        explorarSalas(atual->esq, pistas, tabela);
    else if (opcao == 'd')
        explorarSalas(atual->dir, pistas, tabela);
    else if (opcao == 's')
        return;
    else {
        printf("Opção inválida — tente novamente.\n");
        explorarSalas(atual, pistas, tabela); // repete a mesma sala
    }
}

// ===============================
// Função principal
// ===============================
int main() {
    // Monta o mapa fixo da mansão (árvore binária)
    Sala *inicio = criarSala("Sala de Estar");
    inicio->esq = criarSala("Biblioteca");
    inicio->dir = criarSala("Cozinha");
    inicio->esq->esq = criarSala("Escritorio");
    inicio->esq->dir = criarSala("Jardim");

    // Inicializa a tabela hash (todos os ponteiros NULL)
    HashTable tabela;
    for (int i = 0; i < TAM_HASH; i++) tabela.itens[i] = NULL;

    // Associações pista -> suspeito
    // Aqui Sra. White terá duas pistas (culpada), outros têm 1.
    inserirNaHash(&tabela, "Luvas rasgadas", "Bibliotecária");
    inserirNaHash(&tabela, "Pegadas de lama", "Mordomo");      
    inserirNaHash(&tabela, "Faca suja", "Sra. White");
    inserirNaHash(&tabela, "Bilhete ameaçador", "Sra. White");   
    inserirNaHash(&tabela, "Flor arrancada", "Jardineiro");

    // Exploração interativa
    Pista *pistasColetadas = NULL;
    printf("🔍 Bem-vindo à Mansão Enigma!\n");
    explorarSalas(inicio, &pistasColetadas, &tabela);

    // Exibe todas as pistas coletadas em ordem alfabética
    printf("\n===== PISTAS COLETADAS (ordem alfabética) =====\n");
    if (pistasColetadas == NULL)
        printf("Nenhuma pista coletada.\n");
    else
        exibirPistasInOrder(pistasColetadas);

    // Fase final: acusação
    char suspeito[50];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    printf("\nBibliotecária");
    printf("\nMordomo");
    printf("\nSra. White");
    printf("\nJardineiro");

    scanf(" %[^\n]", suspeito);

    verificarSuspeitoFinal(pistasColetadas, &tabela, suspeito);

    printf("\nFim da investigação.\n");
    return 0;
}