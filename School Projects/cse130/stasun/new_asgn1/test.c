#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	if (!strncmp("fire", "fires", 5)) {
		printf("ye\n");
	} else {
		printf("nah\n");
	}
	
	return 0;
}

