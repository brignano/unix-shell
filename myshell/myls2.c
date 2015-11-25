//
//  main.c
//  lab05
//  Objective: write a program that can print the names of files in the current directory in multiple columns.
//
//  Created by Anthony Brignano on 10/8/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/ioctl.h>
#import <math.h>

void do_ls(char []);
int dir_len(char[]);
int longest_str_len(char dirname[]);

int main(int ac, char *av[])
{
    if (ac == 1)
        do_ls( "." );
    else
        while (--ac){
            printf("%s:\n", *++av);
            do_ls(*av);
        }
    return 1;
}

int dir_len(char dirname[]){
    DIR           *dir_ptr;      /* the directory */
    struct dirent *direntp;      /* each entry    */

    int fc = 0;
    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr,"Error: cannot open %s\n", dirname);
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL) {
            if(strncmp(direntp->d_name,".", 1)) {
                fc++;
            }
        }
        closedir(dir_ptr);
    }
    return fc;
}
int longest_str_len(char dirname[]){
    DIR           *dir_ptr;      /* the directory */
    struct dirent *direntp;      /* each entry    */

    int longest_str_index = 0;
    char **str = (char**)malloc(dir_len(dirname)*sizeof(char*));
    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr,"Error: cannot open %s\n", dirname);
    else
    {
        int i = 0;
        while ((direntp = readdir(dir_ptr)) != NULL) {
            if(strncmp(direntp->d_name,".", 1)) {
                str[i] = (char*)malloc(direntp->d_namlen*sizeof(char));
                str[i] = direntp->d_name;
                if(direntp->d_namlen > strlen(str[longest_str_index])){
                    longest_str_index = i;
                }
                i++;
            }
        }
        closedir(dir_ptr);
    }
    return (int) strlen(str[longest_str_index]);
}

void print_dir(char **str, int n_cols, int n_rows, int d_len, int longest_str_len){
    int x = 0;
    for(x = 0; x < n_rows; x++) {

        int y = 0;

        for(y = x; y < d_len; y += n_rows){

            printf("%s", str[y]);

            int str_len = 0;

            for(str_len = strlen(str[y]); str_len <= (longest_str_len + 2); str_len++){
                fflush(stdout);
                printf(" ");
            }

            printf("\t");
        }
        printf("\n");
    }
}

void do_ls(char dirname[])
{
    DIR           *dir_ptr;      /* the directory */
    struct dirent *direntp;      /* each entry    */

    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr,"Error: cannot open %s\n", dirname);
    else
    {
        // printf("\n**TEST** dir_len: %d\n\n", dir_len(dirname));

        char **str = (char**)malloc((unsigned)dir_len(dirname)*sizeof(char*));
        int i = 0;
        int longestStringLength = longest_str_len(dirname);
        struct winsize w;
        int t_width = 0;
        if(ioctl(0, TIOCGWINSZ, &w) != -1){
            t_width = w.ws_col;
        }
//        int n_cols = (int) floor(t_width / (longestStringLength));
        int n_cols = (int) floor(t_width / (longestStringLength+3));
//        int n_cols = (int) floor(t_width / (longestStringLength+6));  // +2 for extra spaces & +4 for tabs ??
        int n_rows = (int) floor(dir_len(dirname) / n_cols) + 1;    // +1 ??


        while ((direntp = readdir(dir_ptr)) != NULL) {
            if(strncmp(direntp->d_name,".", 1)) {

                str[i] = (char*) malloc(direntp->d_namlen * sizeof(char));
                str[i] = direntp->d_name;

                /* TEST CASES BELOW
                *
                * int str_len = direntp->d_namlen;
                * printf("%d: %s", i, str[i]);
                * printf("%d: %s (%d)", i, str[i], str_len);     // TEST CASE
                * printf("**TEST** t_width: %d\n",t_width);
                * printf("**TEST** n_rows: %d\n",n_rows);
                * printf("**TEST** n_cols: %d\n",n_cols);
                */

                i++;
            }
        }
        //printf("\n**TEST** lartgest_str_len :: %d\n\n", longest_str_len(dirname));
        print_dir(str, n_cols, n_rows, dir_len(dirname), longestStringLength);

        // Possibly free the array here?

        closedir(dir_ptr);
    }
}