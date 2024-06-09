/*
** EPITECH PROJECT, 2024
** B-NWP-400-BAR-4-1-myftp-renzo.maggiori
** File description:
** tests
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/logging.h>
#include "../include/my_ftp.h"

// ------------------------- FTP TESTS ------------------------- //

Test(ftp_init_tests, create_ftp) {
    cr_assert_not_null(ftp_server_init(3030, "./commands"), "FTP should create a non-null struct");
}

Test(ftp_destroy_tests, destroy_ftp) {
    int num = 0;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_t *client;
    client_init(client, ftp, 1);
    ftp->clients = client;
    ftp_server_cleanup(ftp);
    if (ftp)
        num = 1;
    cr_assert_eq(num, 1, "FTP should be null after destroy");
}

// ------------------------- CMD TESTS ------------------------- //

Test(user_cmd_tests, user_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    user_cmd(client, "USER\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    user_cmd(client, "USER Anonymous\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 331, "Response should be 331");
}

Test(quit_cmd_tests, quit_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    quit_cmd(client, "QUIT\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 221, "Response should be 530");
}

Test(help_cmd_tests, help_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    help_cmd(client, "HELP\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 214, "Response should be 530");
    help_cmd(client, "HELP NOOP\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 214, "Response should be 331");
}

Test(pass_cmd_tests, pass_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    pass_cmd(client, "PASS\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->username = "Anonymous";
    pass_cmd(client, "PASS\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 230, "Response should be 230");
}

Test(noop_cmd_tests, noop_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    noop_cmd(client, "NOOP\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->authenticated = true;
    noop_cmd(client, "NOOP\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 200, "Response should be 200");
}

Test(cdup_cmd_tests, cdup_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    cdup_cmd(client, "CDUP\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->authenticated = true;
    client->currentdir = "";
    cdup_cmd(client, "CDUP\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 200, "Response should be 200");
}

Test(pwd_cmd_tests, pwd_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    pwd_cmd(client, "PWD\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->authenticated = true;
    pwd_cmd(client, "CDUP\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 257, "Response should be 257");
}

Test(cwd_cmd_tests, cwd_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    cwd_cmd(client, "CWD\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->authenticated = true;
    cwd_cmd(client, "CWD ..\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 250, "Response should be 250");
}

Test(pasv_cmd_tests, pasv_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    pasv_cmd(client, "PASV\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 425, "Response should be 425");
    client->authenticated = true;
    pasv_cmd(client, "PASV\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 227, "Response should be 227");
}

Test(port_cmd_tests, port_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    port_cmd(client, "PORT\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->authenticated = true;
    port_cmd(client, "PORT\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 550, "Response should be 550");
    port_cmd(client, "PORT 127,0,0,1,20,200\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 200, "Response should be 200");
}

Test(dele_cmd_tests, dele_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    dele_cmd(client, "DELE\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->authenticated = true;
    dele_cmd(client, "DELE\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 550, "Response should be 550");
    dele_cmd(client, "DELE hadada\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 550, "Response should be 550");
}

Test(list_cmd_tests, list_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    list_cmd(client, "LIST\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->authenticated = true;
    list_cmd(client, "LIST .\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 425, "Response should be 425");
    pasv_cmd(client, "PASV\n");
    list_cmd(client, "LIST .\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 226, "Response should be 226");

}

Test(retr_cmd_tests, retr_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    retr_cmd(client, "RETR\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->authenticated = true;
    retr_cmd(client, "RETR\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 425, "Response should be 425");
    pasv_cmd(client, "PASV\n");
    retr_cmd(client, "RETR ./tests/test.c\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 227, "Response should be 226");
}

Test(stor_cmd_tests, stor_cmds) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    stor_cmd(client, "STOR\n");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");
    client->authenticated = true;
    stor_cmd(client, "STOR\n");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 425, "Response should be 425");
}


Test(send, send_test) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    client->response = "Test";
    send_response(client);
    cr_assert_null(client->response, "response should be NULL");
}

Test(check_cmd, check_test) {
    client_t *client = malloc(sizeof(client_t));;
    ftpserver_t *ftp = ftp_server_init(3030, "./commands");
    client_init(client, ftp, 1);
    check_cmd(client, "test");
    char str[4];
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    int num = atoi(str);
    cr_assert_eq(num, 500, "Response should be 500");
    check_cmd(client, "     \r");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 500, "Response should be 500");
    check_cmd(client, "     ");
    memset(str, 0, sizeof(str));
    strncpy(str, client->response, 3);
    num = atoi(str);
    cr_assert_eq(num, 530, "Response should be 530");

}
