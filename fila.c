#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

fila_t *fila_cria()
{
    fila_t *f;
    if((f=malloc(sizeof(fila_t))))
    {
        f->ini=NULL;
        f->fim=NULL;
        f->tamanho=-1;
        return f;
    }
    return NULL;
}

fila_t* fila_destroi (fila_t* f)
{
    nodo_f_t *tmp;
    int i;
    tmp=f->ini;
    /* Se a fila existe e esta vazia, apenas libera a fila */
    if(fila_vazia(f))
        free(f);
    
    else
    {
        /* Caso contrario percorre o laco inteiro, liberando nodo por nodo, para depois liberar a lista */
        for(i=0;i<=(f->tamanho);i++)
        {
            f->ini->chave=0;
            tmp=tmp->prox;
            free(f->ini);
            f->ini=tmp;
        }
        free(f->ini);
        f->tamanho=-1;
        free(f);
    }
    return NULL;
}

int fila_vazia (fila_t* f)
{
    if((f->ini==NULL)&&(f->fim==NULL))
            return 1;
    return 0;
}

int fila_tamanho (fila_t* f)
{
    return (f->tamanho+1);
}

int queue(fila_t *f, int elemento)
{
    nodo_f_t *nodo;
    if((nodo=malloc(sizeof(nodo_f_t))))
    {
        nodo->chave=elemento;
        nodo->prox=NULL;
        /* Caso fila esteja vazia, insere o nodo criado na fila, tendo inicio e fim na mesma posicao */
        if(fila_vazia(f)){
            f->ini=nodo;
            f->fim=nodo;
        }
        /* Insere elemento no fim da fila */
        else{
            f->fim->prox=nodo;
            f->fim=nodo;
        }
        (f->tamanho)++;
        return 1;
    }
    return 0;
}

int dequeue (fila_t* f, int* elemento)
{
    nodo_f_t *tmp;
    /* Retorna 0 caso fila vazia */
    if(fila_vazia(f))
        return 0;
    else
    {
        /* Se a fila so possui um elemento, retorna o mesmo */
        if(f->tamanho==0)
        {
            *elemento=f->ini->chave;
            f->ini->chave=0;
            f->ini=NULL;
            f->fim=NULL;
            (f->tamanho)--;
            free(f->ini);
        }
        /* Se a fila possui mais de um elemento, retorna o primeiro, segundo politica FIFO */
        else
        {
            *elemento=f->ini->chave;
            tmp=f->ini;
            f->ini=f->ini->prox;
            tmp->chave=0;
            free(tmp);
            (f->tamanho)--;
        }
        return 1;
    }
    return 0;
}

void fila_imprime (fila_t* f)
{
    int i;
    nodo_f_t *tmp;
    if(!(fila_vazia(f)))
    {
        tmp=f->ini;
        printf("Imprimindo a fila: ");
        /* Percorre a fila de acordo com o tamanho fornecido, imprimindo cada elemento */
        for(i=0;i<=(f->tamanho);i++)
        {
            printf("%d ",tmp->chave);
            tmp=tmp->prox;
        }
        printf("\n");
    }
}
