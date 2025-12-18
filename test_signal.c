#include <stdio.h>
#include <unistd.h>

int main(void) {

    while (1) {
        sleep(3); // pour éviter de spam le CPU
        printf("Je suis test_signal, mon PID est %d\n", getpid());
    }   
}
