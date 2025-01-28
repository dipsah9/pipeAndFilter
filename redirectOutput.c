#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){

    //we need to create one array for pipe 
    int pipe_fd[2];
    pid_t pid;
    char write_msg[] = "I am here to be written somewhere in other process";
    char read_msg[100];

    //lets create a pipe 
    if(pipe(pipe_fd) == 1){
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    //lets create the process
     pid = fork();

     if(pid < 0){
        perror("fork failed");
        exit(EXIT_FAILURE);
     }

     if(pid == 0){
        close(pipe_fd[1]);// close the write end 
        //Redirect the pipe's read end to stdin
        dup2(pipe_fd[0], STDIN_FILENO); 
        close(pipe_fd[0]);

        char buffer[100];
        fgets(buffer, sizeof(buffer), stdin);
        printf("Child reveived: %s", buffer);
        exit(EXIT_FAILURE);
      
      }//else{
    //    close(pipe_fd[0]);
    //    write(pipe_fd[1], write_msg, strlen(write_msg) +1);
    //    close(pipe_fd[1]);
    //     wait(NULL); 
    //  }



    return 0;
}