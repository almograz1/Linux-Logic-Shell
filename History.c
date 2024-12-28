#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void print_file_line_by_line(const char *filename) {
    int fd = open(filename, O_RDONLY); // Open the file in read-only mode
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char buffer[400]; // Buffer to hold data read from the file
    char line[400];   // Buffer to hold a single line
    int bytes_read, i = 0, line_number = 1;
    int line_index = 0; // Index to track line within `line`

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                // End of line detected
                line[line_index] = '\0'; // Null-terminate the line
                printf("%d. %s\n", line_number++, line); // Print line with line number
                line_index = 0; // Reset line buffer index
            } else {
                // Append character to current line
                line[line_index++] = buffer[i];
            }
        }
    }

    // Print the last line if it doesn't end with a newline
    if (line_index > 0) {
        line[line_index] = '\0';
        printf("%d. %s\n", line_number, line);
    }

    if (bytes_read == -1) {
        perror("Error reading file");
    }

    close(fd); // Close the file
}

int main(int argc, char *argv[]) {
    print_file_line_by_line(argv[1]);

    return 0;
}
