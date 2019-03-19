#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skiplist.h"

int main(void)
{
	char *line, *op, *arg1, *arg2;
	size_t buf_size = 210;

    skip_t* var = criar_skip();

	line = (char *)malloc(sizeof(char) * buf_size);
	op = (char *)malloc(sizeof(char) * buf_size);
	arg1 = (char *)malloc(sizeof(char) * buf_size);
	arg2 = (char *)malloc(sizeof(char) * buf_size);  
	
	while(getline(&line, &buf_size, stdin) != -1)
	{
    int verificador = 3;

		sscanf(line, "%s %s %[^\n]", op, arg1, arg2);
     
       if (strcmp(op,"remocao") == 0)
           remocao(var, arg1); /* */

      else if(strcmp(op,"impressao") == 0)
       		   impressao(var, arg1); 

       else if (strcmp(op, "busca") == 0)
       {
        char *retorno;
        retorno = busca(var, arg1);

        if (strcmp(retorno, "OPERACAO INVALIDA") == 0)
          printf("OPERACAO INVALIDA\n");
        else
          {
            printf("%s %s\n", arg1, retorno);

            free(retorno); /* Por causa do strdup*/
            retorno = NULL;
       
          }
       }
           
      else if (strcmp(op, "insercao") == 0)
          insercao(var, arg1, arg2);
 	    
       else if (strcmp(op,"alteracao") == 0)
         alteracao(var, arg1, arg2);
       
       else
        	printf("OPERACAO INVALIDA\n"); /* Caso nao há a funcao que foi digitada */
       
    }

  destruir(var); /* Destruindo a skip list */

	free(line);
	free(op);
	free(arg1);
	free(arg2);

	return EXIT_SUCCESS;
}