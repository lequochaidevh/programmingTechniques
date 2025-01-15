#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int PArray[10] = {37, 55, 36, 43, 46, 53, 35, 54, 98, 102}; // password12 -> 37-> 55-> 36-> 43-> 46-> 53-> 35-> 54-> 98-> 102
int keyStore[10] = {85, 86, 87, 88, 89, 90, 81, 82, 83, 84};

bool checkPass(int *XoredPassword);
int changePass(char* newPass, char* oldPass);

int main(int argc, char *argv[]) {

    // READ PASSWORD
    int fd;
    char *buffer;

    fd = open("password.txt" , O_RDWR);

    ssize_t bytes_read;
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }
    //lseek(fd, 0, SEEK_SET);
    /*if(!fd) {
        close(fd);
        return 1;
    }
    // Use lseek to find the file size
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == (off_t) - 1) {
        perror("Error getting file size");
        close(fd);
        return 1;
    }
    printf("File size: %ld bytes\n", (long)file_size);
    buffer = (char *) malloc((long)file_size + 1);  // Allocate memory for the entire file

    //printf("PASS1 = \n");*/

    buffer = (char *) malloc(2221);  // Allocate memory for the entire file
    if (buffer == NULL) {
        perror("Error allocating memory");
        close(fd);
        free(buffer);
        return 1;
    }
    memset(buffer, 0, 2221);  // Initialize the buffer to zero (null-terminate it)
    if( fd ){
        bytes_read = read(fd, buffer,2220);
        if (bytes_read == -1) {
            perror("Error reading file");
            free(buffer);
            close(fd);
            return 1;
        }
        buffer[bytes_read] = '\0';
        printf("PASS = ");
        printf("%s \n",buffer);
        printf("length buffer = %ld \n",strlen(buffer));
        for (int i=0; i<10; i++ ) {
            PArray[i] = ((int)(buffer[i]))^(keyStore[i]);
            printf("-> %d", PArray[i]);
        }
        printf("\n");
    }

    // CHECK AGRUMENT
    if (argc != 2) {
        // CHANGE PASSWORD
        if (argc == 3) {
            char *tmpCheck = strstr(argv[1], "add");
            if (tmpCheck != NULL){
                char ss[20];
                strncpy(ss, tmpCheck + 3, 11);
                printf("ss = %s \n", ss);
                int CaclXor[10] = {};
                for (int i=0; i<10; i++ ) {
                    CaclXor[i] = ((int)(ss[i]))^(keyStore[i]);
                }printf("write to file\n");
                if (checkPass(CaclXor) == false) goto freeLbl;
                printf("write to file\n");
                changePass(argv[2], ss);
                lseek(fd, 0, SEEK_SET);
                write(fd, argv[2], 11);
                printf("write to file\n");

            }
        }
        printf("Help:\n%s <10 character serial key>\n", argv[0]);
    }
    else {
        int stringLength = strlen(argv[1]);
        if ((stringLength > 10) || (stringLength < 10)) {
            printf("[-] Serial key must be of 10 characters. Please recheck your key\n");
        }
        else {
            int XoredDecimal[10] = {};
            for (int i=0; i<10; i++ ) {
                XoredDecimal[i] = ((int)(argv[1][i]))^(keyStore[i]);
                //printf("-> %d", XoredDecimal[i]);
            }
            printf("\n");
            checkPass(XoredDecimal);
        }
    }
freeLbl :
    free(buffer);
    close(fd);
    return 0;
}

bool checkPass(int *XoredPassword) {
    bool is_equal = true;
    for (int i=0; i<10; i++ ) {
        if (XoredPassword[i] != PArray[i]) {
            is_equal = false;
            break;
        }
    }
    if (is_equal == true) {
        printf ("[ ] Correct Password\n");
        return true;
    }
    else {
        printf ("[-] Incorrect Password\n");
        return false;
    }
}

int changePass(char* oldPass, char* newPass) {
    printf("<> NEW PASS XOR ");
    for (int i=0; i<10; i++ ) {
        PArray[i] = ((int)(newPass[i]))^(keyStore[i]);
        printf("-> %d", PArray[i]);
    }

}


//valgrind --leak-check=full --track-origins=yes -s ./a.out addpassword12 password34
