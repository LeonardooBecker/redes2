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

void buscaConexao(clientes_t *clientes, int *id_cliente, int socket);

void enviaPacotes(clientes_t *clientes, int id_cliente, int s);

void resetaTimeoutServidor(struct timeval *timeout, fd_set *readfds, int s);

#endif