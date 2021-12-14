#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <regex>
#include <iostream>
#include <thread>

#define BUF_SIZE 1024

bool checkAddrFormat(const char *addr) {
    std::regex re("^([0-9]{1,3}\\.){3}[0-9]{1,3}$");
    return std::regex_match(std::string(addr), re);
}

void receiverThread(int server_fd){
    char recv_data[BUF_SIZE] = {0};
    while (true)
    {
        ssize_t res = recv(server_fd, recv_data, BUF_SIZE, 0);
        if (res == 0 || res == -1){
            std::cerr << "recv return " << res << std::endl;
            break;
        }
        recv_data[res] = '\0';
        std::cout << recv_data << std::endl;
    }

    std::cout << "disconnected" << std::endl;
    close(server_fd);
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP address> <port>" << std::endl;
        return 0;
    }

    const char* server_ip = argv[1];
    int server_port;

    try {
        server_port = stoi(std::string(argv[2]));
    } catch (...) {
        std::cout << "Port must be a number" << std::endl;
        return 0;
    }

    if (!checkAddrFormat(server_ip)) {
        std::cout << "wrong IP address format" << std::endl;
        return 0;
    }


    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "socket creation error" << std::endl;
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port); 
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if(connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "connect error" <<std::endl;
        close(server_fd);
        exit(1);
    }

    std::thread *t = new std::thread(receiverThread, server_fd);
    t->detach();

    while (true) {
        std::string s;
        std::getline(std::cin, s);
        
        ssize_t res = send(server_fd, s.c_str(), s.size(), 0);
        if (res == 0 || res == -1) {
            std::cerr << "send return " << res << std::endl;
            close(server_fd);
            exit(1);
        }
    }

    close(server_fd);

    return 0;
}
