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
    fd_set readfds;
    struct timeval timeout;
    int retval;
    int intervalo_pacotes = 500000;
    int encerra_servidor = 0;
    int qnt_tentativas = 0;

    lista_t *lista;
    if ((lista = lista_cria()))
    {
        printf("Iniciei a lista que pode ter no máximo %d clientes\n", TAMANHO_LISTA);
    }
    else
    {
        fprintf(stderr, "Erro ao criar a lista\n");
        exit(1);
    }

    if (argc < 2)
    {
        puts("Uso correto: servidor <porta>");
        exit(1);
    }
    if (argc < 3)
    {
        puts("Não foi passado o intervalo de tempo entre pacotes, o tempo padrão de 500ms será usado");
    }
    else
    {
        intervalo_pacotes = atoi(argv[2]) * 1000;
    }

    gethostname(localhost, MAXHOSTNAME);

    // Obtem o endereço IP do servidor
    if ((hp = gethostbyname(localhost)) == NULL)
    {
        puts("Nao consegui meu proprio IP");
        exit(1);
    }

    // printf("Sou o Servidor\nNome: %s, Porta: %d\n", localhost, atoi(argv[1]));

    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(atoi(argv[1]));
    sa.sin_family = hp->h_addrtype;

    // Realiza a criação do socket
    if ((sock = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0)
    {
        puts("Nao consegui abrir o socket");
        exit(1);
    }

    // Realiza o bind do socket com o endereço do servidor
    if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        puts("Nao consegui fazer o bind");
        exit(1);
    }

    resetaTimeoutServidor(&timeout, &readfds, sock, intervalo_pacotes);

    if (intervalo_pacotes != 0)
        encerra_servidor = 10000000 / intervalo_pacotes;
    else
        encerra_servidor = 10000000 / 1;
    // Laço principal de execução, servidor fica em loop infinito no processo de buscar novas conexões e enviar os pacotes das já existentes
    while (1)
    {

        // Responsável por verificar a variação do tempo de timeout
        retval = select(sock + 1, &readfds, NULL, NULL, &timeout);

        // Caso haja algum erro na função select
        if (retval == -1)
        {
            perror("Erro na função select");
        }

        // Seria o caso do timeout, mas como não estamos trabalhando com timeout no servidor, em todos os momentos de "timeout" ele envia o pacote para o cliente
        else if (retval == 0)
        {
            enviaPacotes(sock, lista);
            resetaTimeoutServidor(&timeout, &readfds, sock, intervalo_pacotes);
            confereInatividade(lista, &qnt_tentativas, encerra_servidor);
        }

        // Caso esteja fora do timeout, ele busca por novas conexões de clientes.
        else
        {
            buscaConexao(sock, lista);
            lista_imprime(lista);
        }
    }

    lista_destroi(lista);
    close(sock);
    return 0;
}