#include "libCliente.h"

void resetaTimeoutCliente(struct timeval *timeout, fd_set *readfds, int s)
{
	FD_ZERO(readfds);
	FD_SET(s, readfds);
	(*timeout).tv_sec = SEGUNDOS_ESPERA_CLIENTE;
	(*timeout).tv_usec = MS_ESPERA_CLIENTE;
}

void recebeDados(int sock, FILE *arq, desempenho_t *desempenho)
{
	unsigned int i;
	datagramaUDP pacote;
	struct sockaddr_in sa;

	i = sizeof(sa);
	memset(&pacote, 0, sizeof(pacote));
	ssize_t bytes_received = recvfrom(sock, &pacote, sizeof(pacote), 0, (struct sockaddr *)&sa, &i);
	if (bytes_received > 0)
	{
		// printf("Sequência recebida: %d \n", pacote.sequencia);
		int diff_pacotes=pacote.sequencia-desempenho->sequencia_anterior;
		if ((diff_pacotes < 0) && (diff_pacotes > -MIN_PACOTES))
		{
			desempenho->pacotes_atrasados+=(-diff_pacotes);
		}
		else if( diff_pacotes < -MIN_PACOTES )
		{
			desempenho->repeticoes_roterio++;
		}
		else if(diff_pacotes==0 && pacote.sequencia!=0)
		{
			desempenho->pacotes_duplicados++;
		}
		else if(diff_pacotes>1)
		{
			desempenho->pacotes_perdidos+=diff_pacotes;
		}
		else if(diff_pacotes==1)
		{
			desempenho->pacotes_corretos++;
		}

		fwrite(pacote.dados, sizeof(char), pacote.tamanho, arq);
		bytes_received = 0;
		desempenho->sequencia_anterior=pacote.sequencia;
		// sleep colocado intencionalmente a fim de gerar algumas perdas de pacotes
		usleep(1000);
	}
}

desempenho_t *inicializa_desempenho()
{
	desempenho_t *desempenho;
	if ((desempenho = malloc(sizeof(desempenho_t))))
	{
		desempenho->sequencia_anterior = 0;
		desempenho->pacotes_corretos = 0;
		desempenho->pacotes_perdidos = 0;
		desempenho->pacotes_duplicados = 0;
		desempenho->repeticoes_roterio = 1;
		desempenho->pacotes_atrasados = 0;
		desempenho->possivel_total = 0;
		return desempenho;
	}
	puts("Erro ao alocar memória para desempenho");
	exit(1);
}

void imprime_log(desempenho_t *desempenho)
{
	printf("Total de pacotes corretos: %d\n", desempenho->pacotes_corretos);
	printf("Total de pacotes perdidos: %d\n", desempenho->pacotes_perdidos);
	printf("Total de pacotes duplicados: %d\n", desempenho->pacotes_duplicados);
	printf("Total de pacotes atrasados: %d\n", desempenho->pacotes_atrasados);
	printf("Repetições de roteiro: %d\n", desempenho->repeticoes_roterio);
	desempenho->possivel_total = desempenho->pacotes_corretos + desempenho->pacotes_atrasados + desempenho->pacotes_duplicados + desempenho->pacotes_perdidos;
	printf("Total possível de pacotes: %d\n", desempenho->possivel_total);
	printf("Taxa de perda: %.2f%%\n", (float)desempenho->pacotes_perdidos / (float)desempenho->possivel_total * 100);
}
