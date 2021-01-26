#include <iostream>
#include <sys/msg.h>

namespace{
const size_t data_size = 20;
}

struct Message {
    long type;
    char data[data_size];
};

int main()
{
    const int project_id = 65;
    key_t key = ftok("queue", project_id);
    int queue_id = msgget(key, 0666 | IPC_CREAT);

    Message message;
    ssize_t received = msgrcv(queue_id, &message, sizeof(message), 1, 0);
    if (received >= 0) {
        std::cout << "Received " << received << " bytes: " << message.data << '\n';
    } else {
        std::cerr << "Cannot receive message\n";
    }

    return 0;
}
