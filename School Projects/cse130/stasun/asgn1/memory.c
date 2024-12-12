#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#define BLOCK 4096

// PATH_MAX = 4096

int main(void) {
    char buffer[BLOCK + 1] = { 0 };
    int gflag = 0, sflag = 0;

    int read_bytes = 0;
    while (read_bytes < 4) { // get/set check
        read_bytes += read(0, &buffer[read_bytes], 4 - read_bytes);
    }
    // read(0, buffer, 4);
    // printf("%s\n", buffer);

    if (buffer[3] == '\n') {
        buffer[3] = '\0';
        if (strncmp(buffer, "get", 3) == 0) {
            gflag = 1;
        }

        else if (strncmp(buffer, "set", 3) == 0) {
            sflag = 1;
        }

        else {
            // printf("1\n");
            write(2, "Invalid Command\n", 16);
            return 1;
        }
    }

    else {
        // printf("2\n");
        write(2, "Invalid Command\n", 16);
        return 1;
    }

    char location[BLOCK + 1] = { 0 };

    read_bytes = read(0, buffer, BLOCK); // location check

    strncpy(location, buffer, read_bytes);
    char *token = strtok(location, "\n");
    unsigned int path_len = strlen(token);

    if (path_len == BLOCK) {
        char check_byte;
        read(0, &check_byte, 1);
        if (check_byte != '\n') {
            // printf("3\n");
            write(2, "Invalid Command\n", 16);
            return 1;
        }
    } else {
        if (buffer[path_len] != '\n') {
            // printf("4\n");
            write(2, "Invalid Command\n", 16);
            return 1;
        }
    }

    // maybe an edge case where location with length < buffer not terminated with \n treated is valid

    if (path_len == BLOCK) {
        read_bytes = read(0, buffer, BLOCK);
    }

    if (gflag) {
        if (path_len == BLOCK && read_bytes != 0) {
            // printf("5\n");
            write(2, "Invalid Command\n", 16);
            return 1;
        }

        else if (buffer[path_len + 1] != '\0') {
            // printf("6\n");
            write(2, "Invalid Command\n", 16);
            return 1;
        }

        int fd = open(location, O_RDONLY);
        if (fd == -1 || open(location, O_DIRECTORY) != -1) {
            // printf("7\n");
            write(2, "Invalid Command\n", 16);
            return 1;
        }

        while ((read_bytes = read(fd, buffer, BLOCK)) > 0) {
            write(1, buffer, read_bytes);
        }
        close(fd);
    }

    else if (sflag) {
        int fd = open(
            location, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if (fd == -1) {
            write(2, "Operation Failed\n", 17);
            return 1;
        }

        if (path_len != BLOCK) {
            strncpy(buffer, &buffer[path_len + 1], BLOCK - path_len - 1);
            for (int i = BLOCK - path_len - 1; i < BLOCK; i++) {
                buffer[i] = '\0';
            }

            read(0, &buffer[BLOCK - path_len - 1], path_len + 1);
        }

        token = strtok(buffer, "\n");
        int len = atoi(token);
        int zero_flag = 0;
        if (len == 0) {
            zero_flag = 1;
            for (unsigned int i = 0; i < strlen(token); i++) {
                if (token[i] != '0') {
                    zero_flag = 0;
                    break;
                }
            }
        }

        if (zero_flag) {
            close(fd);

            write(1, "OK\n", 3);
            return 0;
        }

        else if (len == 0 && !zero_flag) {
            buffer[strlen(token)] = '\n';

            write(fd, buffer, strlen(buffer));
            while ((read_bytes = read(0, buffer, BLOCK) > 0)) {
                int written_bytes = 0;
                while (written_bytes < read_bytes) {
                    written_bytes += write(fd, &buffer[written_bytes], read_bytes - written_bytes);
                }
            }
        }

        else {
            /* for (int i = 0; i < 25; i++) {
                                printf("%d ", buffer[i]);
                        }       

                        printf("\n"); */

            int bytes_left = strlen(&buffer[strlen(token) + 1]);
            if (len < bytes_left) {
                write(fd, &buffer[strlen(token) + 1], len);
            }

            else {
                write(fd, &buffer[strlen(token) + 1], bytes_left);
                len -= bytes_left;
                while (len > 0) {
                    read_bytes = read(0, buffer, BLOCK);
                    if (read_bytes == 0) {
                        break;
                    }

                    if (len < read_bytes) {
                        write(fd, buffer, len);
                    }

                    else {
                        write(fd, buffer, read_bytes);
                    }

                    len -= read_bytes;
                }
            }
        }

        write(1, "OK\n", 3);
    }

    return 0;

    // location_check();

    // if set:
    // 	check_bytes();
}
