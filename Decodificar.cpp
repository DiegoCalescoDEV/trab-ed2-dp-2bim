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

void inserirNo(Tree **raiz, Reg data, int indice)
{
	if (data.codHuffman[indice] == '\0')
	{
		*raiz = criarNo(data.simbolo, -1);
	}
	else
	{
		if (*raiz == NULL)
			*raiz = criarNo(-1, -1);

		if (data.codHuffman[indice] == '0')
		{
			if ((*raiz)->esq == NULL)
				(*raiz)->esq = criarNo(-1, -1);

			inserirNo(&(*raiz)->esq, data, indice + 1);
		}
		else
		{
			if ((*raiz)->dir == NULL)
				(*raiz)->dir = criarNo(-1, -1);

			inserirNo(&(*raiz)->dir, data, indice + 1);
		}
	}
}

void montarArvoreDeco(FILE *arquivo, Tree **raiz)
{
	Reg data;

	fread(&data, sizeof(Reg), 1, arquivo);
	while (!feof(arquivo))
	{
		inserirNo(&*raiz, data, 0);
		fread(&data, sizeof(Reg), 1, arquivo);
	}
}

void buscar(FILE *arquivo, int simbolo, char palavra[])
{
	Reg data;
	int lido;
	palavra[0] = '\0';
	fseek(arquivo, 0, 0);

	lido = fread(&data, sizeof(Reg), 1, arquivo);
	while (lido == 1 && data.simbolo != simbolo)
		lido = fread(&data, sizeof(Reg), 1, arquivo);

	if (lido == 1)
		strcpy(palavra, data.palavra);
	else
		printf("\nPalavra nao encontrada!\n");
}

void decodificarSimbolo(Tree *raiz, FILE *arquivo, char codigo[], int *indice, char palavra[])
{
	if (raiz != NULL)
	{
		if (raiz->simbolo != -1)
			buscar(arquivo, raiz->simbolo, palavra);
		else
		{
			if (codigo[*indice] == '0' && raiz->esq != NULL)
			{
				(*indice)++;
				decodificarSimbolo(raiz->esq, arquivo, codigo, indice, palavra);
			}
			else if (codigo[*indice] == '1' && raiz->dir != NULL)
			{
				(*indice)++;
				decodificarSimbolo(raiz->dir, arquivo, codigo, indice, palavra);
			}
		}
	}
}

void decodificar(Tree *raiz, FILE *arquivo, char codigo[], char frase[], int capacidade, int *sucesso)
{
	int indice = 0;
	char palavra[50];
	frase[0] = '\0';
	*sucesso = 1;

	while (codigo[indice] != '\0' && *sucesso)
	{
		palavra[0] = '\0';
		decodificarSimbolo(raiz, arquivo, codigo, &indice, palavra);
		if (palavra[0] == '\0' && codigo[indice] != '\0')
		{
			printf("Erro ao decodificar um simbolo da frase.\n");
			*sucesso = 0;
		}
		else if (strlen(frase) + strlen(palavra) >= capacidade)
		{
			printf("Frase decodificada excede o limite de %d bytes.\n", capacidade - 1);
			*sucesso = 0;
		}
		else
			strcat(frase, palavra);
	}
}

int main()
{
	FILE *file;
	Tree *raiz;
	char codigo[80001];
	char frase[10000];
	int sucesso;

	setlocale(LC_ALL, "C");
	SetConsoleOutputCP(CP_UTF8);

	file = fopen("dicionario.dat", "rb");
	raiz = NULL;
	montarArvoreDeco(file, &raiz);

	exibeArvore(raiz, 0);
	printf("\n\n");
	exibirArquivo(file);

	printf("\n\n");
	binarioParaString("codificado.dat", codigo, 80001, &sucesso);
	if (sucesso)
	{
		printf("Codigo para decodificar: %s\n\n", codigo);
		decodificar(raiz, file, codigo, frase, 10000, &sucesso);
		if (sucesso)
			printf("Frase: %s\n", frase);
	}

	return 0;
}
