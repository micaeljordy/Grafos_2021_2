#include "grafo.c"
#define SAIR 0
#define ADICIONARVERTICE 1
#define ADICIONARARESTA 2
#define GRAU 3
#define IMPRIMIR 4
#define SAOVIZINHOS 5
#define REMOVERVERTICE 6
#define REMOVERARESTA 7
#define TAMANHOROTULO 10


void menuInterface()
{
    int acao;
    Grafo *grafo = criarGrafo();
    do
    {
        printf("Digite a sua acao:\n");
        printf("Sair -------------------------------> \"0\"\n");
        printf("Adicionar Vertice ------------------> \"1\"\n");
        printf("Adicionar Aresta -------------------> \"2\"\n");
        printf("Verificar Grau do Verice -----------> \"3\"\n");
        printf("Imprimir Grafo ---------------------> \"4\"\n");
        printf("Verificar se Vertices sao vizinhos -> \"5\"\n");
        printf("Remover Vertice --------------------> \"6\"\n");
        printf("Remover Aresta ---------------------> \"7\"\n");
        printf("Insira o ID da operacao: ");
        scanf("%d", &acao);

        if(acao == ADICIONARVERTICE)
        {
            Rotulo rotulo[TAMANHOROTULO];
            printf("Rotulo do vertice a ser criado: ");
            scanf(" %s", rotulo);
            inserirVertice(grafo, rotulo);
            imprimirGrafo(grafo);
        }
        else if(acao == ADICIONARARESTA)
        {
            Rotulo rotuloA[TAMANHOROTULO];
            Rotulo rotuloB[TAMANHOROTULO];
            printf("Rotulo do primeiro vertice: ");
            scanf(" %s", rotuloA);
            printf("Rotulo do segundo vertice: ");
            scanf(" %s", rotuloB);
            inserirAresta(grafo, rotuloA, rotuloB);
            imprimirGrafo(grafo);
        }
        else if(acao == GRAU)
        {
            Rotulo rotulo[TAMANHOROTULO];
            printf("Rotulo do vertice: ");
            scanf(" %s", rotulo);
            printf("Grau: %d\n", grauVertice(rotulo, grafo));
        }
        else if(acao == IMPRIMIR)
        {
            imprimirGrafo(grafo);
        }
        else if(acao == SAOVIZINHOS)
        {
            Rotulo rotuloA[TAMANHOROTULO];
            Rotulo rotuloB[TAMANHOROTULO];
            printf("Rotulo do primeiro vertice: ");
            scanf(" %s", rotuloA);
            printf("Rotulo do segundo vertice: ");
            scanf(" %s", rotuloB);
            bool vizinhos = saoVizinhos(rotuloA, rotuloB, grafo);
            if(vizinhos) printf("SIM\n");
            else printf("NAO\n");
        }
        else if(acao == REMOVERVERTICE)
        {
            Rotulo rotulo[TAMANHOROTULO];
            printf("Rotulo do vertice a ser excluido: ");
            scanf(" %s", rotulo);
            removerVertice(grafo, rotulo, false);
            imprimirGrafo(grafo);
        }
        else if(acao == REMOVERARESTA)
        {
            Rotulo rotuloA[TAMANHOROTULO];
            Rotulo rotuloB[TAMANHOROTULO];
            printf("Rotulo do primeiro vertice: ");
            scanf(" %s", rotuloA);
            printf("Rotulo do segundo vertice: ");
            scanf(" %s", rotuloB);
            removerAresta(grafo, rotuloA, rotuloB, false);
            imprimirGrafo(grafo);
        }
        else if(acao == SAIR) printf("Programa encerrado.\n");
        else printf("Comando inválido!\n");
    } while (acao);
}