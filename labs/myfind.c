//
//  main.c
//  lab06
//  Objective: write a program that can traverse a tree of subdirectories and search for a file with a given name.
//
//  Created by Anthony Brignano on 10/21/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#import <unistd.h>
#include <sys/stat.h>

void search_dir(char dirname[], char * sub_string);
void mode_to_letters(int mode, char str[]);

int main(int ac, const char *av[]) {
    char buf[BUFSIZ];
    char *cwd = getcwd(buf, sizeof(buf));
    char * substr = (char *) av[1];

    if(ac == 2){
        // printf("%s",buf);
        search_dir(cwd, substr);
    }
    else if(ac != 3){
        printf("Error: ./myfind filename [startpath]\n");
    }
    else{
        search_dir((char*)av[2], substr);
    }

    return 0;
}

void search_dir(char dirname[], char * sub_string)
{
    DIR           *dir_ptr = NULL;      /* the directory */
    struct dirent *direntp = NULL;      /* each entry    */
    struct stat statbuf;
    int fc = 0;
    char directory_name[150] = "";


    if ((dir_ptr = opendir(dirname)) == NULL)                       // can't open directory
        fprintf(stderr,"Error: cannot open %s\n", dirname);         // alert user dir_open failed
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL){               // while there's entries left
            strcpy(directory_name, dirname);                        // concatenate ->
            strcat(directory_name, "/");                            // <- directory and entry ->
            strcat(directory_name, direntp->d_name);                // <- into full file path.

            if (stat(directory_name, &statbuf) == -1) {             // cannot stat
                perror(direntp->d_name);                            // say why
                break;
            }
            if(strncmp(direntp->d_name, ".", 1)){
                if (strstr(direntp->d_name, sub_string)) {
                    char modestr[11] = "";
                    if (!fc) {
                        printf("%s\n", dirname);

                    }
                    if (S_ISDIR(statbuf.st_mode)) {                      // is directory
                        //printf("SERACHING: %s\n", directory_name);     // test condition
                        search_dir(directory_name, sub_string);          // recursive self invocation
                    }
                    else{
                    mode_to_letters(statbuf.st_mode, modestr);
                    printf("\t%s (0%o/%s)", direntp->d_name, (statbuf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)), modestr);
                    printf("\n");  }
                    fc++;

                }
                else if(S_ISDIR(statbuf.st_mode)){
                    search_dir(directory_name, sub_string);
                    fc++;
                }

            }
        }
        closedir(dir_ptr);
    }
}

void mode_to_letters(int mode, char str[]){
    strcpy(str, "---------");

    if(S_ISDIR(mode)) str[0] = 'd';
    if(S_ISCHR(mode)) str[0] = 'c';
    if(S_ISBLK(mode)) str[0] = 'b';

    if(mode & S_IRUSR) str[1] = 'r';     // 3 bits for user
    if(mode & S_IWUSR) str[2] = 'w';
    if(mode & S_IXUSR) str[3] = 'x';

    if(mode & S_IRGRP) str[1] = 'r';     // 3 bits for group
    if(mode & S_IWGRP) str[2] = 'w';
    if(mode & S_IXGRP) str[3] = 'x';

    if(mode & S_IROTH) str[1] = 'r';     // 3 bits for other
    if(mode & S_IWOTH) str[2] = 'w';
    if(mode & S_IXOTH) str[3] = 'x';

}

/* Has issues with formatting when directories are before files
 *
 * void search_dir(char dirname[], char * sub_string)
{
    DIR           *dir_ptr = NULL;      *//* the directory *//*
    struct dirent *direntp = NULL;      *//* each entry    *//*
    struct stat statbuf;
    int fc = 0;
    char directory_name[150] = "";


    if ((dir_ptr = opendir(dirname)) == NULL)                       // can't open directory
        fprintf(stderr,"Error: cannot open %s\n", dirname);         // alert user dir_open failed
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL){               // while there's entries left
            strcpy(directory_name, dirname);                        // concatenate ->
            strcat(directory_name, "/");                            // <- directory and entry ->
            strcat(directory_name, direntp->d_name);                // <- into full file path.

            if (stat(directory_name, &statbuf) == -1) {             // cannot stat
                perror(direntp->d_name);                            // say why
                break;
            }
            if(strncmp(direntp->d_name, ".", 1)){
                if (S_ISDIR(statbuf.st_mode)) {                      // is directory
                    //printf("SERACHING: %s\n", directory_name);     // test condition
                    search_dir(directory_name, sub_string);          // recursive self invocation
                }
                else if (strstr(direntp->d_name, sub_string)) {
                    char modestr[11] = "";
                    if (!fc) {
                        printf("%s\n", dirname);

                    }
                    mode_to_letters(statbuf.st_mode, modestr);
                    // questionable octal conversion below
                    printf("\t%s (0%o/%s)", direntp->d_name, (statbuf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)), modestr);
                    printf("\n");
                    fc++;
                }

            }
        }
        closedir(dir_ptr);
    }
}*/