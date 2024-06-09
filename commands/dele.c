/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** dele
*/

#include "../include/my_ftp.h"

void dele_cmd(client_t *client, const char *buffer)
{
    char filename[MAX_PATH_LENGTH];
    char *realfilename;

    if (!client->authenticated) {
        client->response = "530 Error\r\n";
        return;
    }
    if (sscanf(buffer, "DELE %s", filename) != 1) {
        client->response = "550 Error\r\n";
        return;
    }
    chdir(client->currentdir);
    realfilename = realpath(filename, NULL);
    if (realfilename && remove(realfilename) == 0)
        client->response = "250 Requested file action okay, completed\r\n";
    else
        client->response = "550 Error\r\n";
}
