#include <semaphore.h>
#include <thread>
#include <iostream>

int counter = 0;
const int increments = 50;
const int delay = 1;
sem_t semaphore;

void incrementCounter() {
    sem_wait(&semaphore);
    for (int i = 0; i < increments; ++i) {
        ++counter;
        std::cout << "Current increment: " << counter << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    sem_post(&semaphore);
}

void printCounter() {
    sem_wait(&semaphore);
    std::cout << "Counter: " << counter << '\n';
    sem_post(&semaphore);
}

int main() {
    sem_init(&semaphore, 0, 1);
    std::thread increment_thread {incrementCounter};
    std::thread print_thread {printCounter};

    increment_thread.join();
    print_thread.join();

    return 0;
}
