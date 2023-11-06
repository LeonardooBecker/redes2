/* Prof. Elias P. Duarte Jr.
   Servidor UDP              */

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

#define TAMFILA 5
#define MAXHOSTNAME 30
#define TEMPO 1

#define MAX_SEQUENCIA 1000

void buscaConexao(clientes_t *clientes, int *id_cliente, int socket)
{
    datagramaUDP pacote;
    struct sockaddr_in isa;
    unsigned int i = sizeof(isa);
    char str[sizeof(isa)];
    if (recvfrom(socket, &pacote, sizeof(pacote), 0, (struct sockaddr *)&isa, &i))
    {
        printf("BUFFER: %s\n", pacote.dados);
        inet_ntop(AF_INET, &(isa.sin_addr), str, i);
        int host = isa.sin_port;
        int addrs = inet_addr(str);
        clientes[(*id_cliente)].host = host;
        clientes[(*id_cliente)].address = addrs;
        strcat(clientes[(*id_cliente)].stream_cliente, pacote.dados);
        clientes[(*id_cliente)].parte = 0;
        clientes[(*id_cliente)].sequencia_total = 0;
        clientes[(*id_cliente)].arquivo_cliente = abre_arquivo_leitura(pacote.dados);
        (*id_cliente)++;
        printf("Endereço: %s:%d\n", str, host);
    }
}

void enviaPacotes(clientes_t *clientes, int id_cliente, int s)
{
    datagramaUDP pacote;
    struct sockaddr_in isa;
    unsigned int i = sizeof(isa);
    for (int k = 0; k < id_cliente; k++)
    {
        if (retornaFragmento(clientes[k], &pacote) > 0)
        {
            printf("Enviando parte %d\n", clientes[k].parte);
            isa.sin_family = AF_INET;
            isa.sin_port = clientes[k].host;
            isa.sin_addr.s_addr = clientes[k].address;
            sendto(s, &pacote, pacote.tamanho + TAMANHO_HEADER, 0, (struct sockaddr *)&isa, i);
            clientes[k].parte++;
        }
        else
        {
            clientes[k].sequencia_total += clientes[k].parte;
            if (clientes[k].sequencia_total <= MAX_SEQUENCIA)
                clientes[k].parte = 0;
        }
    }
}

void resetaTimeout(struct timeval *timeout, fd_set *readfds, int s)
{
    FD_ZERO(readfds);
    FD_SET(s, readfds);
    (*timeout).tv_sec = SEGUNDOS_ESPERA; // Defina o timeout em segundos
    (*timeout).tv_usec = MS_ESPERA;      // Defina os microssegundos do timeout
}

int main(int argc, char *argv[])
{
    int s;
    struct sockaddr_in sa; /* sa: servidor, isa: cliente */
    struct hostent *hp;
    char localhost[MAXHOSTNAME];
    clientes_t clientes[MAX_CLIENTES];
    int id_cliente = 0;

    if (argc != 2)
    {
        puts("Uso correto: servidor <porta>");
        exit(1);
    }

    gethostname(localhost, MAXHOSTNAME);

    if ((hp = gethostbyname(localhost)) == NULL)
    {
        puts("Nao consegui meu proprio IP");
        exit(1);
    }

    sa.sin_port = htons(atoi(argv[1]));

    printf("Sou o Servidor\nNome: %s, Porta: %d\n", localhost, atoi(argv[1]));

    bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);

    sa.sin_family = hp->h_addrtype;

    if ((s = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0)
    {
        puts("Nao consegui abrir o socket");
        exit(1);
    }

    if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        puts("Nao consegui fazer o bind");
        exit(1);
    }

    fd_set readfds;
    struct timeval timeout;
    int retval;

    resetaTimeout(&timeout, &readfds, s);

    while (1)
    {
        // puts("Vou bloquear esperando mensagem.");

        retval = select(s + 1, &readfds, NULL, NULL, &timeout);

        // Amamos quando retval não da errado
        if (retval == -1)
        {
            perror("Erro na função select");
        }

        // Seria o caso do timeout, mas como não estamos trabalhando com timeout no servidor, em todos os momentos de "timeout" ele envia o pacote para o cliente
        else if (retval == 0)
        {
            // printf("Timeout! Nenhum dado recebido.\n");
            resetaTimeout(&timeout, &readfds, s);
            enviaPacotes(clientes, id_cliente, s);
        }

        // Caso esteja fora do timeout, ele busca por novas conexões de clientes.
        else
        {
            buscaConexao(clientes, &id_cliente, s);
        }
    }
}