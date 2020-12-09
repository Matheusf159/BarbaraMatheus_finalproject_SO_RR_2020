#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

// variáveis universais
char user_input[100];

//***************



void init_shell(){
    printf("\n*******************\n****NOSSO SHELL****\n*******************\n");
}

//função para imprimir o diretório
void print_dir(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

void printHelp(){
    printf(
        "\n**********COMANDOS SUPORTADOS ATÉ O MOMENTO**********\n"
        "ls\nexit\nhelp\nhello\n"
    );
}

//ou a função de pedir o comando
void print_msg(){
    printf("Sim, mestre? ");
}

void get_input(){
    scanf(" %[^\n]", user_input);
}
void exe_command(char* token){
    char *list_cmd[5];
    char *username;

    //quais serão os comandos??
    list_cmd[0] = "exit";
    list_cmd[1] = "ls";
    list_cmd[2] = "help";
    list_cmd[3] = "hello";
    list_cmd[4] = "cd";

    //depois mudar para swicth case
    if(strcmp(token, list_cmd[0]) == 0){
        printf("\nAdeus Mestre\n");
        exit(1); //termina o programa
    }
    
    if(strcmp(token, list_cmd[1]) == 0){
        chdir(token);
    }

    if(strcmp(token, list_cmd[2]) == 0){
        printHelp();
    }

    if(strcmp(token, list_cmd[3]) == 0){
        username = getenv("USER");
        printf("Hello %s, are you ok?\n", username);
    }

}

void exe_parameter(){

}

//primeiro token vai ser um comando, os outros são parametros
void parse_command(){
    //printf("%s\n", user_input);
    char *token;
    token = strtok(user_input," ");
    exe_command(token);
    while (token){
        //printf("%s\n", token);
        exe_parameter();
        token = strtok(NULL, " ");
    }

}


int main(){
    init_shell();
    while(1){
        //print_dir();
        print_msg();
        get_input();
        parse_command();
    }

    return 0;
}
