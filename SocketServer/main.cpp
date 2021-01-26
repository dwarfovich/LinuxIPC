#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        std::cerr << "Cannot open socket\n";
        return fd;
    }

    const unsigned short port = 10000;
    sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(port);

    if (bind(fd, (sockaddr*) &socket_address, sizeof (socket_address)) < 0) {
        std::cerr << "Cannot bind socket to file descriptor\n";
        return -1;
    }

    if (listen(fd, 1) < 0) {
        std::cerr << "Error during listening\n";
        return -1;
    }

    const size_t buffer_size = 20;
    char buffer[buffer_size];
    memset(buffer, '\0', buffer_size);
    sockaddr_in client_address;
    socklen_t client_address_lenght = sizeof (client_address);
    int client_fd = accept(fd, (sockaddr*) &client_address, &client_address_lenght);
    if (client_fd < 0) {
        std::cerr << "Cannot accept client\n";
        return -1;
    }

    int count = read(client_fd, buffer, buffer_size);
    if (count > 0) {
        std::cout << "Message from client: " << buffer << '\n';
    }

    int result = close(client_fd);
    if (result < 0) {
        std::cerr << "Cannot close client file descriptor\n";
    }
    result = close(fd);
    if (result < 0) {
        std::cerr << "Cannot close file descriptor\n";
    }

    return 0;
}
