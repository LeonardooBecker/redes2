#include "libServidor.h"

// Função responsável por criar um novo cliente
clientes_t *novo_cliente()
{
    clientes_t *cliente;
    if ((cliente = malloc(sizeof(clientes_t))))
    {
        cliente->parte = 0;
        cliente->sequencia_total = 0;
        cliente->host = 0;
        cliente->address = 0;
        strcpy(cliente->stream_cliente, "");
        cliente->arquivo_cliente = NULL;
        return cliente;
    }
    return NULL;
}

// Função responsável por buscar as conexões dos clientes
void buscaConexao(int socket, lista_t *lista)
{
    datagramaUDP pacote;
    struct sockaddr_in isa;
    unsigned int i = sizeof(isa);
    char str[sizeof(isa)];
    char nome_arquivo[MAX_NOME];
    int host;
    int addrs;
    clientes_t *cliente = novo_cliente();

    if (recvfrom(socket, &pacote, sizeof(pacote), 0, (struct sockaddr *)&isa, &i))
    {
        // Inicializa o primeiro pacote do cliente

        // Busca o endereço do cliente ( host e address ) e salva nas respectivas posições da struct
        inet_ntop(AF_INET, &(isa.sin_addr), str, i);
        host = isa.sin_port;
        addrs = inet_addr(str);

        // Formatação para acessar a pasta com os roteiros
        strcpy(nome_arquivo, "./roteiros/");
        strcat(nome_arquivo, pacote.dados);

        // Inicializa o cliente com os dados recebidos
        cliente->parte = 0;
        cliente->sequencia_total = 0;
        cliente->host = host;
        cliente->address = addrs;
        strcpy(cliente->stream_cliente, pacote.dados);
        cliente->arquivo_cliente = abre_arquivo_leitura(nome_arquivo);
        lista_insere_fim(lista, cliente);
        printf("Cliente de host = %d - Roteiro desejado: %s\n",cliente->host, pacote.dados);

        // printf("Endereço: %s:%d\n", str, host);
    }
}

// Função responsável por enviar os pacotes para os clientes
void enviaPacotes(int socket, lista_t *lista)
{
    datagramaUDP pacote;
    struct sockaddr_in isa;
    unsigned int i = sizeof(isa);
    clientes_t *cliente;
    nodo_l_t *nodo = lista_retorna_inicio(lista);
    // Para cada cliente encontrado pelo servidor, o mesmo percorre enviando os pacotes sequenciais referentes a cada cliente
    while (nodo != NULL)
    {
        cliente = nodo->cliente;
        // Retorna o pacote referente a parte do arquivo, sequência comandada pelo servidor
        if (retornaFragmento(cliente, &pacote) > 0)
        {
            // printf("Enviando parte %d\n", cliente->parte);
            // Resgata as informações de host e address do cliente para poder enviar o pacote
            isa.sin_family = AF_INET;
            isa.sin_port = cliente->host;
            isa.sin_addr.s_addr = cliente->address;
            sendto(socket, &pacote, pacote.tamanho + TAMANHO_HEADER, 0, (struct sockaddr *)&isa, i);
            cliente->parte++;
        }
        else
        {
            // Caso tenha chegado ao fim, ele retorna para o inicio enquanto possuir menos envios de pacote que a MIN_SEQUENCIA
            cliente->sequencia_total += cliente->parte;
            if (cliente->sequencia_total <= MIN_SEQUENCIA)
                cliente->parte = 0;
            else
            {
                printf("Todos os pacotes do cliente %d foram enviados!\n", cliente->host);
                nodo = percorre_lista(lista, nodo);
                lista_retira_cliente(lista, cliente);
                lista_imprime(lista);
            }
        }
        nodo = percorre_lista(lista, nodo);
    }
}

// Função responsável por resetar o timeout do servidor
void resetaTimeoutServidor(struct timeval *timeout, fd_set *readfds, int s, int intervalo_pacotes)
{
    FD_ZERO(readfds);
    FD_SET(s, readfds);
    (*timeout).tv_sec = 0;
    (*timeout).tv_usec = intervalo_pacotes;
}

void confereInatividade(lista_t *lista, int *qnt_tentativas, int encerra_servidor)
{
    if (*qnt_tentativas == encerra_servidor)
    {
        printf("A lista esta vazia\n");
        fprintf(stderr, "Servidor encerrado por inatividade\n");
        exit(0);
    }
    if (!lista_vazia(lista))
    {
        *qnt_tentativas = 0;
    }
    else
    {
        (*qnt_tentativas)++;
    }
}