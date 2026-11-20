#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#include "TabelaPalavra.h"
#include "Arvore.h"
#include "Arquivo.h"

void inserirNo(Tree **raiz, Reg data, int indice)
{
	if(data.codHuffman[indice] == '\0')
	{
		*raiz = criarNo(data.simbolo, -1);
	}
	else
	{
		if(*raiz == NULL)
			*raiz = criarNo(-1, -1);
		
		if(data.codHuffman[indice] == '0')
		{
			if((*raiz)->esq == NULL)
				(*raiz)->esq = criarNo(-1, -1);
			
			inserirNo(&(*raiz)->esq, data, indice + 1);
		}
		else
		{
			if((*raiz)->dir == NULL)
				(*raiz)->dir = criarNo(-1, -1);
			
			inserirNo(&(*raiz)->dir, data, indice + 1);
		}
	}
}

void montarArvoreDeco(FILE *arquivo, Tree **raiz)
{
	Reg data;
	
	while(fread(&data, sizeof(Reg), 1, arquivo) == 1)
		inserirNo(&*raiz, data, 0);
}

void buscar(FILE *arquivo, int simbolo)
{
    Reg data;
    fseek(arquivo, 0, 0);
    
    while(fread(&data, sizeof(Reg), 1, arquivo) == 1 && data.simbolo != simbolo)
        ;
    
    if(data.simbolo == simbolo)
    {
        if(strcmp(data.palavra, ".") != 0 && strcmp(data.palavra, "!") != 0 &&
           strcmp(data.palavra, "?") != 0 && strcmp(data.palavra, ";") != 0)
            printf("%s", data.palavra);
    }
    else
        printf("\nPalavra nao encontrada!\n");
}
void decodificarSimbolo(Tree *raiz, FILE *arquivo, char codigo[], int *indice)
{
	if(raiz != NULL)
		if(raiz->simbolo != -1)
			buscar(arquivo, raiz->simbolo);
		else
		{
			if(codigo[*indice] == '0' && raiz->esq != NULL)
			{
				(*indice)++;
				decodificarSimbolo(raiz->esq, arquivo, codigo, indice);
			}
			else if(raiz->dir != NULL)
			{
				(*indice)++;
				decodificarSimbolo(raiz->dir, arquivo, codigo, indice);
			}
		}
}

void decodificar(Tree *raiz, FILE *arquivo, char codigo[])
{
	int indice = 0;
	
	while(codigo[indice] != '\0')
		decodificarSimbolo(raiz, arquivo, codigo, &indice);
}

int main()
{
	FILE *file;
	Tree *raiz;
	char *codigo;
	
	setlocale(LC_ALL, "Portuguese");
	
	file = fopen("dicionario.dat", "rb");
	raiz = NULL;
	montarArvoreDeco(file, &raiz);
	
	exibeArvore(raiz, 0);
	printf("\n\n");
	exibirArquivo(file);
	
	printf("\n\n");
	codigo = binarioParaString("codificado.dat");
	printf("Codigo para decodificar: %s\n\n", codigo);
	printf("Frase: ");
	decodificar(raiz, file, codigo);
	printf("\n");
	
	return 0;
}

