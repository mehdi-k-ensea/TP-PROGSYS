#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE_BUFFER_COMMAND 4096


int main(void){
    int shell_destination = STDOUT_FILENO;
    int shell_source = STDIN_FILENO;
    const char* welcome = "Bienvenue dans le Shell ENSEA, question 1 .\n";
    char buffer_command[SIZE_BUFFER_COMMAND];
    write(shell_destination,welcome,strlen(welcome));

    int read_command=read(shell_source,buffer_command,SIZE_BUFFER_COMMAND-1);
    write(shell_destination,buffer_command,strlen(buffer_command));//on écrit dans le terminal, et ça recopie

}   