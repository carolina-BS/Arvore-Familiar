/*
 * Trabalho 3: Árvore Familiar
 * autoras: Ana Carolina e Heloize Mafra
 * 
 * Instruções:
 *  O Arquivo que fornece as informações da familia deve estar formatado da seguinte forma:
 *    "Nome, Nome do cônjuge, Nome do pai/mãe";  
 *    Sendo o pai/mãe que possui ligação direta com a árvore;
 *    Caso algum dos campos não esteja preenchido, deve ser informado como "-";
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore.h"


void limparTela() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
  t_arvore a, aux;
  int nivel = 0;
  int opcao;
  define(&a);
  char arquivo[50], nome1[50], nome2[50];
  
    do 
    {
      limparTela();
      printf("\n=== Menu ===\n");
      printf("1. Ler dados de um arquivo\n");
      printf("2. Listar árvore genealógica de forma endentada\n");
      printf("3. Indicar parentesco entre duas pessoas\n");
      printf("4. Sair e gravar dados em arquivo\n");
      printf("Escolha uma opção: ");
      scanf("%d", &opcao);

      switch (opcao) 
      {
        case 1:
          printf("Digite o nome do arquivo: ");
          scanf("%s", arquivo);
          carregarArquivo(&a, arquivo);
          break;
        case 2:
          printf("\n");
          imprimeArvore(a, 0);
          break;
        case 3:
          printf("Digite o nome da primeira pessoa: ");
          scanf("%s", nome1);
          printf("Digite o nome da segunda pessoa: ");
          scanf("%s", nome2);
          verificarParentesco(a, nome1, nome2);
          break;
        case 4:
          printf("Digite o nome do arquivo para salvar: ");
          scanf("%s", arquivo);
          salvarEmArquivo(a, arquivo);
          printf("Saindo...\n");
          break;
        default:
          printf("Opção inválida. Tente novamente.\n");
      }
      if (opcao != 4) {
          printf("\nPressione Enter para continuar...");
          getchar();
          getchar();
      }
      
    } while (opcao != 4);
    

  return 0;

}
