//
//  main.c
//  lab03
//  Objective: write a program that can concatenate and print files.
//
//  Created by Anthony Brignano on 9/3/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//
// Test cases below:
// $ mycat file1.txt
// $ mycat file1.txt file2.txt file3.txt
// $ mycat -n file1.txt file2.txt
// $ mycat -es file1.txt file2.txt
// $ mycat -ens file1.txt file2.txt file3.txt

#include <stdio.h>
#include <string.h>
#import <fcntl.h>
#include <unistd.h>
#import <stdlib.h>

#define BUFFERSIZE 512      // Was 4096, changed to 512 as per Prof. Kurkovsky

void read_file(char*);
void read_file_ens(char*, char*);

int main(int ac,  char**av) {
    if(ac < 2){                                 // Checks if user entered 1 or less arguments (i.e. /$ mycat)
        printf("Too few arguments: $ mycat [-ens] file1 [file2 ...]\n");
        //exit(1);
        return 1;
    }
    else if(strncmp(av[1],"-", 1) == 0){        // Checks if av[1] contains
        if(ac == 2){
            printf("Too few arguments: $ mycat [-ens] file1 [file2 ...]\n");
            //exit(1);
            return 1;
        }
        else if(ac > 2){        // WHEN -[ens] IS USED
            int i = 0;
            for(i = 2; i < ac; i++){
                read_file_ens(av[1], av[i]);
            }
            printf("\n");
        }

    }

    else{
        int i = 0;
        for(i = 1; i < ac; i++){
            read_file(av[i]);
        }
    }
    return 0;
}

int fc = 0;
void read_file_ens(char *ens, char *fp){
    int lc = 1;

    if((strchr(ens,'n'))!=NULL) {                // If lines are to be numbered start with 1.
        if(fc != 0){
            printf("\n");
        }
        printf("\t%d\t", lc);
        lc += 1;
        fc++;
    }

    int fd = 0, n_chars = 0;
    char buf[BUFFERSIZE];

    if((fd=open(fp, O_RDONLY)) == -1) {                     // Open file as read only
        fprintf(stderr, "Error: Cannot open ");
        perror(fp);
        exit(1);
    }

    while((n_chars = (unsigned)read(fd, buf, BUFFERSIZE)) > 0){       // Read file into buffer
        int i = 0;
        for(i = 0; i < strlen(buf); i++){                   // Loop through 1D character array buffer one at a time

            if(buf[i+1] == '\n'){

                if((strchr(ens,'s'))!=NULL) {
                    while(buf[i+2] == '\n' || buf[i+2] == '\v' || buf[i+2] == '\t' || buf[i+2] == '\r'
                            || buf[i+2] == '\f' || buf[i+2] == ' ' ){
                        i++;
                        if(buf[i+1] == '\n'){
                            break;
                        }
                    }
                }
            }
            else if(buf[i] == '\n'){                        // Check if the character is a new line

                if((strchr(ens,'e'))!=NULL) {
                    printf("$");
                }
                /*if((strchr(ens,'s'))!=NULL) {
                    while(buf[i] == '\n' || buf[i] == '\v' || buf[i] == '\t' || buf[i] == '\r'
                            || buf[i] == '\f' || buf[i] == ' '){
                        i++;
                    }
                }*/

                printf("%c", buf[i]);


                if((strchr(ens,'n'))!=NULL){
                    printf("\t%d\t", lc);
                    lc++;
                }

            }
            else{
                printf("%c",buf[i]);
            }

        }
    }
    /*if((strchr(ens,'e'))!=NULL) {           // If lines are to end with $ place the last $ before closing
        printf("$");
        fc++;
    }*/
    if((strchr(ens,'s'))!=NULL) {
        printf("\n");
        fc++;
    }
    if(n_chars == -1){
        fprintf(stderr, "Error: Read error from ");
        perror(fp);
        exit(1);
    }
    if(close(fd == -1)){
        fprintf(stderr, "Error: Error closing file ");
        perror(fp);
        exit(1);
    }
}
void read_file(char *fp){

    int fd = 0, n_chars = 0;
    char buf[BUFFERSIZE];

    if((fd=open(fp, O_RDONLY)) == -1) {
        fprintf(stderr, "Error: Cannot open ");
        perror(fp);
        exit(1);
    }
    while((n_chars = read(fd, buf, BUFFERSIZE)) > 0){
        if(write(1, buf, n_chars) != n_chars){
            fprintf(stderr, "Error: Write error to ");
            perror(fp);
            exit(1);
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
}
