#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

// Variáveis que vão ser usadas em alguns locais
#define BUFSIZE 1024
#define NUM_DISPOSITIVOS 5
#define NUM_LOCAIS 5

// Struct de dispositivos, contém todos os campos que podem ser instanciados, mas para cada um só instancia os devidos
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

// Struct local, tem o Id e um apontador para uma estruturas de dispositivos
struct local
{
    int id;
    struct dispositivo *dispositivos;
};

// Definindo as váriaveis que vão ser usadas ao longo do programa
struct local *locais;
char mensagemReq[500];
char mensagemRes[500];

void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <server port\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

// Função que vai ser responsável por tratar as mensagens de requisição que são recebidas pelo servidor
void handleMessage(char *buf)
{
    memset(mensagemRes, 0, 500);
    char *params = strtok(buf, " ");
    // Irá analisar qual foi a mensagem recebida e devolver a mensagem de resposta correspondente
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
                            // Marca como instalado
                            locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 1;
                            // Verifica para cada tipo de dispositivo individualmente
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
                    // Desmarca o indivíduo como instalado
                    locais[localId - 1].dispositivos[dispositivoId - 1].instalado = 0;
                    // Reseta os campos para cada dispositivo
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
                            // Irá atualizar os valores dependendo das entradas da mensagem
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
                    // Irá preencher as informações dependendo de qual dispositivo é
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
            strcpy(mensagemRes, "ERROR 04");
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
                // Irá percorrer todos dispositivos e ver qual dos dispositivos do local está instalado
                // Se estiver instalado, adiciona os campos correspondentes a mensagem de resposta
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
    else if (strstr(params, "UNKNOWN_REQ"))
    {
        // Se vier um comando desconhecido, irá retornar uma resposta específica indicando o para o cliente encerrar sua execução
        strcpy(mensagemRes, "UNKNOWN_RES");
    }
    else if (strstr(params, "KILL_REQ"))
    {
        // Se vier um comando kill, irá retornar uma resposta específica indicando para encerrar tanto cliente quanto servidor
        strcpy(mensagemRes, "KILL_RES");
    }
}

int main(int argc, char **argv)
{
    // Irá alocar memória para o número de locais
    locais = (struct local *)calloc(NUM_LOCAIS, sizeof(struct local));
    // Fará um laço preenchendo os valores do id dos locais e alocando memória para o número de locais neles
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

    // Vai criar a estrutura para receber as informações de qual protocolo será utilizado no servidor
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

    // Servidor vai dar o bind no endereço informado
    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage)))
    {
        checkError("Bind");
    }
    // Servidor vai ficar escutando clientes que podem se conectar a ele
    if (0 != listen(s, 10))
    {
        checkError("Listen");
    }
    // char addrstr[BUFSIZE];
    // addrtostr(addr, addrstr, BUFSIZE);
    // printf("bound to %s, waiting connection\n", addrstr);

    while (1)
    {
        // Vai definir a estrutra que vai receber as informações do cliente na conexão
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);
        // Vai aceitar a conexão do cliente
        int cSock = accept(s, caddr, &caddrlen);
        if (cSock == -1)
        {
            checkError("accept");
        }

        // char caddrstr[BUFSIZE];
        // addrtostr(caddr, caddrstr, BUFSIZE);
        // printf("[log] connection from %s\n", caddrstr);
        // Reseta o buffer das mensagens de requisição
        memset(mensagemReq, 0, 500);
        // Recebe os dados em mensagemReq
        size_t count = recv(cSock, mensagemReq, 500, 0);
        // Vai lidar com a mensagem recebida
        handleMessage(mensagemReq);
        // Irá enviar a resposta de volta para o cliente
        count = send(cSock, mensagemRes, strlen(mensagemRes) + 1, 0);
        if (count != strlen(mensagemRes) + 1)
        {
            checkError("send");
        }
        // Fecha o socket
        close(cSock);
        // Se a mensagem for de KILL, sai do loop e encerra
        if (strstr(mensagemRes, "KILL_RES"))
        {
            break;
        }
    }

    exit(EXIT_SUCCESS);
}
