#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

int main()
{
    const char backing_file[] = "/shared_memory";
    int fd = shm_open(backing_file, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        std::cerr << "Cannot initialize shared memory\n";
        return fd;
    }

    const off_t memory_size = 512;
    char* memory_location = (char*) mmap(NULL,       /* let system pick where to put segment */
                                         memory_size,   /* how many bytes */
                                         PROT_READ | PROT_WRITE, /* access protections */
                                         MAP_SHARED, /* mapping visible to other processes */
                                         fd,         /* file descriptor */
                                         0);         /* offset: start at 1st byte */

    if (!memory_location) {
        std::cerr << "Cannot get memory location\n";
        return -1;
    }

    const char semaphore_name[] = "/semaphore";
    sem_t* semaphore = sem_open(semaphore_name, /* name */
                               O_CREAT,       /* create the semaphore */
                               0644,   /* protection perms */
                               0);            /* initial value */
    if (!semaphore) {
        std::cerr << "Cannot open semaphore\n";
        return -1;
    }

    if (!sem_wait(semaphore)) {
        for (int i = 0; i < memory_size; i++) {
            write(STDOUT_FILENO, memory_location + i, 1); /* one byte at a time */
        }
        sem_post(semaphore);
    }

    munmap(memory_location, memory_size);
    close(fd);
    sem_close(semaphore);
    unlink(backing_file);

    return 0;
}
