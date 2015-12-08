
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "myshell.h"

int assign(char *);
int reference(char *);
int okname(char *);

int builtin_command(char **args, int *resultp) {
   int rv=0;
   
   if(strcmp(args[0],"set")==0) {
      VLlist();
      *resultp = 0;
      rv = 1;
   }
   else if(strchr(args[0],'$') != NULL) {
      *resultp = reference(args[0]);
      if(*resultp != -1)
         rv = 1;
   }
   else if(strcmp(args[0],"unset")==0) {
      *resultp = VLunset(args[1]);
      if(*resultp != 1 )
         rv = 1;
   }
   else if(strcmp(args[0],"read")==0) {
      char variable[100];
      char *del_new_line;
      strcat(args[1],"=");
      read(STDIN_FILENO,variable,sizeof(variable));
      del_new_line = strchr(variable, '\n');
      *del_new_line = '\0';
      strcat(args[1],variable);
      *resultp = assign(args[1]);
      if(*resultp != -1)
         rv = 1;
   }
   else if(strchr(args[0], '=') != NULL) {
      *resultp = assign(args[0]);
      if(*resultp != -1)
         rv = 1;
   }
   else if(strcmp(args[0], "export")==0) {
      if(args[1] != NULL && okname(args[1]))
         *resultp = VLexport(args[1]);
      else
         *resultp = 1;
      rv = 1;
   }
   else if(strcmp(args[0],"exit")==0)
      exit(0);
   else if(strcmp(args[0],"cd")==0) {
      char dir[100] = "/";
      getcwd(dir, sizeof(dir));
      if(args[1] != NULL) {
         strcat(dir, "/");
         strcat(dir, args[1]);
      }
      if(chdir(dir) == 0) {
         printf("%s\n",dir);
         rv = 1;
      }
      else
         rv = 0;
   }
   return rv;
}

int assign(char *str) {
   char *cp;
   int rv;
   
   cp = strchr(str,'=');
   *cp = '\0';
   rv = ( okname(str) ? VLstore(str, cp+1) : -1);
   *cp = '=';
   return rv;
}

int reference(char *str) {
   char *cp, *orig_str;
   int rv;
   
   orig_str = str;
   cp = strchr(str,'$');
   str++;
   rv = ( okname(str) ? VLreference(str) : -1);
   str = orig_str;
   return rv;
}

int okname(char *str) {
   char *cp;
   
   for(cp = str; *cp; cp++) {
      if( (isdigit(*cp) && cp==str) || !(isalnum(*cp) || *cp=='_' ))
         return 0;
   }
   return ( cp != str );
}
