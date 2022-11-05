#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 1024
#define NUM_DISPOSITIVOS 5
#define NUM_LOCAIS 5
#define INS_REQ "install"
#define REM_REQ "remove"
#define CH_REQ "change"
#define DEV_REQ "show state"
#define SHOW "show"
#define KILL "kill"

struct dispositivo
{
    int id;
    int ligado;
    float angulo;
    int trancado;
    int aberto;
    int localId;
    float canal;
    int instalado;
    float temperatura;
};

struct local
{
    int id;
    struct dispositivo *dispositivos;
};

struct local *locais;
char mensagem[50];
char mensagemReq[500];
char mensagemRes[500];

void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <server port\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

void handleMessage(char *buf)
{
    memset(mensagemRes, 0, 500);
    char *params = strtok(buf, " ");
    if (strstr(params, "INS_REQ"))
    {
        // Vai obter o local id da mensagem
        params = strtok(NULL, " ");
        int localId = atoi(params);
        if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
        {
            params = strtok(NULL, " ");
            int dispositivoId = atoi(params);
            if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 3 || dispositivoId == 4 || dispositivoId == 5)
            {
                // Checa se o objeto já está instalado
                if (locais[localId - 1].dispositivos[dispositivoId - 1].instalado == 0)
                {
                    params = strtok(NULL, " ");
                    if (params)
                    {
                        int param1 = atoi(params);
                        params = strtok(NULL, " ");
                        if (params)
                        {
                            locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            if (dispositivoId == 1)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].temperatura = param2;
                            }
                            else if (dispositivoId == 2)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].canal = param2;
                            }
                            else if (dispositivoId == 3)
                            {
                                int param2 = atoi(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].localId = param2;
                            }
                            else if (dispositivoId == 4)
                            {
                                int param2 = atoi(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].aberto = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].trancado = param2;
                            }
                            else if (dispositivoId == 5)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].angulo = param2;
                            }
                            strcpy(mensagemRes, "OK 01");
                        }
                    }
                }
            }
            else
            {
                strcpy(mensagemRes, "ERROR 03");
            }
        }
        else
        {
            strcpy(mensagemRes, "ERROR 04");
        }
    }
    else if (strstr(params, "REM_REQ"))
    {
        // Vai obter o local id da mensagem
        params = strtok(NULL, " ");
        int localId = atoi(params);
        if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
        {
            params = strtok(NULL, " ");
            int dispositivoId = atoi(params);
            if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 3 || dispositivoId == 4 || dispositivoId == 5)
            {
                // Checa se o objeto já está instalado
                if (locais[localId - 1].dispositivos[dispositivoId - 1].instalado == 1)
                {
                    locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 0;
                    if (dispositivoId == 1)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].ligado = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].temperatura = 0;
                    }
                    else if (dispositivoId == 2)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].ligado = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].canal = 0;
                    }
                    else if (dispositivoId == 3)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].ligado = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].localId = 0;
                    }
                    else if (dispositivoId == 4)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].aberto = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].trancado = 0;
                    }
                    else if (dispositivoId == 5)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].ligado = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].angulo = 0;
                    }
                    strcpy(mensagemRes, "OK 02");
                }
                else
                {
                    strcpy(mensagemRes, "ERROR 01");
                }
            }
            else
            {
                strcpy(mensagemRes, "ERROR 04");
            }
        }
        else
        {
            strcpy(mensagemRes, "ERROR 03");
        }
    }
    else if (strstr(params, "CH_REQ"))
    {
        // Vai obter o local id da mensagem
        params = strtok(NULL, " ");
        int localId = atoi(params);
        if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
        {
            params = strtok(NULL, " ");
            int dispositivoId = atoi(params);
            if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 3 || dispositivoId == 4 || dispositivoId == 5)
            {
                // Checa se o objeto já está instalado
                if (locais[localId - 1].dispositivos[dispositivoId - 1].instalado == 1)
                {
                    params = strtok(NULL, " ");
                    if (params)
                    {
                        int param1 = atoi(params);
                        params = strtok(NULL, " ");
                        if (params)
                        {
                            locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            if (dispositivoId == 1)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].temperatura = param2;
                            }
                            else if (dispositivoId == 2)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].canal = param2;
                            }
                            else if (dispositivoId == 3)
                            {
                                int param2 = atoi(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].localId = param2;
                            }
                            else if (dispositivoId == 4)
                            {
                                int param2 = atoi(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].aberto = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].trancado = param2;
                            }
                            else if (dispositivoId == 5)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].angulo = param2;
                            }
                            strcpy(mensagemRes, "OK 03");
                        }
                    }
                }
                else
                {
                    strcpy(mensagemRes, "ERROR 01");
                }
            }
            else
            {
                strcpy(mensagemRes, "ERROR 03");
            }
        }
        else
        {
            strcpy(mensagemRes, "ERROR 04");
        }
    }
    else if (strstr(params, "DEV_REQ"))
    {
        strcpy(mensagemRes, "DEV_RES ");
        params = strtok(NULL, " ");
        // Vai obter o local id da mensagem
        int localId = atoi(params);
        if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
        {
            params = strtok(NULL, " ");
            int dispositivoId = atoi(params);
            if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 3 || dispositivoId == 4 || dispositivoId == 5)
            {
                // Checa se o objeto já está instalado
                if (locais[localId - 1].dispositivos[dispositivoId - 1].instalado == 1)
                {
                    if (dispositivoId == 1)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 20, "%d %.2f", locais[localId - 1].dispositivos[dispositivoId - 1].ligado, locais[localId - 1].dispositivos[dispositivoId - 1].temperatura);
                    }
                    else if (dispositivoId == 2)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 20, "%d %.2f", locais[localId - 1].dispositivos[dispositivoId - 1].ligado, locais[localId - 1].dispositivos[dispositivoId - 1].canal);
                    }
                    else if (dispositivoId == 3)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 20, "%d %d", locais[localId - 1].dispositivos[dispositivoId - 1].ligado, locais[localId - 1].dispositivos[dispositivoId - 1].localId);
                    }
                    else if (dispositivoId == 4)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 20, "%d %d", locais[localId - 1].dispositivos[dispositivoId - 1].aberto, locais[localId - 1].dispositivos[dispositivoId - 1].trancado);
                    }
                    else if (dispositivoId == 5)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 20, "%d %.2f", locais[localId - 1].dispositivos[dispositivoId - 1].ligado, locais[localId - 1].dispositivos[dispositivoId - 1].angulo);
                    }
                }
                else
                {
                    strcpy(mensagemRes, "ERROR 01");
                }
            }
            else
            {
                strcpy(mensagemRes, "ERROR 03");
            }
        }
        else
        {
            strcpy(mensagem, "ERROR 04");
        }
    }
    else if (strstr(params, "LOC_REQ"))
    {
        strcpy(mensagemRes, "LOC_RES ");
        params = strtok(NULL, " ");
        // Vai obter o local id da mensagem
        int localId = atoi(params);
        if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
        {
            int dispInstalados = 0;
            // Checa se o objeto já está instalado
            for (int i = 0; i < NUM_DISPOSITIVOS; i++)
            {
                if (locais[localId - 1].dispositivos[i].instalado == 1)
                {
                    if (i == 0)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 16, "%d %d %.2f ", i + 1, locais[localId - 1].dispositivos[i].ligado, locais[localId - 1].dispositivos[i].temperatura);
                        dispInstalados++;
                    }
                    else if (i == 1)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 16, "%d %d %.2f ", i + 1, locais[localId - 1].dispositivos[i].ligado, locais[localId - 1].dispositivos[i].canal);
                        dispInstalados++;
                    }
                    else if (i == 2)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 16, "%d %d %d ", i + 1, locais[localId - 1].dispositivos[i].ligado, locais[localId - 1].dispositivos[i].localId);
                        dispInstalados++;
                    }
                    else if (i == 3)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 16, "%d %d %d ", i + 1, locais[localId - 1].dispositivos[i].aberto, locais[localId - 1].dispositivos[i].trancado);
                        dispInstalados++;
                    }
                    else if (i == 4)
                    {
                        snprintf(mensagemRes + strlen(mensagemRes), 16, "%d %d %.2f ", i + 1, locais[localId - 1].dispositivos[i].ligado, locais[localId - 1].dispositivos[i].angulo);
                        dispInstalados++;
                    }
                }
            }
            if (dispInstalados == 0)
            {
                strcpy(mensagemRes, "ERROR 02");
            }
        }
        else
        {
            strcpy(mensagemRes, "ERROR 04");
        }
    }
}

