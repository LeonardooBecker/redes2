#include "particionaArquivo.h"

size_t retorna_parte(FILE *arquivo, int parte, unsigned char *buffer)
{
    size_t bytes_read;
    fseek(arquivo, (parte * BUFFER_SIZE), SEEK_SET);
    if ((bytes_read = fread(buffer, 1, BUFFER_SIZE, arquivo)) > 0)
        return bytes_read;
    else
        return 0;
}

size_t novoRetornaParte(clientes_t cliente, datagramaUDP *pacote)
{
    size_t bytes_read;
    memset(pacote->dados, 0, BUFFER_SIZE);
    fseek(cliente.arquivo_cliente, (cliente.parte * BUFFER_SIZE), SEEK_SET);
    if ((bytes_read = fread(pacote->dados, 1, BUFFER_SIZE, cliente.arquivo_cliente)) > 0)
    {
        pacote->tamanho = bytes_read;
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