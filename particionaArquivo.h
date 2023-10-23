#ifndef __PARTICIONA_ARQUIVO_H__
#define __PARTICIONA_ARQUIVO_H__

#include <stdio.h>
#include <stdlib.h>
#include "constantes.h"

size_t retorna_parte(FILE *arquivo, int parte, unsigned char *buffer);

FILE *abre_arquivo_leitura(const char *nome_arquivo);

FILE *abre_arquivo_escrita(const char *nome_arquivo);

#endif