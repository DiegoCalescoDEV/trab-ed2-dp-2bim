typedef struct
{
	int simbolo;
	char palavra[50];
	char codHuffman[50];
} Reg;

typedef struct listaPalavra
{
	int simbolo;
	int freq;
	char palavra[50];
	char codHuffman[50];
	struct listaPalavra *prox;
} ListaPalavra;

ListaPalavra *novaPalavra(int simbolo, char palavra[])
{
	ListaPalavra *novo = (ListaPalavra *)malloc(sizeof(ListaPalavra));

	novo->simbolo = simbolo;
	novo->freq = 1;
	strcpy(novo->palavra, palavra);
	novo->codHuffman[0] = '\0';
	novo->prox = NULL;

	return novo;
}

void inicializarListaPalavra(ListaPalavra **l)
{
	*l = NULL;
}

char caractereEspecial(char letra)
{
	return (letra == ' ' ||
			(letra >= '!' && letra <= '/') ||
			(letra >= ':' && letra <= '@') ||
			(letra >= '[' && letra <= '`') ||
			(letra >= '{' && letra <= '~'));
}

void somarFrequenciaPalavra(ListaPalavra **l, char palavra[])
{
	if (*l == NULL)
		*l = novaPalavra(0, palavra);
	else
	{
		ListaPalavra *aux = *l;

		while (aux->prox != NULL && strcmp(aux->palavra, palavra) != 0)
			aux = aux->prox;

		if (strcmp(aux->palavra, palavra) == 0)
			aux->freq++;
		else
			aux->prox = novaPalavra(aux->simbolo + 1, palavra);
	}
}

void processarTexto(ListaPalavra **l, char texto[])
{
	char vetTexto[5000];
	char vetSeparador[2];
	int i = 0, j = 0;

	vetSeparador[1] = '\0';

	while (i <= strlen(texto))
	{
		vetTexto[j++] = tolower((unsigned char)texto[i]);

		if (caractereEspecial(texto[i]) || texto[i] == '\0')
		{
			if (j > 1)
			{
				vetTexto[j - 1] = '\0';
				somarFrequenciaPalavra(l, vetTexto);
			}

			if (texto[i] == ' ')
			{
				vetSeparador[0] = texto[i];
				somarFrequenciaPalavra(l, vetSeparador);
			}

			j = 0;
		}
		i++;
	}
}

ListaPalavra *removerPalavra(ListaPalavra **l)
{
	ListaPalavra *retirado;
	retirado = *l;
	*l = (*l)->prox;
	retirado->prox = NULL;

	return retirado;
}

void exibeListaPalavra(ListaPalavra *l)
{
	ListaPalavra *aux = l;
	if (l != NULL)
	{
		while (aux != NULL)
		{
			printf("Simbolo: %d\tFrequencia: %d\tPalavra: %-20s\t Codigo: %s\n", aux->simbolo, aux->freq, aux->palavra, aux->codHuffman);
			aux = aux->prox;
		}
	}
}
