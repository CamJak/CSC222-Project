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
	char toggle, io_toggle=0;
	char run;
	char debug = 0;
	
	// main shell loop
	while (1) {
		
		// get cwd and display terminal string and await user input
		char *ptr = get_cwd_custom();
		
		// print cwd in color
		printf("%s%s%s$%s ", KGRN, ptr, KCYN, KNRM);
		
		// get user input
		scanf("%[^\n]%*c", input);
		
		// duplicate input (a few times)
		input2 = strdup(input);
		char* input3 = strdup(input);
		
		// check for hardcoded functions
		toggle = keyword_check(input, ptr);
		
		// if none of the above commands run, run input in child process
		if (toggle == 0) {
			
			// checks if I/O is directed in command
			io_toggle = io_check(input3);
      	
      			// if I/O is specified, use it, otherwise just run command
			if (io_toggle == 0) {
				run = execute_command(input2, debug);
			} else
				file_IO(input2, debug);
		}
	}	
}

