/* Prof. Elias P. Duarte Jr.
   Servidor UDP              */

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fila.h"
#include "particionaArquivo.h"
#include "constantes.h"

#define TAMFILA 5
#define MAXHOSTNAME 30
#define TEMPO 1

main(int argc, char *argv[])
{
    int s, t;
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
    while (1)
    {
        i = sizeof(isa);
        puts("Vou bloquear esperando mensagem.");
        if (recvfrom(s, buf, BUFSIZ, 0, (struct sockaddr *)&isa, &i))
        {
            inet_ntop(AF_INET, &(sa.sin_addr), str, i);
            printf("%s\n", str);
            printf("Sou o servidor, recebi a mensagem----> %s\n", buf);
            FILE *arq=abre_arquivo_leitura("cake.png");
			for (int j = 0; j < MAX_PARTS; j++)
            {
                char ns[BUFFER_SIZE];
                sprintf(ns, "%d", j);
				if(retorna_parte(arq, j, buffer)>0)
                	sendto(s, buffer, BUFSIZ, 0, (struct sockaddr *)&isa, i);
				else
					break;
            }
            for(int j=0;j<10;j++)
                sendto(s, "fim", BUFSIZ, 0, (struct sockaddr *)&isa, i);
        }
    }
}