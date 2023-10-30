/* cliente.c
   Elias P. Duarte Jr. */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constantes.h"
#include "particionaArquivo.h"

main(int argc, char *argv[])

{
	int sockdescr;
	int numbytesrecv;
	struct sockaddr_in sa;
	struct hostent *hp;
	char buf[BUFSIZ + 1];
	char buffer[BUFFER_SIZE];
	char *host;
	char *dados;

	unsigned int i;

	if (argc != 4)
	{
		puts("Uso correto: <cliente> <nome-servidor> <porta> <dados>");
		exit(1);
	}

	host = argv[1];
	dados = argv[3];

	if ((hp = gethostbyname(host)) == NULL)
	{
		puts("Nao consegui obter endereco IP do servidor.");
		exit(1);
	}

	bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;

	sa.sin_port = htons(atoi(argv[2]));

	if ((sockdescr = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0)
	{
		puts("Nao consegui abrir o socket.");
		exit(1);
	}

	if (sendto(sockdescr, dados, strlen(dados) + 1, 0, (struct sockaddr *)&sa, sizeof sa) != strlen(dados) + 1)
	{
		puts("Nao consegui mandar os dados");
		exit(1);
	}
	int count = 0;
	/* end while }*/
	FILE *arq = abre_arquivo_escrita("outroBolo.png");

	fd_set readfds;
	struct timeval timeout;
	int retval;

	while (1)
	{
		i = sizeof(sa);
		FD_ZERO(&readfds);
		FD_SET(sockdescr, &readfds);

		timeout.tv_sec = 5;	 // Defina o timeout em segundos
		timeout.tv_usec = 0; // Defina os microssegundos do timeout

		retval = select(sockdescr + 1, &readfds, NULL, NULL, &timeout);

		if (retval == -1)
		{
			perror("Erro na função select");
		}
		else if (retval == 0)
		{
			printf("Timeout! Nenhum dado recebido.\n");
			break;
		}
		else
		{
			ssize_t bytes_received = recvfrom(sockdescr, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sa, &i);
			printf("Recebido %s\n", buffer);
			fwrite(buffer, sizeof(char), bytes_received, arq);
		}
	}

	close(sockdescr);
	exit(0);
}