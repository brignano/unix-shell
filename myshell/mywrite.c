//
//  main.c
//  lab07
//  Objective: write a program that can send a message to another user logged into a Unix computer.
//
//  Created by Anthony Brignano on 10/28/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <utmp.h>


char *get_user_tty(char *longname, char* ttyname);
void create_message(char buf[]);

int main(int ac, const char * av[]) {
    int fd;
    char buf[BUFSIZ];
    char *user_tty = NULL;
    char eof[] = "EOF\n";

    if(ac != 3){
        fprintf(stderr,"usage: ./mywrite username ttyname");
        exit(1);
    }

    if ((user_tty = get_user_tty( (char*)av[1] , (char*)av[2] )) == NULL )
    {
        fprintf(stderr, "User %s is not logged in.\n", av[1]);
        exit(1);
    }

    //sprintf(buf, "/dev/%s", (char*)av[2]);
    sprintf(buf, "/dev/%s", user_tty);
    fd = open( buf, O_WRONLY );
    if (fd == -1)
    {
        perror(buf);
        exit(1);
    }

    create_message(buf);
    if ( write(fd, buf, strlen(buf)) == -1)
    {
        perror("write");
        close(fd);
        exit(1);
    }

    // Send text from stdin
    while( fgets(buf, BUFSIZ, stdin) != NULL)
    {
        if ( write(fd, buf, strlen(buf)) == -1)
            break;
    }
    // Send EOF
    write(fd, eof, strlen(eof));

    return 0;
}

void create_message(char buf[])
{
    char sender_host[256];
    time_t now;

    char *sender_name = getlogin();
    char *sender_tty = ttyname( STDIN_FILENO );
    gethostname ( sender_host , 256 ) ;
    time ( &now ) ;
    struct tm *timeval = localtime( &now );
    sprintf( buf , "\nMessage from %s@%s on %s at %2d:%02d:%02d...\n" ,
            sender_name, sender_host, 5+sender_tty,
            timeval->tm_hour, timeval->tm_min, timeval->tm_sec);
}



char *get_user_tty(char *longname, char * ttyname)
{
    static struct utmp utmp_rec;
    int utmp_rec_size = (unsigned) sizeof(utmp_rec);
    int utmp_fd = 0;
    int namelen = (unsigned) sizeof( utmp_rec.ut_name );
    char *retval = NULL;

    if ( (utmp_fd = open ( UTMP_FILE, O_RDONLY)) == -1 )
        return NULL;

    while (read (utmp_fd, &utmp_rec, (unsigned) utmp_rec_size) == utmp_rec_size)
        if (strncmp(longname, utmp_rec.ut_name, (unsigned) namelen) == 0)
        {
            if (strncmp(ttyname, utmp_rec.ut_line, (unsigned) namelen) == 0) {
                retval = utmp_rec.ut_line;
                break;
            }
        }

    close(utmp_fd);
    return retval;
}
