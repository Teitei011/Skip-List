#include "skiplist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TRUE 1

int seed = 0;

int random_result(int max)
{
    int fase = 1;
    while (rand() % 2)
        fase++;
    if (fase > max)
        return max;
    else
        return fase;

}

/* Ele retorma o nó header que vai "guiar" toda a skip list
    Esta skip list é feito de ponteiro que apontam para vetores, tipo:

    _     _    
   | | ->| | ->
   | | ->| | ->
   | | ->| | ->
   |_| ->|_| ->

   Nao é a forma mais eficiente.
*/

skip_t * criar_skip() 
{
    /* Colocando uma seed no srand*/

    if (!seed) 
    {
        srand(time(NULL));
        seed = 1;
    }

    /* Criando a skip list */

    skip_t * header = calloc(1, sizeof(skip_t));

    if (!header) 
        return NULL; 

    header->altura = altura_maxima;

    return header;
}

/* Liberando os nós da skip list*/

void liberar(skip_t * posicao) 
{
    free(posicao->verbete);
    posicao->verbete = NULL;

    free(posicao->definicao);
    posicao->definicao = NULL;

    free(posicao);
    posicao = NULL;
}


/* Limpando todos os nós da skip list */
void destruir(skip_t * header) 
{
    skip_t * aux_1 = header;
    skip_t * aux_2 = NULL;

    while (aux_1) /* Percorrendo toda a skip list e liberando os nós */
    {
        aux_2 = aux_1->prox[0];
        liberar(aux_1);
        aux_1 = aux_2;
    }
}


char * busca(skip_t * header, char * verbete) 
{
    skip_t * posicao = header;
    int level = header->altura - 1;

    
    /* Procurando a posicao do verbete*/

    while (posicao != NULL && level >= 0) 
    {
        if (posicao->prox[level] == NULL) 
            level--;
       
        else 
        {
            int criterio_de_comparacao = strcmp(posicao->prox[level]->verbete, verbete);

            if (criterio_de_comparacao == 0)  /* Encontramos o verbete na skip list*/
                return strdup(posicao->prox[level]->definicao);
            
            else if (criterio_de_comparacao > 0) /* Quando o proximo verbete é maior do que o que se procura */
                level--;

            else /* Quando o proximo verbete é menor do verbete da busca*/
                posicao = posicao->prox[level];
        }
    }
    /* Caso nao foi possivel encontra-lo*/
    return "OPERACAO INVALIDA";
}


/* 
    O programa da insercao acontece em dois passos, 
    1 - Ele procura onde deve ficar o verbete
    2 - Depois eles os insere
*/

int insercao(skip_t * header, char * verbete, char * definicao) 
{
    skip_t * ant[altura_maxima];
    skip_t * posicao = header;
    int level = header->altura - 1;

    /* Procurando onde o verbete deve ser alocado */
    while (posicao != NULL && level >= 0)
     {
        ant[level] = posicao;
        if (posicao->prox[level] == NULL) 
            level--;
        else 
        {
            int criterio_de_comparacao = strcmp(posicao->prox[level]->verbete, verbete);

            if (criterio_de_comparacao == 0)
            {  /* Foi encontrado o verbete ja na skip list, Portanto tem que dar erro*/
                printf("OPERACAO INVALIDA\n");
                return 1;
            }
            else if (criterio_de_comparacao > 0)  /* Quando o proximo verbete é maior do que o que se procura */
               level--;
            else
                posicao = posicao->prox[level]; /* Quando o proximo verbete é menor do verbete da busca*/
        }
    }

    /*
    Caso nao houver o verbete dentro da skip list para fazer a alteração, temos que criar e 
    inserir um novo nó
    */
  
    skip_t * novo_no = malloc(sizeof(skip_t));
   
    novo_no->altura = random_result(header->altura);
    novo_no->verbete = strdup(verbete);
    novo_no->definicao = strdup(definicao);
   
    int i;
   
    /* Colocando todos os ponteiros do prox como nulo */
    for (i = altura_maxima - 1; i > novo_no->altura; i--) 
        novo_no->prox[i] = NULL;

    /* Juntando com os outros ponteiros */
    for (i = novo_no->altura - 1; i >= 0; i--) 
    {
        novo_no->prox[i] = ant[i]->prox[i];
        ant[i]->prox[i] = novo_no;
    }

    return 0;
}



