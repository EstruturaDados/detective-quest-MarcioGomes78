#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50

// Estrutura para representar um cômodo (nó da árvore binária)
struct Sala {
    char nome[TAM_NOME];
    struct Sala *esquerda;  // Caminho à esquerda
    struct Sala *direita;   // Caminho à direita
};

// Protótipos das funções
struct Sala* criarSala(char *nome);
void explorarSalas(struct Sala *salaAtual);
void liberarArvore(struct Sala *raiz);

/**
 * Função principal
 * Monta o mapa da mansão e inicia a exploração
 */
int main() {
    // Criar o mapa da mansão usando alocação dinâmica
    printf("========================================\n");
    printf("     DETETIVE QUEST - MAPA DA MANSAO\n");
    printf("========================================\n\n");
    printf("Construindo o mapa da mansao...\n\n");
    
    // Criar o Hall de entrada (raiz da árvore)
    struct Sala *hall = criarSala("Hall de Entrada");
    
    // Nível 1 - Salas principais
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Biblioteca");
    
    // Nível 2 - Esquerda (a partir da Sala de Estar)
    hall->esquerda->esquerda = criarSala("Cozinha");
    hall->esquerda->direita = criarSala("Sala de Jantar");
    
    // Nível 2 - Direita (a partir da Biblioteca)
    hall->direita->esquerda = criarSala("Escritorio");
    hall->direita->direita = criarSala("Sala de Musica");
    
    // Nível 3 - Ramificações adicionais
    hall->esquerda->esquerda->esquerda = criarSala("Despensa");
    hall->esquerda->esquerda->direita = criarSala("Adega");
    
    hall->esquerda->direita->esquerda = criarSala("Jardim de Inverno");
    hall->esquerda->direita->direita = criarSala("Varanda");
    
    hall->direita->esquerda->esquerda = criarSala("Cofre Secreto");
    hall->direita->esquerda->direita = criarSala("Sala de Reunioes");
    
    hall->direita->direita->esquerda = criarSala("Sala de Trofeaus");
    hall->direita->direita->direita = criarSala("Jardim Externo");
    
    printf("Mapa construido com sucesso!\n");
    printf("Voce esta no Hall de Entrada.\n");
    printf("Prepare-se para explorar a mansao...\n\n");
    
    // Iniciar a exploração
    explorarSalas(hall);
    
    // Liberar memória alocada
    liberarArvore(hall);
    
    printf("\n========================================\n");
    printf("Obrigado por jogar Detetive Quest!\n");
    printf("========================================\n");
    
    return 0;
}

/**
 * Cria uma nova sala (nó da árvore) dinamicamente
 * @param nome Nome da sala a ser criada
 * @return Ponteiro para a sala criada
 */
struct Sala* criarSala(char *nome) {
    struct Sala *novaSala = (struct Sala*)malloc(sizeof(struct Sala));
    
    if(novaSala == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * Permite a navegação interativa pela mansão
 * O jogador escolhe ir à esquerda (e) ou à direita (d)
 * A exploração continua até chegar a um cômodo sem saídas (nó-folha)
 * @param salaAtual Ponteiro para a sala atual na exploração
 */
void explorarSalas(struct Sala *salaAtual) {
    char escolha;
    
    if(salaAtual == NULL) {
        printf("Sala invalida!\n");
        return;
    }
    
    // Exibir sala atual
    printf("========================================\n");
    printf("Voce esta em: %s\n", salaAtual->nome);
    printf("========================================\n");
    
    // Verificar se é uma sala final (nó-folha)
    if(salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("\nEsta sala nao possui mais saidas.\n");
        printf("Voce chegou ao final deste caminho!\n");
        return;
    }
    
    // Mostrar opções disponíveis
    printf("\nCaminhos disponiveis:\n");
    if(salaAtual->esquerda != NULL) {
        printf("  [E] Esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if(salaAtual->direita != NULL) {
        printf("  [D] Direita  -> %s\n", salaAtual->direita->nome);
    }
    
    // Solicitar escolha do jogador
    printf("\nPara onde deseja ir? ");
    scanf(" %c", &escolha);
    
    // Processar escolha
    if(escolha == 'e' || escolha == 'E') {
        if(salaAtual->esquerda != NULL) {
            printf("\nVoce escolheu ir para a esquerda...\n\n");
            explorarSalas(salaAtual->esquerda);
        } else {
            printf("\nNao ha caminho a esquerda!\n");
            printf("Tente novamente.\n\n");
            explorarSalas(salaAtual);
        }
    } else if(escolha == 'd' || escolha == 'D') {
        if(salaAtual->direita != NULL) {
            printf("\nVoce escolheu ir para a direita...\n\n");
            explorarSalas(salaAtual->direita);
        } else {
            printf("\nNao ha caminho a direita!\n");
            printf("Tente novamente.\n\n");
            explorarSalas(salaAtual);
        }
    } else {
        printf("\nOpcao invalida! Use 'E' para esquerda ou 'D' para direita.\n\n");
        explorarSalas(salaAtual);
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
