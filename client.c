#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

void usage(int argc, char **argv)
{
    printf("usage: %s <server IP> <server port\n", argv[0]);
    printf("example: %s 123.0.0.3 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    // Se não preencher todos os argumentos necessários na chamada
    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != addrParse(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }
    // Cria o socket
    int s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
    {
        checkError("Erro na criação do socket");
    }
    struct sockaddr *addr = (struct sockaddr *)(&storage);
    // Faz a conexão
    if (0 != connect(s, addr, sizeof(storage)))
    {
        printf("Erro na conexão\n");
    }

    // Buffer para armazenar as mensagens e imprimi-las
    char addrstr[BUFSIZE];
    addrtostr(addr, addrstr, BUFSIZE);

    printf("connected to %s\n", addrstr);
    // Buffer para armazenar o dado
    char bufMsg[BUFSIZE];
    unsigned dataReceived = 0;
    while (1)
    {

        memset(bufMsg, 0, BUFSIZE);
        printf("<mensagem> ");
        fgets(bufMsg, BUFSIZE - 1, stdin);
        // Váriavel count que vai receber o número de bytes transmitidos
        size_t count = send(s, bufMsg, strlen(bufMsg) + 1, 0);
        if (count != strlen(bufMsg) + 1)
        {
            checkError("Número de bytes não batem");
        }
        else if (count > 500)
        {
            checkError("A mensagem tem tamanho máximo de 500 bytes");
        }
        memset(bufMsg, 0, BUFSIZE);
        // Receber os dados
        count = recv(s, bufMsg, BUFSIZE, 0);
        printf("buff: %s\n", bufMsg);
        if (count == 0)
        {
            break;
        }
        dataReceived += count;
        puts(bufMsg);
    }
    close(s);

    return (EXIT_SUCCESS);
}