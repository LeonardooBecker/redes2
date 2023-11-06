/* cliente.c
   Elias P. Duarte Jr. */

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

int main(int argc, char *argv[])

{
	int sockdescr;
	struct sockaddr_in sa;
	struct hostent *hp;
	datagramaUDP pacote;
	char *host;

	unsigned int i;

	if (argc != 4)
	{
		puts("Uso correto: <cliente> <nome-servidor> <porta> <dados>");
		exit(1);
	}

	host = argv[1];

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

	char nome_arquivo[100];
	sprintf(nome_arquivo, "retorno_%s", argv[3]);
	FILE *arq = abre_arquivo_escrita(nome_arquivo);

	fd_set readfds;
	struct timeval timeout;
	int retval;

	i = sizeof(sa);
	FD_ZERO(&readfds);
	FD_SET(sockdescr, &readfds);

	timeout.tv_sec = SEGUNDOS_ESPERA_CLIENTE; // Defina o timeout em segundos
	timeout.tv_usec = MS_ESPERA_CLIENTE;	  // Defina os microssegundos do timeout

	sprintf(pacote.dados, "%s", argv[3]);
	pacote.tamanho = sizeof(pacote.dados);
	pacote.sequencia = 0;

	if (sendto(sockdescr, &pacote, pacote.tamanho + TAMANHO_HEADER, 0, (struct sockaddr *)&sa, sizeof sa) != (pacote.tamanho + TAMANHO_HEADER))
	{
		puts("Nao consegui mandar os dados");
		exit(1);
	}

	while (1)
	{
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
			FD_ZERO(&readfds);
			FD_SET(sockdescr, &readfds);

			timeout.tv_sec = SEGUNDOS_ESPERA_CLIENTE; // Defina o timeout em segundos
			timeout.tv_usec = MS_ESPERA_CLIENTE;	  // Defina os microssegundos do timeout
													  // sleep(3);
			memset(&pacote, 0, sizeof(pacote));
			ssize_t bytes_received = recvfrom(sockdescr, &pacote, sizeof(pacote), 0, (struct sockaddr *)&sa, &i);
			if (bytes_received > 0)
			{
				fwrite(pacote.dados, sizeof(char), pacote.tamanho, arq);
				bytes_received = 0;
			}
		}
	}

	close(sockdescr);
	exit(0);
}