#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 400

int main(int argc, char *argv[]) {
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int char_count = 0;
    fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        printf("Error opening file\n");
        return 1;
    }
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        char_count += bytes_read;
    }

    if (bytes_read == -1) {
        printf("Error reading file 1\n");
        return 1;
    }

    close(fd);
    printf("%d\n", char_count);
    return 0;
}
