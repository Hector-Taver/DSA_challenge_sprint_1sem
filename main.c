#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Estrutura base do dispositivo
struct Dispositivo {
  char nome[50];
  float consumo_estimado;
  int prioridade;
};

// Estrutura que será utilizada para alocar os dispositivos dinamicamente em um vetor
struct GerenciadorDeDispositivos {
  struct Dispositivo *dispositivos;
  int capacidade;
  int quantidade;
};

// Função para fazer a alocação dinâmica com malloc e com um parâmetro pra definir a capacidade inicial do vetor
void iniciarGerenciadorDeDispositivos(struct GerenciadorDeDispositivos *gerenciador, int capacidade_inicial) {
  gerenciador->dispositivos = malloc(capacidade_inicial * sizeof(struct Dispositivo));

  // Verificando se houve falha na alocação de memória
  if (gerenciador->dispositivos == NULL) {
    printf("Falha na alocação de memória");
    exit(1);
  }

  gerenciador->capacidade = capacidade_inicial;
  gerenciador->quantidade = 0;
}

// Função para redimensionar o vetor caso seja necessário
void redimensionador(struct GerenciadorDeDispositivos *gerenciador) {
  if (gerenciador->quantidade >= gerenciador->capacidade) {
    gerenciador->capacidade *= 2;
    gerenciador->dispositivos = realloc(gerenciador->dispositivos,
                                        gerenciador->capacidade * sizeof(struct Dispositivo));

    // Verificiando se houve falha na alocação de memória
    if (gerenciador->dispositivos == NULL) {
      printf("Falha na alocação de memória");
      exit(1);
    }
  }
}

// Função para liberar a memória alocada
void liberarMemoria(struct GerenciadorDeDispositivos *gerenciador) {
  free(gerenciador->dispositivos);
  gerenciador->dispositivos = NULL;
  gerenciador->capacidade = 0;
  gerenciador->quantidade = 0;
}

// Função para cadastrar dispositivos
void cadastrarDispositivo(struct GerenciadorDeDispositivos *gerenciador) {
  char buffer[50];

    printf("\n==============================================\n");
    printf("            CADASTRO DE DISPOSITIVO            \n");
    printf("==============================================\n");

    // Usando a função para redimensionar o vetor (caso seja necessário)
    redimensionador(gerenciador);

    struct Dispositivo *dispositivo = &gerenciador->dispositivos[gerenciador->quantidade];

    printf("\nNome do dispositivo: ");
    fgets(dispositivo->nome, sizeof(dispositivo->nome),stdin);
    // removendo o \n adicionado pelo fgets()
    dispositivo->nome[strcspn(dispositivo->nome, "\n")] = '\0';

    printf("\nConsumo estimado (em kWh): ");
    while(1) {
      fgets(buffer, sizeof(buffer), stdin);

      // Verificando se é possível encontrar um valor do tipo float, previnindo que o usuário digite um texto ao invés de um número
      if (sscanf(buffer, "%f", &dispositivo->consumo_estimado) == 1) {
        break;
      }
      printf("\nValor inválido! Informe um número positivo: ");
    }

    printf("\nPrioridade do dispositivos (de 1 a 3): ");
    while(1) {
      fgets(buffer, sizeof(buffer), stdin);

      // Verificnado se é possível encontrar um valor do tipo int e se o valor é maior ou igual à 1 e menor ou igual à 3, previnindo que o usuário digite um texto ou um número inválido
      if (sscanf(buffer, "%d", &dispositivo->prioridade) == 1 && (dispositivo->prioridade >= 1 && dispositivo->prioridade <= 3)) {
        break;
      }

      printf("\nPrioridade inválida! O nível de prioridade deve ser um número de 1 a 3: ");
    }

    gerenciador->quantidade++;
    printf("\n===== DISPOSITIVO CADASTRADO COM SUCESSO =====\n");
}

// Função para exibir os dispositivos cadastrados
void exibirDispositivos(const struct GerenciadorDeDispositivos *gerenciador) {
  printf("\n===============================================\n");
  printf("            DISPOSITIVOS CADASTRADOS            \n");
  printf("===============================================\n");

  // Verificando se há dispositivos cadastrados
  if(gerenciador->quantidade == 0) {
    printf("\nNenhum dispositivo cadastrado\n");
    return;
  }

  // Laço de repetição para mostrar os dispositivos cadastrados
  for (int i = 0; i < gerenciador->quantidade; i++) {
    printf("\nDispositivo %d\n", i + 1);
    printf("Nome: %s\n", gerenciador->dispositivos[i].nome);
    printf("Consumo estimado em kWh: %.2f\n", gerenciador->dispositivos[i].consumo_estimado);
    printf("Prioridade: %d\n", gerenciador->dispositivos[i].prioridade);

    // Mostrando uma mensagem diferente de acordo com a prioridade do dispositivo
    if (gerenciador->dispositivos[i].prioridade == 1) {
      printf("Dispositivo de alta prioridade\n");
    } else if (gerenciador->dispositivos[i].prioridade == 2) {
      printf("Dispositivo de prioridade média\n");
    } else {
      printf("Dispositivo de prioridade baixa\n");
    }
  }
}


int main() {
  int menu_option;
  struct GerenciadorDeDispositivos gerenciador;

  printf("\n============================================================\n");
  printf("            GERENCIADOR DE DISPOSITIVOS ELÉTRICOS            \n");
  printf("============================================================\n");

  // Usando a função para definir um tamanho inicial de 2 dispositivos para o vetor
  iniciarGerenciadorDeDispositivos(&gerenciador, 2);

  do {
    printf("\n1 - Cadastrar dispositivo\n");
    printf("2 - Exibir dispositivos cadastrados\n");
    printf("0 - Sair\n");

    scanf("%d", &menu_option);
    while(getchar() != '\n');

    switch (menu_option) {
      case 1:
        cadastrarDispositivo(&gerenciador);
        break;
      case 2:
        exibirDispositivos(&gerenciador);
        break;
      case 0:
        printf("Encerrando o sistema. Até breve!\n");
        break;
      default:
        printf("Opção inválida!\n");
        break;
    }
  } while (menu_option != 0);

  // Usando a função para liberar a memória alocada
  liberarMemoria(&gerenciador);
  return 0;
}
