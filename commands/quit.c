/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** quit
*/

#include "../include/my_ftp.h"

void quit_cmd(client_t *client, const char *buffer)
{
    (void) buffer;
    client->is_connected = false;
    client->response = "221 Logged out\r\n";
}
