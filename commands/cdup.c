/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** cdup
*/

#include "../include/my_ftp.h"

void cdup_cmd(client_t *client, const char *buffer)
{
    char *absolute_path;
    char *last_separator;

    if (!client->authenticated) {
        client->response = "530 Error\r\n";
        return;
    }
    chdir(client->currentdir);
    absolute_path = realpath("..", NULL);
    if (chdir(absolute_path) == 0) {
        client->currentdir = absolute_path;
        client->response = "200 Command okay.\r\n";
    } else
        client->response = "530 Error\r\n";
}
