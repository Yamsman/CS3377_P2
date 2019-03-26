#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>
int main(int argc, char **argv) {
	//Read option and set word accordingly
	int opt;
	char *word = NULL;
	while ((opt = getopt(argc, argv, "dfb")) != -1) {
		switch (opt) {
			case 'd':	word = "directories";	break;
			case 'f': 	word = "files";		break;
			case 'b':	word = "bytes";		break;
		}
	}
	if (word == NULL) return -1;

	//Read lines from standard input until exhausted
	int word_found = 0;
	size_t bufsize = 256;
	char *buf = malloc(256);
	while (getline(&buf, &bufsize, stdin)) {
		//Stop on EOF
		if (feof(stdin)) break;

		//If the line contains the word, display it and stop
		if (strstr(buf, word)) {
			printf("%s", buf);
			word_found = 1;
			break;
		}
	}
	free(buf);

	//Error message for no relevant word found
	if (!word_found)
		printf("%s: input does not contain '%s'\n", argv[0], word);
	
	return 0;
}
