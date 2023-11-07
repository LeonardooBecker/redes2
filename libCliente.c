#include "libCliente.h"

void resetaTimeoutCliente(struct timeval *timeout, fd_set *readfds, int s)
{
	FD_ZERO(readfds);
	FD_SET(s, readfds);
	(*timeout).tv_sec = SEGUNDOS_ESPERA_CLIENTE;
	(*timeout).tv_usec = MS_ESPERA_CLIENTE;
}

void recebeDados(int sock, FILE *arq)
{
	unsigned int i;
	datagramaUDP pacote;
	struct sockaddr_in sa;

	i = sizeof(sa);
	memset(&pacote, 0, sizeof(pacote));
	ssize_t bytes_received = recvfrom(sock, &pacote, sizeof(pacote), 0, (struct sockaddr *)&sa, &i);
	if (bytes_received > 0)
	{
		printf("Sequência recebida: %d \n", pacote.sequencia);
		fwrite(pacote.dados, sizeof(char), pacote.tamanho, arq);
		bytes_received = 0;
	}
}