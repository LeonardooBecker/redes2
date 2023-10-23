#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *input_file = "cake.png";
    const char *output_file = "output.png";

    FILE *input = fopen(input_file, "rb");
    if (!input)
    {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    FILE *output = fopen(output_file, "wb");
    if (!output)
    {
        perror("Erro ao abrir o arquivo de saída");
        fclose(input);
        return 1;
    }

    // Define um tamanho de buffer para cópia
    size_t buffer_size = 1024;
    unsigned char buffer[buffer_size];
    size_t bytes_read;

    for (int i = 0; i < 100; i++) {
        fseek(input, (i*buffer_size), SEEK_SET);
        bytes_read = fread(buffer, 1, buffer_size, input);
        if (bytes_read > 0) {
            fwrite(buffer, 1, bytes_read, output);
        } else {
            break;
        }
    }

    fclose(input);
    fclose(output);

    printf("Imagem lida de %s e reescrita em %s com sucesso.\n", input_file, output_file);

    return 0;
}