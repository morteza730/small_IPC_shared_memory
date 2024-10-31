#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1 = fork();
    if (pid1 == 0) {
        execl("./m1", "m1", nullptr);  // Replace with full path if needed
        perror("Failed to start app1");
        return 1;
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        execl("./m2", "m2", nullptr);  // Replace with full path if needed
        perror("Failed to start app2");
        return 1;
    }

    // Wait for both processes to finish
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    std::cout << "Both applications have finished.\n";
    return 0;
}
