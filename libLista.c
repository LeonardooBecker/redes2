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
            free(tmp);
        }
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

int lista_insere_fim(lista_t *l, clientes_t *cliente)
{
    nodo_l_t *nodo;
    nodo_l_t *tmp;
    int i;
    if (lista_tamanho(l) > TAMANHO_LISTA)
        printf("Lista cheia\n");
    else
    {
        if ((nodo = malloc(sizeof(nodo_l_t))))
        {
            tmp = l->ini;
            nodo->cliente = cliente;
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
            printf("Inseri cliente de host %d na lista\n", cliente->host);
            return 1;
        }
    }
    return 0;
}

int lista_retira_cliente(lista_t *l, clientes_t *cliente)
{
    nodo_l_t *tmp;
    nodo_l_t *auxmeio;
    int posicao = 0;

    tmp = l->ini;
    /* lista vazia */
    if (lista_vazia(l))
    {
        printf("ERRO: Impossível remover, Lista Vazia!\n");
        return 0;
    }
    /* lista de um cliente */
    else if (l->tamanho == 0)
    {
        /* remove o único cliente */
        if ((tmp->cliente == cliente))
        {
            l->ini = NULL;
            free(l->ini);
            (l->tamanho)--;
            printf("Removi cliente de host = %d da posicao 0 da Lista\n", cliente->host);
            return 1;
        }
        /* cliente inexistente */
        return 0;
    }
    /* lista de dois clientes ou mais */
    else
    {
        /* cliente na primeira posição */
        if (tmp->cliente == cliente)
        {
            l->ini = tmp->prox;
            free(tmp);
            (l->tamanho)--;
            printf("Removi cliente  de host = %d da posicao 0 da Lista\n", cliente->host);
            return 1;
        }
        else
        {
            while (tmp->prox->cliente != cliente)
            {
                tmp = tmp->prox;
                posicao++;
                /* cliente inexistente */
                if ((posicao == (l->tamanho)) && (tmp->cliente != cliente))
                    return 0;
            }
            /* caso cliente esteja na ultima posicao */
            if (posicao == ((l->tamanho) - 1))
            {
                free(tmp->prox);
                tmp->prox = NULL;
                (l->tamanho)--;
                printf("Removi cliente  de host = %d da posicao %d da Lista\n", cliente->host, posicao);
                return 1;
            }
            /* cliente se encontra entre a primeira e a última posição */
            else
            {
                auxmeio = tmp->prox;
                tmp->prox = auxmeio->prox;
                auxmeio = NULL;
                free(auxmeio);
                (l->tamanho)--;
                printf("Removi cliente  de host = %d da posicao %d da Lista\n", cliente->host, posicao);
                return 1;
            }
            return 0;
        }
    }
}

void lista_imprime(lista_t *l)
{
    int i;
    nodo_l_t *tmp;
    tmp = l->ini;
    if (!lista_vazia(l))
    {
        printf("Veja a Lista: [ ");
        for (i = 0; i <= (l->tamanho); i++)
        {
            printf("%d ", tmp->cliente->host);
            tmp = tmp->prox;
        }
        printf("]\n");
    }
}

nodo_l_t *percorre_lista(lista_t *lista, nodo_l_t *nodo)
{
    while (nodo != NULL)
    {
        return nodo->prox;
    }
    return NULL;
}

nodo_l_t *lista_retorna_inicio(lista_t *lista)
{
    if (lista_vazia(lista))
    {
        return 0;
    }
    else
        return lista->ini;
}