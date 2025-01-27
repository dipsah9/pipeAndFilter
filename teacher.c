#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe_fd[2]; // Array for the pipe's file descriptors
    pid_t child;
    char write_msg[] = "Hello from parent!";
    char read_msg[100];

    // Create the pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    child = fork();

    if (child < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child == 0) { // Child process
        close(pipe_fd[1]); // Close the unused write end
        read(pipe_fd[0], read_msg, sizeof(read_msg)); // Read from the pipe
        printf("Child received: %s\n", read_msg);
        close(pipe_fd[0]); // Close the read end
    } else { // Parent process
        close(pipe_fd[0]); // Close the unused read end
        write(pipe_fd[1], write_msg, strlen(write_msg) + 1); // Write to the pipe
        close(pipe_fd[1]); // Close the write end
        wait(NULL); // Wait for the child process to finish
    }

    return 0;
}
