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
		chdir(token);
		return 1;
	}
	
	// if none, return 0
	return 0;
}

