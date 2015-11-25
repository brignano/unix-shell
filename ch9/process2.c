#include	<stdio.h>
#include	"smsh.h"
#include 	<sys/stat.h>
#include 	<stdlib.h>
#include 	<string.h>
#include	<unistd.h>
#include	<signal.h>
#include	<string.h>
#include	<sys/wait.h>
#include	"varlib.h"
/* process2.c   - version 2 - supports builtins
 * command processing layer
 * 
 * The process(char **arglist) function is called by the main loop
 * It sits in front of the execute() function.  This layer handles
 * two main classes of processing:
 * 	a) built-in functions (e.g. exit(), set, =, read, .. )
 * 	b) control structures (e.g. if, while, for)
 */

int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();
int builtin_command(char **, int *);
int pipeinit(char **args)
{
struct stat file;
int pid;
int child_info = -1;
if ( (pid = fork())  == -1 )
		perror("fork");
	else if ( pid == 0 ){
char path1[1024] = {0};
char path2[1024]= {0};
char binpath1[1024] = {0};
char binpath2[1024]= {0};
char finalpath1[1024] = {0};
char finalpath2[1024]= {0};

		strcpy(path1,"/home/jaskaran/ch9/");
		strcpy(path2,"/home/jaskaran/ch9/");
		strcpy(binpath1,"/bin/");
		strcpy(binpath2,"/bin/");
		strcat(path1, args[0]);
		strcat(path2,args[2]);
		strcat(binpath1, args[0]);
		strcat(binpath2,args[2]);
		//printf("%s",path1);
		if(stat(path1,&file)==0)
			strcpy(finalpath1,path1);

		else if(stat(path2,&file)==0)
			strcpy(finalpath2,path2);
		
		 if(stat(binpath1,&file)==0)
			strcpy(finalpath2,args[0]);

		else if(stat(binpath1,&file)==0)
			strcpy(finalpath2,args[2]);
		if(((stat(path1,&file)!=0)&&(stat(binpath1,&file)!=0))
		    ||((stat(path2,&file)!=0)&&(stat(binpath2,&file)!=0)))
		{
		perror("can not open files");
		exit(1);
		}
		
		char *p[3] = {"pipe", finalpath1,finalpath2};
		 pipe1(p);
		
}
else {
		if ( wait(&child_info) == -1 )
			perror("wait");
	}
	return child_info;
}
int process(char **args)
/*
 * purpose: process user command
 * returns: result of processing command
 * details: if a built-in then call appropriate function, if not execute()
 *  errors: arise from subroutines, handled there
 */
{
	int		rv = 0;
	
	if ( args[0] == NULL )
		rv = 0;
	else if ( is_control_command(args[0]) )
		rv = do_control_command(args);
	else if ( ok_to_execute() )
		 if (!(args[1]==NULL) && strcmp(args[1],"|")==0)
		{
	rv = pipeinit(args);
		}
		else if ( !builtin_command(args,&rv))
		rv = execute(args);
	return rv;
}

