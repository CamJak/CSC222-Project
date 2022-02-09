all: functions techshell clean

functions: functions.c
	@gcc -o func functions.c
	
techshell: techshell.c
	@gcc -o techshell techshell.c
	@./techshell
	
clean:
	@rm -f techshell
	@rm -f func
