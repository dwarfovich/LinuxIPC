#include <iostream>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    constexpr char fifo_name[] = "/tmp/fifo_test";

    int result = mknod(fifo_name, S_IFIFO | 0666, 0);
    if (result < 0) {
        std::cerr << "Cannot create FIFO file\n";
        return result;
    }

    std::cerr << "Waiting for readers\n";
    int fd = open(fifo_name, O_WRONLY);
    if (fd < 0) {
        std::cerr << "Cannot open FIFO file\n";
        return fd;
    }

    const char data[] = "Hello\n";
    ssize_t written = write(fd, data, strlen(data));
    if (written < 0) {
        std::cerr << "Error writing to FIFO\n";
        return written;
    }

    return 0;
}
