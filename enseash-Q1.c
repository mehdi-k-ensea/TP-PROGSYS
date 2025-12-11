#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main(void){
    int shell_destination = STDOUT_FILENO;
    const char* welcome = "Bienvenue dans le Shell ENSEA, question 1 .\n";
    write(shell_destination,welcome,strlen(welcome));
}   