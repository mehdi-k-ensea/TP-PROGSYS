#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE_BUFFER_COMMAND 4096
#define SIZE_PROMPT 64

int main(void){
    int shell_destination = STDOUT_FILENO;
    int shell_source = STDIN_FILENO;
    int status;
    int last_exit_code = 0;   // code de retour par défaut
    int last_signal = 0;      // signal par défaut

    const char* welcome = "Bienvenue dans le Shell ENSEA, question 4.\n\n ctrl+d ou exit pour fermer le terminal\n";
    write(shell_destination, welcome, strlen(welcome));

    char buffer_command[SIZE_BUFFER_COMMAND];

    while(1){
        // Construire le prompt en fonction du dernier status
        char prompt[SIZE_PROMPT];
        if (last_signal != 0){
            snprintf(prompt, SIZE_PROMPT, "enseash [sign:%d] %% ", last_signal);
        } else {
            snprintf(prompt, SIZE_PROMPT, "enseash [exit:%d] %% ", last_exit_code);
        }
        write(shell_destination, prompt, strlen(prompt));

        int read_command = read(shell_source, buffer_command, SIZE_BUFFER_COMMAND-1);
        if (read_command <= 0) break;

        buffer_command[read_command-1] = '\0'; // remplace le 'ENTER'

        if (strcmp(buffer_command,"exit")==0){
            const char* bye = "\nCiao, Hasta Luego\n";
            write(shell_destination, bye, strlen(bye));
            break;
        }

        if (buffer_command[0] == '\0') continue;  // ignorer les lignes vides

        pid_t pid = fork();
        if (pid == 0){
            execlp(buffer_command, buffer_command, (char *)NULL);
            perror("Commande non reconnue");
            exit(100); //si execlp bug
        } else if (pid > 0){
            waitpid(pid, &status, 0);

            // mettre à jour le dernier status pour le prochain prompt
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
