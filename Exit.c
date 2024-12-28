#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if(remove("Commands/Advanced/Advanced_Commands") != 0){
        perror("failed to remove Advanced_Commands file\n");
    }
    if(remove("Commands/Security/Security_Commands") != 0){
        perror("failed to remove Security_Commands file\n");
    }
    if(rmdir("Commands/Advanced") == -1){
        perror("failed to remove Commands folder\n");
    }        
    if(rmdir("Commands/Security") == -1){
        perror("failed to remove Security folder\n");
    }
    if(rmdir("Commands") == -1){
        perror("failed to remove Commands folder\n");
    }
    printf("/Commands dir removed\n");
    printf("Goodbye...\n");
    return 0;
}