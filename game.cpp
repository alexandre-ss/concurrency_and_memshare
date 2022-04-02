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

int main()
{
    pthread_t thread1, thread2;
    srand(time(NULL));
    int iret1, iret2;
    struct timespec ts;
    ts.tv_sec = 0.2;
    ts.tv_nsec = (200 % 1000) * 1000000;

    setup_board();

    while (!player_1.blocked || !player_2.blocked)
    {

        iret1 = pthread_create(&thread1, NULL, player_one_play, NULL);
        iret2 = pthread_create(&thread2, NULL, player_two_play, NULL);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        print_board();
        nanosleep(&ts, NULL);
        system("clear");
    }
    counter();
    const char *winner = board.red > board.blue ? "red" : "blue";
    cout << "the winner is " << winner << "!" << endl;
    print_board();

    exit(0);

    return 0;
}

void setup_board()
{

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            board.game_board[i][j] = 'x';
        }
    }

    board.game_board[0][WIDTH / 2] = 'r';
    board.game_board[HEIGHT - 1][WIDTH / 2] = 'b';
    board.red = 0;
    board.blue = 0;

    player_1.x = 0;
    player_1.y = WIDTH / 2;
    player_1.blocked = 0;
    player_1.history_x.push(player_1.x);
    player_1.history_y.push(player_1.y);

    player_2.x = HEIGHT - 1;
    player_2.y = WIDTH / 2;
    player_2.blocked = 0;
    player_2.history_x.push(player_2.x);
    player_2.history_y.push(player_2.y);
}

void *player_one_play(void *)
{
    if (!is_blocked(player_1.x, player_1.y, 'r'))
    {
        random_choice_p1();
        add_piece_player_1();
    }
    else
    {
        while (is_blocked(player_1.x, player_1.y, 'r') && !player_1.blocked)
        {

            player_1.history_x.pop();
            player_1.history_y.pop();
            if (!player_1.history_x.empty())
            {
                player_1.x = player_1.history_x.top();
                player_1.y = player_1.history_y.top();
            }
            else
            {
                player_1.blocked = 1;
            }
        }
    }
    pthread_exit(NULL);
}

void *player_two_play(void *)
{
    if (!is_blocked(player_2.x, player_2.y, 'b'))
    {
        random_choice_p2();
        add_piece_player_2();
    }
    else
    {
        while (is_blocked(player_2.x, player_2.y, 'b') && !player_2.blocked)
        {
            player_2.history_x.pop();
            player_2.history_y.pop();
            if (!player_2.history_x.empty())
            {

                player_2.x = player_2.history_x.top();
                player_2.y = player_2.history_y.top();
            }
            else
            {
                player_2.blocked = 1;
            }
        }
    }
    pthread_exit(NULL);
}

void counter()
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            switch (board.game_board[i][j])
            {
            case 'r':
                board.red += 1;
                break;
            case 'b':
                board.blue += 1;
                break;
            default:
                break;
            }
}

