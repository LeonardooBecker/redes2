#include <stdio.h>
#include <stdlib.h>
#include "libLista.h"

lista_t *lista_cria()
{
    lista_t *l;

    if ((l = malloc(sizeof(lista_t))))
    {
        l->ini = NULL;
        l->tamanho = -1;
        return l;
    }
    return NULL;
}

lista_t *lista_destroi(lista_t *l)
{
    nodo_l_t *tmp;

    if (lista_vazia(l))
        free(l);
    else
    {
        /* Percorre a lista inteira a partir do inicio, liberando nodo por nodo */
        while (l->ini->prox != NULL)
        {
            tmp = l->ini;
            l->ini = tmp->prox;
            tmp->elemento = 0;
            free(tmp);
        }
        l->ini->elemento = 0;
        l->tamanho = -1;
        free(l->ini);
        free(l);
    }
    return NULL;
}

int lista_vazia(lista_t *l)
{
    if ((l->ini == NULL) && (l->tamanho == -1))
    {
        return 1;
    }
    return 0;
}

int lista_tamanho(lista_t *l)
{
    return (l->tamanho) + 1;
}

int lista_insere_inicio(lista_t *l, int elemento)
{

    nodo_l_t *nodo;

    if ((nodo = malloc(sizeof(nodo_l_t))))
    {
        nodo->elemento = elemento;
        nodo->prox = NULL;
        if (lista_vazia(l))
            l->ini = nodo;
        else
        {
            nodo->prox = l->ini;
            l->ini = nodo;
        }
        (l->tamanho)++;
        return 1;
    }
    return 0;
}

int lista_insere_fim(lista_t *l, int elemento)
{
    nodo_l_t *nodo;
    nodo_l_t *tmp;
    int i;
    if ((nodo = malloc(sizeof(nodo_l_t))))
    {
        tmp = l->ini;
        nodo->elemento = elemento;
        nodo->prox = NULL;
        if (lista_vazia(l))
            l->ini = nodo;
        else
        {
            for (i = 0; i < (l->tamanho); i++)
                tmp = tmp->prox;
            tmp->prox = nodo;
        }
        (l->tamanho)++;
        printf("Inseri cliente %d na lista\n", elemento);
        return 1;
    }
    return 0;
}

int lista_insere_ordenado(lista_t *l, int elemento)
{
    nodo_l_t *nodo;
    nodo_l_t *tmp;
    int i = 0;

    if ((nodo = malloc(sizeof(nodo_l_t))))
    {
        tmp = l->ini;
        nodo->elemento = elemento;
        nodo->prox = NULL;

        /* Se lista vazia, insere o nodo no inicio da fila */
        if (lista_vazia(l))
            l->ini = nodo;

        /* Lista de um unico elemento */
        else if (l->tamanho == 0)
        {
            /* Se o elemento a ser inserido for menor, insere no inicio */
            if (elemento < (l->ini->elemento))
            {
                nodo->prox = l->ini;
                l->ini = nodo;
            }
            /* Se não insere na proxima posicao em relacao ao inicio */
            else
            {
                l->ini->prox = nodo;
            }
        }
        /* Lista de dois ou mais elementos */
        else
        {
            /* Laco que retorna a posicao que contenha um elemento maior que o elemento atual */
            for (i = 0; i < (l->tamanho); i++)
            {
                if (elemento < tmp->prox->elemento)
                    break;
                tmp = tmp->prox;
            }
            /* Elemento já presente na lista encontrado na primeira posicao */
            if (i == 0)
            {
                /* Se elemento a ser inserido for menor que o já presente na lista, insere o mesmo no inicio */
                if (elemento < tmp->elemento)
                {
                    l->ini = nodo;
                    nodo->prox = tmp;
                }
                /* Caso contrario insere na posicao seguinte ao inicio */
                else
                {
                    nodo->prox = tmp->prox;
                    tmp->prox = nodo;
                }
            }
            /* Elemento já presente na lista encontrado na ultima posicao, portanto eh inserido no fim */
            else if (i == (l->tamanho))
                tmp->prox = nodo;
            /* Elemento encontrado no meio da lista */
            else
            {
                nodo->prox = tmp->prox;
                tmp->prox = nodo;
            }
        }
        (l->tamanho)++;
        return 1;
    }
    return 0;
}

