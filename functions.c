/*
	This program will be for housing the functions/subroutines
	that will be run inside techshell.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// does all the funky stuff to get cwd and make it look pretty
char* get_cwd_custom () {
	long size;
	char *buf;
	char *ptr;
	
	size = pathconf(".", _PC_PATH_MAX);

	if ((buf = (char *)malloc((size_t)size)) != NULL) {
		ptr = getcwd(buf, (size_t)size);
		return ptr;
	}
}

/* 
	checks for our built keywords and runs appropiate code when detected
	also sets toggle state accordingly
*/

char keyword_check (char input[], char *ptr) {

	// check for exit keyword
	if (strcmp(input, "exit") == 0) {
		exit(0);
	}
	
	// check for pwd keyword
	if (strcmp(input, "pwd") == 0) {
		printf("%s\n", ptr);
		return 1;
	}
		
	// check for cd keyword and argument
	char* token = strtok(input, " ");
	if (strcmp(token, "cd") == 0) {
		token = strtok(NULL, " ");
		int ch = chdir(token);
		if(ch<0) {
            		printf("%s: No such file or directory\n", token);
        	}
		return 1;
	}
	
	// if none, return 0
	return 0;
}

char execute_command (char input[], char debug) {

	int params = 0;
	char toggle = 0;
	char* input2 = strdup(input);
	char* token = strtok(input, " ");
	
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
        	toggle = 1;
        	fflush(stdout);
        	int status_code = execvp(argument_list[0], argument_list);
        	
        	if (status_code == -1) {
        		printf("%s: command not found\n", input);
        		exit(100);
        	}
	}	
        // if in parent process
    	else {
    		toggle = 1;
		wait(NULL);
    	}
    	return 1;
}








