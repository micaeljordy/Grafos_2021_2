#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char Rotulo;

typedef struct vertice
{
    Rotulo *rotulo;
}
Vertice;

typedef struct listaVertice
{
    Vertice *vertice;
    struct listaVertice *proximo;
    struct listaVertice *anterior;
}
ListaVertice;

typedef struct aresta
{
    Vertice* A;
    Vertice* B;
}
Aresta;

typedef struct listaAresta
{
    Aresta *aresta;
    struct listaAresta *proximo;
    struct listaAresta *anterior;
}
ListaAresta;

typedef struct grafo
{
    ListaVertice *V;
    ListaAresta *E;
}
Grafo;

Grafo * criarGrafo()
{
    Grafo * g;
    g = (Grafo*) malloc(sizeof(Grafo));
    if(!g) return NULL;
    g->V = (ListaVertice*) malloc(sizeof(ListaVertice));
    if(!(g->V)) return NULL;
    g->V->proximo = NULL;
    g->V->anterior = NULL;
    g->V->vertice = NULL;
    g->E = (ListaAresta*) malloc(sizeof(ListaAresta));
    if(!(g->E)) return NULL;
    g->E->proximo = NULL;
    g->E->anterior = NULL;
    g->E->aresta = NULL;
    return g; 
}

ListaVertice * buscarVertice(Rotulo *valor, Grafo *grafo)
{
    if(!grafo) return NULL;
    ListaVertice *V = grafo->V;
    while (V)
    {
        if(V->vertice)
        {
            if(!strcmp(V->vertice->rotulo, valor)) return V;
        }
        V = V->proximo;
    }
    return NULL;
}

Vertice * inserirVertice(Grafo *grafo, Rotulo *valor)
{
    if((!grafo) || (buscarVertice(valor, grafo))) return NULL;
    Vertice *vertice;
    vertice = (Vertice*) malloc(sizeof(Vertice));
    if(!vertice) return NULL;
    vertice->rotulo = valor;
    
    if(!(grafo->V->vertice)) 
    {
        grafo->V->vertice = vertice;
        return vertice;
    }

    ListaVertice *V = grafo->V;
    while (V)
    {
        if(!(V->proximo))
        {
            V->proximo = (ListaVertice*) malloc(sizeof(ListaVertice));
            if(!(V->proximo)) return NULL;
            V->proximo->vertice = vertice;
            V->proximo->proximo = NULL;
            V->proximo->anterior = V;
            V = NULL;
        }
        else V = V->proximo;
    }
    return vertice;
}

ListaAresta * buscarAresta(Rotulo *valorA, Rotulo *valorB, Grafo *grafo)
{
    if(!grafo) return false;
    ListaAresta *E = grafo->E;
    while (E)
    {
        if(E->aresta)
        {
            if((!strcmp(E->aresta->A->rotulo, valorA)) || (!strcmp("_", valorA)))
            {
                if(!strcmp(E->aresta->B->rotulo, valorB) || (!strcmp("_", valorB)))
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
    if(!grafo) return false;
    if(!(grafo->E->aresta)) return false;

    ListaAresta *alvo = buscarAresta(valorA, valorB, grafo);
    if(!alvo) return false;
    if((alvo->proximo)) alvo->proximo->anterior = alvo->anterior;
    if((alvo->anterior)) alvo->anterior->proximo = alvo->proximo;
    else if(alvo->proximo) grafo->E = alvo->proximo;
    free(alvo->aresta);
    alvo->aresta = NULL;
    if(alvo->anterior)free(alvo);
    if(recursivo) return removerAresta(grafo, valorA, valorB, true);
    return true;
}

bool removerVertice(Grafo *grafo, Rotulo *valor)
{
    if(!grafo) return false;
    if(!(grafo->V->vertice)) return false;

    ListaVertice *alvo = buscarVertice(valor, grafo);
    if(!alvo) return false;
    removerAresta(grafo, valor, "_", true);
    removerAresta(grafo, "_", valor, true);
    if(alvo->proximo) alvo->proximo->anterior = alvo->anterior;
    if(alvo->anterior) alvo->anterior->proximo = alvo->proximo;
    else if(alvo->proximo) grafo->V = alvo->proximo;
    free(alvo->vertice->rotulo);
    free(alvo->vertice);
    alvo->vertice = NULL;
    if(alvo->anterior)free(alvo);
    return true;
}

Aresta * inserirAresta(Grafo *grafo, Rotulo *valorA, Rotulo *valorB)
{
    if(!grafo) return NULL;
    Aresta* aresta;
    aresta = (Aresta*) malloc(sizeof(Aresta));
    if(!aresta) return NULL;
    aresta->A = buscarVertice(valorA, grafo)->vertice;
    aresta->B = buscarVertice(valorB, grafo)->vertice;
    if(!(aresta->A) || !(aresta->B))
    {
        free(aresta);
        return NULL;
    }
    
    if(!(grafo->E->aresta)) 
    {
        grafo->E->aresta = aresta;
        return aresta;
    }

    ListaAresta *E = grafo->E;
    while (E)
    {
        if(!(E->proximo))
        {
            E->proximo = (ListaAresta*) malloc(sizeof(ListaAresta));
            if(!(E->proximo)) return NULL;
            E->proximo->aresta = aresta;
            E->proximo->proximo = NULL;
            E->proximo->anterior = E;
            E = NULL;
        }
        else E = E->proximo;
    }
    return aresta;
}

int grauVertice(Rotulo *valor, Grafo *grafo)
{
    ListaVertice *aux = grafo->V;
    int grau = 0;
    while (aux)
    {
        if(buscarAresta(valor, aux->vertice->rotulo, grafo)) grau++;
        if(buscarAresta(aux->vertice->rotulo, valor, grafo)) grau++;

        aux = aux->proximo; 
    }
    return grau;
}

bool saoVizinhos(Rotulo *valorA, Rotulo *valorB, Grafo *grafo)
{
    return buscarAresta(valorA, valorB, grafo);
}

void imprimirGrafo(Grafo *grafo)
{
    ListaVertice *listaVertice = grafo->V;
    printf("{");
    while (listaVertice)
    {
        if(listaVertice->vertice)
        {
            printf("%s", listaVertice->vertice->rotulo);
            if(listaVertice->proximo) printf(" ");
        }
        listaVertice = listaVertice->proximo;
    }
    printf("}\n");
    
    ListaAresta *listaAresta = grafo->E;
    printf("{");
    while (listaAresta)
    {
        if(listaAresta->aresta)
        {
            printf("(%s %s)", listaAresta->aresta->A->rotulo, listaAresta->aresta->B->rotulo);
            if(listaAresta->proximo) printf(" ");
        }
        listaAresta = listaAresta->proximo;
    }
    printf("}\n");
}