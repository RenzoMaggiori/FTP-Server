/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** send
*/

#include "../include/my_ftp.h"

void send_response(client_t *client)
{
    if (client->response) {
        write(client->socket, client->response, strlen(client->response));
        client->response = NULL;
    }
}
