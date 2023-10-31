#ifndef __CONSTANTES_H__
#define __CONSTANTES_H__

#define BUFFER_SIZE 2048
#define MAX_PARTS 100000
#define MAX_CLIENTES 100

#define INTERVALO_TEMPO 1
// 1k * 100k = 100M <- Tamanho máximo do arquivo

typedef struct datagrama
{
    unsigned int tamanho : 16;
    unsigned int sequencia : 16;
    unsigned char dados[BUFFER_SIZE];
} datagramaUDP;

typedef struct clientes
{
    char stream_cliente[BUFFER_SIZE];
    unsigned int parte;
    int host;
    int address;
} clientes_t;

#endif