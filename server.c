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

void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <server port\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

void addDevice(char *params)
{
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int dispositivoId = atoi(params);
    if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 3 || dispositivoId == 4 || dispositivoId == 5)
    {
        params = strtok(NULL, " ");
        if (strstr(params, "in"))
        {
            params = strtok(NULL, " ");
            int localId = atoi(params);
            if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
            {
                // Checa se o objeto já está instalado
                if (locais[localId - 1].dispositivos[dispositivoId - 1].instalado == 0)
                {
                    params = strtok(NULL, " ");
                    int param1 = atoi(params);
                    if (param1)
                    {
                        params = strtok(NULL, " ");
                        if (params)
                        {
                            if (dispositivoId == 1)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].temperatura = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            else if (dispositivoId == 2)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].canal = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            else if (dispositivoId == 3)
                            {
                                int param2 = atoi(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].localId = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            else if (dispositivoId == 4)
                            {
                                int param2 = atoi(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].aberto = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].trancado = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            else if (dispositivoId == 5)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].angulo = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            strcpy(mensagem, "succesfull instalation");
                        }
                    }
                }
            }
            else
            {
                strcpy(mensagem, "invalid local");
            }
        }
    }
    else
    {
        strcpy(mensagem, "invalid device");
    }
}

void removeDevice(char *params)
{
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int dispositivoId = atoi(params);
    if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 3 || dispositivoId == 4 || dispositivoId == 5)
    {
        params = strtok(NULL, " ");
        if (strstr(params, "in"))
        {
            params = strtok(NULL, " ");
            int localId = atoi(params);
            if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
            {
                // Checa se o objeto já está instalado
                if (locais[localId - 1].dispositivos[dispositivoId - 1].instalado == 1)
                {
                    if (dispositivoId == 1)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].ligado = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].temperatura = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 0;
                    }
                    else if (dispositivoId == 2)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].ligado = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].canal = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 0;
                    }
                    else if (dispositivoId == 3)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].ligado = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].localId = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 0;
                    }
                    else if (dispositivoId == 4)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].aberto = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].trancado = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 0;
                    }
                    else if (dispositivoId == 5)
                    {
                        locais[localId - 1].dispositivos[dispositivoId - 1].ligado = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].angulo = 0;
                        locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 0;
                    }
                    strcpy(mensagem, "successful removal");
                }
                else
                {
                    strcpy(mensagem, "device not installed");
                }
            }
            else
            {
                strcpy(mensagem, "invalid local");
            }
        }
    }
    else
    {
        strcpy(mensagem, "invalid device");
    }
}

void changeDeviceState(char *params)
{
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int dispositivoId = atoi(params);
    if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 3 || dispositivoId == 4 || dispositivoId == 5)
    {
        params = strtok(NULL, " ");
        if (strstr(params, "in"))
        {
            params = strtok(NULL, " ");
            int localId = atoi(params);
            if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
            {
                // Checa se o objeto já está instalado
                if (locais[localId - 1].dispositivos[dispositivoId - 1].instalado == 1)
                {
                    params = strtok(NULL, " ");
                    int param1 = atoi(params);
                    if (param1)
                    {
                        params = strtok(NULL, " ");
                        if (params)
                        {
                            if (dispositivoId == 1)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].temperatura = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            else if (dispositivoId == 2)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].canal = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            else if (dispositivoId == 3)
                            {
                                int param2 = atoi(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].localId = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            else if (dispositivoId == 4)
                            {
                                int param2 = atoi(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].aberto = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].trancado = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            else if (dispositivoId == 5)
                            {
                                float param2 = atof(params);
                                locais[localId - 1].dispositivos[dispositivoId - 1].ligado = param1;
                                locais[localId - 1].dispositivos[dispositivoId - 1].angulo = param2;
                                locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            }
                            strcpy(mensagem, "successful change");
                        }
                    }
                }
                else
                {
                    strcpy(mensagem, "device not installed");
                }
            }
            else
            {
                strcpy(mensagem, "invalid local");
            }
        }
    }
    else
    {
        strcpy(mensagem, "invalid device");
    }
}

