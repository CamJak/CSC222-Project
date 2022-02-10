#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "./functions.c"

int main (void) {
	// initialize variables
	char input[256];
	char toggle;
	
	// main shell loop
	while (1) {
		
		// get cwd and display terminal string and await user input
		char *ptr = get_cwd_custom();
		
		printf("%s$ ", ptr);
		
		scanf("%[^\n]%*c", input);
		
		toggle = keyword_check(input, ptr);
		
		// if none of the above commands run, run input in child process ...
		if (toggle == 0) {
			// run input inside child process bro ...
			printf("I should be running in a child process...\n");
		}
	}	
}

