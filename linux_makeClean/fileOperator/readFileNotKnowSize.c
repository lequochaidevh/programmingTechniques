// Check memory leak when RUN: valgrind --leak-check=full --track-origins=yes -s ./a.out readFileNotKnowSize.c 0 0

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define INITIAL_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    char *buffer;
    int fd;
    off_t offset;
    int whence;
    ssize_t bytesRead;
    size_t bufferSize = INITIAL_BUFFER_SIZE;
    size_t totalBytesRead = 0;

    printf("Program reads a file at any point from SEEK_SET=0, SEEK_CUR=1, SEEK_END=2\n");

    if (argc != 4) {
        perror("Incorrect syntax. Usage: <program> <file> <offset> <whence>");
        return -1;
    }

    // Allocate an initial buffer
    buffer = (char *) malloc(bufferSize);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        return -1;
    }

    // Open the file
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        free(buffer);
        return -1;
    }

    // Parse the offset and whence arguments
    offset = atoi(argv[2]);
    whence = atoi(argv[3]);

    // Move the file pointer based on the specified whence and offset
    if (lseek(fd, offset, whence) == -1) {
        perror("lseek failed");
        free(buffer);
        close(fd);
        return -1;
    }

    // Read the file in chunks
    while ((bytesRead = read(fd, buffer + totalBytesRead, bufferSize - totalBytesRead)) > 0) {
        totalBytesRead += bytesRead;

        // If buffer is full, reallocate with double the size
        if (totalBytesRead >= bufferSize) {
            bufferSize *= 2;  // Double the buffer size
            buffer = (char *) realloc(buffer, bufferSize);
            if (buffer == NULL) {
                perror("Memory reallocation failed");
                close(fd);
                return -1;
            }
        }
    }

    if (bytesRead == -1) {
        perror("Error reading file");
        free(buffer);
        close(fd);
        return -1;
    }

    // Output the contents of the buffer
    write(1, buffer, totalBytesRead);  // Write all read bytes to stdout

    // Clean up
    free(buffer);
    close(fd);
    return 0;
}

