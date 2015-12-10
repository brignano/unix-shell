//
//  main.c
//  lab09
//  Objective: write a program that utilizes a timer to create a marquee display.
//
//  Created by Anthony Brignano on 11/10/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#import <fcntl.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>

#define BUFFERSIZE 512

char* remove_spaces(char*);

int main(int ac, const char * av[]) {

    if(ac != 6){
        printf("Usage: ./marquee file_name length row col speed\n");
    }
    else{
        char *fp = (char*)av[1];
        int len = atoi(av[2]);
        int row = atoi(av[3]);
        int col = atoi(av[4]);
        int speed = atoi(av[5]);
        int max_y, max_x;                           // max dimensions of terminal window
        int max_marquee_width = 0;

        initscr();                                  // Turn on curses
        getmaxyx(stdscr, max_y, max_x);             // Get terminal dimensions

        if(row <= 0){
            printf("Error: row number (%d) must be positive", row);
            exit(1);
        }
        else if(row > max_y){
            printf("Error: row number (%d) exceeds terminal height (%d).", row, max_y);
            exit(1);
        }

        if (col <= 0){
            printf("Error: column number (%d) must be positive", col);
            exit(1);
        }
        else if(col > max_x){
            printf("Error: column number (%d) exceeds terminal width (%d).", col, max_x);
            exit(1);
        }

        // num of characters that can fit in the current size terminal window
        max_marquee_width = max_x - row;

        if(len > max_marquee_width){
            printf("Error: length (%d) exceeds marquee width (%d).", len, max_marquee_width);
            exit(1);
        }


        int fd = 0, n_chars = 0;
        char buf[BUFFERSIZE];

        if((fd=open(av[1], O_RDONLY)) == -1) {
            fprintf(stderr, "Error: Cannot open ");
            perror(fp);
            exit(1);
        }

        if(speed < 1 && speed > 99) {
            printf("Error: speed must be between 1 and 99");
            exit(1);
        }

        const int marquee_speed = 5000000 / speed;

        curs_set(0);            // Remove cursor in curses


        while((n_chars = (unsigned)read(fd, buf, BUFFERSIZE)) > 0) {
            int i = 0;

            char* no_spaces_buf = remove_spaces(buf);

            for (i = 0; i < strlen(no_spaces_buf); i++) {
                clear();
                move(row, col);                 // Move to (row, col)// Clear the terminal
                char *marquee_text = (char *) malloc((unsigned) len + 1);

                strncpy(marquee_text, no_spaces_buf+i, len);
                addstr(marquee_text);
                refresh();
                usleep((unsigned)marquee_speed);
            }
        }

        if(n_chars == -1){
            fprintf(stderr, "Error: Read error from ");
            perror(fp);
            exit(1);
        }
        if(close(fd) == -1){
            fprintf(stderr, "Error: Error closing file ");
            perror(fp);
            exit(1);
        }

        endwin();
    }
    return 0;
}


char* remove_spaces(char* buf){

    char* no_spaces_buf = malloc(strlen(buf)+1);
    int i = 0;

    for(i = 0; i < strlen(buf); i++){
        if(buf[i] != '\n' && buf[i] != '\r')
            no_spaces_buf[i] = buf[i];
        else
            no_spaces_buf[i] = ' ';

    }

    no_spaces_buf[strlen(buf)] = '\0';

    return no_spaces_buf;
}