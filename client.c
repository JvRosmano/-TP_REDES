#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
// Strings que serão responsáveis por conter as mensagens de requisição e resposta
// Possuem tamanho máximo de 500 bytes como especificado no TP que deveria ser o limite
char mensagemReq[500];
char mensagemRes[500];

void usage(int argc, char **argv)
{
    printf("usage: %s <server IP> <server port\n", argv[0]);
    printf("example: %s 123.0.0.3 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

// Função que será chamada para montar a mensagem de requisão quando houver install
void addDevice(char *params)
{
    strcpy(mensagemReq, "INS_REQ ");
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int dispositivoId = atoi(params);
    // Separará a string
    params = strtok(NULL, " ");
    if (strstr(params, "in"))
    {
        params = strtok(NULL, " ");
        int localId = atoi(params);
        if (localId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
        // Vai colocar um zero para seguir a mensagem se não houver lugar definido
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }
        if (dispositivoId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", dispositivoId);
        }
        // Vai colocar um zero para seguir a mensagem se não houver dispositivo definido
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
        // Vai colocar um zero para seguir a mensagem se não houver o primeiro parâmetro definido
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
        // Vai colocar um zero para seguir a mensagem se não houver o segundo parâmetro definido
        else
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
    }
}

// Função que será chamada para montar a mensagem de requisição de remove
void removeDevice(char *params)
{
    strcpy(mensagemReq, "REM_REQ ");
    // Irá quebrando a string que recebe como parâmetro em tokens
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int dispositivoId = atoi(params);

    params = strtok(NULL, " ");
    if (strstr(params, "in"))
    {
        params = strtok(NULL, " ");
        int localId = atoi(params);
        if (localId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
        // Vai colocar um zero para seguir a mensagem se não houver local Id
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }
        if (dispositivoId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d", dispositivoId);
        }
        // Vai colocar um zero para seguir a mensagem se não houver dispositivo Id
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0");
        }
    }
}

// Função que será chamado para montar a mensagem de requisição de change
void changeDeviceState(char *params)
{
    // Quebrará a string recebida como parâmetro em tokens
    strcpy(mensagemReq, "CH_REQ ");
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int dispositivoId = atoi(params);

    params = strtok(NULL, " ");
    if (strstr(params, "in"))
    {
        params = strtok(NULL, " ");
        int localId = atoi(params);
        if (localId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
        // Vai colocar um zero para seguir a mensagem caso não houver local Id
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }
        if (dispositivoId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", dispositivoId);
        }
        // Vai colocar um zero para seguir a mensagem caso não houver dispositivo Id
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
        // Vai colocar um zero para seguir a mensagem caso não houver parâmetro 1
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
        // Vai colocar um zero para seguir a mensagem caso não houver parâmetro 2
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0");
        }
    }
}

// Função que será chamada para montar mensagem de requisição do tipo show state X in Y
void showDeviceState(char *params)
{
    // Vai quebrar a string em tokens
    strcpy(mensagemReq, "DEV_REQ ");
    // Vai obter o local id da mensagem
    int dispositivoId = atoi(params);

    params = strtok(NULL, " ");
    if (strstr(params, "in"))
    {
        params = strtok(NULL, " ");
        int localId = atoi(params);
        if (localId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d ", localId);
        }
        // Vai colocar um zero para seguir a mensagem caso não local Id
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0 ");
        }
        if (dispositivoId)
        {
            snprintf(mensagemReq + strlen(mensagemReq), 10, "%d", dispositivoId);
        }
        // Vai colocar um zero para seguir a mensagem caso não dispositivo Id
        else
        {
            strcpy(mensagemReq + strlen(mensagemReq), "0");
        }
    }
}

// Função que será chamada para montar mensagem de requisição do tipo show state in X
void showLocalState(char *params)
{
    // Vai montar a string a partir da váriavel recebida como parâmetro
    params = strtok(NULL, " ");
    strcpy(mensagemReq, "LOC_REQ ");
    int localId = atoi(params);
    if (localId)
    {
        snprintf(mensagemReq + strlen(mensagemReq), 10, "%d", localId);
    }
    // Vai colocar um zero para seguir a mensagem caso não tenha local Id
    else
    {
        strcpy(mensagemReq + strlen(mensagemReq), "0");
    }
}

// Função que vai preencher o buffer de mensagem de requisição para mandar na comunicação
void createMessage(char *buf)
{
    // Resetará o buffer da mensagem de requisição para cada requisição
    memset(mensagemReq, 0, 500);
    char *params = strtok(buf, " ");
    // Irá analisar cada caso de comando do teclado e montará a mensagem de requisição adequada
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
        strcpy(mensagemReq, "UNKNOWN_REQ");
    }
}

