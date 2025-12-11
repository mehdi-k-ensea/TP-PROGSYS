#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE_BUFFER_COMMAND 4096

int main(void){
    int shell_destination = STDOUT_FILENO;
    int shell_source = STDIN_FILENO;

    const char* welcome = "Bienvenue dans le Shell ENSEA, question 2c.\n";
    write(shell_destination, welcome, strlen(welcome));

    char buffer_command[SIZE_BUFFER_COMMAND];

    while(1){
        const char* prompt = "enseash % ";
        write(shell_destination, prompt, strlen(prompt));

        int read_command = read(shell_source, buffer_command, SIZE_BUFFER_COMMAND-1);
        if (read_command <= 0) perror("Erreur");

        buffer_command[read_command-1] = '\0'; // remplace le 'ENTER'

        // On a notre processus père (notre shell) qu'on laisse "vivant" tout du long avant un potentiel exit,et a chaque commande 
        // éxécuté on recrée un fils avec fork , qui sert de "kamikaze" pour la fonction excelp
        pid_t pid = fork();
        if (pid == 0){
            execlp(buffer_command, buffer_command, (char *)NULL);
            perror("Commande non reconnu");
            exit(1); // si execlp bug
        } else if (pid > 0){
            int status;
            waitpid(pid, &status, 0); // attend la mort du fils (donc de la commande)
        }
    }

}
