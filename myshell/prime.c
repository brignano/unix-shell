//
//  main.c
//  lab08
//  Objective: write a program that provides a custom handler for the interrupt signal.
//
//  Created by Anthony Brignano on 11/10/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <sys/signal.h>

int is_prime(int);
int get_response();
void intHandler(int);

static volatile int keepRunning = 0;


int main(int ac, const char * av[]) {

    if(ac != 1){
        printf("usage: ./prime");
    }
    else {

        int i = 1, largest_prime = 1, flag = 0;

        while (!flag) {
            signal(SIGINT, intHandler);         /* handle INT		*/
            signal(SIGQUIT, SIG_IGN);           /* ignore QUIT signals	*/

            if (is_prime(i)) {
                largest_prime = i;
            }

            i += 2;

            if (keepRunning) {
                printf("\n%d\t", largest_prime);
                flag = get_response();       /* get some answer	*/
                keepRunning = flag;

            }
        }
    }
    return 0;
}

int is_prime(int n){
    if (n % 2 == 0) {
        return 0;                       // Return FALSE
    }
    int p = 3;
    while (p < sqrt(n)+1){
        if(n % p == 0) {
            return 0;                   // Return FALSE
        }
        p += 2;
    }
    return 1;                           // Return TRUE
}

int get_response() {
/*
 * purpose: ask a question and wait for a y/n answer or timeout
 *  method: use getchar and complain about non-y/n input
 * returns: 1=>yes, 0=>no
 */
    char input;

    printf("Quit [y/n]? ");			                          /* ask		*/
    fflush(stdout);					                          /* force output	*/
    while ( 1 ){
        input = (char)tolower(getchar());                     /* get next chr */
        if ( input == 'y' )
            return 1;
        else
            return 0;
    }
}

void intHandler(int signum){
    keepRunning = 1;
}