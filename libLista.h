#ifndef _LIBlista_t_H
#define _LIBlista_t_H
#include "constantes.h"

struct nodo_l
{
    clientes_t *cliente; /* lista composta por elementos do tipo clientes_t */
    struct nodo_l *prox; /* ponteiro para o proximo   */
};
typedef struct nodo_l nodo_l_t;

struct lista
{
    nodo_l_t *ini; /* ponteiro para o inicio da lista */
    int tamanho;   /* numero de elementos na lista    */
};
typedef struct lista lista_t;

/*
 * Cria uma lista vazia e a retorna, se falhar retorna NULL.
 */
lista_t *lista_cria();

/*
 * Remove todos os elementos da lista, libera espaco e retorna NULL.
 */
lista_t *lista_destroi(lista_t *l);

/*
 * Retorna 1 se a lista esta vazia e 0 caso contrario.
 */
int lista_vazia(lista_t *l);

/*
 * Retorna o tamanho da lista, isto eh, o numero de elementos presentes nela.
 */
int lista_tamanho(lista_t *l);

/*
 * Insere o elemento no final da lista.
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int lista_insere_fim(lista_t *l, clientes_t *cliente);

/*
 * Remove o elemento 'elemento' caso ele exista na lista.
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 * Se o elemento nao for encontrado na lista tambem retorna 0.
 */
int lista_retira_cliente(lista_t *l, clientes_t *cliente);

/*
 * Imprime a lista, do inicio ate o fim, este por ultimo, sem espaco no final.
 * Quando a lista estiver vazia, nao imprime nada, nem um \n.
 * Esta funcao eh somente para facilitar teus testes.
 * Normalmente ela nao existe nas implementacoes de um TAD lista.
 */
void lista_imprime(lista_t *l);

/*
 * Retorna o endereco do proximo nodo caso ele exista na lista.
 * Caso contrario, retorna NULL.
*/
nodo_l_t *percorre_lista(lista_t *lista, nodo_l_t *nodo);

/*
* Retorna o endereço do primeiro nodo pertencente a lista
*/
nodo_l_t *lista_retorna_inicio(lista_t *lista);

#endif