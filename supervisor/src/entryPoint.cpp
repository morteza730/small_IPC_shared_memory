#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

pid_t pid1, pid2;

void signalHandler(int signum) {
    if (pid1 > 0) {
        kill(pid1, SIGTERM);
    }
    if (pid2 > 0) {
        kill(pid2, SIGTERM);
    }
    exit(signum);
}

int main()
{
    signal(SIGINT, signalHandler);

    pid1 = fork();
    if (pid1 == 0)
    {
        execl("./Sender", "Sender", nullptr);
        perror("Failed to start app1");
        return 1;
    }

    pid2 = fork();
    if (pid2 == 0)
    {
        execl("./Receiver", "Receiver", nullptr);
        perror("Failed to start app2");
        return 1;
    }

    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    std::cout << "Both applications have finished.\n";
    return 0;
}
