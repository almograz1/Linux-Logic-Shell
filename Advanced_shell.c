#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void write_to_history(int fd, const char *data){
    if(fd==-1) return;
    write(fd,data,sizeof(char)*strlen(data));
    write(fd, "\n", 1);
}

int main() {
    int fd, fd_history, inputLen, writtenBytes;
    fd_history = open("adv_history.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    char input[400];
    char input_arr[4][100];
    // Ensure the Advanced directory exists
    if (access("Commands/Advanced", F_OK) != 0) {
        if (mkdir("Commands/Advanced", 0777) == -1) {
            perror("Failed to create Advanced directory\n");
            exit(1);
        }
    }
    printf("//Advanced dir created in Commands dir, Advanced_Commands.txt created in Commands/Advanced\n");


    // Open the file for command loggimgs
    if ((fd = open("Commands/Advanced/Advanced_Commands", O_WRONLY | O_APPEND | O_CREAT, 0777)) == -1) {
        perror("Error opening Advanced_Commands file");
        return 1;
    }

    while (1) {
        printf("AdvShell>");

            if(fgets(input,sizeof(input),stdin) == NULL) {
            printf("Failed to read input\n");
            exit(1);
        }

        input[strcspn(input, "\n")] = '\0'; // remove newline char at the end if exists
        // Parse the input into words

        // Parse the input into arguments
        int numOfArgs = sscanf(input, "%s %s %s %s", input_arr[0], input_arr[1], input_arr[2], input_arr[3]);
        if (numOfArgs < 1) {
            printf("No command entered\n");
            continue;
        }
        inputLen = strlen(input);
        // Handle "Esc" command
        if (strcmp(input, "Esc") == 0) {
            write_to_history(fd_history, input);
            close(fd);
            execlp("./Standard_Shell", "Standard_Shell", NULL);
            perror("Failed to return to standard shell.");
            exit(1);
        }

        // Log the command to the file
        if (fd < 0) {
            fprintf(stderr, "Invalid file descriptor before writing\n");
            continue;
        }
        writtenBytes = write(fd, input, inputLen);
        if (writtenBytes == -1) {
            perror("Error writing to file");
            close(fd);
            return 1;
        }
        write(fd, "\n", 1); // Add a newline after the command

        // Fork and execute the command
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) { // Child process
            if (strcmp(input_arr[0], "merge") == 0) {
                if (numOfArgs < 4) {
                    printf("Missing parameters!!!\n");
                    exit(1);
                }
                write_to_history(fd_history, input);
                execlp("./Merge", "Merge", input_arr[1], input_arr[2], input_arr[3], NULL);
            } else if (strcmp(input_arr[0], "unmerge") == 0) {
                if (numOfArgs < 4) {
                    printf("Missing parameters!!!\n");
                    exit(1);
                }
                write_to_history(fd_history, input);
                execlp("./UnMerge", "UnMerge", input_arr[1], input_arr[2], input_arr[3], NULL);
            } else if (strcmp(input_arr[0], "findmax") == 0) {
                if (numOfArgs < 3) {
                    printf("Missing parameters!!!\n");
                    exit(1);
                }
                write_to_history(fd_history, input);
                execlp("./FindMax", "FindMax", input_arr[1], input_arr[2], NULL);
            } else if (strcmp(input_arr[0], "Size") == 0) {
                if (numOfArgs < 2) {
                    printf("Missing parameters!!!\n");
                    exit(1);
                }
                write_to_history(fd_history, input);
                execlp("./Size", "Size", input_arr[1], NULL);
            } else if (strcmp(input_arr[0], "delete") == 0) {
                if (numOfArgs < 2) {
                    printf("Missing parameters!!!\n");
                    exit(1);
                }
                write_to_history(fd_history, input);
                execlp("./Delete", "Delete", input_arr[1], NULL);
            } else if (strcmp(input_arr[0], "History") == 0) {
                write_to_history(fd_history, input);
                execlp("./History", "History","adv_history.txt", NULL);
            } else {
                printf("Not Supported\n");
                exit(1);
            }
        } else { // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }

    close(fd);
    close(fd_history);
    return 0;
}
