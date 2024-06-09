/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** pass
*/

#include "../include/my_ftp.h"

void pass_cmd(client_t *client, const char *buffer)
{
    char password[MAX_SIZE];

    if (!client->username) {
        client->response = "332 Need account for login.\r\n";
        return;
    }
    if (sscanf(buffer, "PASS %s", password) != 1 &&
        strcmp(client->username, "Anonymous") != 0) {
        client->response = "530 Error\r\n";
        return;
    }
    if (strlen(password) != 0 &&
        strcmp(client->username, "Anonymous") != 0) {
        client->response = "530 Error\r\n";
        return;
    }
    snprintf(client->pass, MAX_SIZE, "%s", password);
    client->response = "230 User logged in, proceed.\r\n";
    client->authenticated = true;
}
