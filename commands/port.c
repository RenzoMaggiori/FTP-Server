/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** port
*/

#include "../include/my_ftp.h"


int create_port_data_channel(client_t *client, int port)
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
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((unsigned short)port);
    if (bind(data_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
        listen(data_socket, SOMAXCONN) < 0) {
        close(data_socket);
        return -1;
    }
    return data_socket;
}

void setup_data_channel(client_t *client, int *ip, int port)
{
    char ip_address[MAX_IP_LENGTH];
    int data_socket;

    snprintf(ip_address, sizeof(ip_address),
        "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    data_socket = create_port_data_channel(client, port);
    if (data_socket == -1) {
        client->response = "425 Can't open data connection.\r\n";
        return;
    }
    client->response = "200 Command okay\r\n";
    update_client_data_channel(client, data_socket, port, ip_address);
}

void port_cmd(client_t *client, const char *buffer)
{
    int ip[4];
    int portNum[2];
    int port;

    if (!client->authenticated) {
        client->response = "530 Error\r\n";
        return;
    }
    if (sscanf(buffer, "PORT %d,%d,%d,%d,%d,%d",
        &ip[0], &ip[1], &ip[2], &ip[3], &portNum[0], &portNum[1]) != 6) {
        client->response = "550 Syntax error in parameters or arguments\r\n";
        return;
    }
    port = portNum[0] * 256 + portNum[1];
    setup_data_channel(client, ip, port);
}
