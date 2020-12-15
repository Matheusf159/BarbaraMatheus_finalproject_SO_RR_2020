#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <sys/stat.h>
#include <stdint.h>
#include <dirent.h>
#include <fcntl.h>

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
        "ls\nfim\nhelp\nhello\ncd\nS"
    );
}

//ou a função de pedir o comando
void print_msg(){
    printf("Sim, mestre? ");
}

void get_input(char* user_input){
    scanf(" %[^\n]", user_input);
}
int  exe_command(char* token, char* arg){
    char *list_cmd[5];
    char *username;

    //quais serão os comandos??
    list_cmd[0] = "fim";
    list_cmd[1] = "cd";
    list_cmd[2] = "help";
    list_cmd[3] = "hello";
    list_cmd[4] = "ls";

    if(strcmp(token, list_cmd[0]) == 0){
        printf("\nAdeus Mestre\n");
        exit(1); //termina o programa
        return 1;

    }else if(strcmp(token, list_cmd[1]) == 0){ 
        chdir(arg);
        return 1;

    }else if(strcmp(token, list_cmd[2]) == 0){
        printHelp();
        return 1;

    }else if(strcmp(token, list_cmd[3]) == 0){
        #ifdef __linux__
        username = getenv("USER");
        #elif defined _WIN32
        username = getenv("USERNAME");
        #else
        printf("ERROR");
        #endif

        printf("Hello %s, are you ok?\n", username);
        return 1;

    }else if(strcmp(token, list_cmd[4]) == 0){
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
        return 1;

    }else{
        return 0;
    }

}

void simplesCMD(char** arg) 
{ 
    // Forking a child 
    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\nFailed forking child.."); 
        return; 
    } else if (pid == 0) { 
        if (execvp(arg[0], arg) < 0) { 
            printf("\nCould not execute command.."); 
        } 
        exit(0); 
    } else { 
        // waiting for child to terminate 
        wait(NULL);  
        return; 
    } 
} 
void exePipe(char** listcmd, char** listpipe) 
{ 
    // 0 is read end, 1 is write end 
    int pipefd[2];  
    pid_t p1, p2; 
  
    if (pipe(pipefd) < 0) { 
        printf("\nPipe could not be initialized"); 
        return; 
    } 
    p1 = fork(); 
    if (p1 < 0) { 
        printf("\nCould not fork"); 
        return; 
    } 
  
    if (p1 == 0) { 
        // Child 1 executing.. 
        // It only needs to write at the write end 
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 
  
        if (execvp(listcmd[0], listcmd) < 0) { 
            printf("\nCould not execute command 1.."); 
            exit(0); 
        } 
    } else { 
        // Parent executing 
        p2 = fork(); 
  
        if (p2 < 0) { 
            printf("\nCould not fork"); 
            return; 
        } 
  
        // Child 2 executing.. 
        // It only needs to read at the read end 
        if (p2 == 0) { 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO); 
            close(pipefd[0]); 
            if (execvp(listpipe[0], listpipe) < 0) { 
                printf("\nCould not execute command 2.."); 
                exit(0); 
            } 
        } else { 
            // parent executing, waiting for two children 
            wait(NULL); 
            wait(NULL); 
        } 
    } 
} 
void find_red(char *user_input){
    char *all_args[MAX];
    int i, j = 1;
    all_args[0] = strtok(user_input, " ");
    for (i = 1; i<MAX; i++){
        all_args[i] = strtok(NULL, " "); //pega tudo que foi escrito pelo usuário e divide na lista por espaço
    }
    while(all_args[j]!= NULL){
        if(strcmp(all_args[j], "<=") == 0){
            int inp = open(all_args[j+1], O_RDONLY);
            if(inp < 0){
                perror("minsh");
            }else if(dup2(inp, 0)<0){
                perror("minsh");
            }else{
                close(inp);
                all_args[j] = NULL;
                all_args[j+1] = NULL;
                j+= 2;
            }
            
        }else if(strcmp(all_args[j], "=>") == 0){
            int out = open(all_args[j+1], O_WRONLY|_O_TRUNC|O_CREAT, 0755);
            if( out < 0){
                perror("minsh");
            }else if( dup2(out, 1) < 0){
                perror("minsh");
            }else{
                close(out);
                all_args[j] = NULL;
                all_args[j + 1] = NULL;
                j += 2;
            }
            
        }else{
            j++;
        }
    }

}
void findPipe(char *user_input){
    char *pipes;
    char *command;
    find_red(user_input);
    command = strtok(user_input, "|"); //pega a primeira parte
    pipes = strtok(NULL,""); //pega a segunda parte

    if (pipes){  
        char *listpipes[MAX];
        char *listcmd[MAX];
        //retirar espaços
        listcmd[0] = strtok(command, " ");
        listpipes[0] = strtok(pipes, " ");
        for (int i=1; i<MAX;i++){
            listcmd[i] = strtok(NULL," "); //resto
            listpipes[i] = strtok(NULL, " ");
            //OBS: tem q colocar uma condição de parada para caso não haja mais elementos
            //ele não precisar procurar até o fim de MAX
        }
        //executa pipe
        exePipe(listcmd, listpipes);
        
    }else{
        //tirar espaço
        char *arg[MAX];
        int isbuiltin = 0;
        arg[0] = strtok(command, " "); //primeira parte
        for (int i=1; i<MAX;i++){
            arg[i] = strtok(NULL," "); //resto
        }
        //ve se é um comando builtin
        isbuiltin = exe_command(arg[0], arg[1]);
        if(!isbuiltin){
            //executa comando
            simplesCMD(arg);
        }
    }
}

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