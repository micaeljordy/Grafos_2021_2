#include "grafo.c"
#define SAIR 0
#define ADICIONARVERTICE 1
#define ADICIONARARESTA 2
#define GRAU 3
#define IMPRIMIR 4
#define SAOVIZINHOS 5
#define REMOVERVERTICE 6
#define REMOVERARESTA 7


void menuInterface()
{
    int acao;
    Grafo *grafo = criarGrafo();
    do
    {
        printf("Digite a sua ação:\n");
        printf("Sair -------------------------------> \"0\"\n");
        printf("Adicionar Vértice ------------------> \"1\"\n");
        printf("Adicionar Aresta -------------------> \"2\"\n");
        printf("Verificar Grau do Vérice -----------> \"3\"\n");
        printf("Imprimir Grafo ---------------------> \"4\"\n");
        printf("Verificar se Vértices são vizinhos -> \"5\"\n");
        printf("Remover Vértice --------------------> \"6\"\n");
        printf("Remover Aresta ---------------------> \"7\"\n");

        scanf("%d", &acao);

        Rotulo *rotulo;
        Rotulo *rotuloA;
        Rotulo *rotuloB;

        switch (acao)
        {
        case ADICIONARVERTICE:
            rotulo = (Rotulo*)calloc(10, sizeof(Rotulo));
            printf("Rótulo do vértice a ser criado: ");
            scanf(" %s", rotulo);
            inserirVertice(grafo, rotulo);
            imprimirGrafo(grafo);
            break;
        case ADICIONARARESTA:
            rotuloA = (Rotulo*)calloc(10, sizeof(Rotulo));
            rotuloB = (Rotulo*)calloc(10, sizeof(Rotulo));
            printf("Rótulo do primeiro vértice: ");
            scanf(" %s", rotuloA);
            printf("Rótulo do segundo vértice: ");
            scanf(" %s", rotuloB);
            inserirAresta(grafo, rotuloA, rotuloB);
            imprimirGrafo(grafo);
            break;
        case GRAU:
            rotulo = (Rotulo*)calloc(10, sizeof(Rotulo));
            printf("Rótulo do vértice: ");
            scanf(" %s", rotulo);
            printf("Grau: %d\n", grauVertice(rotulo, grafo));
            break;
        case IMPRIMIR:
            imprimirGrafo(grafo);
            break;
        case SAOVIZINHOS:
            rotuloA = (Rotulo*)calloc(10, sizeof(Rotulo));
            rotuloB = (Rotulo*)calloc(10, sizeof(Rotulo));
            printf("Rótulo do primeiro vértice: ");
            scanf(" %s", rotuloA);
            printf("Rótulo do segundo vértice: ");
            scanf(" %s", rotuloB);
            bool vizinhos = saoVizinhos(rotuloA, rotuloB, grafo);
            if(vizinhos) printf("SIM\n");
            else printf("NÃO\n");
            break;
        case REMOVERVERTICE:
            rotulo = (Rotulo*)calloc(10, sizeof(Rotulo));
            printf("Rótulo do vértice a ser excluído: ");
            scanf(" %s", rotulo);
            removerVertice(grafo, rotulo);
            imprimirGrafo(grafo);
            break;
        case REMOVERARESTA:
            rotuloA = (Rotulo*)calloc(10, sizeof(Rotulo));
            rotuloB = (Rotulo*)calloc(10, sizeof(Rotulo));
            printf("Rótulo do primeiro vértice: ");
            scanf(" %s", rotuloA);
            printf("Rótulo do segundo vértice: ");
            scanf(" %s", rotuloB);
            removerAresta(grafo, rotuloA, rotuloB, false);
            imprimirGrafo(grafo);
            break;
        case SAIR:
        break;
        default:
            printf("Comando inválido!\n");
            break;
        }
    } while (acao);
}