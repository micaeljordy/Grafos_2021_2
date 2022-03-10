#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char Rotulo;

typedef struct vertice
{
    Rotulo *rotulo;
} Vertice;

typedef struct listaVertice
{
    Vertice *vertice;
    struct listaVertice *proximo;
    struct listaVertice *anterior;
} ListaVertice;

typedef struct aresta
{
    Vertice *A;
    Vertice *B;
} Aresta;

typedef struct listaAresta
{
    Aresta *aresta;
    struct listaAresta *proximo;
    struct listaAresta *anterior;
} ListaAresta;

typedef struct grafo
{
    ListaVertice *V;
    ListaAresta *E;
} Grafo;

Grafo *criarGrafo()
{
    Grafo *g;
    g = (Grafo *)malloc(sizeof(Grafo));
    if (!g)
        return NULL;
    g->V = (ListaVertice *)malloc(sizeof(ListaVertice));
    if (!(g->V))
        return NULL;
    g->V->proximo = NULL;
    g->V->anterior = NULL;
    g->V->vertice = NULL;
    g->E = (ListaAresta *)malloc(sizeof(ListaAresta));
    if (!(g->E))
        return NULL;
    g->E->proximo = NULL;
    g->E->anterior = NULL;
    g->E->aresta = NULL;
    return g;
}

bool rotulosIguais(Rotulo *valorA, Rotulo *valorB)
{
    return !strcmp(valorA, valorB);
}

ListaVertice *buscarVertice(Rotulo *valor, Grafo *grafo)
{
    if (!grafo)
        return NULL;
    ListaVertice *V = grafo->V;
    if (rotulosIguais(valor, "_"))
        return V;
    while (V)
    {
        if (V->vertice)
        {
            if (rotulosIguais(V->vertice->rotulo, valor))
                return V;
        }
        V = V->proximo;
    }
    return NULL;
}

Vertice *inserirVertice(Grafo *grafo, Rotulo *valor)
{
    if ((!grafo) || (buscarVertice(valor, grafo)))
        return NULL;
    Vertice *vertice;
    vertice = (Vertice *)malloc(sizeof(Vertice));
    if (!vertice)
        return NULL;
    Rotulo *rotulo = (Rotulo *)calloc(sizeof(valor) / sizeof(Rotulo), sizeof(Rotulo));
    strcpy(rotulo, valor);
    vertice->rotulo = rotulo;

    if (!(grafo->V->vertice))
    {
        grafo->V->vertice = vertice;
        return vertice;
    }

    ListaVertice *V = grafo->V;
    while (V)
    {
        if (!(V->proximo))
        {
            V->proximo = (ListaVertice *)malloc(sizeof(ListaVertice));
            if (!(V->proximo))
                return NULL;
            V->proximo->vertice = vertice;
            V->proximo->proximo = NULL;
            V->proximo->anterior = V;
            V = NULL;
        }
        else
            V = V->proximo;
    }
    return vertice;
}

ListaAresta *buscarAresta(Rotulo *valorA, Rotulo *valorB, Grafo *grafo)
{
    if (!grafo)
        return false;
    ListaAresta *E = grafo->E;
    while (E)
    {
        if (E->aresta)
        {
            if ((rotulosIguais(E->aresta->A->rotulo, valorA)) || (rotulosIguais("_", valorA)))
            {
                if (rotulosIguais(E->aresta->B->rotulo, valorB) || (rotulosIguais("_", valorB)))
                {
                    return E;
                }
            }
        }
        E = E->proximo;
    }
    return NULL;
}

bool removerAresta(Grafo *grafo, Rotulo *valorA, Rotulo *valorB, bool recursivo)
{
    if (!grafo)
        return false;
    if (!(grafo->E->aresta))
        return false;

    ListaAresta *alvo = buscarAresta(valorA, valorB, grafo);
    if (!alvo)
        return false;
    if ((alvo->proximo))
        alvo->proximo->anterior = alvo->anterior;
    if ((alvo->anterior))
        alvo->anterior->proximo = alvo->proximo;
    else if (alvo->proximo)
        grafo->E = alvo->proximo;
    free(alvo->aresta);
    alvo->aresta = NULL;
    if (alvo->anterior)
        free(alvo);
    if (recursivo)
        removerAresta(grafo, valorA, valorB, true);
    return true;
}

