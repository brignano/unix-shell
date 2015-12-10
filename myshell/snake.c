//
//  main.c
//  lab10
//  Objective: implement a classic snake game.
//
//  Created by Anthony Brignano on 11/19/15.
//  Copyright (c) 2015 Anthony Brignano. All rights reserved.
//

#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#import "the_snake.c"
#import "trophy.c"

char* horizontal_border();
void draw_border();
void setup();
void move_snake();
void change_direction(char);
char set_symbol();
void generate_trophy();
void check_position();
void alert(int);
void grow_snake();
void draw_trophy();
void new_trophy(int);
void set_speed();

struct snake_struct snake;
struct snake_struct * root;
struct trophy_struct trophy;
useconds_t speed = 100000;
int max_x = 0, max_y = 0;
int x_dir = 0, y_dir = 0;
const char* winning_string = "Congrats, you won!";
const char* outOfBounds_string = "Out of bounds!";
const char* ateYourself_string = "Oops, you ate yourself!";
const char* userQuit_string = "You quit! Thanks for playing.";
const char DEFAULT_SYMBOL = '@';
char ui;


int main(int argc, const char * argv[]) {

    root = (struct snake_struct *) malloc(sizeof(struct snake_struct));
    root = &snake;

    srand((unsigned) time(NULL));
    WINDOW *w = initscr();                                  // Turn on curses
    cbreak();
    nodelay(w, TRUE);                                       // Disable getchar() delay
    curs_set(0);                                            // Turn cursor off
    getmaxyx(stdscr, max_y, max_x);                         // Get terminal dimensions
    signal(SIGALRM, new_trophy);                            // Sets an alarm to generate a new trophy in trophy.timer seconds
    setup();                                                // Setup terminal window (border, random starting position)
    generate_trophy();                                      // Generate first trophy
    refresh();
    usleep(speed);
    while((ui = (char) tolower(getch())) != 'q'){           // Get user input (ui) from stdin character stream
        fflush(0);
        move_snake();                                       // Move snake as necessary
    }
    alert(3);                                               // Alert user that they have quit the program (via ui == 'q')
    endwin();
    return 0;
}

// Moves the snake based on user input, via key press
void move_snake(){
    struct snake_struct *temp_ptr = root;
    struct snake_struct *last_node = malloc(sizeof(struct snake_struct));
    change_direction(ui);                                   // Change direction on UI
    draw_border();                                          // Clear screen & draw border
    draw_trophy();                                          // Keep trophy on screen

    while(temp_ptr->next){
        mvaddch(temp_ptr->y_pos, temp_ptr->x_pos, DEFAULT_SYMBOL);
        last_node = temp_ptr;
        temp_ptr = temp_ptr->next;
    }

    temp_ptr->x_pos = root->x_pos + x_dir;
    temp_ptr->y_pos = root->y_pos + y_dir;
    if(root->next !=  NULL)
        temp_ptr->next = root;
    last_node->next = NULL;
    root = temp_ptr;


    check_position();

    mvaddch(root->y_pos, root->x_pos, set_symbol());
    refresh();
    usleep(speed);
}

// Grows the snake based on trophy value that was ate
void grow_snake(){
    struct snake_struct *temp_ptr = root;
    int num_links = trophy.value;
    generate_trophy();
    int i = 0;
    draw_border();                                          // Clear screen & draw border
    draw_trophy();                                          // Keep trophy on screen

    for(i = 0; i < num_links; i++) {
        ui = (char) tolower(getch());
        fflush(0);
        change_direction(ui);
        temp_ptr = root;

        while(temp_ptr->next){
            mvaddch(temp_ptr->y_pos, temp_ptr->x_pos, DEFAULT_SYMBOL);
            temp_ptr = temp_ptr->next;
        }

        struct snake_struct *new_head = malloc(sizeof(struct snake_struct));
        new_head->x_pos = root->x_pos + x_dir;
        new_head->y_pos = root->y_pos + y_dir;
        new_head->next = root;
        new_head->symbol = DEFAULT_SYMBOL;
        root = new_head;
        set_symbol();
        set_speed();
        mvaddch(root->y_pos, root->x_pos, root->symbol);
        refresh();
        usleep(speed);
    }
    move_snake();
}

// Sets speed for the snake based on its length
void set_speed(){
    if(speed > 50000)
        speed = (unsigned)(speed - (500*(sizeof(root)-1)));
}

// Checks position of snake and trophy
void check_position(){
    int terminal_perimeter = (2 * max_x + 2 * max_y);
//    if(root->next){
        struct snake_struct *temp_ptr = root;
        while(temp_ptr->next){
            temp_ptr = temp_ptr->next;
            if (root->x_pos == temp_ptr->x_pos && root->y_pos == temp_ptr->y_pos){
                alert(2);
            }

        }
//    }
    if(root->x_pos == -1 || root->x_pos == max_x
            || root->y_pos == -1 || root->y_pos == max_y){
        alert(1);
    }
    //  Method below for if Stan complains about the snake running through borders
    /* if(root->x_pos == 0 || root->x_pos == max_x-1
            || root->y_pos == 0 || root->y_pos == max_y-1){
        alert(1);
    }*/
    else if(root->x_pos == trophy.x_pos && root->y_pos == trophy.y_pos){
        grow_snake();
    }
    else if(sizeof(root) >= terminal_perimeter){   // check snake position with terminal window proportions
        alert(0);
    }
}

