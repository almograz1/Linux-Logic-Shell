#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 400

int main(int argc, char* argv[]) {
    int fd, fd_out1, fd_out2, word_len;
    char buffer[BUFFER_SIZE];
    int writing_out1 = 1;
    ssize_t bytes_read;

    // Open the input file
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("Error opening the first file.\n");
        return 1;
    }

    // Open the first output file
    fd_out1 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd_out1 == -1) {
        printf("Error opening the second file.\n");
        close(fd);
        return 1;
    }

    // Open the second output file
    fd_out2 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd_out2 == -1) {
        printf("Error opening the third file.\n");
        close(fd);
        close(fd_out1);
        return 1;
    }

    while (1) {
        int current_fd_out = writing_out1 ? fd_out1 : fd_out2;
        word_len = 0;

        // Read a single word
        while ((bytes_read = read(fd, &buffer[word_len], 1)) > 0) {
            if (isspace(buffer[word_len])) { // If current char is a space
                if (word_len > 0) break; // End of a word reached
            } else {
                word_len++;
                if (word_len >= BUFFER_SIZE - 1) break; // Prevent overflow
            }
        }

        if (bytes_read <= 0 && word_len == 0) { // If the file reading is finished
            break; // Exit the loop
        }

        // Null-terminate the word and write it to the output file
        buffer[word_len] = '\0';
        write(current_fd_out, buffer, word_len);
        write(current_fd_out, " ", 1);

        // Alternate between output files
        writing_out1 = !writing_out1;
    }

    // Close all file descriptors
    close(fd);
    close(fd_out1);
    close(fd_out2);

    printf("Mission Complete\n");
    return 0;
}