int lista_retira_inicio(lista_t *l, int *elemento)
{
    nodo_l_t *tmp;
    /* Se a lista estiver vazia, retorna 0 */
    if (lista_vazia(l))
        return 0;
    /* Se a lista possuir 1 elemento, retorna este elemento e libera o espaco ocupado pelo mesmo */
    else if (l->tamanho == 0)
    {
        *elemento = l->ini->elemento;
        l->ini->elemento = 0;
        l->ini = NULL;
        free(l->ini);
        (l->tamanho)--;
        return 1;
    }
    /* Se a lista possuir 2 ou mais elementos, remove o elemento inicial e libera o espaco ocupado pelo mesmo */
    else
    {
        *elemento = l->ini->elemento;
        tmp = l->ini->prox;
        l->ini->elemento = 0;
        free(l->ini);
        l->ini = tmp;
        (l->tamanho)--;
        return 1;
    }
}

int lista_retira_fim(lista_t *l, int *elemento)
{
    nodo_l_t *tmp;
    int i;

    tmp = l->ini;
    /* Se a lista estiver vazia, retorna 0 */
    if (lista_vazia(l))
        return 0;
    /* Se a lista possuir 1 elemento, retorna este elemento e libera o espaco ocupado pelo mesmo */
    else if (l->tamanho == 0)
    {
        *elemento = l->ini->elemento;
        l->ini->elemento = 0;
        l->ini = NULL;
        free(l->ini);
        (l->tamanho)--;
        return 1;
    }
    /* Se a lista possuir 2 ou mais elementos, percorre a lista buscando o ultimo elemento para assim remove-lo */
    else
    {
        for (i = 0; i < (l->tamanho) - 1; i++)
            tmp = tmp->prox;
        *elemento = tmp->prox->elemento;
        tmp->prox->elemento = 0;
        tmp->prox = NULL;
        free(tmp->prox);
        (l->tamanho)--;
        return 1;
    }
}

int lista_retira_elemento(lista_t *l, int *elemento)
{
    nodo_l_t *tmp;
    nodo_l_t *auxmeio;
    int posicao = 0;

    tmp = l->ini;
    /* lista vazia */
    if (lista_vazia(l))
    {
        return 0;
    }
    /* lista de um elemento */
    else if (l->tamanho == 0)
    {
        /* remove o único elemento */
        if ((tmp->elemento == *elemento))
        {
            l->ini->elemento = 0;
            l->ini = NULL;
            free(l->ini);
            (l->tamanho)--;
            printf("Removi cliente = %d da posicao 0 da Lista\n", *elemento);
            return 1;
        }
        /* elemento inexistente */
        return 0;
    }
    /* lista de dois elementos ou mais */
    else
    {
        /* elemento na primeira posição */
        if (tmp->elemento == *elemento)
        {
            l->ini = tmp->prox;
            tmp->elemento = 0;
            free(tmp);
            (l->tamanho)--;
            printf("Removi cliente = %d da posicao 0 da Lista\n", *elemento);
            return 1;
        }
        else
        {
            while (tmp->prox->elemento != *elemento)
            {
                tmp = tmp->prox;
                posicao++;
                /* elemento inexistente */
                if ((posicao == (l->tamanho)) && (tmp->elemento != *elemento))
                    return 0;
            }
            /* caso elemento esteja na ultima posicao */
            if (posicao == ((l->tamanho) - 1))
            {
                tmp->prox->elemento = 0;
                free(tmp->prox);
                tmp->prox = NULL;
                (l->tamanho)--;
                printf("Removi cliente = %d da posicao %d da Lista\n", *elemento, posicao);
                return 1;
            }
            /* elemento se encontra entre a primeira e a última posição */
            else
            {
                auxmeio = tmp->prox;
                tmp->prox = auxmeio->prox;
                auxmeio->elemento = 0;
                auxmeio = NULL;
                free(auxmeio);
                (l->tamanho)--;
                printf("Removi cliente = %d da posicao %d da Lista\n", *elemento, posicao);
                return 1;
            }
            return 0;
        }
    }
}

int lista_pertence(lista_t *l, int elemento)
{
    nodo_l_t *tmp;

    tmp = l->ini;
    if (lista_vazia(l))
        return 0;
    while (tmp->prox != NULL)
    {
        if (tmp->elemento == elemento)
            return 1;
        tmp = tmp->prox;
    }
    if (tmp->elemento == elemento)
        return 1;
    return 0;
}

void lista_imprime(lista_t *l)
{
    int i;
    nodo_l_t *tmp;
    tmp = l->ini;
    if (lista_vazia(l))
        printf("Lista vazia\n");
    else
    {
        printf("Veja a Lista: [ ");
        for (i = 0; i <= (l->tamanho); i++)
        {
            printf("%d ", tmp->elemento);
            tmp = tmp->prox;
        }
        printf("]\n");
    }
}