/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** cwd
*/

#include "../include/my_ftp.h"

void cwd_cmd(client_t *client, const char *buffer)
{
    char pathname[MAX_COMMAND_SIZE];
    char *absolute_path;

    if (!client->authenticated) {
        client->response = "530 Error\r\n";
        return;
    }
    sscanf(buffer, "CWD %s", pathname);
    chdir(client->currentdir);
    absolute_path = realpath(pathname, NULL);
    if (chdir(absolute_path) == 0) {
        client->currentdir = absolute_path;
        client->response = "250 Directory successfully changed.\r\n";
    } else
        client->response = "550 Failed to change directory.\r\n";
}