bool removerVertice(Grafo *grafo, Rotulo *valor, bool recursivo)
{
    if (!grafo)
        return false;
    if (!(grafo->V->vertice))
        return false;

    ListaVertice *alvo = buscarVertice(valor, grafo);
    if (!alvo)
        return false;
    removerAresta(grafo, valor, "_", true);
    removerAresta(grafo, "_", valor, true);
    if (alvo->proximo)
        alvo->proximo->anterior = alvo->anterior;
    if (alvo->anterior)
        alvo->anterior->proximo = alvo->proximo;
    else if (alvo->proximo)
        grafo->V = alvo->proximo;
    free(alvo->vertice->rotulo);
    free(alvo->vertice);
    alvo->vertice = NULL;
    if (alvo->anterior)
        free(alvo);
    if (recursivo)
        removerVertice(grafo, valor, true);
    return true;
}

Aresta *inserirAresta(Grafo *grafo, Rotulo *valorA, Rotulo *valorB)
{
    if (!grafo)
        return NULL;
    Aresta *aresta;
    aresta = (Aresta *)malloc(sizeof(Aresta));
    if (!aresta)
        return NULL;
    ListaVertice *A = buscarVertice(valorA, grafo);
    ListaVertice *B = buscarVertice(valorB, grafo);
    if(!A || !B)
    {
        free(aresta);
        return NULL;
    }
    aresta->A = A->vertice;
    aresta->B = B->vertice;
    if (!(aresta->A) || !(aresta->B))
    {
        free(aresta);
        return NULL;
    }

    if (!(grafo->E->aresta))
    {
        grafo->E->aresta = aresta;
        return aresta;
    }

    ListaAresta *E = grafo->E;
    while (E)
    {
        if (!(E->proximo))
        {
            E->proximo = (ListaAresta *)malloc(sizeof(ListaAresta));
            if (!(E->proximo))
                return NULL;
            E->proximo->aresta = aresta;
            E->proximo->proximo = NULL;
            E->proximo->anterior = E;
            E = NULL;
        }
        else
            E = E->proximo;
    }
    return aresta;
}

Grafo *copiarGrafo(Grafo *grafo)
{
    if (!grafo)
        return NULL;
    Grafo *copia = criarGrafo();
    ListaVertice *V = grafo->V;
    while (V)
    {
        if (V->vertice)
            inserirVertice(copia, V->vertice->rotulo);
        V = V->proximo;
    }
    ListaAresta *E = grafo->E;
    while (E)
    {
        if (E->aresta)
            inserirAresta(copia, E->aresta->A->rotulo, E->aresta->B->rotulo);
        E = E->proximo;
    }
    return copia;
}

bool excluirGrafo(Grafo *grafo)
{
    if (!grafo)
        return false;
    removerVertice(grafo, "_", true);
    free(grafo->E);
    free(grafo->V);
    free(grafo);
    return true;
}

int grauVertice(Rotulo *valor, Grafo *grafo)
{
    Grafo *copia = copiarGrafo(grafo);
    ListaVertice *aux = copia->V;
    int grau = 0;
    while (aux)
    {
        if (removerAresta(copia, valor, "_", false))
            grau++;
        if (removerAresta(copia, "_", valor, false))
            grau++;
        aux = aux->proximo;
    }
    excluirGrafo(copia);
    return grau;
}

ListaAresta *saoVizinhos(Rotulo *valorA, Rotulo *valorB, Grafo *grafo)
{
    return buscarAresta(valorA, valorB, grafo);
}

