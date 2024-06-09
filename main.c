/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** main
*/

#include "./include/my_ftp.h"

int main(int ac, char **av)
{
    char *help = "USAGE: ./myftp port path\n"
        "\tport is the port number on which the server socket listens\n"
        "\tpath is the path to the home directory for the Anonymous user";
    int port;
    const char *path;
    ftpserver_t *ftp;

    if (ac != 3) {
        printf("%s\n", help);
        exit(FAILURE);
    }
    port = atoi(av[1]);
    path = av[2];
    ftp = ftp_server_init(port, path);
    printf("Server listening on port %d...\n", port);
    printf("Home directory for Anonymous user: %s\n", path);
    main_loop(ftp);
    ftp_server_cleanup(ftp);
    return SUCCESS;
}
