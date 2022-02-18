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
		if (strcmp(token, "~") == 0) {
			sprintf(token, "%s", getenv("HOME"));
		}
		int ch = chdir(token);
		if(ch<0) {
            		printf("%s: No such file or directory\n", token);
        	}
		return 1;
	}
	
	// if none, return 0
	return 0;
}

/*
	Iterates through given input and checks for < or >
*/

char io_check (char input[]) {
	// Checks to see if I/O is given or not
	char io_toggle=0;
	char* token = strtok(input, " ");
	while(token != NULL) {
	 	if (*token == 0x3E) {
	 		io_toggle=1;
	 		break;
	 	} else if (*token == 0x3C) {
	 		io_toggle=1;
	 		break;
	 	} else
	 		io_toggle=0;
	 	
	 	token = strtok(NULL, " ");
      	}
      	return io_toggle;
}

/*
	Creates child process and runs command given in execvp
*/

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
		fflush(stdout);
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

/*
	redirects I/O to or from a file
*/

char file_IO (char input[], char debug) {

	int params = 0;
	char output_file = 0;
	char output_file_found;
	char input_file = 0;
	char input_file_found = 0;
	char* input2 = strdup(input);
	char* token = strtok(input, " ");
	char* my_output_file;
	char* my_input_file;

	
	if (debug)
		printf("Arguments: ");
		
	// loop through tokens to find any instances of < or >
	while(token != NULL) {
         	params += 1;
         	if (debug)
         		printf("%s ", token);
         	if (output_file_found == 1) {
         		my_output_file = token;
         		output_file_found = 0;
         		output_file = 1;
         	}
         	else if (input_file_found == 1) {
         		my_input_file = token;
         		input_file_found = 0;
         		input_file = 1;
         	}
         	else if (*token == 0x3E) {
         		output_file_found = 1;
         	}
         	else if (*token == 0x3C) {
         		input_file_found = 1;
         	}
         	
         	token = strtok(NULL, " ");
      	}
      		
      	if (debug)
      		printf("\nNumber of Arguments: %d ", params);
      	
	token = strtok(input2, " ");
			
	if (debug) {
		printf("\nArgument List: ");
		fflush(stdout);
	}
	
	// runs if both an input and output file are specified
	if (output_file == 1 && input_file == 1) {
		char* argument_list[params - 3];
    		for(int i = 0; i < params - 3; i++) {
        		if (*token == 0x3C)
    				argument_list[i] = NULL;
    			else
        			argument_list[i] = token;
        		if (debug)
    				printf("%s ", token);
        		token = strtok(NULL, " ");
    		}
    		
    		output_file=0;
    		input_file=0;
    		
    		int status;
    		
    		pid_t pid = fork();
    		
    		pid_t child = getpid();
    		
    		if (pid == 0) {
    			
    			FILE* outfile = fopen(my_output_file, "w");
			dup2(fileno(outfile), 1);
			fclose(outfile);
			
			FILE* infile = fopen(my_input_file, "r");
			dup2(fileno(infile), 0);
			fclose(infile);
			
			int status_code = execvp(argument_list[0], argument_list);
			
			if (status_code == -1) {
        			printf("%s: command not found\n", input);
        			exit(100);
        		}
    		}
    		
    		else {
    			wait(NULL);
    		}
	}
	
	// only runs if an output file is specified
	else if (output_file == 1) {
		char* argument_list[params - 1];
    		for(int i = 0; i < params - 1; i++) {
        		if (*token == 0x3E)
    				argument_list[i] = NULL;
    			else
        			argument_list[i] = token;
        		if (debug)
    				printf("%s ", token);
        		token = strtok(NULL, " ");
    		}
    		
    		if (debug) {
    			printf("\n");
    		}
    		
    		output_file = 0;
    	
    		int status;
			
		pid_t pid = fork();
			
		// if we need the PID of the Child
    		pid_t child = getpid();

		
		// if in child process
		if(pid == 0){
			// run input inside child process bro ...
        		if (debug) {
        			printf("\nIn Child: %d\n", child);
        			fflush(stdout);
        		}
        		
        		// directs all following output to given file
        		FILE* outfile = fopen(my_output_file, "w");
			dup2(fileno(outfile), 1);
			fclose(outfile);
			fflush(stdout);
			
			// execute given command
        		int status_code = execvp(argument_list[0], argument_list);
        	
        		// if execute fails
        		if (status_code == -1) {
        			printf("%s: command not found\n", input);
        			exit(100);
        		}
		}
			
        	// if in parent process
    		else {
			wait(NULL);
    		}
    	}
    	
    	// only runs if an input file is specified (still broken)
    	else if (input_file == 1) {
    		
    		char* argument_list[params - 1];
    		for(int i = 0; i < params - 1; i++) {
    			if (*token == 0x3C)
    				argument_list[i] = NULL;
    			else
        			argument_list[i] = token;
        		if (debug)
    				printf("%s ", token);
        		token = strtok(NULL, " ");
    		}
    		
    		if (debug) {
    			printf("\n");
    		}
    		
    		input_file = 0;
    	
    		int status;
			
		pid_t pid = fork();
			
		// if we need the PID of the Child
    		pid_t child = getpid();

		
		// if in child process
		if(pid == 0){
			// run input inside child process bro ...
        		if (debug) {
        			printf("\nIn Child: %d\n", child);
        			fflush(stdout);
        		}
        		
        		// sets input file
        		FILE* infile = fopen(my_input_file, "r");
			dup2(fileno(infile), 0);
			fclose(infile);
			
			// execute given command
        		int status_code = execvp(argument_list[0], argument_list);
        	
        		// if execute fails
        		if (status_code == -1) {
        			printf("%s: command not found\n", argument_list[0]);
        			exit(100);
        		}
		}
			
        	// if in parent process
    		else {
			wait(NULL);
    		}
    	}
    	else
    		printf("No I/O Specified\n");
}

	
	








