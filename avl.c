#include <stdio.h>
#include <stdlib.h>

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
TpNodo * rightRight(TpNodo * rightRight);

int main(){
    TpArvore * arvore = (TpArvore*)malloc(sizeof(TpArvore));
    arvore = inicializa();

    int c;

    while(scanf("%d", &c), c >= 0){
        arvore = insere(arvore, c);
        imprime(arvore);
    }

    arvore->raiz = rightRight(arvore->raiz);
    imprime(arvore);

    return 0;
}

TpArvore * inicializa(){//aloca memoria para inicializar a arvore
    TpArvore *arvore = (TpArvore*)malloc(sizeof(TpArvore));
    arvore->raiz = NULL;
    return arvore;
}

/*-----função auxiliar para a função insere----*/
int max(int a, int b){
    return a > b ? a : b;
}

TpNodo * rightRight(TpNodo * nodo){
    TpNodo * a, * b;

    a = nodo;
    b = nodo->dir;

    b->nivel--;
    a->nivel++;
    if(a->esq != NULL) a->esq->nivel++;
    b->dir->nivel--;

    b->pai = a->pai;
    a->dir = b->esq;
    if(a->dir != NULL) a->dir->pai = a;
    a->pai = b;
    b->esq = a;

    nodo = b;

    return nodo;
}

TpNodo * leftLeft(TpNodo * nodo){
    TpNodo * a, * b;

    a = nodo;
    b = nodo->esq;

    b->nivel--;
    a->nivel++;
    if(a->dir != NULL) a->dir->nivel++;
    b->esq->nivel--;

    b->pai = a->pai;
    a->esq = b->dir;
    if(a->esq != NULL) a->esq->pai = a;
    a->esq->pai = a;
    a->pai = b;
    b->dir = a;

    nodo = b;
    return nodo;
}

/*
TpNodo * rightLeft(TpNodo * nodo){
    TpNodo * a, * b;

    a = nodo;
    b = nodo->dir;

    b = leftLeft(b);
    a = rightRight(a);

    nodo = b;
    return nodo;
}

TpNodo * leftRight(TpNodo * nodo){
    TpNodo * a, * b;

    a = nodo;
    b = nodo->esq;

    b = rightRight(b);
    a = leftLeft(a);

    nodo = b;
    return nodo;
}
*/

/*-----função auxiliar para a função insere----*/
TpNodo * _insere(TpNodo * pai, TpNodo * nodo){
    nodo->pai = pai;
    nodo->esq = nodo->dir = NULL;
    nodo->altura = nodo->altesquerda = nodo->altdireita = 0;

    if(pai == NULL) return nodo;

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

    if(abs(pai->altesquerda - pai->altdireita) > 1){
        //se o nodo "pai" não estiver balanceado

    }

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
    for(int i = 0; i < nodo->nivel; i++)printf("    ");
    printf("%d a:%d n:%d\n", nodo->chave, nodo->altura, nodo->nivel);
    _imprime(nodo->esq);
    _imprime(nodo->dir);
}

void imprime(TpArvore * arvore){
    _imprime(arvore->raiz);
}
