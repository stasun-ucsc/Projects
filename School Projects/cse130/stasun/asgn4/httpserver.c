#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>

#include "connection.h"
#include "queue.h"
#include "rwlock.h"
#include "request.h"
#include "response.h"
#include "asgn2_helper_funcs.h"
#include "list.h"

#define OPTIONS "t:"

// declaring the request queue
static queue_t *req_q;
static List L;
static pthread_mutex_t lock;

void handle_connection(int connfd);
void handle_get(conn_t *conn);
void handle_unsupported(conn_t *conn);
void handle_put(conn_t *conn);

void *worker() {
    // while loop call queue pop and handle connection
    while (1) {
        int conn_fd;
        queue_pop(req_q, (void *) (&(conn_fd)));
        handle_connection(conn_fd);
        // maybe this can be handled elsewhere?
        close(conn_fd);
    }

    return NULL;
}

int main(int argc, char **argv) {
    if (argc > 4 || argc < 2) {
        printf("incorrect argument count\n");
        return 1;
    }
    int opt, port, threads = 4;
    bool t_flag = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        if (opt == 't') {
            t_flag = true;
            threads = atoi(optarg);
            if (threads <= 0) {
                printf("throw error\n");
                return EXIT_FAILURE;
            }
        }
    }

    if (t_flag) {
        port = atoi(argv[3]);
        if (port > 65535 || port < 1) {
            printf("Invalid port\n");
            return EXIT_FAILURE;
        }
    }

    else {
        port = atoi(argv[1]);
        if (port > 65535 || port < 1) {
            printf("Invalid port\n");
            return EXIT_FAILURE;
        }
    }

    // printf("threads: %d port: %d\n", threads, port);

    signal(SIGPIPE, SIG_IGN);
    Listener_Socket socket;
    listener_init(&socket, port);

    // declare a queue, linked list, and a mutex lock globally
    req_q = queue_new(threads);
    L = newList();
    pthread_mutex_init(&lock, NULL);

    pthread_t t_array[threads];
    // uintptr_t val_ptr[threads];

    // for loop to call pthread create, maybe check out practica
    for (int i = 0; i < threads; i++) {
        // I think we don't need to pass an args parameter?
        pthread_create(&(t_array[i]), NULL, worker, NULL);
    }

    while (1) {
        int connfd = listener_accept(&socket);

        // call queue push and push connfd
        queue_push(req_q, (void *) (uintptr_t) connfd);
    }

    // is there a need to call pthread_join()?

    return EXIT_SUCCESS;
}

void handle_connection(int connfd) {

    conn_t *conn = conn_new(connfd);

    const Response_t *res = conn_parse(conn);

    if (res != NULL) {
        conn_send_response(conn, res);
    } else {
        // debug("%s", conn_str(conn));
        const Request_t *req = conn_get_request(conn);
        if (req == &REQUEST_GET) {
            handle_get(conn);
        } else if (req == &REQUEST_PUT) {
            handle_put(conn);
        } else {
            handle_unsupported(conn);
        }
    }

    conn_delete(&conn);
    // can we put this here?
    // close(connfd);
}

void handle_get(conn_t *conn) {

    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL;

    // debug("GET request not implemented. But, we want to get %s", uri);

    // What are the steps in here?
    // 1. Open the file.
    // If  open it returns < 0, then use the result appropriately
    //   a. Cannot access -- use RESPONSE_FORBIDDEN
    //   b. Cannot find the file -- use RESPONSE_NOT_FOUND
    //   c. other error? -- use RESPONSE_INTERNAL_SERVER_ERROR
    // (hint: check errno for these cases)!

    pthread_mutex_lock(&lock);
    int fd = open(uri, O_RDONLY);
    pthread_mutex_unlock(&lock);
    // error checking
    if (fd < 0) {
        // checking for permissions
        if (errno == EACCES) {
            res = &RESPONSE_FORBIDDEN;
        }

        // checking if file exists
        else if (errno == ENOENT) {
            res = &RESPONSE_NOT_FOUND;
        }

        // handling for other errors
        else {
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
        }

        conn_send_response(conn, res);
    }

    // 2. Get the size of the file.
    // (hint: checkout the function fstat)!

    struct stat stats;
    fstat(fd, &stats);

    // Get the size of the file.

    uint64_t size = stats.st_size;

    // 3. Check if the file is a directory, because directories *will*
    // open, but are not valid.
    // (hint: checkout the macro "S_IFDIR", which you can use after you call fstat!)

    if (S_ISDIR(stats.st_mode)) {
        res = &RESPONSE_FORBIDDEN;
        conn_send_response(conn, res);
    }

    // 4. Send the file
    // (hint: checkout the conn_send_file function!)
    if (res == NULL) {
        conn_send_file(conn, fd, size);
    }
    close(fd);
}

void handle_unsupported(conn_t *conn) {
    // debug("handling unsupported request");

    // send responses
    conn_send_response(conn, &RESPONSE_NOT_IMPLEMENTED);
}

void handle_put(conn_t *conn) {
    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL;
    // debug("handling put request for %s", uri);

    /* rwlock_t *fileLock = search(L, uri);
    writer_lock(fileLock); */
    // Check if file already exists before opening it.
    bool existed = access(uri, F_OK) == 0;
    // debug("%s existed? %d", uri, existed);

    // Open the file..
    pthread_mutex_lock(&lock);
    int fd = open(uri, O_CREAT | O_TRUNC | O_WRONLY, 0600);
    pthread_mutex_unlock(&lock);
    if (fd < 0) {
        // debug("%s: %d", uri, errno);
        if (errno == EACCES || errno == EISDIR || errno == ENOENT) {
            res = &RESPONSE_FORBIDDEN;
            goto out;
        } else {
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            goto out;
        }
    }

    res = conn_recv_file(conn, fd);

    if (res == NULL && existed) {
        res = &RESPONSE_OK;
    } else if (res == NULL && !existed) {
        res = &RESPONSE_CREATED;
    }

    close(fd);

out:
    conn_send_response(conn, res);
}
