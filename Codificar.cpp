#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#include "TabelaPalavra.h"
#include "Arvore.h"
#include "Arquivo.h"
#define TEXTO "O vento soprava devagar, devagar e constante. As folhas dan�avam, dan�avam pelo ch�o enquanto os p�ssaros cantavam, cantavam sem parar. Era uma tarde tranquila, tranquila e agrad�vel, perfeita para caminhar, caminhar sem pressa e aproveitar, aproveitar cada momento."

#define FRASE "O vento soprava devagar, devagar e constante. As folhas dan�avam, dan�avam pelo ch�o enquanto os p�ssaros cantavam, cantavam sem parar.."

int main()
{
    setlocale(LC_ALL, "Portuguese");
    
    ListaPalavra *listaPalavra;
    ListaTree *listaTree = NULL;
    Tree *raiz;
    int colunas;
    BitWriter writer;
    
    inicializarListaPalavra(&listaPalavra);
    processarTexto(&listaPalavra, TEXTO);
    
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
    
    return 0;
}
