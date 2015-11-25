//
//  main.c
//  lab01
//  Objective: write a program that can accept command line parameters and produce output to the terminal.
//
//  Created by Anthony Brignano on 8/31/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//

#include <stdio.h>
#include <string.h>

int main(int ac, const char*av[]){
	int x = 0, i = 0;
	for(x = 1; x < ac; x++){
		for(i = 1; i <= strlen(av[x]); i++){      // preforms the same when (i = 0) and (i = 1)?
			printf("%*.*s\n", 1, i, av[x]);
		}
	}
    return 0;
}