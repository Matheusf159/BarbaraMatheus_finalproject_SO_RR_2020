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

//ou a função de pedir o comando
void print_msg(){
    printf(">>>Qual o seu comando? ");
}

void get_input(){
    scanf(" %[^\n]", user_input);
}
void exe_command(char *token){
    char *list_cmd[4];
    //quais serão os comandos??
    list_cmd[0] = "exit";
    list_cmd[1] = "ls";
    list_cmd[2] = "help";
    list_cmd[3] = "hello";
    
    //depois mudar para swicth case
    if(strcmp(token, list_cmd[0]) == 0){
        printf("\n>>>Tchauzinho\n");
        exit(1); //termina o programa
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