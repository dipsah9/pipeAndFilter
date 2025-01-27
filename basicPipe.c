//Basic pipe examples


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(){
    int pipe_fd[2]; //array for the fds. 
    pid_t child;
    char write_msg[] = "Hello from the parent!";
    char read_msg[100];

    //create the pipe
    if(pipe(pipe_fd) == -1){
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    //fork a child process 
    child = fork();

    if(child < 0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if(child == 0){ //if child is 0, then this means it is child process
            close(pipe_fd[1]);
            read(pipe_fd[0], read_msg, sizeof(read_msg));
            printf("Child received: %s\n", read_msg);
            printf("Lets print the value of the chld: %d\n", child);
            printf("I am the pid of child: %d | %d\n", getpid(), getppid());
            close(pipe_fd[0]);
    }else{
        close(pipe_fd[0]); //this mean closing the unused ends .
        write(pipe_fd[1], write_msg, strlen(write_msg) + 1);
        close(pipe_fd[1]);//close the write end. 
        printf("I am the pid of the parent: %d | %d\n", getpid(), getppid());
        wait(NULL); //wait for the child process to finish
    }




    return 0;
}