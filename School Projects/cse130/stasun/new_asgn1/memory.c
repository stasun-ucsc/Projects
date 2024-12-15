#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#define BLOCK 4096

// PATH_MAX = 4096

int read_bytes(int fd, char *buf, int bytes) {
    int readBytes = 0, temp = 0;
    while (readBytes < bytes && (temp = read(fd, buf + readBytes, bytes - readBytes))) {
        readBytes += temp;
    }

    return readBytes;
}

int method(int fd, char *buf) {
    if (read_bytes(fd, buf, 4) < 0) {
        return -2;
    }
    if (strncmp(buf, "GET\n", 4) == 0) {
        return 0;
    } else if (strncmp(buf, "SET\n", 4) == 0) {
        return 1;
    } else {
        return -1;
    }
}

int filepath(int fd, char *buf, char *path) {
    if ((read_bytes(fd, buf, BLOCK)) < 0) {
        return 1;    
    }
    strtok(buf, "\n");
    int len = strlen(buf);
    path = (char *) calloc(len + 1, sizeof(char));
    strncpy(path, buf, len);
    memmove(buf, buf + len + 1, BLOCK - len - 1);

    return 0;
}

int main(void) {
    char *buf = (char *)calloc(BLOCK + 1, sizeof(char));	
    printf("ERROR\n");
    return -1;
}
