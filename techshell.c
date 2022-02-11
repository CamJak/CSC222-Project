#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "./functions.c"

#define KNRM "\x1B[0m"
#define KGRN "\x1B[92m"
#define KCYN "\x1B[96m"

int main (void) {
	// initialize variables
	char input[256];
	char* input2;
	char toggle;
	char run;
	char debug = 0;
	
	// main shell loop
	while (1) {
		
		// get cwd and display terminal string and await user input
		char *ptr = get_cwd_custom();
		
		printf("%s%s%s$%s ", KGRN, ptr, KCYN, KNRM);
		
		scanf("%[^\n]%*c", input);
		
		input2 = strdup(input);
		
		toggle = keyword_check(input, ptr);
		
		// if none of the above commands run, run input in child process ...
		if (toggle == 0) {
			// run input inside child process bro ...
			// printf("I should be running in a child process...\n");
			//run = execute_command(input2, debug);
			output_to_file(input2, debug);
		}
	}	
}

