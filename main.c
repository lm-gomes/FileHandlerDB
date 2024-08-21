#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu();
void cadastrarAluno();
void consultarAluno();

FILE *file;

int userInput = 0;
int alunosCD = 0;
int idAluno = 0;


int main(){
    file = fopen("text.txt", "r");
    if(file == NULL){
        printf("The file was not found in the directory.\n\n");
        printf("Creating a new file...");
        file = fopen("text.txt", "a");
        fprintf(file, "[ARQUIVO] - 0 alunos cadastrados.\n");
        printf("File created succesfully!\n\n");

    }
    fclose(file);
    menu();

    if(fclose(file) != EOF){
        printf("Couldn't close the file.\n\n");
        return 1;
    }
    return 0;
}

void menu(){
    alunosCD = alunosCadastrados(alunosCD);
    printf("\n - Alunos cadastrados: %d - \n", alunosCD);
    printf("\n- - - CONSULTA - - - \n");
    printf("[1] Cadastrar aluno\n[2] Consultar alunos\n[3] Sair\n>> ");
    scanf("%d", &userInput);
    switch(userInput){
        case 1:{
            cadastrarAluno();
            break;
        }
        case 2:{
            consultarAluno();
            break;
        }
        case 3:{

            break;
        }
    }

}


void cadastrarAluno(){

    userInput = 1;
    while(userInput != 0){
        cleanBuffer();

        file = fopen("text.txt", "a");
        char name[50];
        int matricula = 0;
        printf("Digite o nome do aluno: ");
        scanf("%[^\n]", name);
        printf("Digite a matricula: ");
        scanf("%d", &matricula);
        //Necessário adicionar um contador de alunos cadastrados após essa função.
        fprintf(file, "(%d) Aluno: %s - Matricula: %d\n", alunosCD + 1, name, matricula);
        alunosCD++;
        fclose(file);
        atualizarArquivo(1);


        printf("Aluno cadastrado!\nGostaria de continuar cadastrando?\n>> ");
        scanf("%d", &userInput);
    }

    menu();

}

void consultarAluno(){

    char userConfirm;
    // Stream no topo do arquivo
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
    menu();

}

int alunosCadastrados(int alunosCD){
    file = fopen("text.txt", "r");
    fseek(file, 12, SEEK_SET);
    fscanf(file, "%d", &alunosCD);
    fclose(file);
    return alunosCD;
}

void atualizarArquivo(int op){
    FILE *fileTemp = fopen("temp.txt", "w");
    switch(op){
        case 1:{
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
                    fprintf(fileTemp, "[ARQUIVO] - %d alunos cadastrados.\n", alunosCD - 1);
                    file = fopen("text.txt", "r");
                    char c;
                    for(int i = 1;; i++){
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
                }
                else{
                    printf("Nao existe aluno com esse ID!\n");
                }
            }

            else{
                printf("Nao existem alunos para serem removidos!\n");

            }
                break;

        };
    }

}


void cleanBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
