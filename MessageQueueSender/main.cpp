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

    Message message = {1, "Hello, world"};
    msgsnd(queue_id, &message, sizeof (message), 0);

    return 0;
}
