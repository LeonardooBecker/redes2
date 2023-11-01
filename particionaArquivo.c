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

size_t novoRetornaParte(clientes_t cliente, unsigned char *buffer)
{
    // FILE *arq = abre_arquivo_leitura(cliente.stream_cliente);
    size_t bytes_read;
    fseek(cliente.arquivo_cliente, (cliente.parte * BUFFER_SIZE), SEEK_SET);
    if ((bytes_read = fread(buffer, 1, BUFFER_SIZE, cliente.arquivo_cliente)) > 0)
        return bytes_read;
    else
        return 0;
    // fclose(arq);
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