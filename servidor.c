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

int main(int argc, char *argv[])
{
    int s;
    unsigned int i;
    char buf[BUFSIZ + 1];
    struct sockaddr_in sa, isa; /* sa: servidor, isa: cliente */
    struct hostent *hp;
    char localhost[MAXHOSTNAME];

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

    char str[sizeof(isa)];
    unsigned char buffer[BUFFER_SIZE];

    int vetor_portas[10];
    int vetor_endereço[10];
    int posicao_vetor = 0;
    FILE *arq = abre_arquivo_leitura("shrek3.txt");

    fd_set readfds;
    struct timeval timeout;
    int retval;

    i = sizeof(sa);

    FD_ZERO(&readfds);
    FD_SET(s, &readfds);

    timeout.tv_sec = INTERVALO_TEMPO; // Defina o timeout em segundos
    timeout.tv_usec = 0;              // Defina os microssegundos do timeout

    int pacoteAtual = 0;

    while (1)
    {
        i = sizeof(isa);

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

            FD_ZERO(&readfds);
            FD_SET(s, &readfds);
            timeout.tv_sec = INTERVALO_TEMPO; // Defina o timeout em segundos
            timeout.tv_usec = 0;              // Defina os microssegundos do timeout

            if (retorna_parte(arq, pacoteAtual, buffer) > 0)
            {
                for (int k = 0; k < posicao_vetor; k++)
                {
                    isa.sin_family = AF_INET;
                    isa.sin_port = htons(vetor_portas[k]);
                    isa.sin_addr.s_addr = vetor_endereço[k];
                    sendto(s, buffer, BUFSIZ, 0, (struct sockaddr *)&isa, i);
                }
                printf("Enviando parte %d\n", pacoteAtual);
                pacoteAtual++;
            }
            else
                pacoteAtual = 0;
        }

        // Caso esteja fora do timeout, ele busca por novas conexões de clientes.
        else
        {
            if (recvfrom(s, buf, BUFSIZ, 0, (struct sockaddr *)&isa, &i))
            {
                inet_ntop(AF_INET, &(isa.sin_addr), str, i);
                int porta = ntohs(isa.sin_port);
                vetor_portas[posicao_vetor] = porta;
                vetor_endereço[posicao_vetor] = inet_addr(str);
                posicao_vetor++;
                printf("Endereço: %s:%d\n", str, porta);
            }
        }
    }
}