    #include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// --------------------------------------------------
// Estrutura da peça: tipo e identificador
// --------------------------------------------------
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // ID único da peça
} Peca;

// --------------------------------------------------
// Estrutura da fila circular
// --------------------------------------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio, fim, quantidade;
} Fila;

// --------------------------------------------------
// Estrutura da pilha
// --------------------------------------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ID global para criação de peças
int contadorID = 0;

// --------------------------------------------------
// Gera peças automaticamente
// --------------------------------------------------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

// --------------------------------------------------
// Funções da fila circular
// --------------------------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

int enqueue(Fila *f, Peca p) {
    if (f->quantidade == TAM_FILA) return 0;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
    return 1;
}

int dequeue(Fila *f, Peca *removida) {
    if (f->quantidade == 0) return 0;
    *removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return 1;
}

// --------------------------------------------------
// Funções da pilha
// --------------------------------------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int push(Pilha *p, Peca valor) {
    if (p->topo == TAM_PILHA - 1) return 0;
    p->itens[++p->topo] = valor;
    return 1;
}

int pop(Pilha *p, Peca *removida) {
    if (p->topo == -1) return 0;
    *removida = p->itens[p->topo--];
    return 1;
}

// --------------------------------------------------
// Exibe fila e pilha visualmente
// --------------------------------------------------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n============ ESTADO ATUAL ============\n");

    // Mostrar fila
    printf("Fila de peças: ");
    if (f->quantidade == 0) {
        printf("[vazia]");
    } else {
        int i = f->inicio;
        for (int c = 0; c < f->quantidade; c++) {
            printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
            i = (i + 1) % TAM_FILA;
        }
    }

    // Mostrar pilha
    printf("\nPilha de reserva (Topo -> Base): ");
    if (p->topo == -1) {
        printf("[vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }

    printf("\n=======================================\n");
}

// --------------------------------------------------
// Troca a peça da frente da fila com o topo da pilha
// --------------------------------------------------
void trocaSimples(Fila *f, Pilha *p) {
    if (p->topo == -1) {
        printf("Não é possível trocar: pilha vazia.\n");
        return;
    }

    Peca temp = p->itens[p->topo];
    p->itens[p->topo] = f->itens[f->inicio];
    f->itens[f->inicio] = temp;

    printf("Troca simples realizada.\n");
}

// --------------------------------------------------
// Troca múltipla (3 da fila ↔ 3 da pilha)
// --------------------------------------------------
void trocaMultipla(Fila *f, Pilha *p) {
    if (p->topo < 2) {
        printf("Troca múltipla impossível: pilha não tem 3 peças.\n");
        return;
    }

    if (f->quantidade < 3) {
        printf("Troca múltipla impossível: fila não tem 3 peças.\n");
        return;
    }

    int posFila = f->inicio;

    for (int i = 0; i < 3; i++) {
        Peca temp = f->itens[posFila];
        f->itens[posFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;

        posFila = (posFila + 1) % TAM_FILA;
    }

    printf("Troca múltipla realizada entre fila e pilha.\n");
}

// --------------------------------------------------
// Função principal
// --------------------------------------------------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenchimento inicial da fila
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca jogada;
            if (dequeue(&fila, &jogada)) {
                printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca());
            }
        }

        else if (opcao == 2) {
            if (pilha.topo == TAM_PILHA - 1) {
                printf("A pilha está cheia! Não é possível reservar.\n");
            } else {
                Peca reservada;
                dequeue(&fila, &reservada);
                push(&pilha, reservada);
                printf("Peça enviada para reserva: [%c %d]\n", reservada.nome, reservada.id);
                enqueue(&fila, gerarPeca());
            }
        }

        else if (opcao == 3) {
            Peca usada;
            if (pop(&pilha, &usada)) {
                printf("Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
            } else {
                printf("A pilha está vazia!\n");
            }
        }

        else if (opcao == 4) {
            trocaSimples(&fila, &pilha);
        }

        else if (opcao == 5) {
            trocaMultipla(&fila, &pilha);
        }

    } while (opcao != 0);

    printf("\nEncerrando o programa...\n");
    return 0;
}



    return 0;
}

