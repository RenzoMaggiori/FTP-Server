/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** remove_client
*/

#include "../include/my_ftp.h"

void update_head(client_t *prev, client_t *current, ftpserver_t *ftp)
{
    if (prev != NULL)
        prev->next = current->next;
    else
        ftp->clients = current->next;
}

void remove_client(ftpserver_t *ftp, int socket_fd)
{
    client_t *current = ftp->clients;
    client_t *prev = NULL;

    while (current != NULL) {
        if (current->socket == socket_fd && !current->is_connected) {
            update_head(prev, current, ftp);
            free(current->username);
            free(current->pass);
            free(current);
            close(socket_fd);
            return;
        }
        prev = current;
        current = current->next;
    }
}
