#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LIMITE_INFERIOR -1000
#define LIMITE_SUPERIOR  1000
#define QTD_INSERCOES 100

typedef struct _nodo{
    int chave;
    int altura, nivel;
    int altdireita, altesquerda;
    struct _nodo * esq;
    struct _nodo * dir;
    struct _nodo * pai;
} TpNodo;

typedef struct _arvore{
    TpNodo * raiz;
} TpArvore;

TpArvore * inicializa();
void imprime(TpArvore * arvore);
TpArvore * insere(TpArvore * arvore, int chave);
int checarBalanceamento(TpArvore * arvore);
void freeArvore(TpArvore * arvore);
TpArvore * inserirAutomaticamente(TpArvore * arvore);

int main(){
    TpArvore * arvore = (TpArvore*)malloc(sizeof(TpArvore));
    arvore = inicializa();

    int c;
    int opcao;

    srand((unsigned)time(NULL));

    do{
        printf("1 - Iniserir um elemento\n");
        printf("2 - Listar os elementos inseridos\n");
        printf("3 - Realizar 100 inserções automaticamente\n");
        printf("4 - Testar se está balanceado\n");
        printf("0 - Finalizar o programa\n");

        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                printf("chave: ");
                scanf("%d", &c);
                arvore = insere(arvore, c);
                break;
            case 2:
                printf("\n");
                imprime(arvore);
                printf("\n");
                break;
            case 3:
                arvore = inserirAutomaticamente(arvore);
                break;
            case 4:
                if(checarBalanceamento(arvore)) printf("\nbalanceado!\n\n");
                else printf("\nNão está balanceado!!\n\n");
                break;
            case 0:
                printf("saindo...\n");
                break;
            default:
                printf("\nEscolha uma opção válida!\n\n");
        }
    }while(opcao != 0);

    freeArvore(arvore);
    //a memória foi liberada antes do fim do programa apenas por
    //boa prática, já que o sistema operacional já faz isso

    return 0;
}

TpArvore * inicializa(){
    //aloca memoria para inicializar a arvore
    TpArvore *arvore = (TpArvore*)malloc(sizeof(TpArvore));
    arvore->raiz = NULL;
    return arvore;
}

/*-----função auxiliar para a função insere----*/
int max(int a, int b){
    return a > b ? a : b;
}

void incrementaNivel(TpNodo * nodo){
    //incrementa o nivel do nodo e seus descendentes
    if(nodo == NULL) return;
    nodo->nivel++;
    incrementaNivel(nodo->dir);
    incrementaNivel(nodo->esq);
}

void decrementaNivel(TpNodo * nodo){
    //decrementa o nivel do nodo e seus descendentes
    if(nodo == NULL) return;
    nodo->nivel--;
    decrementaNivel(nodo->dir);
    decrementaNivel(nodo->esq);
}

TpNodo * rightRight(TpNodo * nodo){
    TpNodo * a, * b;

    a = nodo;
    b = nodo->dir;

    b->nivel--;
    a->nivel++;
    incrementaNivel(a->esq);
    decrementaNivel(b->dir);

    b->pai = a->pai;
    a->dir = b->esq;
    if(a->dir != NULL) a->dir->pai = a;
    a->pai = b;
    b->esq = a;

    a->altdireita = a->dir == NULL ? 0 : a->dir->altura + 1;
    a->altura = max(a->altdireita, a->altesquerda);
    b->altesquerda = a->altura + 1;
    b->altdireita = b->dir == NULL ? 0 : b->dir->altura + 1;
    b->altura = max(b->altdireita, b->altesquerda);

    nodo = b;

    return nodo;
}

TpNodo * leftLeft(TpNodo * nodo){
    TpNodo * a, * b;

    a = nodo;
    b = nodo->esq;

    b->nivel--;
    a->nivel++;
    incrementaNivel(a->dir);
    decrementaNivel(b->esq);

    b->pai = a->pai;
    a->esq = b->dir;
    if(a->esq != NULL) a->esq->pai = a;
    a->pai = b;
    b->dir = a;

    a->altesquerda = a->esq == NULL ? 0 : a->esq->altura + 1;
    a->altura = max(a->altdireita, a->altesquerda);
    b->altdireita = a->altura + 1;
    b->altesquerda = b->esq == NULL ? 0 : b->esq->altura + 1;
    b->altura = max(b->altdireita, b->altesquerda);

    nodo = b;
    return nodo;
}

TpNodo * rightLeft(TpNodo * nodo){
    TpNodo * a, * b;

    a = nodo;
    b = nodo->dir;

    b = leftLeft(b);
    a->dir = b;
    a->altdireita = b->altura + 1;
    a->altura = max(a->altdireita, a->altesquerda);
    a = rightRight(a);

    nodo = b;
    return nodo;
}

