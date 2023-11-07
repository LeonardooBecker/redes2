#ifndef __LIB_CLIENTE_H__
#define __LIB_CLIENTE_H__

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

void resetaTimeoutCliente(struct timeval *timeout, fd_set *readfds, int s);

void recebeDados(int sock, FILE *arq, desempenho_t *desempenho);

desempenho_t *inicializa_desempenho();

void imprime_log(desempenho_t *desempenho);

#endif