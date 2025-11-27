#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50
#define TAM_PISTA 100

// Estrutura para representar um cômodo (nó da árvore binária)
struct Sala {
    char nome[TAM_NOME];
    char pista[TAM_PISTA];  // Pista encontrada neste cômodo
    struct Sala *esquerda;  // Caminho à esquerda
    struct Sala *direita;   // Caminho à direita
};

// Estrutura para representar um nó da árvore BST de pistas
struct NoPista {
    char pista[TAM_PISTA];
    struct NoPista *esquerda;
    struct NoPista *direita;
};

// Protótipos das funções - Salas
struct Sala* criarSala(char *nome, char *pista);
void liberarArvore(struct Sala *raiz);

// Protótipos das funções - Pistas (BST)
struct NoPista* inserirPista(struct NoPista *raiz, char *pista);
void exibirPistas(struct NoPista *raiz);
void liberarPistas(struct NoPista *raiz);

// Protótipos das funções - Exploração
void explorarSalasComPistas(struct Sala *salaAtual, struct NoPista **arvorePistas);

/**
 * Função principal
 * Monta o mapa da mansão com pistas e inicia a exploração
 */
int main() {
    struct NoPista *arvorePistas = NULL;  // BST para armazenar pistas coletadas
    
    printf("========================================\n");
    printf("  DETETIVE QUEST - MANSAO DAS PISTAS\n");
    printf("========================================\n\n");
    printf("Construindo o mapa da mansao...\n\n");
    
    // Criar o Hall de entrada (raiz da árvore)
    struct Sala *hall = criarSala("Hall de Entrada", "");
    
    // Nível 1 - Salas principais
    hall->esquerda = criarSala("Sala de Estar", "Vela apagada encontrada no chao");
    hall->direita = criarSala("Biblioteca", "Livro aberto na pagina 237");
    
    // Nível 2 - Esquerda (a partir da Sala de Estar)
    hall->esquerda->esquerda = criarSala("Cozinha", "Faca com manchas suspeitas");
    hall->esquerda->direita = criarSala("Sala de Jantar", "Taca de vinho quebrada");
    
    // Nível 2 - Direita (a partir da Biblioteca)
    hall->direita->esquerda = criarSala("Escritorio", "Carta anonima rasgada");
    hall->direita->direita = criarSala("Sala de Musica", "Partitura com anotacoes");
    
    // Nível 3 - Ramificações adicionais
    hall->esquerda->esquerda->esquerda = criarSala("Despensa", "Porta arrombada por dentro");
    hall->esquerda->esquerda->direita = criarSala("Adega", "Garrafa de veneno vazia");
    
    hall->esquerda->direita->esquerda = criarSala("Jardim de Inverno", "Pegadas na terra");
    hall->esquerda->direita->direita = criarSala("Varanda", "Janela forcada");
    
    hall->direita->esquerda->esquerda = criarSala("Cofre Secreto", "Dinheiro desaparecido");
    hall->direita->esquerda->direita = criarSala("Sala de Reunioes", "Mesa revirada");
    
    hall->direita->direita->esquerda = criarSala("Sala de Trofeaus", "Estatua quebrada");
    hall->direita->direita->direita = criarSala("Jardim Externo", "Rastro de sangue");
    
    printf("Mapa construido com sucesso!\n");
    printf("Voce esta no Hall de Entrada.\n");
    printf("Explore a mansao e colete pistas!\n\n");
    
    // Iniciar a exploração com coleta de pistas
    explorarSalasComPistas(hall, &arvorePistas);
    
    // Exibir todas as pistas coletadas em ordem alfabética
    printf("\n========================================\n");
    printf("       PISTAS COLETADAS\n");
    printf("========================================\n");
    if(arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        printf("Pistas em ordem alfabetica:\n\n");
        exibirPistas(arvorePistas);
    }
    printf("========================================\n");
    
    // Liberar memória alocada
    liberarArvore(hall);
    liberarPistas(arvorePistas);
    
    printf("\nObrigado por jogar Detetive Quest!\n");
    printf("========================================\n");
    
    return 0;
}

/**
 * Cria uma nova sala (nó da árvore) dinamicamente com ou sem pista
 * @param nome Nome da sala a ser criada
 * @param pista Pista encontrada na sala (pode ser string vazia)
 * @return Ponteiro para a sala criada
 */
struct Sala* criarSala(char *nome, char *pista) {
    struct Sala *novaSala = (struct Sala*)malloc(sizeof(struct Sala));
    
