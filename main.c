#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <openssl/sha.h>

// Declaracao de prototipo das funções utilizadas
void menu();
void cadastrarAluno();
void consultarAluno();
int alunosCadastrados(int alunosCD);
void atualizarArquivo(int op);
void ajudaMenu();
void cleanBuffer();
void clearTerminal();


// Declaracao de variaveis globais que serao utilizadas no código
FILE *file;
FILE *loginFile;

int userInput = 0;
int alunosCD = 0;
int idAluno = 0;


typedef struct {
    char user[50];
    unsigned char password[SHA256_DIGEST_LENGTH];
} Login;

void hashPassword(const char *password, unsigned char *hashOutput){
    SHA256((unsigned char*)password, strlen(password), hashOutput);
}


int login(){
    Login login;
    char user[50];
    char password[50];
    unsigned char passwordHash[SHA256_DIGEST_LENGTH];

    loginFile = fopen("login.txt", "rb");
    if(loginFile == NULL){
        loginFile = fopen("login.txt", "ab");
        printf(" ________________________\n");
        printf("|        REGISTRO        |\n");
        printf("|________________________|\n\n");
        printf("Digite o nome do usuario: ");
        scanf("%s", &login.user);
        printf("Digite a senha: ");
        scanf("%s", password);
        hashPassword(password, login.password);
        fwrite(&login, sizeof(Login), 1, loginFile);
        fclose(loginFile);
        printf("Usuario cadastrado!\n");
        Sleep(2000);
        return 1;
    }
    else{
        printf(" ________________________\n");
        printf("|         LOGIN          |\n");
        printf("|________________________|\n\n");
        printf("Usuario: ");
        scanf("%s", user);
        printf("Senha: ");
        scanf("%s", password);
        hashPassword(password, passwordHash);
        fread(&login, sizeof(Login), 1, loginFile);


        if(strcmp(user, login.user) == 0){
            if(memcmp(login.password, passwordHash, SHA256_DIGEST_LENGTH) == 0){
                return 1;
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }

    }

}

int main(){
    if(login() == 1){
        printf("Acesso permitido!\n");
        Sleep(2000);
    }
    else{
        printf("Acesso negado!\n");
        return 0;
    }


    // O programa buscará pelo arquivo .txt no diretório do projeto, e caso não encontre,
    // criará um novo.
    file = fopen("text.txt", "r");

    if(file == NULL){
        printf("O arquivo nao foi encontrado no diretorio.\n\n");
        printf("Criando novo arquivo...");
        file = fopen("text.txt", "a");
        fprintf(file, "[ARQUIVO] - 0 alunos cadastrados.\n");
        printf("Arquivo criado com sucesso!\n");
        Sleep(3000);
    }
    //Após ter encontrado ou não o arquivo text.txt, fechamos o arquivo e chamamos a função
    // que exibirá o MENU.
    fclose(file);
    menu();

    if(fclose(file) != EOF){
        printf("Nao foi possivel fechar o arquivo.\n\n");
        return 1;
    }
    return 0;
}

void menu(){

    clearTerminal();
    // Verificar se essa parte é essencial -> alunosCD = alunosCadastrados(alunosCD);
    printf(" ________________________\n");
    printf("|    MENU DE CONSULTA    |\n");
    printf("|________________________|\n\n");
    printf("[1] Cadastrar aluno\n[2] Consultar alunos\n[3] Ajuda\n[4] Sair\n>> ");
    scanf("%d", &userInput);
    printf("%d", userInput);


    // Após exibir o menu, o valor que o usuario digitar o levara para uma das opcoes do MENU

    switch(userInput){
        case 1:{
            clearTerminal();
            cadastrarAluno();
            break;
        }
        case 2:{
            clearTerminal();
            consultarAluno();
            break;
        }
        case 3:{
            clearTerminal();
            ajudaMenu();
            break;
        }
        case 4:{
            printf("Programa finalizado!\n");
            return 0;
        }
        default:{
            printf("Valor invalido.\n");
            Sleep(1350);
            menu();
        }
    }

}

// Função que lida com o menu de cadastro de alunos
void cadastrarAluno(){
    printf(" ________________________\n");
    printf("|  CADASTRO DE ALUNOS    |\n");
    printf("|________________________|\n\n");
    userInput = 1;
    while(userInput == 1){
        cleanBuffer();

        file = fopen("text.txt", "a");
        char name[50];
        int matricula = 0;
        printf("Digite o nome do aluno: ");
        scanf("%[^\n]", name);
        printf("Digite a matricula: ");
        scanf("%d", &matricula);
        printf("Cadastrar aluno %s de matricula %d?\n>> ", name, matricula);
        scanf("%d", &userInput);
        // Caso o usuário confirmar que deseja cadastrar o aluno, ele será enviado para a função
        // atualizarArquivo, que realizará o cadastro do aluno no arquivo text.txt

        if(userInput == 1){
            fprintf(file, "(%d) Aluno: %s - Matricula: %d\n", alunosCD + 1, name, matricula);
            alunosCD++;
            fclose(file);
            atualizarArquivo(1);
            printf("Aluno cadastrado!\nGostaria de continuar cadastrando?\n>> ");
            scanf("%d", &userInput);
        }
        else{
            fclose(file);
            menu();
        }
    }

    menu();

}

// Função que cuidará do menu de consulta de alunos.
void consultarAluno(){
    // Ao receber o valor retornado da função alunosCadastrados(), alunosCD atualiza seu valor baseado
    // no que está registrado no arquivo.

    alunosCD = alunosCadastrados(alunosCD);
    clearTerminal();
    char userConfirm;

    file = fopen("text.txt", "r");
    rewind(file);
    char info;
    while(!feof(file)){
        info = fgetc(file);
        printf("%c", info);
    }
    fclose(file);

    printf("\n[1] Remover aluno\n[2] Voltar para o menu\n>> ");
    scanf("%d", &userConfirm);
    if(userConfirm == 1){
        atualizarArquivo(2);

    }
    else{
        menu();
    }

}

// A função alunosCadastrados posiciona a Stream do arquivo no lugar exato do numero de alunos
// cadastrados, pegando esse valor e armazenando em um variável para ser tratado pelo programa.

int alunosCadastrados(int alunosCD){
    file = fopen("text.txt", "r");
    fseek(file, 12, SEEK_SET);
    fscanf(file, "%d", &alunosCD);
    fclose(file);
    return alunosCD;
}

// A função atualizarArquivo é responsável por

void atualizarArquivo(int op){

    switch(op){
        case 1:{
            FILE *fileTemp = fopen("temp.txt", "w");
            int nCount = 0;
            fprintf(fileTemp, "[ARQUIVO] - %d alunos cadastrados.\n", alunosCD);
            file = fopen("text.txt", "r");
            //printf("Frase: \n");
            char c = fgetc(file);
            while(c != EOF){
                //printf("%c", c);
                c = fgetc(file);
                if(c == EOF) break;
                if(nCount >= 1){
                    fputc(c, fileTemp);
                }
                if(c == '\n'){nCount++;}
            }
            fclose(file);
            fclose(fileTemp);
            if(fclose(file) != EOF){
                perror("Arquivo erro > ");
            }
            if(remove("text.txt") != 0){
                perror("Fechamento erro > ");
            }
            remove("text.txt");
            rename("temp.txt", "text.txt");
            break;
        };

        case 2:{
            if(alunosCD != 0){
                int userID = 0, nCount = 0, idCount = 0;
                printf("Digite o ID do aluno que deseja REMOVER: ");
                scanf("%d", &userID);
                if(userID <= alunosCD){
                    FILE *fileTemp = fopen("temp.txt", "w");
                    fprintf(fileTemp, "[ARQUIVO] - %d alunos cadastrados.\n", alunosCD - 1);
                    file = fopen("text.txt", "r");
                    char c;

                    while(c != EOF){
                                                c = fgetc(file);
                        if(c == EOF) break;
                        if(nCount >= 1){
                            if(nCount != userID){
                                fputc(c, fileTemp);
                                if(c == '('){
                                    idCount++;
                                    fseek(file, 1, SEEK_CUR);
                                    fprintf(fileTemp, "%d", idCount);
                                    while(c != ')'){
                                        c = fgetc(file);
                                    }
                                    fputc(c, fileTemp);
                                    }
                                }

                            }
                        if(c == '\n'){nCount++;};
                    }


                    /*for(int i = 1;; i++){
                        c = fgetc(file);
                        if(c == EOF) break;
                        if(nCount >= 1){
                            if(nCount != userID){
                                fputc(c, fileTemp);
                                if(c == '('){
                                    idCount++;
                                    fseek(file, 1, SEEK_CUR);
                                    fprintf(fileTemp, "%d", idCount);
                                    while(c != ')'){
                                        c = fgetc(file);
                                    }
                                    fputc(c, fileTemp);
                                    }
                                }

                            }
                        if(c == '\n'){nCount++;};

                    }
                    */
                    fclose(file);
                    fclose(fileTemp);
                    if(fclose(file) != EOF){
                        perror("Arquivo erro > ");
                    }
                    if(remove("text.txt") != 0){
                        perror("Fechamento erro > ");
                    }
                    remove("text.txt");
                    rename("temp.txt", "text.txt");
                    printf("\nAluno removido!");
                    Sleep(1350);
                    //consultarAluno();

                }
                else{
                    printf("Nao existe aluno com esse ID!\n");
                    Sleep(2000);

                }
            }

            else{
                printf("Nao existem alunos para serem removidos!\n");
                Sleep(2000);

            }
            consultarAluno();
                break;

        };
    }

}


void ajudaMenu(){
    int userAjuda = 0;
    printf("Bem-vindo ao programa de cadastro e consulta de alunos!\nAtraves deste programa e possivel cadastrar alunos e remove-los atraves de seus IDs.\n\nImportante:\n-> Utilize 1 para confirmar operacoes e 0 para cancelar.\n\n-> Os arquivos devem ser manipulados apenas atraves do programa.\n\n-> Os dados ficarao salvos apos o programa ser finalizado.\n\n-> O arquivo 'text.txt' deve ser mantido com o mesmo nome e na mesma pasta do projeto para que o programa funcione corretamente.\n\n");
    printf("[1]Voltar para menu\n>> ");
    scanf("%d", &userAjuda);
    if(userAjuda == 1){
        menu();
    }

}


void cleanBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Sua função é basicamente limpar o que esta printado no terminal
void clearTerminal(){
    system("cls");
}
