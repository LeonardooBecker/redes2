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

FILE *abre_arquivo_leitura(const char *nome_arquivo)
{
    FILE *arquivo;
    arquivo=fopen(nome_arquivo, "rb");
    if(!arquivo)
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
    arquivo=fopen(nome_arquivo, "wb");
    if(!arquivo)
    {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
    else
        return arquivo;
}

// int main()
// {
//     const char *input_file = "cake.png";
//     const char *output_file = "output.png";

//     FILE *input = abre_arquivoRB(input_file);
//     FILE *output = fopen(output_file, "wb");

//     unsigned char buffer[BUFFER_SIZE];

//     for (int i = 0; i < MAX_PARTS; i++)
//     {
//         if(retorna_parte(input, i, buffer) > 0)
//             fwrite(buffer, 1, BUFFER_SIZE, output);
//         else
//             break;
//     }

//     fclose(input);
//     fclose(output);

//     printf("Imagem lida de %s e reescrita em %s com sucesso.\n", input_file, output_file);

//     return 0;
// }