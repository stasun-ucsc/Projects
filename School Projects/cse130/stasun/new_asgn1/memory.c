#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#define BLOCK 8192

// PATH_MAX = 4096

int read_bytes(int infile, char *buf, int to_read) {
    /* concerns with the buffer being overwritten */

    int readBytes = 0;
    int temp = 0;
    while (readBytes < to_read && (temp = read(infile, buf + readBytes, to_read - readBytes)) > 0) {
        readBytes += temp;
    }
    return readBytes;
}

int write_bytes(int outfile, uint8_t *buf, int to_write) {
    /* concerns with the buffer being overwritten */

    // same as read but with write()
    int writtenBytes = 0;
    int temp = 0;
    while (writtenBytes < to_write && (temp = write(outfile, buf + writtenBytes, to_write - writtenBytes)) >= 0) {
        /* printf("temp = %d\n", temp); */
        writtenBytes += temp;
    }
    return writtenBytes;
}

void field_init(char **fields, char *tok, int *tok_no) {
    fields[tokens] = (char *)calloc(strlen(tok) + 1, sizeof(char));
    strncpy(fields[tokens], tok, strlen(tok));
    (*tok_no)++;

    return;
}

void free_fields(char **fields, int tok_no) {
    for (int i = 0; i < tok_no; i++) {
        free(fields[i]);
        fields[i] = NULL;
    }
}

int main(int argc, char **argv) {
    bool gFlag = false, sFlag = false;
    char buf[BLOCK + 1] = {0};
    char *fields[3];
    int tokens = 0;
    while (read_bytes(0, buf, BLOCK)) {
        if (tokens == 0) {
            char *tok = strtok(buf, "\n");
            if (tok != NULL) {
                field_init(fields, tok, &tokens);
                if (strncmp(fields[0], "GET", 4) == 0) {
                    gFlag = true;
                } else if (strncmp(fields[0], "SET", 4) == 0) {
                    sFlag = true;
                } else {
                    printf("Invalid method\n");
                    return 1
                }
            }
        }
    
        
        // this section seems kinda weird and can be rewritten
        while (tok != NULL && tokens < 3) {
            tok = strtok(NULL, "\n");
            if (tok != NULL) {
                field_init(fields, tok, &tokens);
            }
        }
    }

    if (gFlag) {
        // need to check for the number of tokens?
        int fd = open(fields[1], O_RDONLY);
        int read;
        while ((read = read_bytes(fd, buf, BLOCK))) {
            buf[read] = 0;
            write_bytes(1, buf, read);
        }
        free_fields(fields, tokens);
        close(fd)

        return 0;
    } else if (sFlag) {
        // check for correct number of tokens?
        int fd = open(fields[1], O_WRONLY | O_TRUNC | O_CREAT);
        int len = atoi(fields[2]);
        if (len == 0) {
            // check if len is actually zero or invalid content length
            for (int i = 0; i < strlen(fields[2]); i++) {
                if (fields[2][i] != '0') {
                    write_bytes(2, "Invalid Content Length\n", 24);
                    return 1;
                }
            }
        }
        if ((tok + strlen(tok) + 1) - buf > BLOCK) {
            // too tired to calculate how many bytes to write, fix later
            char *data = tok + strlen(tok) + 1;
            len -= write_bytes(fd, tok + strlen(tok) + 1, BLOCK - (int)(data - buf));
        }
        while ((read = readBytes(fd, buf, BLOCK)) && len > 0) {
            buf[read] = 0;
            if (read > len) {
                write_bytes(fd, buf, len);
            } else {
                write_bytes(fd, buf, read);
            }
            len -= read;
        }
        free_fields(fields, tokens);
        close(fd);

        return 0;
    } else {
        // somehow neither flag was set and it wasn't picked up earlier
        return 1;
    }
}

