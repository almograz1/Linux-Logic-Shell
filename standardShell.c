#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 400

int main()
{
    if (mkdir("Commands", 0777) == 0) {
        printf("//Commands dir created\n");
    }

    // Create the Encryption_File directory
    if (mkdir("Encryption_File", 0777) == 0) {
        printf("//Encryption_File dir created\n");

    }

    // Create the Adv_Enc directory inside Encryption_File
    if (mkdir("Encryption_File/Adv_Enc", 0777) == 0) {
        
        printf("//Adv_Enc dir created\n");
    }

    
    char input[BUFFER_SIZE];
    char input_arr[4][100];
    int pid, status;

    while (1)
    {
        printf("StandShell>");

        if(fgets(input,sizeof(input),stdin) == NULL) {
            printf("Failed to read input\n");
            exit(1);
        }

        input[strcspn(input, "\n")] = '\0'; // remove newline char at the end if exists
        // Parse the input into words
        int numOfArgs = sscanf(input, "%s %s %s %s", input_arr[0], input_arr[1], input_arr[2], input_arr[3]);
        if (numOfArgs < 1)
        {
            continue; // No command entered
        }
        
        if(strcmp(input_arr[0], "Adv") == 0){
            execlp("./Advanced_Shell", "Advanced_Shell", NULL);
            perror("exec exit failed");
            exit(1);
        }
        if(strcmp(input_arr[0], "Security") == 0){
            execlp("./Security_Shell", "Security_Shell", NULL);
            perror("exec exit failed");
            exit(1);
        }        
        // Exit command
        if (strcmp(input_arr[0], "exit") == 0)
        {
            execlp("./exit", "exit", NULL);
            perror("exec exit failed");
            exit(1);
        }

        // Fork process here
        pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) // Child process that executes any command prompted in a switch case with number of arguments
        {
            switch (numOfArgs) 
            {
            case 1:
                execlp(input_arr[0], input_arr[0], NULL); 
                break;
            case 2:
                execlp(input_arr[0], input_arr[0], input_arr[1], NULL);
                break;
            case 3:
                execlp(input_arr[0], input_arr[0], input_arr[1], input_arr[2], NULL);
                break;
            case 4:
                execlp(input_arr[0], input_arr[0], input_arr[1], input_arr[2], input_arr[3], NULL);
                break;
            default:
                printf("Not Supported\n");
                exit(1);
            }
            printf("Not Supported\n");
            exit(1);
        }


        else wait(&status);
    }

    return 0;
}
