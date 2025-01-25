#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int file_fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (file_fd < 0) {
        perror("Error opening file");
        return 1;
    }

    // Redirect stdout (file descriptor 1) to the file
    if (dup2(file_fd, STDOUT_FILENO) == -1) {
        perror("dup2 failed");
        close(file_fd);
        return 1;
    }

    // Now, printf will write to the file instead of the terminal
    printf("This will be written to output.txt!\n");

    // Clean up
    close(file_fd);
    return 0;
}
