/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** stor
*/

#include "../include/my_ftp.h"

void upload_file(client_t *client, const char *buffer)
{
    char pathname[MAX_PATH_LENGTH];
    char data_buffer[MAX_PATH_LENGTH];
    ssize_t bytes_read;
    int fd;

    sscanf(buffer, "STOR %s", pathname);
    fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        write(client->data_socket,
            "550 Failed to open file for writing.\r\n", 39);
        return;
    }
    while (read(client->data_socket, data_buffer, 1024) > 0)
        write(fd, data_buffer, bytes_read);
    close(fd);
    write(client->data_socket, "226 File transfer successful.\r\n", 32);
}

void stor_fork(client_t *client, const char *buffer)
{
    char response[MAX_RESPONSE_SIZE];
    pid_t pid = 0;

    pid = fork();
    if (pid == -1) {
        client->response = "425 Data connection not established.\r\n";
        return;
    } else if (pid == 0) {
        client->response =
            "150 Opening data connection for directory listing.\r\n";
        send_response(client);
        upload_file(client, buffer);
        exit(0);
    }
    close(client->data_socket);
    client->data_port = 0;
    memset(client->data_ip, 0, sizeof(client->data_ip));
}

void stor_cmd(client_t *client, const char *buffer)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    if (!client->authenticated) {
        client->response = "530 Error\r\n";
        return;
    }
    if (client->data_socket == -1 || !client->data_port == -1 ||
        strlen(client->data_ip) == 0) {
        client->response = "425 Data connection not established.\r\n";
        return;
    }
    client->data_socket = accept(client->data_socket,
        (struct sockaddr *)&client_addr, &addr_len);
    stor_fork(client, buffer);
}
