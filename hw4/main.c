#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int read_line(int fd, char *word) {
	*word = '\0';
	char buf[2] = "";
	int not_eof;

	while ( (not_eof = read(fd, buf, 1)) && *buf != '\n' ) {
		strcat(word, buf);
	}

	return not_eof;
}

int main() {

	char *ptr[1000] = { NULL };
	int fd = open("test.txt", O_RDONLY, 0777);
	char line[128];
	int id, size, not_eof;

	do {
		not_eof = read_line(fd, line);

		if ( sscanf(line, "A\t%d\t%d", &id, &size) ) {
			ptr[id] = malloc(size);
			for (int i = 0; i < size; ++i) {
				ptr[id][i] = rand();
			}
		}
		else if ( sscanf(line, "D\t%d", &id) ) {
			free(ptr[id]);
		}

	} while (not_eof);

	malloc(0);
	return 0;
}