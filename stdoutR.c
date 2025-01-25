#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//here we will use how can we redirect output to another process


int main(){
    //here it will return 3, because 0 1 and 2 are reserved for stdin, stdout and stderror respectively. 
    int file_fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    //lets try creating one other fd. 
    int file_fd1= open("otuput1.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);


    printf("here my file descriptor is: %d\n", file_fd);
    
    if(file_fd < 0){
        perror("file can not be opened");
        return 1;
    }

    //Redirect the stdout (file descriptor 1) to the file
    if(dup2(file_fd, STDOUT_FILENO) == -1){
        perror("dup2 failed");
        return 1;
    }

    //redirect the stdout (file descriptor 1 ) to the file 
    if(dup2(file_fd1, STDOUT_FILENO) == -1){
        perror("dup2 failed");
    }

    //now printf will write to the file instead of the terminal
    printf("This is going to be written in output.txt!\n");

    //clean up
    close(file_fd);



    return 0;
}