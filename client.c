#include "serv_cli_fifo.h"
#include "Handlers_Cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>


int main() {
    // Déclarations
    int fd_fifo1, fd_fifo2;
    ClientQuestion question;
    ServerResponse response;

    // Ouverture des tubes nommés
    fd_fifo1 = open(FIFO1, O_WRONLY);
    fd_fifo2 = open(FIFO2, O_RDONLY);

    if (fd_fifo1 == -1 || fd_fifo2 == -1) {
        perror("Erreur lors de l'ouverture des tubes nommés.");
        exit(EXIT_FAILURE);
    }

    // Installation du gestionnaire de signal pour SIGUSR1
    if (signal(SIGUSR1, hand_reveil) == SIG_ERR) {
        perror("Erreur lors de la configuration du gestionnaire SIGUSR1");
        exit(EXIT_FAILURE);
    }

    // Construction et envoi d'une question
    question.client_number = getpid();
    question.n = rand() % NMAX + 1;

    if (write(fd_fifo1, &question, sizeof(ClientQuestion)) == -1) {
        perror("Erreur lors de l'envoi de la question.");
        exit(EXIT_FAILURE);
    }

    // Attente de la réponse
    pause();

    // Lecture de la réponse
    if (read(fd_fifo2, &response, sizeof(ServerResponse)) == -1) {
        perror("Erreur lors de la lecture de la réponse.");
        exit(EXIT_FAILURE);
    }

    // Envoi du signal SIGUSR1 au serveur
    kill(getpid(), SIGUSR1);

    // Traitement local de la réponse
    printf("Client %d a reçu une réponse avec %d nombres aléatoires:\n", getpid(), response.n);
    for (int i = 0; i < response.n; i++) {
        printf("%d ", response.random_numbers[i]);
    }
    printf("\n");

    // Libération de la mémoire allouée
    free(response.random_numbers);

    return 0;
}
