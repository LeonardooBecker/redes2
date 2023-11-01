#ifndef __PARTICIONA_ARQUIVO_H__
#define __PARTICIONA_ARQUIVO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constantes.h"

size_t retorna_parte(FILE *arquivo, int parte, unsigned char *buffer);

size_t novoRetornaParte(clientes_t cliente, datagramaUDP *pacote);

FILE *abre_arquivo_leitura(const char *nome_arquivo);

FILE *abre_arquivo_escrita(const char *nome_arquivo);

#endif