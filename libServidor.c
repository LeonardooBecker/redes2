#include "libServidor.h"


// Função responsável por buscar as conexões dos clientes
void buscaConexao(clientes_t *clientes, int *id_cliente, int socket)
{
    datagramaUDP pacote;
    struct sockaddr_in isa;
    unsigned int i = sizeof(isa);
    char str[sizeof(isa)];
    char nome_arquivo[MAX_NOME];
    int host;
    int addrs;

    if (recvfrom(socket, &pacote, sizeof(pacote), 0, (struct sockaddr *)&isa, &i))
    {
        //Inicializa o primeiro pacote do cliente
        printf("Roteiro desejado: %s\n", pacote.dados);
        clientes[(*id_cliente)].parte = 0;
        clientes[(*id_cliente)].sequencia_total = 0;
        
        // Busca o endereço do cliente ( host e address ) e salva nas respectivas posições da struct
        inet_ntop(AF_INET, &(isa.sin_addr), str, i);
        host = isa.sin_port;
        addrs = inet_addr(str);
        clientes[(*id_cliente)].host = host;
        clientes[(*id_cliente)].address = addrs;

        // Registra o nome do roteiro que o cliente deseja
        strcat(clientes[(*id_cliente)].stream_cliente, pacote.dados);
        
        // Formatação para acessar a pasta com os roteiros
        strcpy(nome_arquivo, "./roteiros/");
        strcat(nome_arquivo, pacote.dados);
        clientes[(*id_cliente)].arquivo_cliente = abre_arquivo_leitura(nome_arquivo);
        // printf("Endereço: %s:%d\n", str, host);
        (*id_cliente)++;
    }
}


// Função responsável por enviar os pacotes para os clientes
void enviaPacotes(clientes_t *clientes, int id_cliente, int s)
{
    datagramaUDP pacote;
    struct sockaddr_in isa;
    unsigned int i = sizeof(isa);
    
    // Para cada cliente encontrado pelo servidor, o mesmo percorre enviando os pacotes sequenciais referentes a cada cliente
    for (int k = 0; k < id_cliente; k++)
    {
        // Retorna o pacote referente a parte do arquivo, sequência comandada pelo servidor
        if (retornaFragmento(clientes[k], &pacote) > 0)
        {
            printf("Enviando parte %d\n", clientes[k].parte);
            // Resgata as informações de host e address do cliente para poder enviar o pacote
            isa.sin_family = AF_INET;
            isa.sin_port = clientes[k].host;
            isa.sin_addr.s_addr = clientes[k].address;
            sendto(s, &pacote, pacote.tamanho + TAMANHO_HEADER, 0, (struct sockaddr *)&isa, i);
            clientes[k].parte++;
        }
        else
        {
            // Caso tenha chegado ao fim, ele retorna para o inicio enquanto possuir menos envios de pacote que a MAX_SEQUENCIA
            clientes[k].sequencia_total += clientes[k].parte;
            if (clientes[k].sequencia_total <= MIN_SEQUENCIA)
                clientes[k].parte = 0;
        }
    }
}


// Função responsável por resetar o timeout do servidor
void resetaTimeoutServidor(struct timeval *timeout, fd_set *readfds, int s)
{
    FD_ZERO(readfds);
    FD_SET(s, readfds);
    (*timeout).tv_sec = SEGUNDOS_ESPERA;
    (*timeout).tv_usec = MS_ESPERA;
}