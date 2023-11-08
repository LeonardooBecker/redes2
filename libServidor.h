#ifndef __LIB_SERVIDOR_H__
#define __LIB_SERVIDOR_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "constantes.h"
#include "particionaArquivo.h"
#include "libLista.h"

// Função responsável por inicializar a estrutura de clientes
clientes_t *novo_cliente();

// Função responsável pela busca de novos clientes por parte do servidor
void buscaConexao(int socket, lista_t *lista);

// Função responsável por enviar os pacotes para os clientes
void enviaPacotes(int socket, lista_t *lista);

// Função responsável por resetar o timeout do servidor
void resetaTimeoutServidor(struct timeval *timeout, fd_set *readfds, int s, int intervalo_pacotes);

// Função responsável por conferir a inatividade do servidor
void confereInatividade(lista_t *lista, int *qnt_tentativas, int encerra_servidor);

#endif