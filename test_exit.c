#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Je suis test_signal, mon PID est %d\n", getpid());
    return 3;
}
