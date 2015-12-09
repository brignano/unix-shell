#include	<stdio.h>
#include	<unistd.h>
#include    <stdlib.h>
#include	<fcntl.h>
#include	<string.h>
 
#define	oops(m,x)	{ perror(m); exit(x); }

int redirect(char **av)
{
	int	thepipe[2],			/* two file descriptors	*/
		newfd;				/* useful for pipes	*/
		//pid;				/* and the pid		*/

	if ( pipe( thepipe ) == -1 )		/* get a pipe		*/
		oops("Cannot get a pipe", 1);

	//if ( (pid = fork()) == -1 )			/* get a proc	*/
		//oops("Cannot fork", 2);

	/* ------------------------------------------------------------ */
	/* 	Right Here, there are two processes			*/
	/*             parent will read from pipe			*/

	if(strcmp(av[1],">>")==0){
	// create file for writing, overwrite previous one
	// implies av[3] is an output file
		int fd = open(av[3], O_CREAT | O_APPEND | O_WRONLY, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execlp(av[2], av[2], NULL);
	}
	else if(strcmp(av[1],"<<")==0){
		
	}
	else if(strcmp(av[1],">")==0){
	// open file for writing, append previous one
		int fd = open(av[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execlp(av[2], av[2], NULL);
	}
	else if(strcmp(av[1],"<")==0){
	// open file for reading, output user appropriate text
	// implies av[3] is an output file
		int fd = open(av[3], O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
		execlp(av[2], av[2], NULL);
	}
	else{
		oops("Invalid redirection input ", 1);
	}

	return 1;
}
