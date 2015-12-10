/* Handles three, out of four, functionalities of redirection in a unix shell:
 * 1. Append to stdout '>>'
 * 2. Redirect to stdout '>'
 * 3. Redirect stdin '<'
 * Here-documents '<<' have not yet to be implemented into this version of our shell
 *
 * Author: Anthony Brignano
 */

#include	<stdio.h>
#include	<unistd.h>
#include    <stdlib.h>
#include	<fcntl.h>
#include	<string.h>
 
#define	oops(m,x)	{ perror(m); exit(x); }
#define BUFFSIZE 512

int redirect(char **av)
{
	if(strcmp(av[1],">>")==0){		// Append standard output
		int fd = open(av[3], O_CREAT | O_APPEND | O_WRONLY, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execlp(av[2], av[2], NULL);
	}
	else if(strcmp(av[1],"<<")==0){		// Here-document
		// need to implement a Here Document here
		// takes input until it reaches av[3] & runs it all through the command stored in av[2]
		
	}
	else if(strcmp(av[1],">")==0){		// Redirect standard output
		int fd = open(av[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execlp(av[2], av[2], NULL);
	}
	else if(strcmp(av[1],"<")==0){		// Redirect standard input
		int fd = open(av[3], O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
		execlp(av[2], av[2], NULL);
	}
	else{								// Error handling
		oops("Invalid redirection input ", 1);
	}

	return 1;
}