// Changes direction on user key input
void change_direction(char key_press){
    if ((char) tolower(key_press) == 'w'){
        if(y_dir == 1 && root->next){
            alert(2);
        }
        else{
            y_dir = -1;
            x_dir = 0;
        }
    }
    else if ((char) tolower(key_press) == 'a'){
        if(x_dir == 1 && root->next){
            alert(2);
        }
        else{
            y_dir = 0;
            x_dir = -1;
        }
    }
    else if ((char) tolower(key_press) == 's'){
        if(y_dir == -1 && root->next){
            alert(2);
        }
        else{
            y_dir = 1;
            x_dir = 0;
        }
    }
    else if ((char) tolower(key_press) == 'd'){
        if(x_dir == -1 && root->next){
            alert(2);
        }
        else{
            y_dir = 0;
            x_dir = 1;
        }
    }
    else if ((char) tolower(key_press) == 'q'){
        alert(3);
    }
}

// Sets up the terminal window for snake game
void setup(){
    draw_border();

    root->x_pos = rand() % (max_x-2) + 1;
    root->y_pos = rand() % (max_y-2) + 1;

    x_dir =  rand() / RAND_MAX ;
    y_dir = 0;
    if(x_dir == 0){
        y_dir = 1;
    }

    root->symbol = set_symbol();
    root->next = NULL;

    mvaddch(root->y_pos, root->x_pos, root->symbol);

}

// Generates a trophy randomly
void generate_trophy(){
    trophy.value = rand() % (9 - 1 + 1) + 1;
    trophy.timer = rand() % (9 - 1 + 1) + 1;

    trophy.x_pos = rand() % (max_x-2) + 1;
    trophy.y_pos = rand() % (max_y-2) + 1;

    trophy.symbol = (char) (trophy.value+48);
    mvaddch(trophy.y_pos, trophy.x_pos, trophy.symbol);
    alarm((unsigned)trophy.timer);
}

// Sets symbol for snake head depending on its direction, if error '~' is used
char set_symbol(){
    if(x_dir == -1 && y_dir == 0){
        return '<';
    }
    else if(x_dir == 1 && y_dir == 0){
        return '>';
    }
    else if(y_dir == -1 && x_dir == 0){
        return '^';
    }
    else if(y_dir == 1 && x_dir == 0){
        return 'v';
    }
    else{
        return '~';
    }
}

// Redraws the trophy onto the screen
void draw_trophy(){
    mvaddch(trophy.y_pos, trophy.x_pos, trophy.symbol);
}

// Draws a border around the terminal window
void draw_border(){
    int i = 0;
    clear();

    move(0,1);
    addstr(horizontal_border());

    mvaddch(0, 0, '-');
    mvaddch(0, max_x - 1, '-');
    for(i = 1; i < max_y-1; i++) {
            mvaddch(i, 0, '|');
            mvaddch(i, max_x - 1, '|');
    }
    mvaddch(max_y-1, 0, '-');
    mvaddch(max_y-1, max_x - 1, '-');
    move(max_y-1, 1);
    addstr(horizontal_border());

}

// Returns a string of '-' characters to fill the horizontal borders
char* horizontal_border(){
    char* horizontal_border = malloc((sizeof(char)*max_x-2) + 1) ;
    int i = 0;

    for(i = 0; i < max_x-2; i++)
        if(i%2)
            horizontal_border[i] = '-';
        else
            horizontal_border[i] = ' ';

    return horizontal_border;
}

// Alerts user if they won or lost & exits program
void alert(int flag){
    clear();
    if(flag == 0){
        move((max_y / 2), (max_x / 2) - ((int)sizeof(winning_string) / 2));
        addstr(winning_string);
    }
    else if(flag == 1){
        move((max_y / 2), (max_x / 2) - ((int)sizeof(outOfBounds_string) / 2));
        addstr(outOfBounds_string);
    }
    else if(flag == 2){
        move((max_y / 2), (max_x / 2) - ((int)sizeof(ateYourself_string) / 2));
        addstr(ateYourself_string);
    }
    else if(flag == 3){
        move((max_y / 2), (max_x / 2) - ((int)sizeof(userQuit_string) / 2));
        addstr(userQuit_string);
    }
    refresh();
    sleep(2);
    clear();
    refresh();
    exit(1);
}

// Called when trophy.timer expires, generates a new trophy
void new_trophy(int sig){
    generate_trophy();
}