int is_blocked(int pos_x, int pos_y, char value)
{
    if (board.game_board[pos_x][pos_y] == value)
    {
        if (pos_x == 0)
        {
            if (pos_y == 0)
            {
                if (!(board.game_board[pos_x + 1][pos_y] == 'x' ||
                      board.game_board[pos_x][pos_y + 1] == 'x'))
                    return 1;
            }
            else if (pos_y == WIDTH - 1)
            {
                if (!(board.game_board[pos_x + 1][pos_y] == 'x' ||
                      board.game_board[pos_x][pos_y - 1] == 'x'))
                    return 1;
            }
            else
            {
                if (!(board.game_board[pos_x + 1][pos_y] == 'x' ||
                      board.game_board[pos_x][pos_y - 1] == 'x' ||
                      board.game_board[pos_x][pos_y + 1] == 'x'))
                    return 1;
            }
        }

        else if (pos_x == HEIGHT - 1)
        {
            if (pos_y == 0)
            {
                if (!(board.game_board[pos_x - 1][pos_y] == 'x' ||
                      board.game_board[pos_x][pos_y + 1] == 'x'))
                    return 1;
            }
            else if (pos_y == WIDTH - 1)
            {
                if (!(board.game_board[pos_x - 1][pos_y] == 'x' ||
                      board.game_board[pos_x][pos_y - 1] == 'x'))
                    return 1;
            }
            else
            {
                if (!(board.game_board[pos_x - 1][pos_y] == 'x' ||
                      board.game_board[pos_x][pos_y - 1] == 'x' ||
                      board.game_board[pos_x][pos_y + 1] == 'x'))
                    return 1;
            }
        }

        else if (pos_y == 0)
        {
            if (!(board.game_board[pos_x - 1][pos_y] == 'x' ||
                  board.game_board[pos_x + 1][pos_y] == 'x' ||
                  board.game_board[pos_x][pos_y + 1] == 'x'))
                return 1;
        }
        else if (pos_y == WIDTH - 1)
        {
            if (!(board.game_board[pos_x - 1][pos_y] == 'x' ||
                  board.game_board[pos_x + 1][pos_y] == 'x' ||
                  board.game_board[pos_x][pos_y - 1] == 'x'))
                return 1;
        }

        else if (!(board.game_board[pos_x - 1][pos_y] == 'x' ||
              board.game_board[pos_x + 1][pos_y] == 'x' ||
              board.game_board[pos_x][pos_y - 1] == 'x' ||
              board.game_board[pos_x][pos_y + 1] == 'x'))
            return 1;
    }
    return 0;
}

void add_piece_player_1()
{
    board.game_board[player_1.x][player_1.y] = 'r';
    player_1.history_x.push(player_1.x);
    player_1.history_y.push(player_1.y);
}

void add_piece_player_2()
{
    board.game_board[player_2.x][player_2.y] = 'b';
    player_2.history_x.push(player_2.x);
    player_2.history_y.push(player_2.y);
}

void print_board()
{
    cout << "***************" << endl;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            cout << char(board.game_board[i][j]) << ' ';
        }
        cout << endl;
    }

    cout << "***************" << endl;
}

void random_choice_p1()
{

    int rand_pos_j1 = 1 + rand() % 4;
    switch (rand_pos_j1)
    {
    case UP:
        if (player_1.x > 0 && board.game_board[player_1.x - 1][player_1.y] == 'x')
        {
            player_1.x -= 1;
            break;
        }
    case DOWN:
        if (player_1.x < HEIGHT - 1 && board.game_board[player_1.x + 1][player_1.y] == 'x')
        {
            player_1.x += 1;
            break;
        }
    case LEFT:
        if (player_1.y > 0 && board.game_board[player_1.x][player_1.y - 1] == 'x')
        {
            player_1.y -= 1;
            break;
        }
    case RIGHT:
        if (player_1.y < WIDTH - 1 && board.game_board[player_1.x][player_1.y + 1] == 'x')
        {
            player_1.y += 1;
            break;
        }

    default:
        break;
    }
}

void random_choice_p2()
{
    int rand_pos_j2 = 1 + rand() % 4;
    switch (rand_pos_j2)
    {
    case UP:
        if (player_2.x > 0 && board.game_board[player_2.x - 1][player_2.y] == 'x')
        {
            player_2.x -= 1;
            break;
        }

    case DOWN:
        if (player_2.x < HEIGHT - 1 && board.game_board[player_2.x + 1][player_2.y] == 'x')
        {
            player_2.x += 1;
            break;
        }

    case LEFT:
        if (player_2.y > 0 && board.game_board[player_2.x][player_2.y - 1] == 'x')
        {
            player_2.y -= 1;
            break;
        }

    case RIGHT:
        if (player_2.y < WIDTH - 1 && board.game_board[player_2.x][player_2.y + 1] == 'x')
        {
            player_2.y += 1;
            break;
        }
    default:
        break;
    }
}