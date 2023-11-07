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
#include "libServidor.h"
#include "libLista.h"

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in sa; /* sa: servidor */
    struct hostent *hp;
    char localhost[MAXHOSTNAME];
    clientes_t clientes[MAX_CLIENTES];
    fd_set readfds;
    struct timeval timeout;
    int retval;
    int id_cliente = 0;
    lista_t *lista = lista_cria();

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

    // printf("Sou o Servidor\nNome: %s, Porta: %d\n", localhost, atoi(argv[1]));

    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(atoi(argv[1]));
    sa.sin_family = hp->h_addrtype;

    if ((sock = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0)
    {
        puts("Nao consegui abrir o socket");
        exit(1);
    }

    if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        puts("Nao consegui fazer o bind");
        exit(1);
    }

    resetaTimeoutServidor(&timeout, &readfds, sock);

    // Laço principal de execução, servidor fica em loop infinito no processo de buscar novas conexões e enviar os pacotes das já existentes
    while (1)
    {
        retval = select(sock + 1, &readfds, NULL, NULL, &timeout);

        // Caso haja algum erro na função select
        if (retval == -1)
        {
            perror("Erro na função select");
        }

        // Seria o caso do timeout, mas como não estamos trabalhando com timeout no servidor, em todos os momentos de "timeout" ele envia o pacote para o cliente
        else if (retval == 0)
        {
            resetaTimeoutServidor(&timeout, &readfds, sock);
            enviaPacotes(clientes, id_cliente, sock, lista);
        }

        // Caso esteja fora do timeout, ele busca por novas conexões de clientes.
        else
        {
            buscaConexao(clientes, &id_cliente, sock, lista);
            lista_imprime(lista);
        }
    }
}