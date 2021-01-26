#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET; /* base for seek offsets */
    lock.l_start = 0;         /* 1st byte in file */
    lock.l_len = 0;           /* 0 here means 'until EOF' */
    lock.l_pid = getpid();    /* process id */

    const char filename[] = "/tmp/shared_file.txt";
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Cannot open file\n";
        return fd;
    }

    fcntl(fd, F_GETLK, &lock); /* sets lock.l_type to F_UNLCK if no write lock */
    if (lock.l_type != F_UNLCK) {
        std::cerr << "File is still locked\n";
        return -1;
    }

    lock.l_type = F_RDLCK; /* prevents any writing during the reading */
    if (fcntl(fd, F_SETLK, &lock) < 0) {
        std::cerr << "Cannot lock file\n";
        return -1;
    }

    char c;
    while (read(fd, &c, 1) > 0) {    /* 0 signals EOF */
        write(STDOUT_FILENO, &c, 1); /* write one byte to the standard output */
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) < 0){
        std::cerr << "Cannot unlock file\n";
    }

    close(fd);

    return 0;
}
