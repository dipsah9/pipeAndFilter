#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int pipefd[2];
    pid_t pid;
    char write_msg[] = "Hello from parent!";
    char read_msg[100];

    //pipe cretion 
    if(pipe(pipefd) == -1){
        perror("Pipe failed");
        return 1;
    }

    //fork a child process
    pid = fork();

    if(pid < 0){
        perror("Fork failed");
        return 1;
    }

    //this means it is a child process
    if(pid > 0){
        close(pipefd[0]); //closed unused  read end 
        write(pipefd[1], write_msg, strlen(write_msg)+1);
        close(pipefd[1]);
    }else{
        close(pipefd[1]);
        read(pipefd[0], read_msg, sizeof(read_msg));
        printf("Child received the sent message: %s\n", read_msg);
        close(pipefd[0]);
    }


    return 0;
}
