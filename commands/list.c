/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** list
*/

#include "../include/my_ftp.h"

void ls_cmd(client_t *client)
{
    char response[MAX_RESPONSE_SIZE];
    pid_t pid = 0;

    pid = fork();
    if (pid == -1) {
        client->response = "425 Data connection not established.\r\n";
        return;
    } else if (pid == 0) {
        client->response =
            "150 Opening data connection for directory listing.\r\n";
        send_response(client);
        exit(0);
    }
    close(client->data_socket);
    client->data_port = 0;
    memset(client->data_ip, 0, sizeof(client->data_ip));
    client->response = "226 Directory send OK.\r\n";
}

void list_cmd(client_t *client, const char *buffer)
{
    if (!client->authenticated) {
        client->response = "530 Error\r\n";
        return;
    }
    if (client->data_socket == -1 || !client->data_port == -1 ||
        strlen(client->data_ip) == 0) {
        client->response = "425 Data connection not established.\r\n";
        return;
    }
    ls_cmd(client);
}
