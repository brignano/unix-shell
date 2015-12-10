/** the snake **/

struct snake_struct {
    int	y_pos, x_pos;
    char symbol;
    struct snake_struct * next;
} ;