#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define SIZE_BUFFER_COMMAND 4096
#define SIZE_PROMPT 80
#define MAX_ARGS 64

int main(void){
    int shell_destination = STDOUT_FILENO;
    int shell_source = STDIN_FILENO;
    int status;
    int last_exit_code = 0;
    int last_signal = 0;
    long last_time_ms = 0; // dernier temps d'exécution en ms

    const char* welcome = "Bienvenue dans le Shell ENSEA, question 6.\n\n ctrl+d ou exit pour fermer le terminal\n";
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
        if (read_command <= 0) break; // ctrl+D

        buffer_command[read_command-1] = '\0'; // remplace le 'ENTER'

        if (strcmp(buffer_command,"exit")==0){
            const char* bye = "\nCiao, Hasta Luego\n";
            write(shell_destination, bye, strlen(bye));
            break;
        }

        if (buffer_command[0] == '\0') continue;

        char **argv = malloc(MAX_ARGS * sizeof(char *));
        int argc = 0;
        int i = 0;

        argv[argc++] = buffer_command;  // argv[0] = nom de la commande

        while (buffer_command[i] != '\0' && argc < MAX_ARGS - 1) {
            if (buffer_command[i] == ' ') {
                buffer_command[i] = '\0';
                argv[argc++] = &buffer_command[i + 1];
            }
            i++;
        }

        argv[argc] = NULL;  // obligatoire pour execvp


        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        pid_t pid = fork();
        if (pid == 0){
            execvp(argv[0], argv);
            perror("Commande non reconnue");
            _exit(1);
        } else if (pid > 0){
            waitpid(pid, &status, 0);

            clock_gettime(CLOCK_MONOTONIC, &end);
            last_time_ms = (end.tv_sec - start.tv_sec) * 1000
                           + (end.tv_nsec - start.tv_nsec) / 1000000;

            last_signal = 0;
            if (WIFEXITED(status)){
                last_exit_code = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)){
                last_signal = WTERMSIG(status);
            }
        }
    }

    return 0;
}
