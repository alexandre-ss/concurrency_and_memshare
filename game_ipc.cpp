#include "game.hpp"
#include <signal.h>
#include <sys/wait.h>

int main()
{
  pid_t process_1, process_2;
  process_1 = fork();
  process_2 = fork();

  setup_board();

  srand(time(NULL));
  struct timespec ts;
  ts.tv_sec = 0.7;
  ts.tv_nsec = (700 % 1000) * 1000000;

  int status;
  while (!(player_1.blocked && player_2.blocked))
  {

    if (process_1 == 0)
    {
      cout << "player 1 playing!" << endl;
      
    }

    if (process_2 == 0)
    {
      cout << "player 2 playing!" << endl;
    }
    for (int i = 0; i < 2; i++)
      wait(&status);

    nanosleep(&ts, NULL);
    print_board();
    system("clear");
  }

  if (player_1.blocked && player_2.blocked)
  {
    counter();
    const char *winner = board.red > board.blue ? "red" : "blue";
    cout << "the winner is " << winner << "!" << endl;
    print_board();
  }

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

    while (signal_att)
      ;
    if (board.game_board[player_1.x][player_1.y] == 'x')
    {
      signal_att = 1;
      add_piece_player_1();
      signal_att = 0;
    }
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
  return 0;
}

void *player_two_play(void *)
{

  if (!is_blocked(player_2.x, player_2.y, 'b'))
  {
    random_choice_p2();

    while (signal_att)
      ;
    if (board.game_board[player_2.x][player_2.y] == 'x')
    {
      signal_att = 1;
      add_piece_player_2();
      signal_att = 0;
    }
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
  return 0;
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

      if (board.game_board[i][j] == 'r')
      {
        printf("\x1b[31m"
               "* "
               "\x1b[0m");
      }
      else if (board.game_board[i][j] == 'b')
      {
        printf("\x1b[34m"
               "* "
               "\x1b[0m");
      }
      else
      {
        printf("x ");
      }

      // cout << char(board.game_board[i][j]) << ' ';
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