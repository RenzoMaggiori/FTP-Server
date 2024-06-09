/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** socket
*/

#include "../include/my_ftp.h"

socket_t create_socket(int domain, int type, int protocol)
{
    socket_t sock;
    int opt = -1;

    sock.sockfd = socket(domain, type, protocol);
    if (sock.sockfd < 0) {
        perror("Socket creation failed");
        exit(FAILURE);
    }
    if (setsockopt(sock.sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        &opt, sizeof(opt)) == -1)
        exit(FAILURE);
    return sock;
}

void bind_socket(socket_t *sock, const char *ip, int port)
{
    memset(&(sock->addr), 0, sizeof(sock->addr));
    sock->addr.sin_family = AF_INET;
    sock->addr.sin_port = htons(port);
    sock->addr.sin_addr.s_addr = inet_addr(ip);
    if (bind(sock->sockfd, (struct sockaddr *)&(sock->addr),
        sizeof(sock->addr)) < 0) {
        perror("Bind failed");
        exit(FAILURE);
    }
}

void listen_socket(socket_t *sock, int backlog)
{
    if (listen(sock->sockfd, backlog) < 0) {
        perror("Listen failed");
        exit(FAILURE);
    }
}