/*
    Remocao do verbete 
    Assim como no modulo de insercao ele primeiro procura o nó e somente depois o remove
*/


int remocao(skip_t * header, char * verbete) 
{
    skip_t * ant[altura_maxima];
    skip_t * posicao = header;

    int Verificador = 0;
    int level = header->altura - 1;
    int criterio_de_comparacao = 1;

    while (posicao != NULL && level >= 0) /* Procurando pela posicao */
    {
        ant[level] = posicao;

        if (posicao->prox[level] == NULL)
            level--;
        else 
        {
            criterio_de_comparacao = strcmp(posicao->prox[level]->verbete, verbete);

            if (criterio_de_comparacao >= 0) 
                level--;
            else 
                posicao = posicao->prox[level];
        }
    }

    /* Gambiarra temporária */

        char *retorno;
        retorno = busca(header, verbete);

        if (strcmp(retorno, "OPERACAO INVALIDA") == 0)
          printf("OPERACAO INVALIDA\n");
        else
          {
            free(retorno); /* Por causa do strdup*/
            retorno = NULL;
           }

    /* O proximo nó é o nó que esta sendo procurado */

    if (posicao && !criterio_de_comparacao) 
    { 
        skip_t * lixeira = posicao->prox[0];

        /* Eliminado o nó lixeira*/
        int i;
        for (i = lixeira->altura - 1; i >= 0; -- i) 
          ant[i]->prox[i] = lixeira->prox[i];

        liberar(lixeira);
        lixeira = NULL;

        return 0;
    }
}

/* O programa da alteracao usa o mesmo codigo para a insercao*/

int alteracao(skip_t * header, char * verbete, char * definicao) 
{
    skip_t * posicao = header;

    int level = header->altura - 1;

    if (posicao == NULL)
    {
        printf("OPERACAO INVALIDA\n");
        return 0;
    }

    /* Procurando onde o verbete deve ser alocado */
    while (posicao != NULL && level >= 0)
     {
        if (posicao->prox[level] == NULL) 
            level--;
        else 
        {
            int criterio_de_comparacao = strcmp(posicao->prox[level]->verbete, verbete);
            if (criterio_de_comparacao == 0)
                
            {  /* Foi encontrado o verbete ja na skip list, portanto vai ser alterado a sua definicao*/
                free(posicao->prox[level]->definicao);
                posicao->prox[level]->definicao = strdup(definicao);
                return 1;
            }
            else if (criterio_de_comparacao > 0)  /* Quando o proximo verbete é maior do que o que se procura */
               level--;
            else
                posicao = posicao->prox[level]; /* Quando o proximo verbete é menor do verbete da busca*/
        }
    }

    printf("OPERACAO INVALIDA\n");
    return 0;
}

int impressao(skip_t * header, char * verbete)
{
    int Verificador = 0; /* Para caso não nenhuma palavra iniciada com o caracter*/

    skip_t * posicao = header;
    int level = header->altura - 1;

    
    while (posicao != NULL && level > -1) 
    {
        if (posicao->prox[level] == NULL) 
            level--;
       
        else 
        {
           
            if (posicao->prox[level]->verbete[0] == *verbete)  /* Encontramos o verbete na skip list*/
            {
                printf("%s %s\n", posicao->prox[level]->verbete,posicao->prox[level]->definicao);
                Verificador = 1;
            }   
            posicao = posicao->prox[level];
        }
    }
    
    if (Verificador == 0)
        printf("OPERACAO INVALIDA\n");
    return 0;
}