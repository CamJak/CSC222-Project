all: techshell clean

techshell: techshell.c
	@gcc -o techshell techshell.c
	@./techshell
	
clean:
	@rm -f techshell
