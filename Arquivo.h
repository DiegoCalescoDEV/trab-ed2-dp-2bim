struct bits
{
    unsigned char b7 : 1;
    unsigned char b6 : 1;
    unsigned char b5 : 1;
    unsigned char b4 : 1;
    unsigned char b3 : 1;
    unsigned char b2 : 1;
    unsigned char b1 : 1;
    unsigned char b0 : 1;
};

union byte_union
{
    struct bits bi;
    unsigned char byte;
};
typedef union byte_union ByteUnion;

typedef struct
{
    FILE *arquivo;
    ByteUnion buffer;
    int contador;
} BitWriter;

void iniciarBitWriter(BitWriter *writer, const char *nomeArquivo)
{
    writer->arquivo = fopen(nomeArquivo, "wb");
    writer->buffer.byte = 0;
    writer->contador = 0;
}

void escreverBit(BitWriter *writer, char bitChar)
{
    unsigned char valor;
    
    if(bitChar == '1')
        valor = 1; 
    else 
        valor = 0;

    switch (writer->contador)
    {
    case 0:
        writer->buffer.bi.b7 = valor;
        break;
    case 1:
        writer->buffer.bi.b6 = valor;
        break;
    case 2:
        writer->buffer.bi.b5 = valor;
        break;
    case 3:
        writer->buffer.bi.b4 = valor;
        break;
    case 4:
        writer->buffer.bi.b3 = valor;
        break;
    case 5:
        writer->buffer.bi.b2 = valor;
        break;
    case 6:
        writer->buffer.bi.b1 = valor;
        break;
    case 7:
        writer->buffer.bi.b0 = valor;
        break;
    }

    writer->contador++;

    if (writer->contador == 8)
    {
        fwrite(&writer->buffer.byte, 1, 1, writer->arquivo);
        writer->buffer.byte = 0;
        writer->contador = 0;
    }
}

void fecharBitWriter(BitWriter *writer)
{
    if (writer->contador > 0)
    {
        while (writer->contador < 8)
        {
            switch (writer->contador)
            {
            case 0:
                writer->buffer.bi.b7 = 0;
                break;
            case 1:
                writer->buffer.bi.b6 = 0;
                break;
            case 2:
                writer->buffer.bi.b5 = 0;
                break;
            case 3:
                writer->buffer.bi.b4 = 0;
                break;
            case 4:
                writer->buffer.bi.b3 = 0;
                break;
            case 5:
                writer->buffer.bi.b2 = 0;
                break;
            case 6:
                writer->buffer.bi.b1 = 0;
                break;
            case 7:
                writer->buffer.bi.b0 = 0;
                break;
            }
            writer->contador++;
        }
        fwrite(&writer->buffer.byte, 1, 1, writer->arquivo);
    }
    fclose(writer->arquivo);
}

void gravarArquivo(ListaPalavra *l)
{
    Reg aux;
    FILE *file = fopen("dicionario.dat", "wb");

    if (file == NULL)
    {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    while (l != NULL)
    {
        aux.simbolo = l->simbolo;
        strcpy(aux.palavra, l->palavra);
        strcpy(aux.codHuffman, l->codHuffman);
        fwrite(&aux, sizeof(Reg), 1, file);
        l = l->prox;
    }

    fclose(file);
    printf("\n\nDicionario gravado em dicionario.dat");
}

void exibirArquivo(FILE *file)
{
    Reg data;
    fseek(file, 0, 0);
    while (fread(&data, sizeof(Reg), 1, file) == 1)
        printf("Simbolo: %d\tPalavra: %-20s\t Codigo: %s\n", data.simbolo, data.palavra, data.codHuffman);
}

void codificarPalavra(char palavra[], ListaPalavra *lista, BitWriter *writer)
{
    while (lista != NULL && strcmp(lista->palavra, palavra) != 0)
        lista = lista->prox;

    if (lista != NULL)
    {
        int i, tam = strlen(lista->codHuffman);
        for (i = 0; i < tam; i++)
            escreverBit(writer, lista->codHuffman[i]);
    }
}

void codificarFrase(char texto[], ListaPalavra *lista, BitWriter *writer)
{
    char vetTexto[5000];
    char vetSeparador[2];
    int i = 0, j = 0;

    vetSeparador[1] = '\0';

    while (i <= (int)strlen(texto))
    {
        vetTexto[j++] = tolower(texto[i]);

        if (caractereEspecial(texto[i]) || texto[i] == '\0')
        {

            if (j > 1)
            {
                vetTexto[j - 1] = '\0';
                codificarPalavra(vetTexto, lista, writer);
            }

            if (caractereEspecial(texto[i]))
            {
                vetSeparador[0] = texto[i];
                codificarPalavra(vetSeparador, lista, writer);
            }

            j = 0;
        }
        i++;
    }
}

char *binarioParaString(const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo)
    {
        fseek(arquivo, 0, 2);
        int tamanhoBytes = ftell(arquivo);
        rewind(arquivo);

        int tamanhoString = (tamanhoBytes * 8) + 1;
        char *stringBits = (char *)malloc(sizeof(char) * tamanhoString);
        if (stringBits != NULL)
        {
            ByteUnion byteUnion;
            int indice = 0;

            while (fread(&byteUnion.byte, 1, 1, arquivo) == 1)
            {
                stringBits[indice++] = byteUnion.bi.b7 + '0';
                stringBits[indice++] = byteUnion.bi.b6 + '0';
                stringBits[indice++] = byteUnion.bi.b5 + '0';
                stringBits[indice++] = byteUnion.bi.b4 + '0';
                stringBits[indice++] = byteUnion.bi.b3 + '0';
                stringBits[indice++] = byteUnion.bi.b2 + '0';
                stringBits[indice++] = byteUnion.bi.b1 + '0';
                stringBits[indice++] = byteUnion.bi.b0 + '0';
            }

            stringBits[indice] = '\0';
            fclose(arquivo);
            return stringBits;
        }
        fclose(arquivo);
    }

    printf("Erro\n");
    return NULL;
}
