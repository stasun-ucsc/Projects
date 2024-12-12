#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <regex.h>
#include <errno.h>
#include <sys/stat.h>
#include "asgn2_helper_funcs.h"

#define BLOCK 4096
extern int errno;

int main(int argc, char **argv) {
    regex_t reg;
    char *regex
        = "^([a-zA-Z]{1,8}) /([a-zA-Z0-9._]{1,63}) "
          "(HTTP/[0-9]\\.[0-9])\r\n(([-a-zA-Z0-9.]{1,128}:[ -~]{0,128}\r\n)*)\r\n((.*[\n]*)*)$";
    regmatch_t pmatch[8];

    regex_t reg_header;
    char *regex_header = "^([-a-zA-Z0-9.]{1,128}): ([ -~]{0,128})$";
    regmatch_t pmatch_header[3];

    char buf[BLOCK + 1] = { 0 };
    char field[BLOCK + 1] = { 0 };
    // Listening section
    if (argc != 2) {
        write_n_bytes(2, "Invalid Port\n", 13);

        return 1;
    }

    int port = atoi(argv[1]);
    if (port > 65535 || port < 1) {
        write_n_bytes(2, "Invalid Port\n", 13);

        return 1;
    }

    Listener_Socket socket;
    if (listener_init(&socket, port) != 0) {
        write_n_bytes(2, "Invalid Port\n", 13);

        return 1;
    }

    // Accepting section
    while (69) {
        if (regcomp(&reg, regex, REG_NEWLINE | REG_EXTENDED)) {
            printf("broken regex compile\n");

            return 1;
        }

        if (regcomp(&reg_header, regex_header, REG_NEWLINE | REG_EXTENDED)) {
            printf("broken regex compile\n");

            return 1;
        }

        int new_socket = listener_accept(&socket);

        /* int bytes_read = */ read(new_socket, buf, BLOCK);
        // printf("Error code %d: %s\n", errno, strerror(errno));
        // printf("%d\n", bytes_read);

        if (regexec(&reg, buf, 8, pmatch, 0)) {
            // should return a bad request message
            write_n_bytes(new_socket,
                "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n", 60);

            close(new_socket);
            regfree(&reg);
            regfree(&reg_header);

            for (int i = 0; i < BLOCK; i++) {
                buf[i] = 0;
                field[i] = 0;
            }

            continue;
        }

        if (pmatch[4].rm_eo + 2 > 2048) {
            // should return a bad request message
            write_n_bytes(new_socket,
                "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n", 60);

            close(new_socket);
            regfree(&reg);
            regfree(&reg_header);

            for (int i = 0; i < BLOCK; i++) {
                buf[i] = 0;
                field[i] = 0;
            }

            continue;
        }

        int method = -1;
        strncpy(field, &buf[pmatch[1].rm_so], pmatch[1].rm_eo - pmatch[1].rm_so);
        /* printf("%s, %lu\n", field, strlen(field));
		for (int i = 0; i < 8; i++) {
			printf("%d ", field[i]);
		}
		printf("\n-----------------------------------------\n"); */

        // 4 chars may not be enough or may be weird
        if (strncmp(field, "GET", 4) == 0) {
            method = 0;
        }

        else if (strncmp(field, "PUT", 4) == 0) {
            method = 1;
        }

        else {
            // return unsupported method
            write_n_bytes(new_socket,
                "HTTP/1.1 501 Not Implemented\r\nContent-Length: 16\r\n\r\nNot Implemented\n", 68);

            close(new_socket);
            regfree(&reg);
            regfree(&reg_header);

            for (int i = 0; i < BLOCK; i++) {
                buf[i] = 0;
                field[i] = 0;
            }

            continue;
        }

        strncpy(field, &buf[pmatch[3].rm_so], pmatch[3].rm_eo - pmatch[3].rm_so);

        if (strncmp(field, "HTTP/1.1", 9) != 0) {
            // return unsupported version
            write_n_bytes(new_socket,
                "HTTP/1.1 505 Version Not Supported\r\nContent-Length: 22\r\n\r\nVersion Not "
                "Supported\n",
                80);

            close(new_socket);
            regfree(&reg);
            regfree(&reg_header);

            for (int i = 0; i < BLOCK; i++) {
                buf[i] = 0;
                field[i] = 0;
            }

            continue;
        }

        for (int i = 0; i < 8; i++) {
            field[i] = 0;
        }

        strncpy(field, &buf[pmatch[2].rm_so], pmatch[2].rm_eo - pmatch[2].rm_so);

        if (method == 0) {
            int fd = open(field, O_RDONLY);
            if (fd == -1) {
                printf("Error code %d: %s\n", errno, strerror(errno));
                if (errno == EACCES) {
                    // return forbidden error
                    write_n_bytes(new_socket,
                        "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n", 56);

                    close(new_socket);
                    regfree(&reg);
                    regfree(&reg_header);

                    for (int i = 0; i < BLOCK; i++) {
                        buf[i] = 0;
                        field[i] = 0;
                    }

                    continue;
                }

                else if (errno == ENOENT) {
                    // return not found
                    write_n_bytes(new_socket,
                        "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n", 56);

                    close(new_socket);
                    regfree(&reg);
                    regfree(&reg_header);

                    for (int i = 0; i < BLOCK; i++) {
                        buf[i] = 0;
                        field[i] = 0;
                    }

                    continue;
                }

                else {
                    // return 500 internal service error
                    write_n_bytes(new_socket,
                        "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\n\r\nInternal "
                        "Server Error\n",
                        80);

                    close(new_socket);
                    regfree(&reg);
                    regfree(&reg_header);

                    for (int i = 0; i < BLOCK; i++) {
                        buf[i] = 0;
                        field[i] = 0;
                    }

                    continue;
                }
            }

            struct stat size;
            fstat(fd, &size);

            if (S_ISDIR(size.st_mode)) {
                write_n_bytes(new_socket,
                    "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n", 56);

                close(new_socket);
                regfree(&reg);
                regfree(&reg_header);

                for (int i = 0; i < BLOCK; i++) {
                    buf[i] = 0;
                    field[i] = 0;
                }

                continue;
            }

            int total_bytes = size.st_size;
            char fileSize[16] = { 0 };
            sprintf(fileSize, "%d", total_bytes);

            write_n_bytes(new_socket, "HTTP/1.1 200 OK\r\nContent-Length: ", 33);
            write_n_bytes(new_socket, fileSize, strlen(fileSize));
            write_n_bytes(new_socket, "\r\n\r\n", 4);

            int bytes_read = 0;

            for (int i = 0; i < BLOCK; i++) {
                buf[i] = 0;
            }

            int eflag = 0;
            while (total_bytes > 0) {
                bytes_read = read(fd, buf, BLOCK);
                total_bytes -= bytes_read;
                if (bytes_read == -1) {
                    // throw internal server error
                    ftruncate(new_socket, 0);
                    write_n_bytes(new_socket,
                        "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\n\r\nInternal "
                        "Server Error\n",
                        80);

                    close(new_socket);
                    regfree(&reg);
                    regfree(&reg_header);

                    for (int i = 0; i < BLOCK; i++) {
                        buf[i] = 0;
                        field[i] = 0;
                    }

                    eflag = 69;
                    break;
                }

                // write to buffer
                write_n_bytes(new_socket, buf, bytes_read);
            }

            if (eflag) {
                continue;
            }

            close(new_socket);
            regfree(&reg);
            regfree(&reg_header);

            for (int i = 0; i < BLOCK; i++) {
                buf[i] = 0;
                field[i] = 0;
            }
        }

        else if (method == 1) {
            int fd = open(field, O_WRONLY | O_TRUNC);
            int new = 0;
            if (fd == -1 && errno == ENOENT) {
                new = 1;
            }

            else if (fd == -1 && errno == EACCES) {
                write_n_bytes(new_socket,
                    "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n", 56);

                close(new_socket);
                regfree(&reg);
                regfree(&reg_header);

                for (int i = 0; i < BLOCK; i++) {
                    buf[i] = 0;
                    field[i] = 0;
                }

                continue;
            }

            else if (fd == -1) {
                write_n_bytes(new_socket,
                    "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\n\r\nInternal "
                    "Server Error\n",
                    80);

                close(new_socket);
                regfree(&reg);
                regfree(&reg_header);

                for (int i = 0; i < BLOCK; i++) {
                    buf[i] = 0;
                    field[i] = 0;
                }

                continue;
            }

            if (new) {
                fd = open(field, O_WRONLY | O_CREAT | O_TRUNC, 0664);
                if (fd == -1 && errno == EACCES) {
                    write_n_bytes(new_socket,
                        "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n", 56);

                    close(new_socket);
                    regfree(&reg);
                    regfree(&reg_header);

                    for (int i = 0; i < BLOCK; i++) {
                        buf[i] = 0;
                        field[i] = 0;
                    }

                    continue;
                }

                else if (fd == -1) {
                    write_n_bytes(new_socket,
                        "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\n\r\nInternal "
                        "Server Error\n",
                        80);

                    close(new_socket);
                    regfree(&reg);
                    regfree(&reg_header);

                    for (int i = 0; i < BLOCK; i++) {
                        buf[i] = 0;
                        field[i] = 0;
                    }

                    continue;
                }
            }

            for (int i = 0; i < BLOCK; i++) {
                field[i] = 0;
            }

            strncpy(field, &buf[pmatch[4].rm_so], pmatch[4].rm_eo - pmatch[4].rm_so);

            for (int i = 0; i < 2048; i++) {
                if (field[i] == '\r' || field[i] == '\n') {
                    field[i] = 0;
                }
            }

            char key[129] = { 0 };
            char value[129] = { 0 };
            int index = 0;
            int lenFlag = 0;
            int len = -1;

            while (field[index] != 0) {
                if (regexec(&reg_header, &field[index], 3, pmatch_header, 0)) {
                    break;
                }

                // printf("%s\n", &field[index]);

                strncpy(key, &field[index + pmatch_header[1].rm_so],
                    pmatch_header[1].rm_eo - pmatch_header[1].rm_so);
                strncpy(value, &field[index + pmatch_header[2].rm_so],
                    pmatch_header[2].rm_eo - pmatch_header[2].rm_so);

                // printf("key: %s\n", key);
                // printf("value: %s\n", value);
                // printf("-------------------------\n");

                if (strncmp(key, "Content-Length", 15) == 0) {
                    len = atoi(value);
                    if (len == 0) {
                        for (unsigned int i = 0; i < strlen(value); i++) {
                            if (value[i] != '0') {
                                len = -1;
                            }
                        }
                    }

                    if (len < 0) {
                        break;
                    }

                    lenFlag = 1;
                    break;
                }

                for (int i = 0; i < 128; i++) {
                    key[i] = 0;
                    value[i] = 0;
                }

                index += (strlen(&field[index]) + 2);
            }

            if (!lenFlag) {
                write_n_bytes(new_socket,
                    "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n", 60);

                close(new_socket);
                regfree(&reg);
                regfree(&reg_header);

                for (int i = 0; i < BLOCK; i++) {
                    buf[i] = 0;
                    field[i] = 0;
                }

                continue;
            }

            for (int i = 0; i < 2048; i++) {
                field[i] = 0;
            }

            strncpy(field, &buf[pmatch[6].rm_so], pmatch[6].rm_eo - pmatch[6].rm_so);
            do {
                if (len >= (long) strlen(field)) {
                    len -= write_n_bytes(fd, field, strlen(field));
                }

                else {
                    len -= write_n_bytes(fd, field, len);
                }

                if (len > 0) {
                    for (unsigned int i = 0; i < BLOCK; i++) {
                        field[i] = 0;
                    }
                    if (read(new_socket, field, BLOCK) <= 0) {
                        break;
                    }
                }
            } while (len > 0);

            if (new) {
                write_n_bytes(
                    new_socket, "HTTP/1.1 201 Created\r\nContent-Length: 8\r\n\r\nCreated\n", 51);
            }

            else {
                write_n_bytes(new_socket, "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nOK\n", 41);
            }

            close(fd);
            close(new_socket);
            regfree(&reg);
            regfree(&reg_header);

            for (int i = 0; i < BLOCK; i++) {
                buf[i] = 0;
                field[i] = 0;
            }
        }

        else {
            write_n_bytes(new_socket,
                "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\n\r\nInternal Server "
                "Error\n",
                80);

            close(new_socket);
            regfree(&reg);
            regfree(&reg_header);

            for (int i = 0; i < BLOCK; i++) {
                buf[i] = 0;
                field[i] = 0;
            }

            continue;
        }
    }
}
