#ifndef SERV_CLI_FIFO_H
#define SERV_CLI_FIFO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define NMAX 100

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

typedef struct {
    int client_number;
    int n;
} ClientQuestion;

typedef struct {
    int client_number;
    int* random_numbers;
    int n;
} ServerResponse;

#endif
