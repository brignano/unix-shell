//
//  main.c
//  lab02
//  Objective: write a program that can display the list of users currently logged in to the Unix computer.
//
//  Created by Anthony Brignano on 9/2/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHOWHOST

void showtime(long);
void show_info(struct utmp *);

int main() {

    struct utmp utbuf;
    int utmpfd = 0;

    if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
        perror(UTMP_FILE);
	return 1;        
    }
    while(read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf)){
        show_info(&utbuf);
    }
    close(utmpfd);
    return 0;
}

void show_info(struct utmp *utbufp){
    if(utbufp->ut_type != USER_PROCESS) return;
    printf("%-8.8s ", utbufp->ut_name);
    printf("%-8.8s     ", utbufp->ut_line);
    showtime(utbufp->ut_time);
#ifdef SHOWHOST
    if(utbufp->ut_host[0] != '\0'){
        printf(" (%s)", utbufp->ut_host);
    }
#endif
    printf("\n");
}

void showtime(long timeval){
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M", localtime(&timeval));
    printf("%s", buffer);
}
