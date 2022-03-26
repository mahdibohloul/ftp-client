#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define BUFFER_SIZE 1024
#define BUFFER_DATA_SIZE 1048576
#define COMMAND_CHANNEL_PORT 8000
#define DATA_CHANNEL_PORT 8001
#define RUNNING 1
#define RECIEVEING 1

int main() {
    //declaring important data
    int client_command_fd;
    int client_data_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char buffer_data[BUFFER_DATA_SIZE];

    //Creating a server socket
    if (!(client_command_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        printf("Failed to create socket!\nExiting..");
        exit(1);
    }

    if (!(client_data_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        printf("Failed to create socket!\nExiting..");
        exit(1);
    }

    //Assigning attributes to server_address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COMMAND_CHANNEL_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((connect(client_command_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
        printf("Failed to connect to the server!\nExiting\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DATA_CHANNEL_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((connect(client_data_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
        printf("Failed to connect to the server!\nExiting\n");
        exit(1);
    }

    std::cout << "client command fd: " << client_command_fd << std::endl;
    std::cout << "client data fd: " << client_data_fd << std::endl;
    while (RUNNING) {
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        char cmd_line[BUFFER_SIZE];
        strcpy(cmd_line, buffer);
        char *cmd = strtok(cmd_line, " ");


        send(client_command_fd, buffer, BUFFER_SIZE, 0);
        memset(buffer, 0, BUFFER_SIZE);
        read(client_command_fd, buffer, BUFFER_SIZE);
        std::cout << buffer << std::endl;
        std::string command_resp(buffer);
        if (command_resp.substr(0, 3) == "226")
        {
            memset(buffer_data, 0, sizeof(buffer_data));
            read(client_data_fd, buffer_data, BUFFER_DATA_SIZE);
            std::cout << buffer_data << std::endl;
        }
    }
}
