#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

int main (void) {
	// initialize variables
	bool debug = 1;
	char input[256];
	long size;
	char *buf;
	char *ptr;
	bool toggle=0;
	int params = 0;
	
	char input2[256];
	
	size = pathconf(".", _PC_PATH_MAX);
	
	// main shell loop
	while (1) {
		toggle = 0;
		params = 0;
		
		// get cwd and display terminal string and await user input
		if ((buf = (char *)malloc((size_t)size)) != NULL) {
			ptr = getcwd(buf, (size_t)size);
		}
		printf("%s$ ", ptr);
		scanf("%[^\n]%*c", input);
		
		for(int i = 0; i < (sizeof(input)/sizeof(input[0])); i++) {
			input2[i] = input[i];
		}
		
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
			
			if (debug)
				printf("Arguments: ");
			while(token != NULL) {
         			params += 1;
         			if (debug)
         				printf("%s ", token);
         			token = strtok(NULL, " ");
      			}
      		
      			if (debug)
      				printf("\nNumber of Arguments: %d ", params);
      		
      			char* argument_list[params];
		
			token = strtok(input2, " ");
			
			if (debug)
				printf("\nArgument List: ");
    			for(int i = 0; i < params + 1; i++) {
        			argument_list[i] = token;
        			if (debug)
    					printf("%s ", token);
        			token = strtok(NULL, " ");
    			}
    			
			int status;
			
			pid_t pid = fork();
			
			// if we need the PID of the Child
    			pid_t child = getpid();

		
			// if in child process
			if(pid == 0){
				// run input inside child process bro ...
        			if (debug) 
        				printf("\nIn Child: %d\n", child);
        			fflush(stdout);
        			execvp(argument_list[0], argument_list);
			}
        		
        		// if in parent process
    			else {
				wait(&status);
    			}
		
		}
	
	}	
	
}

