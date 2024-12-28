#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 256

// Function to check if the file is already in the Adv_Enc directory
int is_advanced_encrypted(const char *filepath) {
    char adv_enc_path[BUFFER_SIZE];
    snprintf(adv_enc_path, sizeof(adv_enc_path), "%s", filepath);

    // Check if the file exists in the main directory
    if (access(adv_enc_path, F_OK) == 0) { // File exists
        return 0; // it is not adv encrypted
    }
    return 1; // File is adv encrypted
}

int main(int argc, char *argv[]) {
    char encryption_char = (argc == 3) ? argv[2][0] : ' '; // Default to space if not provided

    // Check if the file is already advanced encrypted
    if (is_advanced_encrypted(argv[1])==0) {
        printf("Adv Decryption option not Supported\n");
        return 1;
    }

    // Prepare the input file path (from Adv_Enc folder)
    char input_file_path[BUFFER_SIZE];
    snprintf(input_file_path, sizeof(input_file_path), "Encryption_File/Adv_Enc/%s", argv[1]);

    if(chmod(input_file_path,0644)==-1){
        printf("Adv Decryption option not Supported");
    }
    // Open the encrypted file
    int input_fd = open(input_file_path, O_RDONLY);
    if (input_fd == -1) {
        printf("Adv Decryption option not Supported\n");
        return 1;
    }

    // Prepare the output file path (move to the current directory)
    char output_file_path[BUFFER_SIZE];
    snprintf(output_file_path, sizeof(output_file_path), "./%s", argv[1]);

    // Open the output file
    int output_fd = open(output_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(input_fd);
        return 1;
    }

    // Perform XOR decryption and write to the output file
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
        perror("Error reading encrypted file");
        close(input_fd);
        close(output_fd);
        return 1;
    }

    // Close file descriptors
    close(input_fd);
    close(output_fd);

    // Restore permissions for the decrypted file
    if (chmod(output_file_path, 0777) == -1) {
        perror("Error restoring file permissions");
        return 1;
    }

    // Delete the original encrypted file
    if (unlink(input_file_path) == -1) {
        perror("Error deleting the encrypted file");
        return 1;
    }

    if(chmod(output_file_path,0777)==-1){
        perror("Error restoring premissions for the decrypted file");
        return 1;
    }

    printf("Mission Completed\n");
}
