#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


#define READ 0
#define WRITE 1


//here we need one more function to redirect the output of hte terminal to the desired file
//and we can also assign the permission flags
static void set_outfile(const char *filename){
     int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

     if(fd == -1){
        perror("failed to create file");
        exit(EXIT_FAILURE);
     }

     //redirect the terminal output to the file . 
     if(dup2(fd, STDOUT_FILENO) == -1){
        perror("redirection issue");
        close(fd);
        exit(EXIT_FAILURE);
     }

     //clean the fd
     close(fd);
}


void create_child(char *const argv[], int (*in_pipe)[2], int (*out_pipe)[2]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        if (in_pipe) {  // If input pipe exists, redirect stdin
            dup2((*in_pipe)[READ], STDIN_FILENO);
            close((*in_pipe)[READ]);
            close((*in_pipe)[WRITE]);
        }

        if (out_pipe) { // If output pipe exists, redirect stdout
            dup2((*out_pipe)[WRITE], STDOUT_FILENO);
            close((*out_pipe)[READ]);
            close((*out_pipe)[WRITE]);
        }

        execvp(argv[0], argv);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <seq_param> <awk_param> <grep_param>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int in_pipe[2][2], out_pipe[2][2];

    char *cmd1[] = {"seq", "2", argv[1], NULL};
    char *cmd2[] = {"awk", argv[2], NULL};
    char *cmd3[] = {"grep", argv[3], NULL};

    // Create first pipe
    if (pipe(out_pipe[0]) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // First process (seq) -> writes to out_pipe[0]
    create_child(cmd1, NULL, &out_pipe[READ]);
    close(out_pipe[READ][WRITE]); // Parent closes write end of first pipe

    // Create second pipe
    if (pipe(out_pipe[1]) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // Second process (awk) -> reads from out_pipe[0], writes to out_pipe[1]
    create_child(cmd2, &out_pipe[READ], &out_pipe[WRITE]);
    close(out_pipe[READ][READ]); // Parent closes read end of first pipe
    close(out_pipe[WRITE][WRITE]); // Parent closes write end of second pipe

    // Third process (grep) -> reads from out_pipe[1], writes to stdout
    create_child(cmd3, &out_pipe[WRITE], NULL);
    close(out_pipe[WRITE][READ]); // Parent closes read end of second pipe


    if(argv[4]){
        printf(" i am here \n");
        set_outfile(argv[4]);
    }

    // Parent waits for all child processes
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    return 0;
}
