#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    const char *filename = argv[1];

    if (remove(filename) != 0) {
        perror("Error deleting file");
        return 1;
    }
    return 0;

}
