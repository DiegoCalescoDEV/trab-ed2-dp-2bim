typedef struct tree {
	int simbolo;
	int freq;
	struct tree *esq, *dir;
} Tree;

typedef struct listaTree {
	Tree *no;
	struct listaTree *prox;
} ListaTree;

Tree *criarNo(int simbolo, int freq)
{
    Tree *no = (Tree*)malloc(sizeof(Tree));
    no->simbolo = simbolo;
    no->freq = freq;
    no->esq = no->dir = NULL;
    return no;
}

void inserirListaTree(ListaTree **lista, Tree *no)
{
    ListaTree *novo = (ListaTree*)malloc(sizeof(ListaTree));
    novo->no = no;
    novo->prox = NULL;
    
    if(*lista == NULL || no->freq < (*lista)->no->freq)
    {
        novo->prox = *lista;
        *lista = novo;
    }
    else
    {
        ListaTree *aux = *lista;
        while(aux->prox != NULL && aux->prox->no->freq <= no->freq)
            aux = aux->prox;
        novo->prox = aux->prox;
        aux->prox = novo;
    }
}

void preencherListaTree(ListaTree **lista, ListaPalavra *l)
{
    while(l != NULL)
    {
        inserirListaTree(lista, criarNo(l->simbolo, l->freq));
        l = l->prox;
    }
}

Tree *removerListaTree(ListaTree **lista)
{
    ListaTree *retirado = *lista;
    Tree *no = retirado->no;
    *lista = retirado->prox;
    free(retirado);
    return no;
}

Tree *montarArvore(ListaTree **lista)
{
    Tree *esq, *dir, *novo;
    
    while(*lista != NULL && (*lista)->prox != NULL)
    {
        esq = removerListaTree(lista);
        dir = removerListaTree(lista);
        
        novo = criarNo(-1, esq->freq + dir->freq);
        novo->esq = esq;
        novo->dir = dir;
        
        inserirListaTree(lista, novo);
    }
    
    return (*lista)->no;
}

void exibeArvore(Tree *raiz, int nivel)
{
    int i;
    if(raiz != NULL)
    {
        exibeArvore(raiz->dir, nivel + 1);
        printf("\n");
        
        for(i = 0; i < nivel; i++)
            printf("\t");
        
        if(raiz->esq == NULL && raiz->dir == NULL)
            printf("--> [%d]", raiz->simbolo);
        else
            printf("--> %d", raiz->simbolo);

        exibeArvore(raiz->esq, nivel + 1);
    }
}

int alturaArvore(Tree *raiz)
{
    int esq, dir;
    if(raiz == NULL)
        return -1;
    
    esq = alturaArvore(raiz->esq) + 1;
    dir = alturaArvore(raiz->dir) + 1;
    
    return esq > dir ? esq : dir;
}

void gravarCodHuffman(ListaPalavra *l, int simbolo, char codHuffman[])
{
    while(l != NULL && l->simbolo != simbolo)
        l = l->prox;
    
    if(l != NULL)
        strcpy(l->codHuffman, codHuffman);
}

void gerarCodHuffman(ListaPalavra *l, Tree *raiz, char caminho[], int colunas)
{
    char esquerda[colunas], direita[colunas];
    
    if(raiz != NULL)
    {
	    if(raiz->esq == NULL && raiz->dir == NULL)
	        gravarCodHuffman(l, raiz->simbolo, caminho);
	    else
	    {
	    	strcpy(esquerda, caminho);
		    strcpy(direita, caminho);
		    strcat(esquerda, "0");
		    strcat(direita, "1");
		    
		    gerarCodHuffman(l, raiz->esq, esquerda, colunas);
		    gerarCodHuffman(l, raiz->dir, direita, colunas);
		}
	}
}
