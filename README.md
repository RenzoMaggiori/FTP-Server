# FTP Server in C

## Project Overview

The goal of this project is to create a File Transfer Protocol (FTP) server. This FTP server will handle multiple clients simultaneously using TCP sockets and the select system call for command management and fork for data transfer. The server will be compliant with RFC959 for FTP protocols.

## Features

- Authentication with an Anonymous account (empty password)
- Active and Passive modes for data transfers
- Simultaneous client handling
- Command management using `select`
- Data transfer using `fork`

## Commands

The FTP server supports the following commands, which are essential for client-server interactions:

- `USER <SP> <username> <CRLF>`: Specify user for authentication.
- `PASS <SP> <password> <CRLF>`: Specify password for authentication.
- `CWD <SP> <pathname> <CRLF>`: Change working directory.
- `CDUP <CRLF>`: Change working directory to parent directory.
- `QUIT <CRLF>`: Disconnect from the server.
- `DELE <SP> <pathname> <CRLF>`: Delete a file on the server.
- `PWD <CRLF>`: Print the current working directory.
- `PASV <CRLF>`: Enable passive mode for data transfer.
- `PORT <SP> <host-port> <CRLF>`: Enable active mode for data transfer.
- `HELP [<SP> <string>] <CRLF>`: List available commands.
- `NOOP <CRLF>`: Do nothing.
- `RETR <SP> <pathname> <CRLF>`: Download a file from the server.
- `STOR <SP> <pathname> <CRLF>`: Upload a file to the server.
- `LIST [<SP> <pathname>] <CRLF>`: List files in the current working directory.

The server must follow specific command-reply sequences, adhering to RFC959. Each server reply contains a 3-digit code followed by a space and one line of text, terminated by the Telnet end-of-line code `<CRLF>`.

## Usage

### Prerequisites

- C Compiler (GCC recommended)
- Make
- A Unix-like operating system (Linux, macOS)

### Compilation

To compile the project and run it, navigate to the project directory and run:

``` bash
make
./myftp port path
```

- `port` is the port number on which the server socket listens.
- `path` is the path to the home directory for the ftp server.

Once the program is running you have a ftp server running, for connecting clients open a new terminal and use `telnet` or `netcat`.

### Example

``` bash
./myftp 2121 /home/ftp
```

## Authors

| [<img src="https://github.com/RenzoMaggiori.png?size=85" width=85><br><sub>Renzo Maggiori</sub>](https://github.com/RenzoMaggiori)
|:---:|