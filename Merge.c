#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define BUFFER_SIZE 400


int main(int argc, char* argv[]){ 
    int fd1, fd2, fd_out;
    char buffer[BUFFER_SIZE];
    int reading_file1 = 1;
    ssize_t bytes_read;
    fd1 = open(argv[1], O_RDONLY);
    if(fd1 == -1){
        printf("Error opening the first file. \n");
        return 1;
    }
    fd2 = open(argv[2], O_RDONLY);
    if(fd2 == -1){
        printf("Error opening the second file. \n");
        close(fd1);
        return 1;
    }
    fd_out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(fd_out == -1){
        printf("output path: %s\n", argv[3]);
        printf("Error opening the output file.\n");
        close(fd1);
        close(fd2);
        return 1;
    }
    while(1){
        int fd = reading_file1? fd1: fd2;
        int word_len = 0;
        while((bytes_read = read(fd, &buffer[word_len], 1)) > 0) { // Read a single word
            if(isspace(buffer[word_len])){// if current char is a space
                if(word_len>0) break; // end of a word reached

            }
            else{
                word_len++;
                if(word_len>=BUFFER_SIZE-1) break; // prevent overflow.
            }
        }
        if(bytes_read<=0 && word_len == 0){ // if the file is finished, switch to the other file
            if(reading_file1) fd1 = -1;
            else fd2 = -1;
        
            if((fd==fd1 && fd1 == -1) || (fd==fd2 && fd2 == -1)) break; // both files are finished
            reading_file1 = !reading_file1;
            continue;
        }

        //null terminate the word and write it to the output
        buffer[word_len] = '\0';
        write(fd_out, buffer, word_len); // write the word to the output file
        write(fd_out, " ", 1); // add a space after the word

        reading_file1 = !reading_file1;
    }

    close(fd1);
    close(fd2);
    close(fd_out);
    printf("Mission Complete\n");
    return 0; // successfully merged the files.




}
