//
//  main.c
//  lab04
//  Objective: write a program that can print the names of files in the current directory.
//
//  Created by Anthony Brignano on 10/5/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//

#include <stdio.h>
#include <dirent.h>
#import <string.h>
#include <unistd.h>

void do_ls(char []);
void do_ls_hidden(char []);

int main(int ac, char *av[])
{
    char buf[BUFSIZ];
    char *cp = NULL;
    cp = getcwd(buf, sizeof(buf));
    if (ac == 1) {
        do_ls(buf);
    }
    else if(strncmp(av[1],"-", 1) == 0){
        if(ac == 2) {
            if (strchr(av[1], 'a') != NULL) {
                do_ls_hidden(buf);
                //printf("Contains -a\n");
            }
            else {
                printf("Error: $ myls1 [-a] [directory]\n");
            }
        }
        else{
            int i = 0;
            for(i = 2; i < ac; i++){
                printf("%s:\n", av[i]);
                do_ls_hidden(av[i]);
            }
        }
    }
    else{
        while (--ac >= 1){
            printf("%s:\n", *++av);
            do_ls(*av);
        }
    }
    return 1;
}

void do_ls_hidden(char dirname[])
{
    DIR           *dir_ptr;      /* the directory */
    struct dirent *direntp;      /* each entry    */

    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr,"ls1: cannot open %s\n", dirname);
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL) {
            printf("%s\n", direntp->d_name);
        }
        closedir(dir_ptr);
    }
}

void do_ls(char dirname[])
{
    DIR           *dir_ptr;      /* the directory */
    struct dirent *direntp;      /* each entry    */

    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr,"ls1: cannot open %s\n", dirname);
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL)
            if(strncmp(direntp->d_name,".", 1)) {
                printf("%s\n", direntp->d_name);
            }
        closedir(dir_ptr);
    }
}