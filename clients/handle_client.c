/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** handle_client
*/

#include "../include/my_ftp.h"

void check_cmd(client_t *client, char *cmd)
{
    char *cmds[] = {CMD_NAMES, CMD_NAMES2, NULL};
    cmd_ptr cmd_func[] = {CMD_FUNC, CMD_FUNC2, CMD_FUNC3, NULL};
    int num = 0;

    while (cmd[num + 1] != '\n' && cmd[num] != ' ')
        num++;
    for (int count = 0; cmd[count + 1] && cmd[count] == ' '; count++) {
        if (cmd[count + 1] == '\r') {
            client->response = "500 Error\r\n";
            return;
        }
    }
    for (int i = 0; i < CMD_NBR; i++) {
        if (strncmp(cmd, cmds[i], num) == 0) {
            cmd_func[i](client, cmd);
            return;
        }
    }
    client->response = "500 Error\r\n";
}

void handle_client(client_t *client)
{
    char buffer[MAX_COMMAND_SIZE];
    ssize_t nread = 0;

    nread = read(client->socket, buffer, MAX_COMMAND_SIZE);
    if (nread <= 0 || !client->is_connected) {
        client->is_connected = false;
        client->response = "221 Logged out\r\n";
        return;
    }
    buffer[nread] = '\0';
    if (buffer[0] != '\0')
        check_cmd(client, buffer);
}
