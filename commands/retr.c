/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** retr
*/

#include "../include/my_ftp.h"

void open_and_send_file(client_t *client, const char *pathname)
{
    char file_contents[1024];
    size_t bytes_read = 1;
    ssize_t bytes_sent;
    FILE *file = fopen(pathname, "r");

    if (file == NULL) {
        write(client->data_socket,
            "550 File not found or cannot be opened.\r\n", 42);
        return;
    }
    while (bytes_read > 0) {
        bytes_read = fread(file_contents, 1, 1024, file);
        bytes_sent = write(client->data_socket, file_contents, bytes_read);
        if (bytes_sent != bytes_read || bytes_read < 0) {
            write(client->data_socket, "550 Error sending file contents.\r\n", 33);
            break;
        }
    }
    write(client->data_socket, "226 command success.\r\n", 23);
    fclose(file);
}

void send_file(client_t *client, const char *buffer)
{
    char pathname[MAX_PATH_LENGTH];
    char *truepath;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_socket = accept(client->data_socket,
        (struct sockaddr *)&client_addr, &addr_len);

    client->data_socket = client_socket;
    if (sscanf(buffer, "RETR %s", pathname) != 1)
        return;
    truepath = realpath(pathname, NULL);
    open_and_send_file(client, truepath);
}

void retr_fork(client_t *client, const char *buffer)
{
    char response[MAX_RESPONSE_SIZE];
    pid_t pid = 0;

    pid = fork();
    if (pid == -1) {
        client->response = "425 Data connection not established.\r\n";
        return;
    } else if (pid == 0) {
        write(client->socket,
            "150 Opening data connection for directory listing.\r\n", 53);
        send_file(client, buffer);
        exit(0);
    }
    close(client->data_socket);
    client->data_port = 0;
    memset(client->data_ip, 0, sizeof(client->data_ip));
}

void retr_cmd(client_t *client, const char *buffer)
{
    if (!client->authenticated) {
        client->response = "530 Error\r\n";
        return;
    }
    if (client->data_socket == -1 || !client->data_port == -1 ||
        strlen(client->data_ip) == 0) {
        client->response = "425 Data connection not established.\r\n";
        return;
    }
    chdir(client->currentdir);
    retr_fork(client, buffer);
}
