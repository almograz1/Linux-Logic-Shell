#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 256

// Function to check if the file is already Adv encrypted
int is_advanced_encrypted(const char *filepath) {
    char adv_enc_path[BUFFER_SIZE];
    snprintf(adv_enc_path, sizeof(adv_enc_path), "Encryption_File/Adv_Enc/%s", filepath);

    // Check if the file exists in the Adv_Enc directory
    if (access(adv_enc_path, F_OK) == 0) { // File exists
        return 1;
    }
    return 0; // File is not encrypted
}

int main(int argc, char *argv[]) {
    char encryption_char = (argc == 3) ? argv[2][0] : ' '; // Default to space if not provided
    // Check if the file is already encrypted
    if ((is_advanced_encrypted(argv[1])) == 1) {
        printf("Encryption option not Supported\n");
        return 1;
    }

    // Ensure the Adv_Enc directory exists
    if (mkdir("Encryption_File/Adv_Enc", 0777) == -1 && errno != EEXIST) {
        perror("Failed to create Adv_Enc directory");
        return 1;
    }
    // Open the input file
    int input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        return 1;
    }

    // Prepare the output file path
    char output_file_path[BUFFER_SIZE];
    snprintf(output_file_path, sizeof(output_file_path), "Encryption_File/Adv_Enc/%s", argv[1]);

    // Open the output file
    int output_fd = open(output_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(input_fd);
        return 1;
    }

    // Perform XOR encryption and write to the output file
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            buffer[i] ^= encryption_char; // XOR each byte with the encryption character
        }
        if (write(output_fd, buffer, bytes_read) != bytes_read) {
            perror("Error writing to output file");
            close(input_fd);
            close(output_fd);
            return 1;
        }
    }

    if (bytes_read == -1) {
        perror("Error reading input file");
        close(input_fd);
        close(output_fd);
        return 1;
    }

    // Close file descriptors
    close(input_fd);
    close(output_fd);

    // Delete the original file
    if (unlink(argv[1]) == -1) {
        perror("Error deleting the original file");
        return 1;
    }

    // Lock the file by removing all permissions
    if (chmod(output_file_path, 0) == -1) {
        perror("Error changing file permissions");
        return 1;
    }

    // Success message
    printf("Mission Complete\n");
    return 0;
}
