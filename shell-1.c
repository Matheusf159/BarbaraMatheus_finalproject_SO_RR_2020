#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <dirent.h>

// variáveis universais
#define MAX 100 
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
        "\n**********COMANDOS SUPORTADOS ATE O MOMENTO**********\n"
        "ls\nexit\nhelp\nhello\ncd\nS"
    );
}

//ou a função de pedir o comando
void print_msg(){
    printf("Sim, mestre? ");
}

void get_input(char* user_input){
    scanf(" %[^\n]", user_input);
}
void exe_command(char* token, char* arg){
    char *list_cmd[5];
    char *username;

    //quais serão os comandos??
    list_cmd[0] = "exit";
    list_cmd[1] = "cd";
    list_cmd[2] = "help";
    list_cmd[3] = "hello";
    list_cmd[4] = "ls";

    //depois mudar para swicth case
    if(strcmp(token, list_cmd[0]) == 0){
        printf("\nAdeus Mestre\n");
        exit(1); //termina o programa
    }
    
    if(strcmp(token, list_cmd[1]) == 0){ // esse aqui nao ta funcionando
        //printf("%s\n", arg);
        chdir(arg);
    }

    if(strcmp(token, list_cmd[2]) == 0){
        printHelp();
    }

    if(strcmp(token, list_cmd[3]) == 0){
        #ifdef LINUX
        username = getenv("USER");
        #elif defined WIN32
        username = getenv("USERNAME");
        #else
        printf("ERROR");
        #endif

        printf("Hello %s, are you ok?\n", username);

    }

    if(strcmp(token, list_cmd[4]) == 0){
        DIR *dirp;
        struct dirent *dp;
        struct stat statbuf;
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        dirp = opendir(cwd);
        while((dp = readdir(dirp)) !=NULL){

            stat(dp->d_name, &statbuf);
            printf("%s\n", dp->d_name);
        }


    }

}

void exe_parameter(){

}
void findPipe(char *user_input){
    char *pipes;
    char *command;
    command = strtok(user_input, "|"); //pega a primeira parte
    pipes = strtok(NULL,""); //pega a segunda parte
    //printf("%s\n", command);
    //printf("%s\n", pipes);
    if (pipes){  
        printf("%s\n", pipes);
        //executar, pode ser um simples ou não
        
    }else{
        //printf("1");
        //tirar espaço
        char *arg;
        command = strtok(command, " "); //primeira parte
        arg = strtok(NULL,""); //segunda parte
        //printf("%s\n", arg);
        exe_command(command, arg);
    }
    //printf("%s\n", pipes);
}
//primeiro token vai ser um comando, os outros são parametros
/*void parse_command(char *user_input){
    //printf("%s\n", user_input);
    char *token;
    
    token = strtok(user_input," ");
    
    exe_command(token);
    while (token){
        //printf("%s\n", token);
        exe_parameter();
        token = strtok(NULL, " ");
    }

}*/


int main(){
    char user_input[MAX];
    init_shell();
    while(1){
        //print_dir();
        print_msg();
        get_input(user_input);
        //ver se tem pipe
        findPipe(user_input);
        //parse_command(user_input);
    }

    return 0;
}
