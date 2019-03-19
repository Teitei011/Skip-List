#define altura_maxima 15

typedef struct skip_t 
{
    int altura;
    char * verbete;
    char * definicao;
    struct skip_t * prox[altura_maxima];
} skip_t;

skip_t * criar_skip();
void destruir_skip(skip_t * header);
void destruir(skip_t * header);
char * busca(skip_t * header, char * verbete);
int insercao(skip_t * header, char * verbete, char * definicao);
int alteracao(skip_t * header, char * verbete, char * definicao);
int remocao(skip_t * header, char * verbete);
int impressao(skip_t * header, char * verbete); 

