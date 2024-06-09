/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** pasv
*/

#include "../include/my_ftp.h"

void pasv_cmd(client_t *client, const char *buffer)
{
    int data_socket = (!client->authenticated) ? -1 :
        create_data_channel(client);
    char ip_address[MAX_IP_LENGTH];
    char response[MAX_RESPONSE_SIZE];
    int port;

    if (data_socket == -1) {
        client->response = "425 Can't open data connection.\r\n";
        return;
    }
    (void) buffer;
    port = get_data_channel_port(data_socket);
    printf("%d\n", port);
    get_ip_address(client->socket, ip_address, sizeof(ip_address));
    snprintf(response, sizeof(response),
        "227 Entering Passive Mode (%s,%d,%d).\r\n",
        ip_address, port / 256, port % 256);
    client->response = response;
    update_client_data_channel(client, data_socket, port, ip_address);
}
