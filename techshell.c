#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

int main (void) {
	// initialize variables
	char input[256];
	long size;
	char *buf;
	char *ptr;
	bool toggle=0;
	
	size = pathconf(".", _PC_PATH_MAX);
	
	// main shell loop
	while (1) {
		
		// get cwd and display terminal string and await user input
		if ((buf = (char *)malloc((size_t)size)) != NULL) {
			ptr = getcwd(buf, (size_t)size);
		}
		printf("%s$ ", ptr);
		scanf("%[^\n]%*c", input);
		
		// check for exit command and exit
		if (strcmp(input, "exit") == 0) {
			exit(0);
			toggle=1;
		}
		
		// check for exit command and return pwd
		else if (strcmp(input, "pwd") == 0) {
			printf("%s\n", ptr);
			toggle=1;
		}
		
		// check for cd and perform proper command
		char* token = strtok(input, " ");
		if (strcmp(token, "cd") == 0) {
			token = strtok(NULL, " ");
			chdir(token);
			toggle=1;
		}
		
		// if none of the above commands run, run input in child process ...
		if (!toggle) {
			// run input inside child process bro ...
			printf("I should be running in a child process...\n");
		}
	}	
	
}

