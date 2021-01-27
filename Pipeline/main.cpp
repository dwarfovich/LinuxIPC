#include <iostream>
#include <QDebug>

#include <sys/wait.h>
#include <unistd.h>

enum IOEnds {
    ReadEnd = 0,
    WriteEnd = 1
};

int main()
{
    int pipe_fds[2]; // file descriptors
    const size_t buffer_size = 1; // in bytes
    char buffer;
    const char* message = "Hello, world\n";

    if (pipe(pipe_fds) < 0) {
        std::cerr << "Cannot create pipe\n";
        return -1;
    }

    pid_t child_id = fork();
    if (child_id < 0) {
        std::cerr << "Cannot create child process with fork()\n";
        return -1;
    } else if (child_id > 0) {
        std::cerr << "Created child process with pid" << child_id;
    }

    if (child_id == 0) { // Child process
        close(pipe_fds[WriteEnd]);

        while (read(pipe_fds[ReadEnd], &buffer, buffer_size) > 0) {
            write(STDOUT_FILENO, &buffer, buffer_size);
        }

        close(pipe_fds[ReadEnd]);{
            return 0;
        }
    } else { // Parent process
        close(pipe_fds[ReadEnd]);

        write(pipe_fds[WriteEnd], message, strlen(message));
        close(pipe_fds[WriteEnd]); // done writing: generate eof

        wait(NULL); /* wait for child to exit */
        exit(0);
    }

    return 0;
}
