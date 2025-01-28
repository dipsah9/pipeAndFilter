#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to create a child process
static pid_t create_child(char *const argv[], int (*in_pipe)[2], int (*out_pipe)[2]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        if (in_pipe != NULL) {
            close((*in_pipe)[1]);
            dup2((*in_pipe)[0], STDIN_FILENO);
            close((*in_pipe)[0]);
        }

       

        if (out_pipe != NULL) {
            close((*out_pipe)[0]);
            dup2((*out_pipe)[1], STDOUT_FILENO);
            close((*out_pipe)[1]);
        }
         printf("here no stdin and stdout\n");

        execvp(argv[0], argv);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }

    // Parent process cleanup
    if (in_pipe != NULL) {
        close((*in_pipe)[0]);
    }
    if (out_pipe != NULL) {
        close((*out_pipe)[1]);
    }

    return pid;
}

int main() {
    int pipe_fd1[2], pipe_fd2[2];

    // Create the first pipe
    if (pipe(pipe_fd1) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // Create the second pipe
    if (pipe(pipe_fd2) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // First process: seq 1 10
    char *seq_cmd[] = {"seq", "1", "10", NULL};
    create_child(seq_cmd, NULL, &pipe_fd1);

    // Second process: awk '{print $1 * $1}'
    char *awk_cmd[] = {"awk", "{print $1 * $1}", NULL};
    create_child(awk_cmd, &pipe_fd1, &pipe_fd2);

    // Parent process reads from the output of the second process
    close(pipe_fd2[1]); // Close write end of the second pipe
    char buffer[128];
    while (read(pipe_fd2[0], buffer, sizeof(buffer)) > 0) {
        printf("%s", buffer); // Print the output
    }
    close(pipe_fd2[0]);

    // Wait for children to finish
    while (wait(NULL) > 0);

    return 0;
}
