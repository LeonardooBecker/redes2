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

    int vetor[10];
    int posicao_vetor = 0;

    fd_set readfds;
    struct timeval timeout;
    int retval;

    i = sizeof(sa);

    FD_ZERO(&readfds);
    FD_SET(s, &readfds);

    timeout.tv_sec = 5;  // Defina o timeout em segundos
    timeout.tv_usec = 0; // Defina os microssegundos do timeout

    while (1)
    {
        i = sizeof(isa);

        puts("Vou bloquear esperando mensagem.");

        retval = select(s + 1, &readfds, NULL, NULL, &timeout);

        if (retval == -1)
        {
            perror("Erro na função select");
        }
        else if (retval == 0)
        {
            printf("Timeout! Nenhum dado recebido.\n");

            FD_ZERO(&readfds);
            FD_SET(s, &readfds);

            timeout.tv_sec = 5;  // Defina o timeout em segundos
            timeout.tv_usec = 0; // Defina os microssegundos do timeout

            // timeout.tv_sec = 5;
            // printf("%ld",timeout.tv_sec);
        }
        else
        {
            if (recvfrom(s, buf, BUFSIZ, 0, (struct sockaddr *)&isa, &i))
            {
                inet_ntop(AF_INET, &(isa.sin_addr), str, i);
                int porta = ntohs(isa.sin_port);
                vetor[posicao_vetor] = porta;
                printf("%s\n", str);
                printf("%d\n", porta);

                printf("Endereço: %s:%d\n", str, porta);

                FILE *arq = abre_arquivo_leitura("cake.png");

                for (int j = 0; j < MAX_PARTS; j++)
                {
                    if (retorna_parte(arq, j, buffer) > 0)
                    {
                        char ns[BUFFER_SIZE];
                        sprintf(ns, "%d", j);
                        sendto(s, buffer, BUFSIZ, 0, (struct sockaddr *)&isa, i);
                        printf("Enviando parte %s\n", ns);
                    }
                    else
                        break;
                    usleep(70000);
                }
            }
            else
                printf("OIIII\nOIIII\nOIIIII");
        }
    }
}