#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"
#include "redirect.c"

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
      int i = 1;
      // for(i = 1; i < sizeof(args)-1; i++){
         if((args[1] != NULL) && (strcmp(args[1],"|")==0)) {
         // if((args[i] != NULL) && (strcmp(args[i],"|")==0)) {
            char *p[3] = {"pipe", args[0],args[2]};
            // char *p[3] = {"pipe", args[i-1],args[i+1]};
            if( (pid = fork()) == -1) {
               perror("cannot fork");
               exit(2);
            }
            if(pid == 0)
               pipe1(p);
            rv = 1;
         }
         else if((args[1] != NULL) && ( (char)args[1][0] == '>' || (char)args[1][0] == '<') ) { // does this work?
            // redirection goes here
            char *r[3] = {"redirection", args[0], args[2]};
            // char *r[3] = {"redirection", args[i-1], args[i+1]};
            if((pid = fork() == -1)){
               perror("cannot fork");
               exit(2);
            }
            if(pid == 0)
               redirect(r);
            rv = 1;
         }
         else if(!builtin_command(args, &rv) ) {
            rv = execute(args);
         }
      // }
   }
   return rv;
}
