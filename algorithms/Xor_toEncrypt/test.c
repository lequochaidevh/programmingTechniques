#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    // OPEN FILE (READ-ONLY MODE)
    int fd = open("password.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // FIND FILE SIZE
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("Error getting file size");
        close(fd);
        return 1;
    }
    lseek(fd, 0, SEEK_SET); // Reset file pointer to the start

    // ALLOCATE BUFFER FOR THE FILE CONTENT
    char *buffer = (char *) malloc(file_size + 1); // +1 for null terminator
    if (buffer == NULL) {
        perror("Memory allocation failed");
        close(fd);
        return 1;
    }

    // READ THE FILE CONTENT INTO BUFFER
    ssize_t bytes_read = read(fd, buffer, file_size);
    if (bytes_read == -1) {
        perror("Error reading file");
        free(buffer);
        close(fd);
        return 1;
    }

    // NULL-terminate the string
    buffer[bytes_read] = '\0';

    // PRINT THE CONTENT OF THE FILE
    printf("buf = '%s'\n", buffer);

    // CLEAN UP
    free(buffer);
    close(fd);

    return 0;
}

