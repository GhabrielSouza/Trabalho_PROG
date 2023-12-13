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

// Funç?o para obter a data atual
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

        printf("\nData : %s\nNome: %s\nTelefone: %s\nEmail: %s\nPeso: %.2f\nAltura: %.2f\nCPF: %s\nIMC: %.2f\n",
               cliente.data, cliente.nome, cliente.telefone, cliente.email,
               cliente.peso, cliente.altura, cliente.cpf,
               cliente.imc);
    }

    fclose(arquivo);
}

void excluirRegistro(char nomeArquivo[], char dataKey[])
{
    FILE *input = fopen(nomeArquivo, "r");
    FILE *temp = fopen("temp.txt", "w");
    


    // Lendo e exibindo todos os registros antes da exclus?o
    char linha[1000];




    fseek(input, 0, SEEK_SET); // Retorna ao in?cio do arquivo para a leitura subsequente

    dataKey[strcspn(dataKey, "\n")] = '\0';        // Remover o caractere de nova linha
    converterMinusculas(dataKey, sizeof(dataKey)); // Converter para min?sculas

    fseek(input, 0, SEEK_SET); // Garante que o cursor do arquivo esteja no in?cio

    int linhasParaExcluir = 0;

    while (fgets(linha, sizeof(linha), input) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';        // Remover o caractere de nova linha
        converterMinusculas(linha, sizeof(linha)); // Converter para min?sculas

        if (linhasParaExcluir > 0)
        {
            // Ignora a linha se linhasParaExcluir for maior que zero
            linhasParaExcluir--;
            continue;
        }

        if (strncmp(linha, dataKey, strlen(dataKey)) == 0)
        {
            // Inicia a exclus?o da linha atual e das pr?ximas 7 linhas
            linhasParaExcluir = 7;
        }
        else
        {
            // Se a linha n?o deve ser exclu?da, copia para o arquivo tempor?rio
            fputs(linha, temp);
            fputc('\n', temp); // Adiciona uma quebra de linha ap?s cada linha copiada
        }
    }

    fclose(input);
    fclose(temp);

    // Remover o arquivo original
    remove(nomeArquivo);
    // Renomear o arquivo tempor?rio para o nome do arquivo original
    rename("temp.txt", nomeArquivo);
}

// Funç?o para criar um novo arquivo com as informaç?es do cliente
void criarArquivo(struct Cliente cliente, char nomeArquivo[])
{
    FILE *arquivo;

    arquivo = fopen(nomeArquivo, "w");

    fprintf(arquivo, "%s\n%s\n%s\n%s\n%f\n%f\n%s\n%f", cliente.data, cliente.nome, cliente.telefone, cliente.email, cliente.peso, cliente.altura, cliente.cpf, cliente.imc);
    fclose(arquivo);
}

// Funç?o para ler as informaç?es do cliente de um arquivo


