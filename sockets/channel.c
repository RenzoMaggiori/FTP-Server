/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** channel
*/

#include "../include/my_ftp.h"

int create_data_channel(client_t *client)
{
    struct sockaddr_in addr;
    int data_socket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (data_socket == -1)
        return -1;
    if (setsockopt(data_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        close(data_socket);
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0;
    if (bind(data_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
        listen(data_socket, SOMAXCONN) < 0) {
        close(data_socket);
        return -1;
    }
    return data_socket;
}

int get_data_channel_port(int data_socket)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if (getsockname(data_socket, (struct sockaddr *)&addr, &addrlen) == -1) {
        perror("Getsockname failed");
        return -1;
    }
    return ntohs(addr.sin_port);
}

void get_ip_address(int server_socket, char *ip_address, size_t size)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if (getsockname(server_socket, (struct sockaddr *)&addr, &addrlen) == -1) {
        perror("Getsockname failed");
        return;
    }
    inet_ntop(AF_INET, &(addr.sin_addr), ip_address, size);
}

void update_client_data_channel(client_t *client, int data_socket,
    int data_port, const char *data_ip)
{
    client->data_socket = data_socket;
    client->data_port = data_port;
    strncpy(client->data_ip, data_ip, MAX_IP_LENGTH - 1);
    client->data_ip[MAX_IP_LENGTH - 1] = '\0';
}
