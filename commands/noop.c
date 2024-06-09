/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** noop
*/

#include "../include/my_ftp.h"

void noop_cmd(client_t *client, const char *buffer)
{
    if (!client->authenticated) {
        client->response = "530 Error\r\n";
        return;
    }
    client->response = "200 NOOP\r\n";
}