void editarArquivo(char nomeArquivo[], char dataKey[])
{
    struct Cliente cliente;
    FILE *input = fopen(nomeArquivo, "r");
    FILE *temp = fopen("temp.txt", "w");


    fseek(input, 0, SEEK_SET);
    excluirRegistro(nomeArquivo, dataKey);

    
    dataKey[strcspn(dataKey, "\n")] = '\0';
    converterMinusculas(dataKey, sizeof(dataKey));

    fseek(input, 0, SEEK_SET);

    // Flag para indicar se o registro foi encontrado
    int registroEncontrado = 0;

    while (fscanf(input, "%s\n%s\n%s\n%s\n%f\n%f\n%s\n%f",
                  cliente.data, cliente.nome, cliente.telefone, cliente.email,
                  &cliente.peso, &cliente.altura, cliente.cpf,
                  &cliente.imc) == 8)
    {
        if (strcmp(cliente.data, dataKey) == 0)
        {
            // Encontrou o registro a ser editado

            // Solicitar ao usu?rio os novos valores para todos os campos
            printf("Digite o novo nome: ");
            fgets(cliente.nome, sizeof(cliente.nome), stdin);
            cliente.nome[strcspn(cliente.nome, "\n")] = '\0';

            printf("Digite o novo telefone: ");
            fgets(cliente.telefone, sizeof(cliente.telefone), stdin);
            cliente.telefone[strcspn(cliente.telefone, "\n")] = '\0';

            printf("Digite um novo email: ");
            fgets(cliente.email, sizeof(cliente.email), stdin);
            cliente.email[strcspn(cliente.email, "\n")] = '\0';

            printf("Digite um novo peso: ");
            scanf("%f", &cliente.peso);
            limparBufferEntrada();

            printf("Digite uma nova altura: ");
            scanf("%f", &cliente.altura);
            limparBufferEntrada();

            printf("Digite um novo CPF: ");
            fgets(cliente.cpf, sizeof(cliente.cpf), stdin);
            cliente.cpf[strcspn(cliente.cpf, "\n")] = '\0';

            // Atualizar a linha no arquivo tempor?rio
            fprintf(temp, "%s\n%s\n%s\n%s\n%f\n%f\n%s\n%f\n",
                    cliente.data, cliente.nome, cliente.telefone, cliente.email,
                    cliente.peso, cliente.altura, cliente.cpf,
                    cliente.imc);

            // Marcar que o registro foi encontrado
            registroEncontrado = 1;
        }
        else
        {
            // Se n?o for o registro a ser editado, copiar a linha original.
            fprintf(temp, "%s\n%s\n%s\n%s\n%f\n%f\n%s\n%f\n",
                    cliente.data, cliente.nome, cliente.telefone, cliente.email,
                    cliente.peso, cliente.altura, cliente.cpf,
                    cliente.imc);
        }
    }

    fclose(input);
    fclose(temp);

    // Remover o arquivo original apenas se o registro foi encontrado
    if (registroEncontrado)
    {
        remove(nomeArquivo);
        // Renomear o arquivo tempor?rio para o nome do arquivo original
        rename("temp.txt", nomeArquivo);
    }
    else
    {
        // Remover o arquivo tempor?rio se o registro n?o foi encontrado
        remove("temp.txt");
        printf("Registro n?o encontrado.\n");
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

    fprintf(arquivo, "\n%s\n%s\n%s\n%s\n%f\n%f\n%s\n%f", cliente.data, cliente.nome, cliente.telefone, cliente.email, cliente.peso, cliente.altura, cpf, cliente.imc);
    fclose(arquivo);
}


int main()
{

    int change = 0;
    
    char cpf[15];
    inicio:
    printf("Digite o CPF do cliente: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';
    char dataKey[15];

    struct Cliente cliente;

    char nomeArquivo[20];
    strcpy(nomeArquivo, cpf);
    strcat(nomeArquivo, ".txt");

    FILE *arquivo = fopen(nomeArquivo, "r");
    int opcao;

    if (arquivo != NULL)
    {
        fclose(arquivo);
        printf("Cliente encontrado!\n");

        do {

        printf("\nEscolha uma opç?o:\n");
        printf("1. Inserç?o\n");
        printf("2. Consulta\n");
        printf("3. Exclus?o\n");
        printf("4. Ediç?o\n");
        printf("5. Sair\n\n");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao)
        {
        case 1:
            system("cls");
            inserir(cliente, nomeArquivo, cpf);

            break;
        case 2:
            system("cls");
            lerArquivo(nomeArquivo);

            break;
        case 3:
            system("cls");
            printf("Digite uma das datas a seguir:\n");
            lerArquivo(nomeArquivo);
            printf("\n");
            fgets(dataKey, sizeof(dataKey), stdin);
            excluirRegistro(nomeArquivo, dataKey);
            printf("Registro exclu?do com sucesso!\n");
            break;
        case 4:
            system("cls");
            printf("Digite uma das datas a seguir para alterar\n");
            lerArquivo(nomeArquivo);
            printf("\n");
            fgets(dataKey, sizeof(dataKey), stdin);
            editarArquivo(nomeArquivo, dataKey);
            printf("Registro editado com sucesso!\n");
            break;
        case 5:
            printf("Saindo ...\n");
            break;
        default:
            printf("Opç?o inv?lida\n");
        }
        } while (opcao != 5);
    }

    else
    {
        printf("Cliente n?o encontrado. Criando novo cliente...\n");

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
        limparBufferEntrada();
    }
    printf("Agora digite 1 para voltar e entrar no menu ou 2 para fechar o programa :\n");
    scanf("%d", &change);
    if (change == 1)
    {
        limparBufferEntrada();
        goto inicio;
    }
    else if (change == 0)
    {
        exit(0);
    }

    return 0;
}
