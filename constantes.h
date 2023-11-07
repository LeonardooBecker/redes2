#ifndef __CONSTANTES_H__
#define __CONSTANTES_H__

#define BUFFER_SIZE 1024
#define TAMANHO_HEADER 32

#define SEGUNDOS_ESPERA_CLIENTE 5
#define MS_ESPERA_CLIENTE 0

#define MAXHOSTNAME 30

#define MIN_SEQUENCIA 100000

#define MAX_NOME 400

#define MIN_PACOTES 10

#define TAMANHO_LISTA 5

typedef struct datagrama
{
    unsigned int tamanho : 16;
    unsigned int sequencia : 16;
    char dados[BUFFER_SIZE];
} datagramaUDP;

typedef struct clientes
{
    char stream_cliente[BUFFER_SIZE];
    unsigned int parte;
    int host;
    int address;
    FILE *arquivo_cliente;
    // utilizado para testes exaustivos
    int sequencia_total;
} clientes_t;

typedef struct desempenho
{
    int sequencia_anterior;
    int pacotes_corretos;
    int pacotes_perdidos;
    int pacotes_duplicados;
    int pacotes_atrasados;
    int repeticoes_roterio;
    int possivel_total;

}desempenho_t;

#endif