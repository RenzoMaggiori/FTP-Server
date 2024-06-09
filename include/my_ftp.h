/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** my_ftp
*/

#ifndef MY_FTP_H_
    #define MY_FTP_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <fcntl.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <sys/wait.h>
    #include <dirent.h>

    #define FAILURE 84
    #define SUCCESS 0
    #define MAX_CLIENTS 10
    #define MAX_COMMAND_SIZE 512
    #define MAX_PATH_LENGTH 1024
    #define MAX_IP_LENGTH 16
    #define MAX_RESPONSE_SIZE 256
    #define MAX_SIZE 64
    #define CMD_NBR 14
    #define CMD_NAMES "PASS", "USER", "HELP", "CWD", "CDUP", "PWD", "NOOP"
    #define CMD_NAMES2 "QUIT", "DELE", "PASV", "PORT", "LIST", "RETR", "STOR"
    #define CMD_FUNC &pass_cmd, &user_cmd, &help_cmd, &cwd_cmd, &cdup_cmd
    #define CMD_FUNC2 &pwd_cmd, &noop_cmd, &quit_cmd, &dele_cmd, &pasv_cmd
    #define CMD_FUNC3 &port_cmd, &list_cmd, &retr_cmd, &stor_cmd

typedef struct client_t {
    int socket;
    char *username;
    char *pass;
    bool is_connected;
    bool authenticated;
    char *currentdir;
    char *root;
    struct client_t *next;
    char *response;
    int data_socket;
    int data_port;
    char data_ip[MAX_IP_LENGTH];
} client_t;

typedef void (*cmd_ptr) (client_t *client, const char *cmd);
typedef struct {
    int sockfd;
    struct sockaddr_in addr;
} socket_t;

typedef struct {
    socket_t server_socket;
    client_t *clients;
    const char *path;
} ftpserver_t;

// struct_init.c
ftpserver_t *ftp_server_init(int port, const char *path);
void ftp_server_cleanup(ftpserver_t *ftp);

// socket.c
void bind_socket(socket_t *sock, const char *ip, int port);
socket_t create_socket(int domain, int type, int protocol);
void listen_socket(socket_t *sock, int backlog);

// send.c
void send_response(client_t *client);

// channel.c
void get_ip_address(int server_socket, char *ip_address, size_t size);
int create_data_channel(client_t *client);
int get_data_channel_port(int data_socket);
void update_client_data_channel(client_t *client, int data_socket,
    int data_port, const char *data_ip);

// clients/
void main_loop(ftpserver_t *ftp);
void check_cmd(client_t *client, char *cmd);
void handle_client(client_t *client);
void remove_client(ftpserver_t *ftp, int socket_fd);
void client_init(client_t *client, ftpserver_t *ftp, int client_socket);

// commands/
void user_cmd(client_t *client, const char *buffer);
void pass_cmd(client_t *client, const char *buffer);
void help_cmd(client_t *client, const char *args);
void cwd_cmd(client_t *client, const char *buffer);
void cdup_cmd(client_t *client, const char *buffer);
void pwd_cmd(client_t *client, const char *buffer);
void noop_cmd(client_t *client, const char *buffer);
void quit_cmd(client_t *client, const char *buffer);
void dele_cmd(client_t *client, const char *buffer);
void pasv_cmd(client_t *client, const char *buffer);
void port_cmd(client_t *client, const char *buffer);
void list_cmd(client_t *client, const char *buffer);
void ls_cmd(client_t *client);
void retr_cmd(client_t *client, const char *buffer);
void stor_cmd(client_t *client, const char *buffer);

#endif
