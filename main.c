#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void sig_handler(int signo);

int main(void) {
    printf("main is waiting for a signal\n");
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        perror("signal error");
        exit(EXIT_FAILURE);
    }
    printf("signal!\n");
    int a = 1 / 0;
    for (;;);
    return EXIT_SUCCESS;
}

void sig_handler(int signo) {
    printf("catch the signal SIGINT %d\n", signo);
}