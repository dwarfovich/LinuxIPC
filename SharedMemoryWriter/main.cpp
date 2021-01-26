#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <thread>

int main()
{
    const char backing_file[] = "/shared_memory";
    int fd = shm_open(backing_file, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        std::cerr << "Cannot initialize shared memory\n";
        return fd;
    }

    const off_t memory_size = 512;
    int result = ftruncate(fd, memory_size);
    if (result < 0) {
        std::cerr << "Cannot truncate file\n";
        return result;
    }

    char* memory_location = (char*) mmap(nullptr,              /* let system pick where to put segment */
                                         memory_size,            /* how many bytes */
                                         PROT_READ | PROT_WRITE, /* access protections */
                                         MAP_SHARED,             /* mapping visible to other processes */
                                         fd,                     /* file descriptor */
                                         0);                     /* offset: start at 1st byte */
    if (!memory_location) {
        std::cerr << "Cannot map memory\n";
        return -1;
    }

    const char semaphore_name[] = "/semaphore";
    sem_t* semaphore = sem_open(semaphore_name, /* name */
                                O_CREAT,        /* create the semaphore */
                                0644,           /* protection perms */
                                0);             /* initial value */
    if (!semaphore) {
        std::cerr << "Cannot open semaphore\n";
        return -1;
    }

    const char message[] = "Hello, world\n";
    strcpy(memory_location, message);
    std::cout << "Writing to memory finished\n";
    if (sem_post(semaphore) < 0) {
        std::cerr << "Cannot post semaphore\n";
        return -1;
    }

    int delay = 10;
    std::cout << "Waiting for " << delay << " seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    std::cout << "Waiting finished\n";

    munmap(memory_location, memory_size);
    close(fd);
    sem_close(semaphore);
    shm_unlink(backing_file);

    return 0;
}