Grafo *ehEuleriano(Grafo *grafo)
{
    Grafo *copia = copiarGrafo(grafo);
    Grafo *ciclos = criarGrafo();
    ListaVertice *V = copia->V;
    while (V->vertice)
    {
        inserirVertice(ciclos, V->vertice->rotulo);
        V = V->proximo;
    }
    V = copia->V;
    ListaVertice *verticeApontado = V;
    ListaAresta *E;
    while (V->vertice)
    {
        if (verticeApontado)
        {
            if (E = saoVizinhos(V->vertice->rotulo, verticeApontado->vertice->rotulo, copia))
                V = verticeApontado;
            if (E)
            {
                if (rotulosIguais(E->aresta->A->rotulo, E->aresta->B->rotulo))
                    V = NULL;
                removerAresta(copia, E->aresta->A->rotulo, E->aresta->B->rotulo, false);
                inserirAresta(ciclos, E->aresta->A->rotulo, E->aresta->B->rotulo);
            }
            verticeApontado = verticeApontado->proximo;
        }
        else
        {
            excluirGrafo(copia);
            excluirGrafo(ciclos);
            return NULL;
        }
    }
    while (copia->E->aresta)
    {
        V = copia->V;
        while (V->vertice)
        {
            if (!grauVertice(V->vertice->rotulo, copia))
                V = V->proximo;
        }
        verticeApontado = V;
        while (V->vertice)
        {
            if (verticeApontado)
            {
                if (E = saoVizinhos(V->vertice->rotulo, verticeApontado->vertice->rotulo, copia))
                    V = verticeApontado;
                if (E)
                {
                    if (rotulosIguais(E->aresta->A->rotulo, E->aresta->B->rotulo))
                        V = NULL;
                    removerAresta(copia, E->aresta->A->rotulo, E->aresta->B->rotulo, false);
                    inserirAresta(ciclos, E->aresta->A->rotulo, E->aresta->B->rotulo);
                }
                verticeApontado = verticeApontado->proximo;
            }
            else
            {
                excluirGrafo(copia);
                excluirGrafo(ciclos);
                return NULL;
            }
        }
    }
    return ciclos;
}

bool excluirArrayStrings(char ** array, int tamanho)
{
    if(!array || (tamanho == 0)) return false;
    int i = 0;
    for(i; i < tamanho; i++)
    {
        free(array[i]);
    }
    free(array);
}

