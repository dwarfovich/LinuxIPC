#include <QTextStream>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

QTextStream& qout() {
    static QTextStream stream {stdout};
    return stream;
}
QTextStream& qerr() {
    static QTextStream stream {stderr};
    return stream;
}

int main()
{
    constexpr char fifo_name[] = "/tmp/fifo_test";

    qout() << "Waiting for writers\n";
    int fd = open(fifo_name, O_RDONLY);
    if (fd < 0) {
        qerr() << "Cannot open FIFO file\n";
        return fd;
    }

    const int length = 6;
    char data[length];
    ssize_t red = read(fd, data, length);
    if (red < 0) {
        qerr() << "Error reading from FIFO\n";
        return red;
    }

    qout() << "Red from FIFO: " << data << '\n';

    return 0;
}
