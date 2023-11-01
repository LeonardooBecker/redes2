#ifndef __CONSTANTES_H__
#define __CONSTANTES_H__

#define BUFFER_SIZE 1024
#define MAX_PARTS 100000
#define MAX_CLIENTES 100

#define SEGUNDOS_ESPERA 0
#define MS_ESPERA 300
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
    FILE *arquivo_cliente;
} clientes_t;

#endif