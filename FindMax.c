#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 400

int main(int argc, char *argv[]) {
    int fd1, fd2, max_chars = 0;
    char *largest_file = argv[1];
    fd1 = open(argv[1], O_RDONLY);
    if(fd1 == -1){
        printf("Error opening file");
        return 1;
    }
    fd2 = open(argv[2], O_RDONLY);
    if(fd2 == -1){
        printf("Error opening file");
        return 1;
    }
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int char_count = 0;

    // Count chars in file 1
    while ((bytes_read = read(fd1, buffer, BUFFER_SIZE)) > 0) {
        char_count += bytes_read;
    }

    if (bytes_read == -1) {
        printf("Error reading file 1");
        return 1;
    }

    close(fd1);
    max_chars = char_count;
    char_count = 0;
    while((bytes_read = read(fd2, buffer, BUFFER_SIZE))>0){ // Count chars in file 2
        char_count+=bytes_read;
    }
    if (bytes_read == -1) {
        printf("Error reading file 1");
        return 1;
    }
    // Update the largest file if needed
    if (char_count > max_chars) { // file 2 is larger, print accordingly
        max_chars = char_count;
        largest_file = argv[2];
        printf("%s", largest_file);
        return 0;
    }
    else {
        printf("%s\n", largest_file); // file 1 is larger, print accordingly
        return 0;
    }
}

