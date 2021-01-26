#include <iostream>
#include <atomic>
#include <thread>
#include <signal.h>
#include <unistd.h>

namespace {
volatile sig_atomic_t do_shotdown = 0;
std::atomic<bool> shutdown_requested {false};
}

void signalHandler(int /*signal*/) {
    do_shotdown = 1;
    shutdown_requested = true;
    static const char message[] = "Received signal\n";
    write(STDERR_FILENO, message, sizeof (message));
}

int main()
{
    {
        struct sigaction action;
        action.sa_handler = signalHandler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGINT, &action, nullptr);
    }

    while (!do_shotdown && !shutdown_requested.load()) {
        std::cout << "Waiting...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Shutting down\n";

    return 0;
}