void showDeviceState(char *params)
{
    // Vai obter o local id da mensagem
    int dispositivoId = atoi(params);
    if (dispositivoId == 1 || dispositivoId == 2 || dispositivoId == 3 || dispositivoId == 4 || dispositivoId == 5)
    {
        params = strtok(NULL, " ");
        if (strstr(params, "in"))
        {
            params = strtok(NULL, " ");
            int localId = atoi(params);
            if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
            {
                // Checa se o objeto já está instalado
                if (locais[localId - 1].dispositivos[dispositivoId - 1].instalado == 1)
                {
                    if (dispositivoId == 1)
                    {
                        snprintf(mensagem, 30, "device %d in %d: %d %f", dispositivoId, localId, locais[localId - 1].dispositivos[dispositivoId - 1].ligado, locais[localId - 1].dispositivos[dispositivoId - 1].temperatura);
                    }
                    else if (dispositivoId == 2)
                    {
                        snprintf(mensagem, 30, "device %d in %d: %d %f", dispositivoId, localId, locais[localId - 1].dispositivos[dispositivoId - 1].ligado, locais[localId - 1].dispositivos[dispositivoId - 1].canal);
                    }
                    else if (dispositivoId == 3)
                    {
                        snprintf(mensagem, 30, "device %d in %d: %d %d", dispositivoId, localId, locais[localId - 1].dispositivos[dispositivoId - 1].ligado, locais[localId - 1].dispositivos[dispositivoId - 1].localId);
                    }
                    else if (dispositivoId == 4)
                    {
                        snprintf(mensagem, 30, "device %d in %d: %d %d", dispositivoId, localId, locais[localId - 1].dispositivos[dispositivoId - 1].aberto, locais[localId - 1].dispositivos[dispositivoId - 1].trancado);
                    }
                    else if (dispositivoId == 5)
                    {
                        snprintf(mensagem, 30, "device %d in %d: %d %f", dispositivoId, localId, locais[localId - 1].dispositivos[dispositivoId - 1].ligado, locais[localId - 1].dispositivos[dispositivoId - 1].angulo);
                    }
                }
                else
                {
                    strcpy(mensagem, "device not installed");
                }
            }
            else
            {
                strcpy(mensagem, "invalid local");
            }
        }
    }
    else
    {
        strcpy(mensagem, "invalid device");
    }
}

void showLocalState(char *params)
{
    // Vai obter o local id da mensagem
    params = strtok(NULL, " ");
    int localId = atoi(params);
    if (localId == 1 || localId == 2 || localId == 3 || localId == 4 || localId == 5)
    {
        snprintf(mensagem, 12, "local %d: ", localId);
        // Checa se o objeto já está instalado
        for (int i = 0; i < NUM_DISPOSITIVOS; i++)
        {
            if (locais[localId - 1].dispositivos[i].instalado == 1)
            {
                if (i == 0)
                {
                    snprintf(mensagem + strlen(mensagem), 16, "%d (%d %.2f) ", i + 1, locais[localId - 1].dispositivos[i].ligado, locais[localId - 1].dispositivos[i].temperatura);
                }
                else if (i == 1)
                {
                    snprintf(mensagem + strlen(mensagem), 16, "%d (%d %.2f) ", i + 1, locais[localId - 1].dispositivos[i].ligado, locais[localId - 1].dispositivos[i].canal);
                }
                else if (i == 2)
                {
                    snprintf(mensagem + strlen(mensagem), 16, "%d (%d %d) ", i + 1, locais[localId - 1].dispositivos[i].ligado, locais[localId - 1].dispositivos[i].localId);
                }
                else if (i == 3)
                {
                    snprintf(mensagem + strlen(mensagem), 16, "%d (%d %d) ", i + 1, locais[localId - 1].dispositivos[i].aberto, locais[localId - 1].dispositivos[i].trancado);
                }
                else if (i == 4)
                {
                    snprintf(mensagem + strlen(mensagem), 16, "%d (%d %.2f) ", i + 1, locais[localId - 1].dispositivos[i].ligado, locais[localId - 1].dispositivos[i].angulo);
                }
            }
        }
    }
    else
    {
        strcpy(mensagem, "invalid local");
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
        memset(buf, 0, BUFSIZE);
        memset(mensagem, 0, 50);
        size_t count = recv(cSock, buf, BUFSIZE, 0);

        char *params = strtok(buf, " ");
        if (strstr(params, INS_REQ))
        {
            addDevice(params);
        }
        else if (strstr(params, REM_REQ))
        {
            removeDevice(params);
        }
        else if (strstr(params, CH_REQ))
        {
            changeDeviceState(params);
        }
        else if (strstr(params, SHOW))
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
        else if (strstr(buf, KILL))
        {
            break;
        }
        else
        {
            strcpy(mensagem, "kill");
        }
        sprintf(buf, "%s", mensagem);
        count = send(cSock, buf, strlen(buf) + 1, 0);
        if (count != strlen(buf) + 1)
        {
            checkError("send");
        }
        close(cSock);
    }

    exit(EXIT_SUCCESS);
}
