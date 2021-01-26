#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    struct flock lock;
    lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
    lock.l_whence = SEEK_SET; /* base for seek offsets */
    lock.l_start = 0;         /* 1st byte in file */
    lock.l_len = 0;           /* 0 here means 'until EOF' */
    lock.l_pid = getpid();    /* process id */

    const char filename[] = "/tmp/shared_file.txt";
    int fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        std::cerr << "Cannot open file\n";
        return fd;
    }

    const char message[] = "Hello, World\n";
    if (fcntl(fd, F_SETLK, &lock) < 0) { /** F_SETLK doesn't block, F_SETLKW does **/
        std::cerr << "Cannot lock file\n";
    } else {
        write(fd, message, strlen(message)); /* populate data file */
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) < 0) {
        std::cerr << "Cannot unlock file\n";
    }

    close(fd); /* close the file: would unlock if needed */

    return 0;
}
