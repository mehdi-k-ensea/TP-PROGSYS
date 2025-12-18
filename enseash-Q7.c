#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

#define SIZE_BUFFER_COMMAND 4096
#define SIZE_PROMPT 80
#define MAX_ARGS 64

int main(void){
    int shell_destination = STDOUT_FILENO;
    int shell_source = STDIN_FILENO;
    int status;
    int last_exit_code = 0;
    int last_signal = 0;
    long last_time_ms = 0;

    const char* welcome = "Bienvenue dans le Shell ENSEA, question 7.\n\n ctrl+d ou exit pour fermer le terminal\n";
    write(shell_destination, welcome, strlen(welcome));

    char buffer_command[SIZE_BUFFER_COMMAND];

    while(1){
        char prompt[SIZE_PROMPT];
        if (last_signal != 0){
            snprintf(prompt, SIZE_PROMPT, "enseash [sign:%d|%ldms] %% ", last_signal, last_time_ms);
        } else {
            snprintf(prompt, SIZE_PROMPT, "enseash [exit:%d|%ldms] %% ", last_exit_code, last_time_ms);
        }
        write(shell_destination, prompt, strlen(prompt));

        int read_command = read(shell_source, buffer_command, SIZE_BUFFER_COMMAND-1);
        if (read_command <= 0) break;

        buffer_command[read_command-1] = '\0';

        if (strcmp(buffer_command,"exit")==0){
            write(shell_destination, "\nCiao, Hasta Luego\n", 20);
            break;
        }

        if (buffer_command[0] == '\0') continue;

        char **argv = malloc(MAX_ARGS * sizeof(char *));
        int argc = 0;
        int fd_in = -1, fd_out = -1;

        int i = 0;
        while(buffer_command[i] != '\0' && argc < MAX_ARGS - 1){
            // ignorer les espaces
            while(buffer_command[i] == ' ') i++;
            if(buffer_command[i] == '\0') break;

            // gestion de la redirection input
            if(buffer_command[i] == '<'){
                i+=2;
                fd_in = open(&buffer_command[i], O_RDONLY);
                if(fd_in == -1){
                    perror("open input");
                    break;
                }
                while(buffer_command[i] != ' ' && buffer_command[i] != '\0') i++;
                continue;
            }

            // gestion de la redirection output
            if(buffer_command[i] == '>'){
                i+=2;
                fd_out = open(&buffer_command[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(fd_out == -1){
                    perror("open output");
                    break;
                }
                while(buffer_command[i] != ' ' && buffer_command[i] != '\0') i++;
                continue;
            }

            argv[argc++] = &buffer_command[i];
            while(buffer_command[i] != ' ' && buffer_command[i] != '\0') i++;
            if(buffer_command[i] == ' ') buffer_command[i++] = '\0';
        }
        argv[argc] = NULL;

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        pid_t pid = fork();
        if(pid == 0){
            if(fd_in != -1){
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }
            if(fd_out != -1){
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }
            execvp(argv[0], argv);
            perror("Commande non reconnue");
            exit(1);
        } else if(pid > 0){
            waitpid(pid, &status, 0);

            clock_gettime(CLOCK_MONOTONIC, &end);
            last_time_ms = (end.tv_sec - start.tv_sec) * 1000
                           + (end.tv_nsec - start.tv_nsec) / 1000000;

            last_signal = 0;
            if(WIFEXITED(status)) last_exit_code = WEXITSTATUS(status);
            else if(WIFSIGNALED(status)) last_signal = WTERMSIG(status);
        }
    }

    return 0;
}
