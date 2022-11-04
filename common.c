#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

void checkError(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int addrParse(const char *addrstr, const char *portstr, struct sockaddr_storage *storage)
{
    // Se não tiver nem endereço nem porta retorna com erro
    if (addrstr == NULL || portstr == NULL)
    {
        return -1;
    }

    // Transformar de string da porta para um inteiro de 16 bits
    uint16_t port = (uint16_t)atoi(portstr);
    if (port == 0)
    {
        return -1;
    }
    // Número da porta é sempre big endian
    port = htons(port);

    // Vai checar se é o IPv4, se não, passa pro próximo
    struct in_addr inaddr4;
    if (inet_pton(AF_INET, addrstr, &inaddr4))
    {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_port = port;
        addr4->sin_addr = inaddr4;
        return 0;
    }
    // Vai checar se é o IPv6, se não, houve erro
    struct in6_addr inaddr6;
    if (inet_pton(AF_INET6, addrstr, &inaddr4))
    {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = port;
        addr6->sin6_addr = inaddr6;
        memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
        return 0;
    }

    return -1;
}

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize)
{
    int version;
    char addrstr[INET6_ADDRSTRLEN + 1] = "";
    uint16_t port;

    if (addr->sa_family == AF_INET)
    {
        version = 4;
        struct sockaddr_in *addr4 = (struct sockaddr_in *)addr;
        if (!inet_ntop(AF_INET, &(addr4->sin_addr), addrstr, INET6_ADDRSTRLEN + 1))
        {
            checkError("ntop");
        }
        port = ntohs(addr4->sin_port);
    }
    else if (addr->sa_family == AF_INET6)
    {
        version = 6;
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)addr;
        if (!inet_ntop(AF_INET6, &(addr6->sin6_addr), addrstr, INET6_ADDRSTRLEN + 1))
        {
            checkError("ntop");
        }
        port = ntohs(addr6->sin6_port);
    }
    else
    {
        checkError("Tipo de protocolo IP inválido");
    }
    if (str)
    {
        snprintf(str, strsize, "IPv%d %s %hu", version, addrstr, port);
    }
}

int server_sockaddr_init(const char *proto, const char *portstr, struct sockaddr_storage *storage)
{
    // Transformar de string da porta para um inteiro de 16 bits
    uint16_t port = (uint16_t)atoi(portstr);
    if (port == 0)
    {
        return -1;
    }
    // Número da porta é sempre big endian
    port = htons(port);

    memset(storage, 0, sizeof((*storage)));
    if (0 == strcmp(proto, "v4"))
    {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_port = port;
        addr4->sin_addr.s_addr = INADDR_ANY;
        return 0;
    }
    else if (0 == strcmp(proto, "v6"))
    {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = port;
        addr6->sin6_addr = in6addr_any;
        return 0;
    }
    else
    {
        return -1;
    }
}