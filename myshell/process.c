#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"

int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();
int builtin_command(char **, int *);

int process(char **args) {
   int rv=0;
   int pid;
   
   if(args[0] == NULL)
      rv = 0;
   else if(is_control_command(args[0]))
      rv = do_control_command(args);
   else if(ok_to_execute()) {
      if((args[1] != NULL) && (strcmp(args[1],"|")==0)) {
         char *p[3] = {"pipe", args[0],args[2]};
         if( (pid = fork()) == -1) {
            perror("cannot fork");
            exit(2);
         }
         if(pid == 0)
            pipe1(p);
         rv = 1;
      }
      else if((args[1] != NULL) && ( args[1][1] == '>' || args[1][1] == '>') ) { // does this work? - args[1][1] to check first character
         // redirection goes here
      }
      else if(!builtin_command(args, &rv) ) {
         rv = execute(args);
      }
   }
   return rv;
}
