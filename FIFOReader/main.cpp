#include <iostream>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    constexpr char fifo_name[] = "/tmp/fifo_test";

    std::cout << "Waiting for writers\n";
    int fd = open(fifo_name, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Cannot open FIFO file\n";
        return fd;
    }

    const int length = 6;
    char data[length];
    ssize_t red = read(fd, data, length);
    if (red < 0) {
        std::cerr << "Error reading from FIFO\n";
        return red;
    }

    std::cout << "Red from FIFO: " << data << '\n';
    close(fd);

    return 0;
}