bool carregarArquivoJson(Grafo *grafo, char *endereco)
{
    FILE *arquivo;
    if (!(arquivo = fopen(endereco, "r")))
    {
        printf("NOT FOUND FILE!\n");
        return false;
    }
    char ch;
    int i = 2;
    char* stringAux = (char*) calloc(i, sizeof(char));

    do
    {
        ch = fgetc(arquivo);
        if(ch != EOF)
        {
            stringAux[i - 2] = ch;
            i++;
            stringAux = (char*) realloc(stringAux, sizeof(char)*i);
        }
        else
        {
            printf("Erro, arquivo esta com formatacao invalida!\n");
            free(stringAux);
            excluirGrafo(grafo);
            return false;
        }
    }
    while (!strstr(stringAux, "VERTICES"));

    i = 2;
    free(stringAux);
    stringAux = (char*) calloc(i, sizeof(char));

    while((ch = fgetc(arquivo)) != '[')
    {
        if(ch == EOF) 
        {
            printf("Erro, arquivo esta com formatacao invalida!\n");
            excluirGrafo(grafo);
            return false;
        }
    }

    do
    {
        ch = fgetc(arquivo);
        if(ch == EOF)
        {
            printf("Erro, arquivo esta com formatacao invalida!\n");
            free(stringAux);
            excluirGrafo(grafo);
            return false;
        }
        if(ch == '\"')
        {
            do
            {
                if(ch != '\"')
                {
                    stringAux[i - 2] = ch;
                    i++;
                    stringAux = (char*) realloc(stringAux, sizeof(char)*i);
                    stringAux[i - 2] = 0;
                }
                ch = fgetc(arquivo);
            }
            while ((ch != '\"') && (ch != ']'));
            if(strstr(stringAux, "VERTICES") || strstr(stringAux, "ARESTAS")) 
            {
                printf("Erro, rotulo invalido!\n");
                free(stringAux);
                excluirGrafo(grafo);
                return false;
            }
            if(!inserirVertice(grafo, stringAux))
            {
                printf("Erro, nao foi possivel inserir vertice: %s!\n", stringAux);
                free(stringAux);
                excluirGrafo(grafo);
                return false;
            }
            i = 2;
            free(stringAux);
            stringAux = (char*) calloc(i, sizeof(char));
        }
    } while (ch != ']');
    
    i = 2;
    free(stringAux);
    stringAux = (char*) calloc(i, sizeof(char));
    rewind(arquivo);
    do
    {
        ch = fgetc(arquivo);
        if(ch != EOF)
        {
            stringAux[i - 2] = ch;
            i++;
            stringAux = (char*) realloc(stringAux, sizeof(char)*i);
        }
        else
        {
            printf("Erro, arquivo esta com formatacao invalida!\n");
            free(stringAux);
            excluirGrafo(grafo);
            return false;
        }
    }
    while (!strstr(stringAux, "ARESTAS"));

    i = 2;
    free(stringAux);
    stringAux = (char*) calloc(i, sizeof(char));

    while((ch = fgetc(arquivo)) != '[')
    {
        if(ch == EOF)
        {
            printf("Erro, arquivo esta com formatacao invalida!\n");
            free(stringAux);
            excluirGrafo(grafo);
            return false;
        }
    }
    int j = 1;
    char **vertices = (char**) malloc(sizeof(char*)*j);
    do
    {
        ch = fgetc(arquivo);
        if(ch == EOF)
        {
            printf("Erro, arquivo esta com formatacao invalida!\n");
            free(stringAux);
            excluirArrayStrings(vertices, j);
            excluirGrafo(grafo);
            return false;
        }
        if(ch == '\"')
        {
            do
            {
                if(ch != '\"')
                {
                    stringAux[i - 2] = ch;
                    i++;
                    stringAux = (char*) realloc(stringAux, sizeof(char)*i);
                    stringAux[i - 2] = 0;
                }
                ch = fgetc(arquivo);
            }
            while ((ch != '\"') && (ch != ']'));
            if(strstr(stringAux, "VERTICES") || strstr(stringAux, "ARESTAS")) 
            {
                printf("Erro, rotulo invalido!\n");
                free(stringAux);
                excluirArrayStrings(vertices, j);
                excluirGrafo(grafo);
                return false;
            }
            vertices[j - 1] = stringAux;
            j++;
            vertices = (char**) realloc(vertices, j*sizeof(char*));
            vertices[j - 1] = NULL;
            i = 2;
            stringAux = (char*) calloc(i, sizeof(char));
        }
    } while (ch != ']');
    j--;
    i = 2;
    free(stringAux);
    if(j%2 != 0)
    {
        printf("Erro, existe aresta com falta de vertice!\n");
        excluirArrayStrings(vertices, j);
        excluirGrafo(grafo);
        return false;
    }
    for(i; i < j + 2; i = i + 2)
    {
        if(vertices[i - 2] && vertices[i - 1])
        {
            if(!inserirAresta(grafo, vertices[i - 2], vertices[i - 1]))
            {
                printf("Erro ao criar aresta (%s,%s)! O grafo possui estes vertices?\n", vertices[i - 2], vertices[i - 1]);
                excluirArrayStrings(vertices, j);
                excluirGrafo(grafo);
                return false;
            }
        }
        else
        {
            printf("Erro, vetor de caracteres aponta para NULL!\n");
            excluirArrayStrings(vertices, j);
            excluirGrafo(grafo);
            return false;
        }
    }
    excluirArrayStrings(vertices, j);
    fclose(arquivo);
    return true;
}

void imprimirGrafo(Grafo *grafo)
{
    ListaVertice *listaVertice = grafo->V;
    printf("{");
    while (listaVertice)
    {
        if (listaVertice->vertice)
        {
            printf("%s", listaVertice->vertice->rotulo);
            if (listaVertice->proximo)
                printf(" ");
        }
        listaVertice = listaVertice->proximo;
    }
    printf("}\n");

    ListaAresta *listaAresta = grafo->E;
    printf("{");
    while (listaAresta)
    {
        if (listaAresta->aresta)
        {
            printf("(%s %s)", listaAresta->aresta->A->rotulo, listaAresta->aresta->B->rotulo);
            if (listaAresta->proximo)
                printf(" ");
        }
        listaAresta = listaAresta->proximo;
    }
    printf("}\n");
}