#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
char mensagemReq[500];
char mensagemRes[500];

void usage(int argc, char **argv)
{
    printf("usage: %s <server IP> <server port\n", argv[0]);
    printf("example: %s 123.0.0.3 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

void addDevice(char *params)
{
    strcpy(mensagemReq, "INS_REQ ");
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int dispositivoId = atoi(params);
    params = strtok(NULL, " ");
    if (strstr(params, "in"))
    {
        params = strtok(NULL, " ");
        int localId = atoi(params);
        // Vai colocar um zero para seguir a mensagem
        if (localId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }
        if (dispositivoId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", dispositivoId);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }

        params = strtok(NULL, " ");
        int param1 = atoi(params);
        if (param1)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", param1);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }

        params = strtok(NULL, " ");
        if (params)
        {
            if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 5)
            {
                float param2 = atof(params);
                snprintf(mensagemReq + strlen(mensagemReq), 10, "%.2f ", param2);
            }
            else if (dispositivoId == 4)
            {
                int param2 = atoi(params);
                snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", param2);
            }
            else if (dispositivoId == 3)
            {
                snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
            }
        }
        else
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
    }
}

void removeDevice(char *params)
{
    strcpy(mensagemReq, "REM_REQ ");
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int dispositivoId = atoi(params);

    params = strtok(NULL, " ");
    if (strstr(params, "in"))
    {
        params = strtok(NULL, " ");
        int localId = atoi(params);
        // Vai colocar um zero para seguir a mensagem
        if (localId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }
        if (dispositivoId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d", dispositivoId);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0");
        }
    }
}

void changeDeviceState(char *params)
{
    strcpy(mensagemReq, "CH_REQ ");
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int dispositivoId = atoi(params);

    params = strtok(NULL, " ");
    if (strstr(params, "in"))
    {
        params = strtok(NULL, " ");
        int localId = atoi(params);
        // Vai colocar um zero para seguir a mensagem
        if (localId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }
        if (dispositivoId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", dispositivoId);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }

        params = strtok(NULL, " ");
        int param1 = atoi(params);
        if (param1)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", param1);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }

        params = strtok(NULL, " ");
        if (params)
        {
            if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 5)
            {
                float param2 = atof(params);
                snprintf(mensagemReq + strlen(mensagemReq), 10, "%.2f ", param2);
            }
            else if (dispositivoId == 3 || dispositivoId == 4)
            {
                int param2 = atoi(params);
                snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", param2);
            }
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0");
        }
    }
}

void showDeviceState(char *params)
{
    strcpy(mensagemReq, "DEV_REQ ");
    // Vai obter o local id da mensagem
    int dispositivoId = atoi(params);

    params = strtok(NULL, " ");
    if (strstr(params, "in"))
    {
        params = strtok(NULL, " ");
        int localId = atoi(params);
        // Vai colocar um zero para seguir a mensagem
        if (localId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }
        if (dispositivoId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d", dispositivoId);
        }
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0");
        }
    }
}

void showLocalState(char *params)
{
    params = strtok(NULL, " ");
    strcpy(mensagemReq, "LOC_REQ ");
    // Vai obter o local id da mensagem
    int localId = atoi(params);
    if (localId)
    {
        snprintf(mensagemReq + strlen(mensagemReq), 10, "%d", localId);
    }
    else
    {
        strcpy(mensagemReq + strlen(mensagemReq), "0");
    }
}

void createMessage(char *buf)
{
    memset(mensagemReq, 0, 500);
    char *params = strtok(buf, " ");
    if (strstr(params, "install"))
    {
        addDevice(params);
    }
    else if (strstr(params, "remove"))
    {
        removeDevice(params);
    }
    else if (strstr(params, "change"))
    {
        changeDeviceState(params);
    }
    else if (strstr(params, "show"))
    {
        params = strtok(NULL, " ");
        if (strstr(params, "state"))
        {
            params = strtok(NULL, " ");
            if (strstr(params, "in"))
            {
                showLocalState(params);
            }
            else
            {
                showDeviceState(params);
            }
        }
    }
    else if (strstr(buf, "kill"))
    {
        strcpy(mensagemReq, "KILL_REQ");
    }
    else
    {
        strcpy(mensagemReq, "INVALID_REQ");
    }
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
    int s;
    while (1)
    {
        // Cria o socket
        s = socket(storage.ss_family, SOCK_STREAM, 0);
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
        char conteudoMsg[BUFSIZE];
        memset(conteudoMsg, 0, BUFSIZE);
        printf("<mensagem> ");
        fgets(conteudoMsg, BUFSIZE - 1, stdin);
        createMessage(conteudoMsg);
        // Váriavel count que vai receber o número de bytes transmitidos
        size_t count = send(s, mensagemReq, strlen(mensagemReq) + 1, 0);
        if (count != strlen(mensagemReq) + 1)
        {
            checkError("Número de bytes não batem");
        }

        memset(mensagemRes, 0, BUFSIZE);
        // Receber os dados
        count = recv(s, mensagemRes, BUFSIZE, 0);
        // if (strstr(mensagemRes, "kill") || count == 0)
        // {
        //     break;
        // }
        puts(mensagemRes);
    }
    close(s);

    return (EXIT_SUCCESS);
}