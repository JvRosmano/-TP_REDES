#pragma once
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int addrParse(const char *addrstr, const char *portstr, struct sockaddr_storage *storage);

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize);

int server_sockaddr_init(const char *proto, const char *portstr, struct sockaddr_storage *storage);

void checkError(const char *msg);