TpNodo * leftRight(TpNodo * nodo){
    TpNodo * a, * b;

    a = nodo;
    b = nodo->esq;

    b = rightRight(b);
    a->esq = b;
    a->altesquerda = b->altura + 1;
    a->altura = max(a->altdireita, a->altesquerda);
    a = leftLeft(a);

    nodo = b;
    return nodo;
}

TpNodo * balancearNodo(TpNodo * nodo){
    int bal = nodo->altdireita - nodo->altesquerda;
    int dbal = nodo->dir == NULL ? 0 : 
        nodo->dir->altdireita - nodo->dir->altesquerda;
    int ebal = nodo->esq == NULL ? 0 :
        nodo->esq->altdireita - nodo->esq->altesquerda;

    if(bal > 1 && dbal >= 1) return rightRight(nodo);
    if(bal > 1 && dbal <= -1) return rightLeft(nodo);
    if(bal < -1 && ebal <= -1) return leftLeft(nodo);
    if(bal < -1 && ebal >= 1) return leftRight(nodo);

    return nodo;
}

/*-----função auxiliar para a função insere----*/
TpNodo * _insere(TpNodo * pai, TpNodo * nodo){
    nodo->pai = pai;
    nodo->esq = nodo->dir = NULL;
    nodo->altura = nodo->altesquerda = nodo->altdireita = 0;

    if(pai == NULL) return nodo;

    if(nodo->chave == pai->chave){
        free(nodo);
        return pai;
    }

    if(pai->dir == NULL && nodo->chave >= pai->chave){
        //inserir nodo diretamente à direita do nodo "pai"
        pai->dir = nodo;
        nodo->nivel = pai->nivel + 1;
        pai->altdireita = 1;
        pai->altura = max(pai->altdireita, pai->altesquerda);
    }
    else if(pai->esq == NULL && nodo->chave < pai->chave){
        //inserir nodo diretamente à esquerda do nodo "pai"
        pai->esq = nodo;
        nodo->nivel = pai->nivel + 1;
        pai->altesquerda = 1;
        pai->altura = max(pai->altdireita, pai->altesquerda);
    }
    else if(pai->dir != NULL && nodo->chave >= pai->chave){
        //inserir nodo no filho da direita do nodo "pai"
        pai->dir = _insere(pai->dir, nodo);
        pai->altdireita = pai->dir->altura + 1;
        pai->altura = max(pai->altdireita, pai->altesquerda);
    }
    else if(pai->esq != NULL && nodo->chave < pai->chave){
        //inserir nodo no filho da esquerda do nodo "pai"
        pai->esq = _insere(pai->esq, nodo);
        pai->altesquerda = pai->esq->altura + 1;
        pai->altura = max(pai->altdireita, pai->altesquerda);
    }

    pai = balancearNodo(pai);

    return pai;
}

TpArvore * insere(TpArvore * arvore, int chave){
    TpNodo * nodo = (TpNodo*)malloc(sizeof(TpNodo));
    nodo->chave = chave;
    arvore->raiz = _insere(arvore->raiz, nodo);
    return arvore;
}

/*----função auxiliar para a função imprime-----*/
void _imprime(TpNodo * nodo){
    if(nodo == NULL) return;
    _imprime(nodo->dir);
    for(int i = 0; i < nodo->nivel; i++)printf("    ");
    printf("%d a:%d n:%d ae:%d ad:%d\n", nodo->chave, nodo->altura, 
            nodo->nivel, nodo->altesquerda, nodo->altdireita);
    _imprime(nodo->esq);
}

void imprime(TpArvore * arvore){
    _imprime(arvore->raiz);
}

/*-----função auxiliar para a função checarBalanceamento----*/
int _checarBalanceamento(TpNodo * nodo){
    if(nodo == NULL) return 1;
    if(abs(nodo->altdireita - nodo->altesquerda) > 1) return 0;
    return _checarBalanceamento(nodo->dir) && _checarBalanceamento(nodo->esq);
}

int checarBalanceamento(TpArvore * arvore){
    //checa se a árvore está balanceada
    return _checarBalanceamento(arvore->raiz);
}

/*-----função auxiliar para a função freeArvore----*/
void _freeArovre(TpNodo * nodo){
    if(nodo == NULL) return;
    _freeArovre(nodo->dir);
    _freeArovre(nodo->esq);
    free(nodo);
}

void freeArvore(TpArvore * arvore){
    _freeArovre(arvore->raiz);
}

TpArvore * inserirAutomaticamente(TpArvore * arvore){
    for(int i = 0; i < QTD_INSERCOES; i++){
        arvore = insere(arvore, 
                rand() % (LIMITE_SUPERIOR - LIMITE_INFERIOR) + LIMITE_INFERIOR);
    }
    return arvore;
}
