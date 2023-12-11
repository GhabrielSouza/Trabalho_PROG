#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

struct Cliente
{
    char nome[100];
    char telefone[15];
    char email[100];
    float peso;
    float altura;
    char cpf[15];
    float imc;
    char data[11]; // Formato DD/MM/AAAA
};

// Função para obter a data atual
void obterDataAtual(char *data)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(data, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void converterMinusculas(char *str, size_t tamanho)
{
    for (size_t i = 0; i < tamanho && str[i]; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void excluirArquivo(char nomeArquivo[])
{
    FILE *input = fopen(nomeArquivo, "r");
    FILE *temp = fopen("temp.txt", "w");
    char dataKey[15];

    printf("Registros salvos:\n");

    // Lendo e exibindo todos os registros antes da exclusão
    char linha[1000];
    size_t tamanhoLinha;

    while (fgets(linha, sizeof(linha), input) != NULL)
    {
        tamanhoLinha = strlen(linha);
        printf("%.*s", (int)(tamanhoLinha - 1), linha);
    }

    fseek(input, 0, SEEK_SET); // Retorna ao início do arquivo para a leitura subsequente

    printf("\nDigite a data do registro que deseja excluir incluindo / :\n");
    fgets(dataKey, sizeof(dataKey), stdin);
    dataKey[strcspn(dataKey, "\n")] = '\0';        // Remover o caractere de nova linha
    converterMinusculas(dataKey, sizeof(dataKey)); // Converter para minúsculas

    fseek(input, 0, SEEK_SET); // Garante que o cursor do arquivo esteja no início

    printf("\nRegistros após a exclusão:\n");

    // Ler e escrever registros, excluindo aqueles que correspondem à dataKey e as próximas 7 linhas
    int linhasParaExcluir = 0;

    while (fgets(linha, sizeof(linha), input) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';        // Remover o caractere de nova linha
        converterMinusculas(linha, sizeof(linha)); // Converter para minúsculas

        if (linhasParaExcluir > 0)
        {
            // Ignora a linha se linhasParaExcluir for maior que zero
            linhasParaExcluir--;
            continue;
        }

        if (strncmp(linha, dataKey, strlen(dataKey)) == 0)
        {
            // Inicia a exclusão da linha atual e das próximas 7 linhas
            linhasParaExcluir = 7;
        }
        else
        {
            // Se a linha não deve ser excluída, copia para o arquivo temporário
            fputs(linha, temp);
            fputc('\n', temp); // Adiciona uma quebra de linha após cada linha copiada
        }
    }

    fclose(input);
    fclose(temp);

    // Remover o arquivo original
    remove(nomeArquivo);
    // Renomear o arquivo temporário para o nome do arquivo original
    rename("temp.txt", nomeArquivo);
}

// Função para criar um novo arquivo com as informações do cliente
void criarArquivo(struct Cliente cliente, char nomeArquivo[])
{
    FILE *arquivo;

    arquivo = fopen(nomeArquivo, "w");

    fprintf(arquivo, "%s\n%s\n%s\n%s\n%f\n%f\n%s\n%f", cliente.data, cliente.nome, cliente.telefone, cliente.email, cliente.peso, cliente.altura, cliente.cpf, cliente.imc);
    fclose(arquivo);
}

// Função para ler as informações do cliente de um arquivo
void lerArquivo(char nomeArquivo[])
{
    struct Cliente cliente;
    FILE *arquivo;

    arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        exit(1);
    }

    while (fscanf(arquivo, "%s\n%s\n%s\n%s\n%f\n%f\n%s\n%f",
                  cliente.data, cliente.nome, cliente.telefone, cliente.email,
                  &cliente.peso, &cliente.altura, cliente.cpf,
                  &cliente.imc) == 8)
    {

        printf("Data : %s\nNome: %s\nTelefone: %s\nEmail: %s\nPeso: %.2f\nAltura: %.2f\nCPF: %s\nIMC: %.2f\n\n",
               cliente.data, cliente.nome, cliente.telefone, cliente.email,
               cliente.peso, cliente.altura, cliente.cpf,
               cliente.imc);
    }

    fclose(arquivo);
}

void editarArquivo(char nomeArquivo[])
{
    FILE *input = fopen(nomeArquivo, "r");
    FILE *temp = fopen("temp.txt", "w");
    char dataKey[15];

    printf("Registros salvos:\n");

    char linha[1000];
    size_t tamanhoLinha;

    lerArquivo(nomeArquivo);

    fseek(input, 0, SEEK_SET);

    printf("\nDigite a data do registro que deseja editar incluindo / :\n");
    fgets(dataKey, sizeof(dataKey), stdin);
    dataKey[strcspn(dataKey, "\n")] = '\0';
    converterMinusculas(dataKey, sizeof(dataKey));

    fseek(input, 0, SEEK_SET);

    printf("\nRegistros após a edição:\n");

    // Flag para indicar se o registro foi encontrado
    int registroEncontrado = 0;

    while (fgets(linha, sizeof(linha), input) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';
        converterMinusculas(linha, sizeof(linha));

        if (strncmp(linha, dataKey, strlen(dataKey)) == 0)
        {
            // Encontrou o registro a ser editado
            struct Cliente cliente;
            sscanf(linha, "%s %s %s %s %f %f %s %f",
                   cliente.data, cliente.nome, cliente.telefone, cliente.email,
                   &cliente.peso, &cliente.altura, cliente.cpf,
                   &cliente.imc);

            // Solicitar ao usuário qual campo editar
            int opcao;
            printf("\nEscolha um campo para editar:\n");
            printf("1. Nome\n");
            printf("2. Telefone\n");
            printf("3. Email\n");
            printf("4. Peso\n");
            printf("5. Altura\n");
            printf("6. CPF\n");

            scanf("%d", &opcao);
            limparBufferEntrada();

            switch (opcao)
            {
            case 1:
                printf("Digite o novo nome: ");
                fgets(cliente.nome, sizeof(cliente.nome), stdin);
                cliente.nome[strcspn(cliente.nome, "\n")] = '\0';
                break;
            case 2:
                printf("Digite o novo telefone: ");
                fgets(cliente.telefone, sizeof(cliente.telefone), stdin);
                cliente.telefone[strcspn(cliente.telefone, "\n")] = '\0';
                break;
            case 3:
                printf("Digite um novo email: ");
                fgets(cliente.email, sizeof(cliente.email), stdin);
                cliente.email[strcspn(cliente.email, "\n")] = '\0';
                break;
            case 4:
                printf("Digite um novo peso: ");
                scanf("%f", &cliente.peso);
                limparBufferEntrada();
                break;
            case 5:
                printf("Digite uma nova altura: ");
                scanf("%f", &cliente.altura);
                limparBufferEntrada();
                break;
            case 6:
                printf("Digite um novo CPF: ");
                fgets(cliente.cpf, sizeof(cliente.cpf), stdin);
                cliente.cpf[strcspn(cliente.cpf, "\n")] = '\0';
                break;
            default:
                printf("Opção inválida\n");
                break;
            }

            // Atualizar a linha no arquivo temporário
            fprintf(temp, "%s %s %s %s %.2f %.2f %s %.2f\n",
                    cliente.data, cliente.nome, cliente.telefone, cliente.email,
                    cliente.peso, cliente.altura, cliente.cpf,
                    cliente.imc);

            // Marcar que o registro foi encontrado
            registroEncontrado = 1;
        }
        else
        {
            // Se não for o registro a ser editado, copiar a linha original.
            fputs(linha, temp);
            fputc('\n', temp);
        }
    }

    fclose(input);
    fclose(temp);

    // Remover o arquivo original apenas se o registro foi encontrado
    if (registroEncontrado)
    {
        remove(nomeArquivo);
        // Renomear o arquivo temporário para o nome do arquivo original
        rename("temp.txt", nomeArquivo);
        printf("Registro editado com sucesso!\n");
    }
    else
    {
        // Remover o arquivo temporário se o registro não foi encontrado
        remove("temp.txt");
        printf("Registro não encontrado.\n");
    }
}

void inserir(struct Cliente cliente, char nomeArquivo[], char cpf[])
{
    obterDataAtual(cliente.data);

    printf("Digite o nome do cliente: ");
    fgets(cliente.nome, sizeof(cliente.nome), stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = '\0'; // Remover o caractere de nova linha
    printf("Digite o telefone do cliente: ");
    fgets(cliente.telefone, sizeof(cliente.telefone), stdin);
    cliente.telefone[strcspn(cliente.telefone, "\n")] = '\0';
    printf("Digite o email do cliente: ");
    fgets(cliente.email, sizeof(cliente.email), stdin);
    cliente.email[strcspn(cliente.email, "\n")] = '\0';
    printf("Digite o peso do cliente: ");
    scanf("%f", &cliente.peso);
    printf("Digite a altura do cliente: ");
    scanf("%f", &cliente.altura);
    cliente.imc = cliente.peso / (cliente.altura * cliente.altura);
    
    

    FILE *arquivo;

    arquivo = fopen(nomeArquivo, "a");

    fprintf(arquivo, "%s\n%s\n%s\n%s\n%f\n%f\n%s\n%f\n", cliente.data, cliente.nome, cliente.telefone, cliente.email, cliente.peso, cliente.altura, cpf, cliente.imc);
    fclose(arquivo);
}

int main()
{
    char cpf[15];
    printf("Digite o CPF do cliente: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    struct Cliente cliente;

    char nomeArquivo[20];
    strcpy(nomeArquivo, cpf);
    strcat(nomeArquivo, ".txt");

    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo != NULL)
    {
        fclose(arquivo);
        printf("Cliente encontrado!\n");

        int opcao;
        printf("Escolha uma opção:\n");
        printf("1. Inserção\n");
        printf("2. Consulta\n");
        printf("3. Exclusão\n");
        printf("4. Edição\n");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao)
        {
        case 1:
        inserir(cliente, nomeArquivo, cpf);
            // Lógica para inserção
            break;
        case 2:
            lerArquivo(nomeArquivo);
            // Lógica para consulta
            break;
        case 3:
            excluirArquivo(nomeArquivo);
            printf("Registro excluído com sucesso!\n");
            break;
        case 4:
            editarArquivo(nomeArquivo);
            printf("Registro editado com sucesso!\n");
            break;
        default:
            printf("Opção inválida\n");
        }
    }
    else
    {
        printf("Cliente não encontrado. Criando novo cliente...\n");

        obterDataAtual(cliente.data);

        printf("Digite o nome do cliente: ");
        fgets(cliente.nome, sizeof(cliente.nome), stdin);
        cliente.nome[strcspn(cliente.nome, "\n")] = '\0'; // Remover o caractere de nova linha
        printf("Digite o telefone do cliente: ");
        fgets(cliente.telefone, sizeof(cliente.telefone), stdin);
        cliente.telefone[strcspn(cliente.telefone, "\n")] = '\0';
        printf("Digite o email do cliente: ");
        fgets(cliente.email, sizeof(cliente.email), stdin);
        cliente.email[strcspn(cliente.email, "\n")] = '\0';
        printf("Digite o peso do cliente: ");
        scanf("%f", &cliente.peso);
        printf("Digite a altura do cliente: ");
        scanf("%f", &cliente.altura);
        strcpy(cliente.cpf, cpf);
        cliente.imc = cliente.peso / (cliente.altura * cliente.altura);

        printf("\n%s\n", nomeArquivo);

        criarArquivo(cliente, nomeArquivo);
        printf("Novo cliente criado com sucesso!\n");
    }

    return 0;
}
