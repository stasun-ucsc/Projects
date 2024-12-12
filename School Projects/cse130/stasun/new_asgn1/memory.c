#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#define BLOCK 4096

// PATH_MAX = 4096

void *resize(void *ptr, unsigned *currSize) {
	void *res = realloc(ptr, *currSize + BLOCK);
	if (res != NULL) { 
		memset(res + *currSize, 0, BLOCK);
		*currSize += BLOCK;
	}
	
	return res;
}

void process(char *buf, char *ptr1, char *ptr2, char *ptr3, char *ptr4) {
	char *ptrs[4] = {ptr1, ptr2, ptr3, ptr4};
	for (int i = 0; i < 3; i++) {
		ptrs[i] = strtok(buf, "\n");	
	}
	ptrs[3] += (strlen(ptrs[2]) + 1);

	return;
}

int method(char *method) {
	if (!strncmp(method, "get", 4)) {
		return 0;	
	} else if (!strncmp(method, "set", 4)) {
		return 1;
	} else {
		return -1;
	}
}

int get(char *path) {
	int fd = open(path, O_RDONLY);
	char buf[BLOCK + 1];
	memset(buf, 0, BLOCK + 1);
	while (read(fd, buf, BLOCK) > 0) {
		printf("%s", buf);
		memset(buf, 0, BLOCK + 1);
	}

	close(fd);
	return 0;
}

int set(char *path, char *len, char *data) {
	int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC);
	int toWrite = atoi(len), idx = 0;
	while (toWrite > 0) {
		int written;
		if (toWrite > BLOCK) {
			written = write(fd, data + idx, BLOCK);
		} else {
			written = write(fd, data + idx, toWrite);
		}
		toWrite -= written;
		idx += written;
	}

	close(fd);	
	return 0;
}

int main(void) {
	char *buf = (char *)calloc(BLOCK, sizeof(char));
	int idx = 0, bytesRead;
	unsigned currSize = BLOCK;
	while ((bytesRead = read(0, buf, currSize - idx)) > 0) {
		idx += bytesRead;
		if (idx == currSize) {
			buf = (char *)resize(buf, &currSize);
		}
	}
	char *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL;
	process(buf, ptr1, ptr2, ptr3, ptr4);
	int op = method(ptr1);
	if (op < 0) {
		// throw error and say invalid method
		printf("Invalid Method\n");
		exit(EXIT_FAILURE);
	} else if (!op) { 
		if (!get(ptr2)) {
			return 0;
		}
	 	/* else:
			throw error */
	} else {
		if (!set(ptr2, ptr3, ptr4)) {
	 		return 0;
		}
	 	/* else:
	 		throw error */
	}

	printf("ERROR\n");
	return -1;
}
