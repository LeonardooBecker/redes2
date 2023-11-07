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
#include "libCliente.h"

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in sa;
	struct hostent *hp;
	datagramaUDP pacote;
	desempenho_t *desempenho = inicializa_desempenho();
	char *host;
	fd_set readfds;
	struct timeval timeout;
	int retval;
	char arquivo_retorno[100];

	if (argc != 4)
	{
		puts("Uso correto: <cliente> <nome-servidor> <porta> <dados>");
		exit(1);
	}

	host = argv[1];

	// Obtem o endereço IP do servidor
	if ((hp = gethostbyname(host)) == NULL)
	{
		puts("Nao consegui obter endereco IP do servidor.");
		exit(1);
	}

	// Realiza a criação do socket
	if ((sock = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0)
	{
		puts("Nao consegui abrir o socket.");
		exit(1);
	}

	sa.sin_addr = *(struct in_addr *)hp->h_addr;
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons(atoi(argv[2]));

	// Formata o nome do arquivo de retorno e abre o arquivo para escrita
	sprintf(arquivo_retorno, "retorno_%s", argv[3]);
	FILE *arq = abre_arquivo_escrita(arquivo_retorno);

	// Formata o pacote que será enviado ao servidor
	sprintf(pacote.dados, "%s", argv[3]);
	pacote.tamanho = sizeof(pacote.dados);
	pacote.sequencia = 0;

	// Envia a solicitação ao servidor do arquivo que deseja receber
	if (sendto(sock, &pacote, pacote.tamanho + TAMANHO_HEADER, 0, (struct sockaddr *)&sa, sizeof sa) != (pacote.tamanho + TAMANHO_HEADER))
	{
		puts("Nao consegui mandar os dados");
		exit(1);
	}

	resetaTimeoutCliente(&timeout, &readfds, sock);

	// Enquanto não receber o timeout para encerramento, recebe os pacotes enviados pelo servidor
	while (1)
	{
		// Responsável por verificar a variação do tempo de timeout
		retval = select(sock + 1, &readfds, NULL, NULL, &timeout);

		// Caso haja algum erro na função select
		if (retval == -1)
		{
			perror("Erro na função select");
		}

		// Assim que o cliente receber todos os pacotes do servidor ele encerra a conexão
		else if (retval == 0)
		{
			fprintf(stderr,"Timeout! Nenhum dado recebido.\n");
			fprintf(stderr,"Encerrando conexão...\n");
			imprime_log(desempenho);
			break;
		}

		// Enquanto não receber o timeout para encerramento, recebe os pacotes enviados pelo servidor
		else
		{
			// Enquanto esta recebendo pacote, não temos interesse em timeout.
			resetaTimeoutCliente(&timeout, &readfds, sock);
			recebeDados(sock, arq, desempenho);
		}
	}

	close(sock);
	exit(0);
}