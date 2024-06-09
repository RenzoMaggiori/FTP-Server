/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** ftp_init
*/

#include "./include/my_ftp.h"

ftpserver_t *ftp_server_init(int port, const char *path)
{
    ftpserver_t *ftp = calloc(1, sizeof(ftpserver_t));
    DIR *dir = opendir(path);

    if (ftp == NULL)
        exit(FAILURE);
    if (dir == NULL) {
        perror("Failed to open directory");
        exit(FAILURE);
    }
    closedir(dir);
    ftp->server_socket = create_socket(AF_INET, SOCK_STREAM, 0);
    bind_socket(&(ftp->server_socket), "127.0.0.1", port);
    listen_socket(&(ftp->server_socket), MAX_CLIENTS);
    ftp->clients = NULL;
    ftp->path = path;
    return ftp;
}

void ftp_server_cleanup(ftpserver_t *ftp)
{
    client_t *current;
    client_t *tmp;

    close(ftp->server_socket.sockfd);
    current = ftp->clients;
    while (current != NULL) {
        tmp = current;
        current = current->next;
        remove_client(ftp, tmp->socket);
    }
    free(ftp);
}
