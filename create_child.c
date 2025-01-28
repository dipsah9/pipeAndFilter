// TODO: includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

// TODO: globals



// TODO: helpers

// open the file by given name and set it as STDOUT of the current process
static void set_outfile(const char *filename) {
        //TODO open file
        //TODO set as STDOUT
        //TODO cleanup
}

//create a child and set it's STDIN and STDOUT from/to the given pipes if not null
static pid_t create_child(char * const argv[], int (*in_pipe)[2], int (*out_pipe)[2]) {
        //TODO: create child
        pid_t child = fork();
        if(child == 0){
        //TODO: set STDIN if in_pipe is not null
        if(in_pipe != NULL){
            close((*in_pipe)[1]);
            dup2((*in_pipe)[0], STDIN_FILENO);
            close((*in_pipe)[0]);
        }
        //TODO: set STDOUT if out_pipe is not null
        if(out_pipe != NULL){
            close((*out_pipe)[0]);
            dup2((*out_pipe)[1], STDOUT_FILENO);
            close((*out_pipe)[1]);
        }

        }
        //TODO: cleanup
        if((*in_pipe) != NULL){
            close((*in_pipe)[0]);
        }
        if((*out_pipe) != NULL){
            close((*out_pipe)[1]);
        }

        //TODO: run target program using argv
        execvp(argv[0], argv);
        perror("exec failed");
        exit(EXIT_FAILURE);

        return pid;
       
}



// seq 1 MAX | awk AWK_VORSCHRIFT | grep GREP_VORSCHRIFT > OUTPUT_FILE

int main(int argc, char *argv[]) {
    return 1;
}