// Função que vai preencher o buffer de mensagem de resposta que receber na comunicação
void receiveMessage(char *buf)
{
    // Reseta o buffer da mensagem de resposta
    memset(mensagemRes, 0, 500);
    char *params = strtok(buf, " ");
    // Irá analisar cada caso da string recebida como parâmetro tokenizada
    if (strstr(params, "OK"))
    {
        params = strtok(NULL, " ");
        int codigo = atoi(params);
        // Irá analisar o código recebido e colocar o respectivo texto para o cliente
        if (codigo == 1)
        {
            strcpy(mensagemRes, "successful installation");
        }
        else if (codigo == 2)
        {
            strcpy(mensagemRes, "successful removal");
        }
        else if (codigo == 3)
        {
            strcpy(mensagemRes, "successful change");
        }
    }
    else if (strstr(params, "ERROR"))
    {
        params = strtok(NULL, " ");
        int codigo = atoi(params);
        // Irá analisar o código recebido e colocar o respectivo texto para o cliente
        if (codigo == 1)
        {
            strcpy(mensagemRes, "device not installed");
        }
        else if (codigo == 2)
        {
            strcpy(mensagemRes, "no devices");
        }
        else if (codigo == 3)
        {
            strcpy(mensagemRes, "invalid device");
        }
        else if (codigo == 4)
        {
            strcpy(mensagemRes, "invalid local");
        }
    }
    else if (strstr(params, "DEV_RES"))
    {
        // Irá analisar a resposta recebida e pegará os dados de local Id e dispositivo Id da mensagem de requisição
        params = strtok(NULL, " ");
        int param1 = atoi(params);
        params = strtok(NULL, " ");
        float param2 = atof(params);
        char *paramsReq = strtok(mensagemReq, " ");
        if (strstr(paramsReq, "DEV_REQ"))
        {
            paramsReq = strtok(NULL, " ");
            int localId = atoi(paramsReq);
            paramsReq = strtok(NULL, " ");
            int dispositivoId = atoi(paramsReq);
            // Cada dispositivo tem os próprios campos, fará o devido caso para cada um
            if (dispositivoId == 1)
            {
                snprintf(mensagemRes, 30, "device %d in %d: %d %.2f", dispositivoId, localId, param1, param2);
            }
            else if (dispositivoId == 2)
            {
                snprintf(mensagemRes, 30, "device %d in %d: %d %.2f", dispositivoId, localId, param1, param2);
            }
            else if (dispositivoId == 3)
            {
                snprintf(mensagemRes, 30, "device %d in %d: %d %d", dispositivoId, localId, param1, (int)param2);
            }
            else if (dispositivoId == 4)
            {
                snprintf(mensagemRes, 30, "device %d in %d: %d %d", dispositivoId, localId, param1, (int)param2);
            }
            else if (dispositivoId == 5)
            {
                snprintf(mensagemRes, 30, "device %d in %d: %d %.2f", dispositivoId, localId, param1, param2);
            }
        }
    }
    else if (strstr(params, "LOC_RES"))
    {
        // Pega o local ID da mensagem de requisição
        char localId = mensagemReq[8];
        snprintf(mensagemRes, 16, "local %c: ", localId);
        for (int i = 0; i < 15; i++)
        {
            // Irá preencher a segunda parte da string de acordo com as informações dos campos
            params = strtok(NULL, " ");
            if (!params)
            {
                break;
            }
            if (i % 3 == 0)
            {
                snprintf(mensagemRes + strlen(mensagemRes), 16, "%s (", params);
            }
            else if ((i - 1) % 3 == 0)
            {
                snprintf(mensagemRes + strlen(mensagemRes), 16, "%s ", params);
            }
            else if ((i - 2) % 3 == 0)
            {
                snprintf(mensagemRes + strlen(mensagemRes), 16, "%s) ", params);
            }
        }
    }
}

int main(int argc, char **argv)
{
    // Se não preencher todos os argumentos necessários na chamada
    if (argc < 3)
    {
        usage(argc, argv);
    }

    // Vai instanciar a estrutura que pode receber tanto IPv4 e IPv6
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
            checkError("Erro na conexão");
        }

        // Buffer para armazenar as mensagens e imprimi-las
        // char addrstr[BUFSIZE];
        // addrtostr(addr, addrstr, BUFSIZE);

        // printf("connected to %s\n", addrstr);
        // Buffer para armazenar o dado
        char conteudoMsg[BUFSIZE];
        memset(conteudoMsg, 0, BUFSIZE);
        // printf("<mensagem> ");
        fgets(conteudoMsg, BUFSIZE - 1, stdin);
        createMessage(conteudoMsg);
        // Váriavel count que vai receber o número de bytes transmitidos
        size_t count = send(s, mensagemReq, strlen(mensagemReq) + 1, 0);
        if (count != strlen(mensagemReq) + 1)
        {
            checkError("Número de bytes não batem");
        }

        char conteudoResp[BUFSIZE];
        // Receber os dados
        count = recv(s, conteudoResp, BUFSIZE, 0);
        receiveMessage(conteudoResp);
        if (strstr(conteudoResp, "UNKNOWN_RES") || strstr(conteudoResp, "KILL_RES") || count == 0)
        {
            break;
        }
        puts(mensagemRes);
    }
    close(s);

    return (EXIT_SUCCESS);
}