#ifndef __CONSTANTES_H__
#define __CONSTANTES_H__

#define BUFFER_SIZE 1024
#define MAX_PARTS 100000
// 1k * 100k = 100M <- Tamanho máximo do arquivo


typedef struct datagrama{
    unsigned int porta_origem : 16;
    unsigned int porta_destino : 16;
    unsigned int tamanho : 16;
    unsigned int sequencia : 16;
    unsigned char dados[BUFFER_SIZE];
} datagramaUDP;

#endif