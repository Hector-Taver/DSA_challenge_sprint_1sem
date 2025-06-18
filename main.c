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

// Estrutura para guardar o resultado das recomendações
struct ResultadoRecomendacao {
  int *dispositivos_ligados;
  int *dispositivos_desligados;
  int quantidade_ligados;
  int quantidade_desligados;
  float consumo_total;
  float energia_restante;
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

// Função para inicializar o resultado da recomendação
void iniciarResultadoRecomendacao(struct ResultadoRecomendacao *resultado, int capacidade) {
  resultado->dispositivos_ligados = malloc(capacidade * sizeof(int));
  resultado->dispositivos_desligados = malloc(capacidade * sizeof(int));
  resultado->quantidade_ligados = 0;
  resultado->quantidade_desligados =0;
  resultado->consumo_total=0.0;
  resultado->energia_restante=0.0;
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

// Função para liberar a memória alocada do gerenciador de dispositivos
void liberarMemoriaGerendiadorDeDispositivos(struct GerenciadorDeDispositivos *gerenciador) {
  free(gerenciador->dispositivos);
  gerenciador->dispositivos = NULL;
  gerenciador->capacidade = 0;
  gerenciador->quantidade = 0;
}

// Função para liberar a memória alocada do sistema de recomendações
void liberarMemoriaSistemaDeRecomendacoes(struct ResultadoRecomendacao *resultado) {
  free(resultado->dispositivos_ligados);
  free(resultado->dispositivos_desligados);
  resultado->dispositivos_ligados == NULL;
  resultado->dispositivos_desligados == NULL;
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
      if (sscanf(buffer, "%f", &dispositivo->consumo_estimado) == 1 && dispositivo->consumo_estimado >= 0) {
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


// Função para trocar dois dispositivos de posição
void trocarDispositivos(struct Dispositivo *a, struct Dispositivo *b) {
  struct Dispositivo temp = *a;
  *a = *b;
  *b = temp;
}

// Função para ordenar dispositivos por prioridade usando bubble sort
void ordenarDispositivosPorPrioridade(struct GerenciadorDeDispositivos *gerenciador) {
  // Se não houver dispositivos cadastrados a ordenação não é feita
  if (gerenciador->quantidade <= 1) {
    return;
  }

  printf("\n==============================================\n");
  printf("        ORDENANDO POR PRIORIDADE               \n");
  printf("==============================================\n");

  // Ordenando do maior para o menor
  for (int i = 0; i < gerenciador->quantidade - 1; i++) {
    for (int j = 0; j < gerenciador->quantidade - i - 1; j++) {
      // Se a prioridade atual é menor que a próxima, troca
      if (gerenciador->dispositivos[j].prioridade > gerenciador->dispositivos[j + 1].prioridade) {
        trocarDispositivos(&gerenciador->dispositivos[j], &gerenciador->dispositivos[j + 1]);
      }
    }
  }
}

// Função do sistema de recomendação inteligente
void sistemaRecomendacaoInteligente(struct GerenciadorDeDispositivos *gerenciador) {
  float energia_disponivel;
  char buffer[50];

  printf("\n==============================================\n");
  printf("         SISTEMA DE RECOMENDAÇÃO               \n");
  printf("==============================================\n");

  printf("\nDigite a quantidade de energia disponível (em kWh): ");
  while(1) {
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%f", &energia_disponivel) == 1 && energia_disponivel >= 0) {
      break;
    }
    printf("\nValor inválido! Informe um número positivo: ");
  }

  printf("\nAnalisando dispositivos por ordem de prioridade...\n");
  // Criar uma cópia temporária dos índices para ordenação
  int *indices = malloc(gerenciador->quantidade * sizeof(int));
  for (int i = 0; i < gerenciador->quantidade; i++) {
    indices[i] = i;
  }

  // Usando a função para ordenar os dispositivos por ordem de prioridade
  ordenarDispositivosPorPrioridade(&gerenciador);

  // Inicializar resultado
  struct ResultadoRecomendacao resultado;
  iniciarResultadoRecomendacao(&resultado, gerenciador->quantidade);

  float consumo_acumulado = 0.0;

  // Laço de repetição para analisar cada dispositivo em ordem de prioridade
  for (int i = 0; i < gerenciador->quantidade; i++) {
    int indice = indices[i];
    float consumo_dispositivo = gerenciador->dispositivos[indice].consumo_estimado;

    // Verificar se ainda há energia suficiente para este dispositivo
    if (consumo_acumulado + consumo_dispositivo <= energia_disponivel) {
      // Dispositivo pode ficar ligado
      resultado.dispositivos_ligados[resultado.quantidade_ligados] = indice;
      resultado.quantidade_ligados++;
      consumo_acumulado += consumo_dispositivo;
    } else {
      // Dispositivo deve ser desligado
      resultado.dispositivos_desligados[resultado.quantidade_desligados] = indice;
      resultado.quantidade_desligados++;
    }
  }

  resultado.consumo_total = consumo_acumulado;
  resultado.energia_restante = energia_disponivel - consumo_acumulado;

  // Exibir resultados
  exibirResultadosRecomendacao(&resultado, gerenciador, energia_disponivel);

  // Liberar memória
  free(indices);
  liberarResultadoRecomendacao(&resultado);
}

// Função para exibir os resultados da recomendação
void exibirResultadosRecomendacao(const struct ResultadoRecomendacao *resultado, const struct GerenciadorDeDispositivos *gerenciador, float energia_disponivel) {
  printf("\n==============================================\n");
  printf("         RESULTADO DA RECOMENDAÇÃO             \n");
  printf("==============================================\n");

  printf("\nEnergia disponível: %.2f kWh\n", energia_disponivel);

  printf("\nDispositivos recomendados para se manter ligados:\n");
  if (resultado->quantidade_ligados == 0) {
    printf("Nenhum dispositivo pode ser mantido ligado.\n");
  } else {
    for (int i = 0; i < resultado->quantidade_ligados; i++) {
      int indice = resultado->dispositivos_ligados[i];
      printf("   • %s (%.2f kWh) - Prioridade %d\n",
      gerenciador->dispositivos[indice].nome,
      gerenciador->dispositivos[indice].consumo_estimado,
      gerenciador->dispositivos[indice].prioridade);
    }
  }

  printf("\nDispositivos recomendados para serem desligados:\n");
  if (resultado->quantidade_desligados == 0) {
    printf("Todos os dispositivos podem permanecer ligados!\n");
  } else {
    for (int i = 0; i < resultado->quantidade_desligados; i++) {
      int indice = resultado->dispositivos_desligados[i];
      printf("   • %s (%.2f kWh) - Prioridade %d\n",
        gerenciador->dispositivos[indice].nome,
        gerenciador->dispositivos[indice].consumo_estimado,
        gerenciador->dispositivos[indice].prioridade
      );
    }
  }

  printf("\nResumo:\n");
  printf(" - Consumo total dos dispositivos ligados: %.2f kWh\n", resultado->consumo_total);
  printf(" - Energia restante: %.2f kWh\n", resultado->energia_restante);
  printf(" - Eficiência energética: %.1f%%\n", (resultado->consumo_total / energia_disponivel) * 100);

  if (resultado->energia_restante > 0) {
    printf("\nVocê ainda tem %.2f kWh disponíveis\n", resultado->energia_restante);
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
    printf("3 - Ordenar dispositivos por prioridade\n");
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
      case 3:
        ordenarDispositivosPorPrioridade(&gerenciador);
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
  liberarMemoriaGerendiadorDeDispositivos(&gerenciador);
  return 0;
}
