/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** accept_client
*/

#include "../include/my_ftp.h"

// void display_client(client_t *client) {
//     printf("Client [");
//     printf("Socket: %d\n", client->socket);
//     printf("Response: %s\n", client->response);
//     printf("Username: (%s)\n", client->username);
//     printf("Password: (%s)\n", client->pass);
//     printf("Authenticated: %s", (client->authenticated ? "true" : "false"));
//     printf("Connected: %s", (client->is_connected ? "true" : "false"));
//     printf("]\n");
// }

void client_init(client_t *client, ftpserver_t *ftp, int client_socket)
{
    client->socket = client_socket;
    client->next = ftp->clients;
    client->is_connected = true;
    client->authenticated = false;
    client->currentdir = (char *)ftp->path;
    client->root = (char *)ftp->path;
    client->pass = calloc(MAX_SIZE, sizeof(char));
    client->username = calloc(MAX_SIZE, sizeof(char));
    client->response = "220 Service ready for new user.\r\n";
    ftp->clients = client;
}

void accept_new_connection(ftpserver_t *ftp)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_socket = accept(ftp->server_socket.sockfd,
        (struct sockaddr *)&client_addr, &addr_len);
    client_t *new_client;

    if (client_socket < 0)
        return;
    printf("New connection accepted from client %d\n", client_socket);
    new_client = calloc(1, sizeof(client_t));
    if (new_client == NULL) {
        perror("Failed to allocate memory for client");
        close(client_socket);
        return;
    }
    client_init(new_client, ftp, client_socket);
}

void setup_fd_set(ftpserver_t *ftp, fd_set *readfds, fd_set *writefds,
    int *max_socket)
{
    client_t *current;

    FD_ZERO(readfds);
    FD_SET(ftp->server_socket.sockfd, readfds);
    FD_ZERO(writefds);
    current = ftp->clients;
    while (current != NULL) {
        FD_SET(current->socket, readfds);
        if (current->socket > *max_socket)
            *max_socket = current->socket;
        if (current->response)
            FD_SET(current->socket, writefds);
        current = current->next;
    }
}

void accept_clients(ftpserver_t *ftp, int activity, client_t *current)
{
    fd_set readfds;
    fd_set writefds;
    int max_socket = ftp->server_socket.sockfd;

    setup_fd_set(ftp, &readfds, &writefds, &max_socket);
    activity = select(max_socket + 1, &readfds, &writefds, NULL, NULL);
    if (activity < 0)
        exit(FAILURE);
    if (FD_ISSET(ftp->server_socket.sockfd, &readfds))
        accept_new_connection(ftp);
    current = ftp->clients;
    while (current != NULL) {
        if (FD_ISSET(current->socket, &readfds))
            handle_client(current);
        if (FD_ISSET(current->socket, &writefds)) {
            send_response(current);
            remove_client(ftp, current->socket);
        }
        current = current->next;
    }
}

void main_loop(ftpserver_t *ftp)
{
    int activity;
    client_t *current;

    while (1)
        accept_clients(ftp, activity, current);
}
