#include "particionaArquivo.h"

// Como a mensagem tem um tamanho superior ao do buffer, é necessário particiona-la em diversos pacotes.
// A função é responsável por retornar o pacote de acordo com a sequência registrada no servidor.
size_t retornaFragmento(clientes_t cliente, datagramaUDP *pacote)
{
    size_t bytes_read;
    memset(pacote->dados, 0, BUFFER_SIZE);
    fseek(cliente.arquivo_cliente, (cliente.parte * BUFFER_SIZE), SEEK_SET);
    if ((bytes_read = fread(pacote->dados, 1, BUFFER_SIZE, cliente.arquivo_cliente)) > 0)
    {
        pacote->tamanho = bytes_read;
        pacote->sequencia = cliente.parte;
        return bytes_read;
    }
    else
        return 0;
}

FILE *abre_arquivo_leitura(const char *nome_arquivo)
{
    FILE *arquivo;
    arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo)
    {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
    else
        return arquivo;
}

FILE *abre_arquivo_escrita(const char *nome_arquivo)
{
    FILE *arquivo;
    arquivo = fopen(nome_arquivo, "wb");
    if (!arquivo)
    {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
    else
        return arquivo;
}