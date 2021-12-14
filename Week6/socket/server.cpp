#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>

#define BUF_SIZE 1024
#define BACKLOG 5

void receiverThread(int cli_sd, bool e){
    char recv_data[BUF_SIZE] = {0};
    while (true)
    {
        ssize_t res = recv(cli_sd, recv_data, BUF_SIZE, 0);
        if (res == 0 || res == -1){
            std::cerr << "recv return " << res << std::endl;
            break;
        }
        recv_data[res] = '\0';
        std::cout << recv_data << std::endl;
        if (e) {
            std::string s(recv_data);
            ssize_t res = send(cli_sd, s.c_str(), s.size(), 0);
            if (res == 0 || res == -1) {
                std::cerr << "send return " << res << std::endl;
                close(cli_sd);
                exit(1);
            }
        }
    }

    std::cout << "disconnected" << std::endl;
    close(cli_sd);
}


int main(int argc, char **argv) {
    bool e = false;
    int port;

    // check arguments
    if (argc != 2) {
        if (argc == 3) {
            if (std::string(argv[2]).compare("-e") != 0) {
                std::cout << "Usage: " << argv[0] << " <port> [-e]" << std::endl;
                return 0;
            } else {
                e = true;
            }
        } else {
            std::cout << "Usage: " << argv[0] << " <port> [-e]" << std::endl;
            return 0;
        }
    }

    try {
        port = stoi(std::string(argv[1]));
    } catch (...) {
        std::cout << "Port must be a number" << std::endl;
        return 0;
    }

    int server_fd, client_fd;

    // create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation error" << std::endl;
        exit(1);
    }

    // set socket option
    int optval = 1;
    int res = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (res == -1) {
        std::cerr << "setsockopt error" << std::endl;
        exit(1);
    }

    // server address setting
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket to server_addr
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "bind error" << std::endl;
        exit(1);
    }

    if (listen(server_fd, BACKLOG) == -1) {
        std::cerr << "listen error" <<std::endl;
        exit(1);
    }

    while (true) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cerr << "client accept failed" << std::endl;
            exit(0);
        }

        char ip_addr_clients[20];
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip_addr_clients, sizeof(ip_addr_clients));
        std::cout << ip_addr_clients << " client connect" << std::endl;

        std::thread *t = new std::thread(receiverThread, client_fd, e);
        t->detach();
    }

    close(server_fd);
    return 0;
}
