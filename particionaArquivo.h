#ifndef __PARTICIONA_ARQUIVO_H__
#define __PARTICIONA_ARQUIVO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constantes.h"

// Função que retorna o pacote a ser enviado ao cliente
size_t retornaFragmento(clientes_t *cliente, datagramaUDP *pacote);

// Função responsável por abrir o arquivo para leitura
FILE *abre_arquivo_leitura(const char *nome_arquivo);

// Função responsável por abrir o arquivo para escrita
FILE *abre_arquivo_escrita(const char *nome_arquivo);

#endif