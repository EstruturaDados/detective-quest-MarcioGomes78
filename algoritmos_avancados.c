#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50
#define TAM_PISTA 100
#define TAM_SUSPEITO 50
#define TAM_HASH 20

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

// Estrutura para um nó da lista encadeada na tabela hash
struct NoHash {
    char pista[TAM_PISTA];
    char suspeito[TAM_SUSPEITO];
    struct NoHash *proximo;
};

// Tabela hash (array de ponteiros para listas encadeadas)
struct TabelaHash {
    struct NoHash *tabela[TAM_HASH];
};

// Protótipos das funções - Salas
struct Sala* criarSala(char *nome, char *pista);
void liberarArvore(struct Sala *raiz);

// Protótipos das funções - Pistas (BST)
struct NoPista* inserirPista(struct NoPista *raiz, char *pista);
void exibirPistas(struct NoPista *raiz);
void contarPistas(struct NoPista *raiz, int *contador);
void liberarPistas(struct NoPista *raiz);

// Protótipos das funções - Tabela Hash
void inicializarHash(struct TabelaHash *hash);
int funcaoHash(char *chave);
void inserirNaHash(struct TabelaHash *hash, char *pista, char *suspeito);
char* encontrarSuspeito(struct TabelaHash *hash, char *pista);
void contarPistasPorSuspeito(struct NoPista *raiz, struct TabelaHash *hash, char *suspeito, int *contador);
void liberarHash(struct TabelaHash *hash);

// Protótipos das funções - Exploração e Julgamento
void explorarSalasComPistas(struct Sala *salaAtual, struct NoPista **arvorePistas);
void verificarSuspeitoFinal(struct NoPista *arvorePistas, struct TabelaHash *hash);

/**
 * Função principal
 * Monta o mapa da mansão com pistas e inicia a exploração
 */
int main() {
    struct NoPista *arvorePistas = NULL;  // BST para armazenar pistas coletadas
    struct TabelaHash tabelaHash;         // Tabela hash para associar pistas a suspeitos
    
    printf("========================================\n");
    printf("  DETETIVE QUEST - MANSAO DAS PISTAS\n");
    printf("========================================\n\n");
    printf("Construindo o mapa da mansao...\n\n");
    
    // Inicializar tabela hash
    inicializarHash(&tabelaHash);
    
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
    
    // Associar pistas a suspeitos na tabela hash
    printf("Carregando banco de dados de suspeitos...\n\n");
    inserirNaHash(&tabelaHash, "Vela apagada encontrada no chao", "Sr. Viktor");
    inserirNaHash(&tabelaHash, "Livro aberto na pagina 237", "Dra. Helena");
    inserirNaHash(&tabelaHash, "Faca com manchas suspeitas", "Chef Marcelo");
    inserirNaHash(&tabelaHash, "Taca de vinho quebrada", "Sra. Beatriz");
    inserirNaHash(&tabelaHash, "Carta anonima rasgada", "Sr. Viktor");
    inserirNaHash(&tabelaHash, "Partitura com anotacoes", "Maestro Eduardo");
    inserirNaHash(&tabelaHash, "Porta arrombada por dentro", "Chef Marcelo");
    inserirNaHash(&tabelaHash, "Garrafa de veneno vazia", "Dra. Helena");
    inserirNaHash(&tabelaHash, "Pegadas na terra", "Jardineiro Carlos");
    inserirNaHash(&tabelaHash, "Janela forcada", "Sra. Beatriz");
    inserirNaHash(&tabelaHash, "Dinheiro desaparecido", "Sr. Viktor");
    inserirNaHash(&tabelaHash, "Mesa revirada", "Chef Marcelo");
    inserirNaHash(&tabelaHash, "Estatua quebrada", "Maestro Eduardo");
    inserirNaHash(&tabelaHash, "Rastro de sangue", "Dra. Helena");
    
    printf("========================================\n");
    printf("SUSPEITOS CONHECIDOS:\n");
    printf("========================================\n");
    printf("1. Sr. Viktor\n");
    printf("2. Dra. Helena\n");
    printf("3. Chef Marcelo\n");
    printf("4. Sra. Beatriz\n");
    printf("5. Maestro Eduardo\n");
    printf("6. Jardineiro Carlos\n");
    printf("========================================\n\n");
    
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
        printf("========================================\n");
    } else {
        printf("Pistas em ordem alfabetica:\n\n");
        exibirPistas(arvorePistas);
        printf("========================================\n");
        
        // Fase de julgamento final
        verificarSuspeitoFinal(arvorePistas, &tabelaHash);
    }
    
    // Liberar memória alocada
    liberarArvore(hall);
    liberarPistas(arvorePistas);
    liberarHash(&tabelaHash);
    
    printf("\n========================================\n");
    printf("Obrigado por jogar Detetive Quest!\n");
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
 * Conta o número total de pistas na árvore
 * @param raiz Raiz da árvore de pistas
 * @param contador Ponteiro para o contador
 */
