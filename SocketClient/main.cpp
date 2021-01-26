#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <iostream>


int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        std::cerr << "Cannot open socket\n";
        return socket_fd;
    }

    hostent* host = gethostbyname("localhost");
    if (!host) {
        std::cerr << "Cannot get host by name\n";
        return -1;
    }

    const unsigned short port = 10000;
    sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof (socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = ((in_addr*) host->h_addr_list[0])->s_addr;
    socket_address.sin_port = htons(port);

    if (connect(socket_fd, (sockaddr*) &socket_address, sizeof (socket_address)) < 0) {
        std::cerr << "Cannot connect\n";
        return -1;
    }

    const char message[20] = "Hello, world";
    if (write(socket_fd, message, sizeof (message)) < 0) {
        std::cerr << "Cannot write message\n";
    }

    close(socket_fd);

    return 0;
}
