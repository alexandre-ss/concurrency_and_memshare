#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stack>

using namespace std;
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define HEIGHT 8
#define WIDTH 8

void *player_one_play(void *);
void *player_two_play(void *);
void counter();
int is_blocked(int, int, char);
void setup_board();
void add_piece_player_1();
void add_piece_player_2();
void print_board();
void random_choice_p1();
void random_choice_p2();

class Player
{
public:
    int x, y, blocked;
    stack<int> history_x;
    stack<int> history_y;
};

class Board
{
public:
    int red, blue;
    char game_board[HEIGHT][WIDTH];
};

Player player_1;
Player player_2;
Board board;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;