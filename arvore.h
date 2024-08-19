#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
   char nome[40];
   char conjuge[40];
} pessoa;

typedef char* palavra;
const pessoa VL_NULO = {'\0'};

typedef struct nodo *ptrNodo; 
typedef ptrNodo t_arvore;

typedef struct nodo{
  pessoa info;
  ptrNodo 	primogenito;
  ptrNodo   irmao;
}no;


void define(t_arvore *a) { *a = NULL; }  
int vazia (t_arvore a) { return (a==NULL); }

//* Aloca memoria para um nodo e passa as informações do elemento *//
t_arvore novoNodo( pessoa P) {
  t_arvore no = (ptrNodo)malloc(sizeof(struct nodo));
  no->primogenito = NULL; 
  no->irmao = NULL; 
  no->info = P ;
  return no;
}

//* Função que insere as informações do elemento *//
pessoa novaPessoa( palavra nome, palavra conjuge){
  pessoa P; 
  strcpy(P.nome, nome);                        // Copia o nome
  strcpy(P.conjuge, conjuge);                  // Copia o nome do cônjuge 
  return P;
}


//* Função para adiciona filho a um no *//
void adicionarFilho(t_arvore raiz, pessoa P) 
{
  if (raiz == NULL) {
    // Se não encontrar o pai, mostra mensagem de erro
    printf("Pai não encontrado, impossivel adicionar filho.\n");   
    return ;
  }
  
  t_arvore novaPessoa = novoNodo(P);       // Cria uma nova pessoa
  if (raiz->primogenito == NULL)
    raiz->primogenito = novaPessoa;           // Adiciona como primeiro filho da raiz
  else {
      ptrNodo filho = raiz->primogenito;
      while (filho->irmao != NULL) {          // Percorre os irmãos até o último
        filho = filho->irmao;
      }
      filho->irmao = novaPessoa;              // Adiciona como próximo irmão
  }  
}


//* função recursiva para localizar no na arvore *//
ptrNodo localiza(t_arvore a, palavra nome)
{
  ptrNodo locesq, locdir;
  locesq = NULL;
  locdir = NULL;
  if ( strcmp(a->info.nome ,nome) != 0 )
  {  
      if (a->primogenito != NULL)
        locesq = localiza(a->primogenito,nome);
      if (locesq == NULL)
      {
        if (a->irmao != NULL)
          locdir = localiza(a->irmao,nome);
        if (locdir == NULL)
          return NULL;
        else
          return locdir;
      }
      else
        return locesq;
  }
  else
      return a;
}


//* Função para imprimir a arvore de forma endentada *//
void imprimeArvore(t_arvore a, int nivel) 
{ // o nivel iniciara sempre como 0
  if (a == NULL) return;
  for (int i = 0; i < nivel; i++){
     printf("   |");
  }
  if(strcmp(a->info.conjuge, "-") == 0){
    printf("%s\n", a->info.nome);
  }else{
    printf("%s, cônjuge:%s\n", a->info.nome, a->info.conjuge);
  }
  if(a->primogenito != NULL){
    imprimeArvore(a->primogenito, nivel+1); //caso a esquerda, por ser filho, nivel+1
  } 
  if(a->irmao != NULL){
    imprimeArvore(a->irmao, nivel); // caso a direita, por ser irmao, nivel se mantem
  }  
}





// Função para carregar de um arquivo, os dados da arvóre familiar 
void carregarArquivo(t_arvore *raiz, palavra nomeArquivo) {
  FILE* arquivo = fopen(nomeArquivo, "r");               // Abre o arquivo para leitura
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
    return;
  }

  char linha[240];
  while (fgets(linha, sizeof(linha), arquivo)) {         // Lê cada linha do arquivo
    char nome[50], conjuge[50], nomePai[50];
    int campos = sscanf(linha, "%[^,],%[^,],%s", nome, conjuge, nomePai);  // Lê os campos da linha
    if (campos == 3) 
    {
      if(strcmp(nomePai, "-") == 0 || strcmp(nomePai, " -") == 0)
        *raiz = novoNodo(novaPessoa(nome, conjuge));   //não há referencia pai
      else
        adicionarFilho(localiza(*raiz, nomePai), novaPessoa( nome, conjuge)); //procura o pai e adiciona
    }
    else
        printf("Linha mal formatada: [%s]. Ignorando.\n", linha); 
  }

  fclose(arquivo);                            
}


void salvarArvore(t_arvore a, FILE* arquivo,int  nivel) {
  if (a == NULL) return;

  for (int i = 0; i < nivel; i++){
     fprintf(arquivo, "   |");
  }
  if(strcmp(a->info.conjuge, "-") == 0){
    fprintf(arquivo, "%s\n", a->info.nome);
  }else{
    fprintf(arquivo, "%s, cônjuge:%s\n", a->info.nome, a->info.conjuge);
  }
  if(a->primogenito != NULL){
    salvarArvore(a->primogenito, arquivo, nivel+1); //caso a esquerda, por ser filho, nivel+1
  } 
  if(a->irmao != NULL){
      salvarArvore(a->irmao, arquivo, nivel); // caso a direita, por ser irmao, nivel se mantem
  } 
  
}

void salvarEmArquivo(t_arvore a, palavra nomeArquivo) {
  FILE* arquivo = fopen(nomeArquivo, "w");
  if (arquivo == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      return;
  }
  salvarArvore(a, arquivo, 0);
  fclose(arquivo);
  printf("\nÁrvore genealógica salva em %s.\n", nomeArquivo);
}

//verificar parentescos 

void verificarParentesco(t_arvore a, palavra nome1, palavra nome2){
  ptrNodo pessoa1 = localiza(a, nome1);
  ptrNodo pessoa2 = localiza(a, nome2);

//verificar se as pessoas existem no arquivo
  if(pessoa1 == NULL || pessoa2 == NULL){
    printf("Uma pessoa ou ambas não encontradas.\n");
    return;
  }

//verificar se as pessoas são a mesma pessoa
  if(pessoa1 == pessoa2){
    printf("As pessoas são a mesma pessoa.\n");
    return;
  }

//verificar se as pessoas são pai e filho
  ptrNodo temp = pessoa1->primogenito;
    while(temp != NULL){
      if(temp == pessoa2){
        printf("%s é filho(a) de %s.\n", pessoa2->info.nome, pessoa1->info.nome);
        return;
      }
      temp = temp->irmao;
    }
  
  temp = pessoa2->primogenito;
    while(temp != NULL){
      if(temp == pessoa1){
        printf("%s é filho(a) de %s.\n", pessoa1->info.nome, pessoa2->info.nome);
        return;
      }
      temp = temp->irmao;
    }

//verificar se as pessoas são irmãos

  temp = pessoa1->irmao;
    while(temp != NULL){
      if(temp == pessoa2){
        printf("%s é irmão(a) de %s.\n", pessoa2->info.nome, pessoa1->info.nome);
        return;
    }
    temp = temp->irmao;
  }

  temp = pessoa2->irmao;
    while(temp != NULL){
      if(temp == pessoa1){
        printf("%s é irmão(a) de %s.\n", pessoa2->info.nome, pessoa1->info.nome);
        return;
    }
    temp = temp->irmao;
  }

  //se não for nenhum dos casos de parentesco acima 
  printf("%s e %s não são parentes.\n", pessoa1->info.nome, pessoa2->info.nome);
}
