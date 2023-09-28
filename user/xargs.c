#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char**
parseToWords(char* input) {
	int words = 0;
	for(int i = 0; i < strlen(input) - 1; ++i) {
		if(input[i] == ' ')
			words++;
	}
	words++;
	printf("%d\n", words);
	if(words == 0) {
		fprintf(2, "xargs: no arguments for command\n");
		exit(1);
	}

	char** lines = (char**)malloc(words * sizeof(char*));
	char* temp = (char*)malloc(100);
	int idx = 0;
	int idxtemp = 0;
	for(int i = 0; i < strlen(input); ++i) {
		if(input[i] == ' ' || input[i] == '\n') {
			temp[i] = '\0';
			lines[idx] = (char*) malloc(strlen(temp)+1);
			strcpy(lines[idx], temp);
			free(temp);
			temp = (char*)malloc(100);
			printf("%s\n", lines[idx]);
			idx++;
			idxtemp = 0;
			continue;
		}
		temp[idxtemp] = input[i];
		idxtemp++;
	}

	return lines;
}

int
main(int argc, char *argv[]) {
	
	if(argc < 2) {
		fprintf(2, "xargs: no command\n");
		exit(1);
	}

	char buf[128];
	int bytes = read(0, buf, sizeof(buf));

	if(bytes == -1) {
		fprintf(2, "xargs: cannot read\n");
		exit(1);
	}

	char** lines = parseToWords(buf);
	free(lines); //before it's needed to dealloc pointers we point to
	exit(0);
}
