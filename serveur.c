#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include "Serv_cli_fifo.h"
#include "Handlers_Serv.h"


int main() {
    // Déclarations
    int fd_fifo1, fd_fifo2;
    ClientQuestion question;
    ServerResponse response;
    pid_t client_pid;

    // Initialisation du générateur de nombres aléatoires
    srand(getpid());

    // Ouverture des tubes nommés
    fd_fifo1 = open(FIFO1, O_RDONLY);
    fd_fifo2 = open(FIFO2, O_WRONLY);

    if (fd_fifo1 == -1 || fd_fifo2 == -1) {
        perror("Erreur lors de l'ouverture des tubes nommés.");
        exit(EXIT_FAILURE);
    }

    // Installation des Handlers
    if (signal(SIGUSR1, hand_reveil) == SIG_ERR) {
        perror("Erreur lors de la configuration du gestionnaire SIGUSR1");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = fin_serveur;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    for (int i = 1; i < NSIG; i++) {
        if (i != SIGKILL && i != SIGSTOP) {
            sigaction(i, &sa, NULL);
        }
    }

    while (1) {
        // Lecture d'une question
        if (read(fd_fifo1, &question, sizeof(ClientQuestion)) < 0) {
            perror("Erreur lors de la lecture de la question.");
            continue;
        }

        // Construction de la réponse (génération de nombres aléatoires)
        response.client_number = question.client_number;
        response.n = question.n;
        response.random_numbers = (int *)malloc(question.n * sizeof(int));

        for (int i = 0; i < question.n; i++) {
            response.random_numbers[i] = rand();
        }

        // Envoi de la réponse
        write(fd_fifo2, &response, sizeof(ServerResponse));

        // Envoi du signal SIGUSR1 au client concerné
        client_pid = question.client_number;
        kill(client_pid, SIGUSR1);

        // Libération de la mémoire allouée
        free(response.random_numbers);
    }

    return 0;
}