void contarPistas(struct NoPista *raiz, int *contador) {
    if(raiz == NULL) {
        return;
    }
    
    contarPistas(raiz->esquerda, contador);
    (*contador)++;
    contarPistas(raiz->direita, contador);
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
 * Inicializa a tabela hash com todas as posições NULL
 * @param hash Ponteiro para a tabela hash
 */
void inicializarHash(struct TabelaHash *hash) {
    int i;
    for(i = 0; i < TAM_HASH; i++) {
        hash->tabela[i] = NULL;
    }
}

/**
 * Função hash simples que calcula o índice baseado na soma dos caracteres
 * @param chave String da pista
 * @return Índice na tabela hash
 */
int funcaoHash(char *chave) {
    int soma = 0;
    int i;
    
    for(i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    
    return soma % TAM_HASH;
}

/**
 * Insere uma associação pista-suspeito na tabela hash
 * Trata colisões usando encadeamento (lista encadeada)
 * @param hash Ponteiro para a tabela hash
 * @param pista String da pista (chave)
 * @param suspeito Nome do suspeito (valor)
 */
void inserirNaHash(struct TabelaHash *hash, char *pista, char *suspeito) {
    int indice = funcaoHash(pista);
    
    struct NoHash *novoNo = (struct NoHash*)malloc(sizeof(struct NoHash));
    if(novoNo == NULL) {
        printf("Erro ao alocar memoria para hash!\n");
        exit(1);
    }
    
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = hash->tabela[indice];
    hash->tabela[indice] = novoNo;
}

/**
 * Encontra o suspeito associado a uma pista na tabela hash
 * @param hash Ponteiro para a tabela hash
 * @param pista String da pista a buscar
 * @return Nome do suspeito ou NULL se não encontrado
 */
char* encontrarSuspeito(struct TabelaHash *hash, char *pista) {
    int indice = funcaoHash(pista);
    struct NoHash *atual = hash->tabela[indice];
    
    while(atual != NULL) {
        if(strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return NULL;
}

/**
 * Conta quantas pistas coletadas apontam para um suspeito específico
 * @param raiz Raiz da árvore de pistas
 * @param hash Ponteiro para a tabela hash
 * @param suspeito Nome do suspeito a verificar
 * @param contador Ponteiro para o contador
 */
void contarPistasPorSuspeito(struct NoPista *raiz, struct TabelaHash *hash, char *suspeito, int *contador) {
    if(raiz == NULL) {
        return;
    }
    
    contarPistasPorSuspeito(raiz->esquerda, hash, suspeito, contador);
    
    char *suspeitoEncontrado = encontrarSuspeito(hash, raiz->pista);
    if(suspeitoEncontrado != NULL && strcmp(suspeitoEncontrado, suspeito) == 0) {
        (*contador)++;
    }
    
    contarPistasPorSuspeito(raiz->direita, hash, suspeito, contador);
}

/**
 * Libera toda a memória da tabela hash
 * @param hash Ponteiro para a tabela hash
 */
void liberarHash(struct TabelaHash *hash) {
    int i;
    struct NoHash *atual, *temp;
    
    for(i = 0; i < TAM_HASH; i++) {
        atual = hash->tabela[i];
        while(atual != NULL) {
            temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

/**
 * Conduz a fase de julgamento final
 * Solicita acusação do jogador e verifica se há evidências suficientes
 * @param arvorePistas Árvore BST com pistas coletadas
 * @param hash Tabela hash com associações pista-suspeito
 */
void verificarSuspeitoFinal(struct NoPista *arvorePistas, struct TabelaHash *hash) {
    char acusado[TAM_SUSPEITO];
    int pistasEncontradas = 0;
    int totalPistas = 0;
    
    contarPistas(arvorePistas, &totalPistas);
    
    printf("\n========================================\n");
    printf("       FASE DE JULGAMENTO\n");
    printf("========================================\n");
    printf("Total de pistas coletadas: %d\n\n", totalPistas);
    
    printf("Com base nas evidencias, quem voce acusa?\n");
    printf("Digite o nome completo do suspeito: ");
    getchar(); // Limpar buffer
    fgets(acusado, TAM_SUSPEITO, stdin);
    acusado[strcspn(acusado, "\n")] = '\0';
    
    printf("\n========================================\n");
    printf("Analisando evidencias contra %s...\n", acusado);
    printf("========================================\n\n");
    
    // Contar pistas que apontam para o acusado
    contarPistasPorSuspeito(arvorePistas, hash, acusado, &pistasEncontradas);
    
    printf("Pistas encontradas apontando para %s: %d\n\n", acusado, pistasEncontradas);
    
    // Verificar se há evidências suficientes (mínimo 2 pistas)
    if(pistasEncontradas >= 2) {
        printf("========================================\n");
        printf("       CASO RESOLVIDO!\n");
        printf("========================================\n");
        printf("Parabens, detetive!\n");
        printf("Voce reuniu evidencias suficientes contra %s.\n", acusado);
        printf("O culpado foi preso e a justica prevaleceu!\n");
        printf("========================================\n");
    } else if(pistasEncontradas == 1) {
        printf("========================================\n");
        printf("       EVIDENCIAS INSUFICIENTES\n");
        printf("========================================\n");
        printf("Voce encontrou apenas 1 pista contra %s.\n", acusado);
        printf("Sao necessarias pelo menos 2 pistas para uma acusacao.\n");
        printf("O suspeito foi liberado por falta de provas...\n");
        printf("========================================\n");
    } else {
        printf("========================================\n");
        printf("       ACUSACAO INCORRETA\n");
        printf("========================================\n");
        printf("Nenhuma pista aponta para %s.\n", acusado);
        printf("O verdadeiro culpado escapou!\n");
        printf("Tente novamente explorando mais comodos.\n");
        printf("========================================\n");
    }
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
