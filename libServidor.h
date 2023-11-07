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

clientes_t *novo_cliente();

void buscaConexao(int socket, lista_t *lista);

void enviaPacotes(int socket, lista_t *lista);

void resetaTimeoutServidor(struct timeval *timeout, fd_set *readfds, int s, int intervalo_pacotes);

void confereInatividade(lista_t *lista, int *qnt_tentativas, int encerra_servidor);

#endif