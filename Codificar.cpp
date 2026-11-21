/*----------------------------------------------*
 * Diego Felippe da Fonseca Calesco - 262516187 *
 *----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <windows.h>

#include "TabelaPalavra.h"
#include "Arvore.h"
#include "Arquivo.h"

#define FRASE "O vento soprava devagar, devagar e constante. As folhas dançavam, dançavam pelo chão enquanto os pássaros cantavam, cantavam sem parar.."

void lerTextoArquivo(char nomeArquivo[], char texto[], int capacidade)
{
    FILE *arquivo;
    int caractere, lidos, i;

    texto[0] = '\0';
    arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL)
        printf("Erro ao abrir o texto base: %s\n", nomeArquivo);
    else
    {
        lidos = 0;
        caractere = fgetc(arquivo);
        while (!feof(arquivo) && lidos < capacidade - 1)
        {
            texto[lidos++] = caractere;
            caractere = fgetc(arquivo);
        }
        texto[lidos] = '\0';

        if (!feof(arquivo))
        {
            printf("Texto base excede o limite de %d bytes.\n", capacidade - 1);
            texto[0] = '\0';
        }
        else if (lidos == 0)
            printf("Texto base vazio.\n");
        else
        {
            for (i = 0; i < lidos; i++)
                if (texto[i] == '\r' || texto[i] == '\n' || texto[i] == '\t')
                    texto[i] = ' ';
        }
        fclose(arquivo);
    }
}

int main(void)
{
    setlocale(LC_ALL, "C");
    SetConsoleOutputCP(CP_UTF8);

    ListaPalavra *listaPalavra;
    ListaTree *listaTree = NULL;
    Tree *raiz;
    int colunas;
    BitWriter writer;
    char texto[10000];

    lerTextoArquivo("Texto.txt", texto, 10000);
    if (texto[0] != '\0')
    {
        inicializarListaPalavra(&listaPalavra);
        processarTexto(&listaPalavra, texto);

        preencherListaTree(&listaTree, listaPalavra);
        raiz = montarArvore(&listaTree);

        colunas = alturaArvore(raiz) + 2;
        gerarCodHuffman(listaPalavra, raiz, "", colunas);

        exibeListaPalavra(listaPalavra);
        exibeArvore(raiz, 0);

        gravarArquivo(listaPalavra);

        iniciarBitWriter(&writer, "codificado.dat");
        codificarFrase(FRASE, listaPalavra, &writer);
        fecharBitWriter(&writer);

        printf("\n\nFrase codificada: %s", FRASE);
        printf("\nFrase codificada e gravada em codificado.dat\n");
    }
}
