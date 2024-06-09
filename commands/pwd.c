/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** pwd
*/

#include "../include/my_ftp.h"

void pwd_cmd(client_t *client, const char *buffer)
{
    char response[MAX_PATH_LENGTH];

    if (!client->authenticated) {
        client->response = "530 Error\r\n";
        return;
    }
    snprintf(response, sizeof(response),
        "257 \"%s\" created.\r\n", client->currentdir);
    client->response = response;
}
