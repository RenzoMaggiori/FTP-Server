/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** user
*/

#include "../include/my_ftp.h"

void user_cmd(client_t *client, const char *buffer)
{
    char username[MAX_SIZE];
    int username_len;

    username_len = sscanf(buffer, "USER %s", username);
    if (username_len != 1) {
        client->response = "530 Error\r\n";
        return;
    }
    if (strlen(username) > 0) {
        snprintf(client->username, MAX_SIZE, "%s", username);
        client->response = "331 User name okay, need password\r\n";
    } else
        client->response = "530 Error\r\n";
}
