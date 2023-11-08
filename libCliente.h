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


// Função responsável por redefinir o tempo de timeout do cliente
void resetaTimeoutCliente(struct timeval *timeout, fd_set *readfds, int s);

// Função responsável por receber os dados enviados pelo servidor, e escrever no arquivo de retorno assim como registrar os dados adicionais
void recebeDados(int sock, FILE *arq, desempenho_t *desempenho);

// Função responsável por inicializar a estrutura de desempenho
desempenho_t *inicializa_desempenho();

// Função responsável por imprimir os dados de desempenho gerados pelo cliente
void imprime_log(desempenho_t *desempenho);

#endif