int main(int argc, char **argv)
{
    locais = (struct local *)calloc(NUM_LOCAIS, sizeof(struct local));
    for (int i = 0; i < NUM_LOCAIS; i++)
    {
        locais[i].id = i + 1;
        locais[i].dispositivos = (struct dispositivo *)calloc(NUM_DISPOSITIVOS, sizeof(struct dispositivo));
        for (int j = 0; j < NUM_DISPOSITIVOS; j++)
        {
            locais[i].dispositivos[j].id = j + 1;
        }
    }

    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }

    // Cria o socket
    int s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
    {
        checkError("Erro na criação do socket");
    }

    // Função para continuar reutilizando a porta no socket
    int enable = 1;
    if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        checkError("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage)))
    {
        checkError("Bind");
    }

    if (0 != listen(s, 10))
    {
        checkError("Listen");
    }
    char addrstr[BUFSIZE];
    addrtostr(addr, addrstr, BUFSIZE);
    printf("bound to %s, waiting connection\n", addrstr);

    while (1)
    {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);
        int cSock = accept(s, caddr, &caddrlen);
        if (cSock == -1)
        {
            checkError("accept");
        }

        char caddrstr[BUFSIZE];
        addrtostr(caddr, caddrstr, BUFSIZE);
        printf("[log] connection from %s\n", caddrstr);
        char buf[BUFSIZE];
        memset(mensagemReq, 0, 500);
        memset(buf, 0, BUFSIZE);
        memset(mensagem, 0, 50);
        size_t count = recv(cSock, mensagemReq, 500, 0);
        handleMessage(mensagemReq);
        count = send(cSock, mensagemRes, strlen(mensagemRes) + 1, 0);
        if (count != strlen(mensagemRes) + 1)
        {
            checkError("send");
        }
        close(cSock);
    }

    exit(EXIT_SUCCESS);
}