    if(novaSala == NULL) {
        printf("Erro ao alocar memoria para sala!\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * Insere uma nova pista na árvore BST de pistas
 * Organiza alfabeticamente para facilitar a visualização
 * @param raiz Raiz da árvore de pistas
 * @param pista Texto da pista a ser inserida
 * @return Ponteiro para a raiz atualizada
 */
struct NoPista* inserirPista(struct NoPista *raiz, char *pista) {
    if(raiz == NULL) {
        struct NoPista *novoNo = (struct NoPista*)malloc(sizeof(struct NoPista));
        if(novoNo == NULL) {
            printf("Erro ao alocar memoria para pista!\n");
            exit(1);
        }
        strcpy(novoNo->pista, pista);
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        return novoNo;
    }
    
    // Inserir de forma ordenada (alfabeticamente)
    if(strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if(strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se for igual, não insere duplicata
    
    return raiz;
}

/**
 * Exibe todas as pistas em ordem alfabética (in-order traversal)
 * @param raiz Raiz da árvore de pistas
 */
void exibirPistas(struct NoPista *raiz) {
    if(raiz == NULL) {
        return;
    }
    
    exibirPistas(raiz->esquerda);
    printf("  - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/**
 * Libera toda a memória da árvore de pistas
 * @param raiz Raiz da árvore de pistas
 */
void liberarPistas(struct NoPista *raiz) {
    if(raiz == NULL) {
        return;
    }
    
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

/**
 * Controla a navegação entre salas e coleta de pistas
 * O jogador escolhe ir à esquerda (e), direita (d) ou sair (s)
 * Pistas são automaticamente adicionadas à BST quando encontradas
 * @param salaAtual Ponteiro para a sala atual
 * @param arvorePistas Ponteiro para o ponteiro da árvore BST de pistas
 */
void explorarSalasComPistas(struct Sala *salaAtual, struct NoPista **arvorePistas) {
    char escolha;
    
    if(salaAtual == NULL) {
        printf("Sala invalida!\n");
        return;
    }
    
    // Exibir sala atual
    printf("========================================\n");
    printf("Voce esta em: %s\n", salaAtual->nome);
    printf("========================================\n");
    
    // Verificar se há pista nesta sala
    if(strlen(salaAtual->pista) > 0) {
        printf("\n[!] PISTA ENCONTRADA!\n");
        printf(">>> %s\n", salaAtual->pista);
        
        // Adicionar pista à árvore BST
        *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
        printf("\n[Pista adicionada ao caderno de investigacao]\n");
    } else {
        printf("\nNenhuma pista encontrada nesta sala.\n");
    }
    
    // Verificar se é uma sala final (nó-folha)
    if(salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("\nEsta sala nao possui mais saidas.\n");
        printf("Deseja encerrar a investigacao? (s/n): ");
        scanf(" %c", &escolha);
        if(escolha == 's' || escolha == 'S') {
            return;
        } else {
            printf("\n[Voce esta preso nesta sala sem saidas]\n");
            return;
        }
    }
    
    // Mostrar opções disponíveis
    printf("\nOpcoes:\n");
    if(salaAtual->esquerda != NULL) {
        printf("  [E] Ir para esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if(salaAtual->direita != NULL) {
        printf("  [D] Ir para direita  -> %s\n", salaAtual->direita->nome);
    }
    printf("  [S] Sair e encerrar investigacao\n");
    
    // Solicitar escolha do jogador
    printf("\nEscolha uma opcao: ");
    scanf(" %c", &escolha);
    
    // Processar escolha
    if(escolha == 'e' || escolha == 'E') {
        if(salaAtual->esquerda != NULL) {
            printf("\nVoce foi para a esquerda...\n\n");
            explorarSalasComPistas(salaAtual->esquerda, arvorePistas);
        } else {
            printf("\nNao ha caminho a esquerda!\n");
            printf("Tente novamente.\n\n");
            explorarSalasComPistas(salaAtual, arvorePistas);
        }
    } else if(escolha == 'd' || escolha == 'D') {
        if(salaAtual->direita != NULL) {
            printf("\nVoce foi para a direita...\n\n");
            explorarSalasComPistas(salaAtual->direita, arvorePistas);
        } else {
            printf("\nNao ha caminho a direita!\n");
            printf("Tente novamente.\n\n");
            explorarSalasComPistas(salaAtual, arvorePistas);
        }
    } else if(escolha == 's' || escolha == 'S') {
        printf("\nEncerrando a exploracao...\n");
        return;
    } else {
        printf("\nOpcao invalida! Use 'E', 'D' ou 'S'.\n\n");
        explorarSalasComPistas(salaAtual, arvorePistas);
    }
}

/**
 * Libera toda a memória alocada para a árvore (pós-ordem)
 * @param raiz Ponteiro para a raiz da árvore
 */
void liberarArvore(struct Sala *raiz) {
    if(raiz == NULL) {
        return;
    }
    
    // Liberar subárvores primeiro
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    
    // Liberar o nó atual
    free(raiz);
}
