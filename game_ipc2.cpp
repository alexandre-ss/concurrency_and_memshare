#include "game.hpp"
#include <semaphore.h>
#include <signal.h>
#include <wait.h>

void setup_board_ipc(Board *);
void counter_ipc(Board *);
void random_choice_p1_ipc(Board *);
void random_choice_p2_ipc(Board *);
void print_board_ipc(Board *);
void add_piece(pid_t, Board *);
int is_blocked_ipc(int, int, char, Board *);
void start_process(pid_t process_1, Board *board_aux);

struct timespec ts;

sem_t smutex;

int main()
{
  pid_t process;
  sem_init(&smutex, 0, 1);

  srand(time(NULL));

  key_t key = ftok("shmfile", 65); // gerando uma chave única

  int shm_id = shmget(key, sizeof(Board), 0666 | IPC_CREAT); // identificador para a memória compartilhada

  Board *board_aux = (Board *)shmat(shm_id, NULL, 0);

  setup_board_ipc(board_aux);

  process = fork();
  start_process(process, board_aux);
  return 0;
}

void start_process(pid_t process_1, Board *board_aux)
{
  ts.tv_sec = 0.5;
  ts.tv_nsec = (500 % 1000) * 1000000;
  while (!(board_aux->blocked1 && board_aux->blocked2))
  {
    if (process_1 > 0)
    {
      play(process_1, board_aux);
      board_aux->blocked1 = player_1.blocked ? 1 : 0;
    }
    else if (process_1 == 0)
    {
      play(process_1, board_aux);
      board_aux->blocked2 = player_2.blocked ? 1 : 0;
    }
    nanosleep(&ts, NULL);
  }
  print_board_ipc(board_aux);
  sem_destroy(&smutex);
  shmdt(board_aux);
  exit(0);
}

void play(pid_t id, Board *board_game)
{
  if (id > 0)
  {
    cout << "player 1 playing!" << endl;
    if (!is_blocked_ipc(player_1.x, player_1.y, 'r', board_game))
    {
      random_choice_p1_ipc(board_game);
    }
    else
    {
      while (is_blocked_ipc(player_1.x, player_1.y, 'r', board_game) && !player_1.blocked)
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
  }
  else if (id == 0)
  {
    cout << "player 2 playing!" << endl;

    if (!is_blocked_ipc(player_2.x, player_2.y, 'b', board_game))
    {
      random_choice_p2_ipc(board_game);
    }
    else
    {
      while (is_blocked_ipc(player_2.x, player_2.y, 'b', board_game) && !player_2.blocked)
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
  }
  sem_wait(&smutex);
  add_piece(id, board_game);
  sem_post(&smutex);
}

void setup_board_ipc(Board *board_ipc)
{

  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      board_ipc->game_board[i][j] = 'x';
    }
  }

  board_ipc->game_board[0][WIDTH / 2] = 'r';
  board_ipc->game_board[HEIGHT - 1][WIDTH / 2] = 'b';
  board_ipc->red = 0;
  board_ipc->blue = 0;
  board_ipc->blocked1 = 0;
  board_ipc->blocked2 = 0;
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

void counter_ipc(Board *board_ipc)
{
  for (int i = 0; i < HEIGHT; i++)
    for (int j = 0; j < WIDTH; j++)
      switch (board_ipc->game_board[i][j])
      {
      case 'r':
        board_ipc->red += 1;
        break;
      case 'b':
        board_ipc->blue += 1;
        break;
      default:
        break;
      }
}

int is_blocked_ipc(int pos_x, int pos_y, char value, Board *board_ipc)
{
  if (board_ipc->game_board[pos_x][pos_y] == value)
  {
    if (pos_x == 0)
    {
      if (pos_y == 0)
      {
        if (!(board_ipc->game_board[pos_x + 1][pos_y] == 'x' ||
              board_ipc->game_board[pos_x][pos_y + 1] == 'x'))
          return 1;
      }
      else if (pos_y == WIDTH - 1)
      {
        if (!(board_ipc->game_board[pos_x + 1][pos_y] == 'x' ||
              board_ipc->game_board[pos_x][pos_y - 1] == 'x'))
          return 1;
      }
      else
      {
        if (!(board_ipc->game_board[pos_x + 1][pos_y] == 'x' ||
              board_ipc->game_board[pos_x][pos_y - 1] == 'x' ||
              board_ipc->game_board[pos_x][pos_y + 1] == 'x'))
          return 1;
      }
    }

    else if (pos_x == HEIGHT - 1)
    {
      if (pos_y == 0)
      {
        if (!(board_ipc->game_board[pos_x - 1][pos_y] == 'x' ||
              board_ipc->game_board[pos_x][pos_y + 1] == 'x'))
          return 1;
      }
      else if (pos_y == WIDTH - 1)
      {
        if (!(board_ipc->game_board[pos_x - 1][pos_y] == 'x' ||
              board_ipc->game_board[pos_x][pos_y - 1] == 'x'))
          return 1;
      }
      else
      {
        if (!(board_ipc->game_board[pos_x - 1][pos_y] == 'x' ||
              board_ipc->game_board[pos_x][pos_y - 1] == 'x' ||
              board_ipc->game_board[pos_x][pos_y + 1] == 'x'))
          return 1;
      }
    }

    else if (pos_y == 0)
    {
      if (!(board_ipc->game_board[pos_x - 1][pos_y] == 'x' ||
            board_ipc->game_board[pos_x + 1][pos_y] == 'x' ||
            board_ipc->game_board[pos_x][pos_y + 1] == 'x'))
        return 1;
    }
    else if (pos_y == WIDTH - 1)
    {
      if (!(board_ipc->game_board[pos_x - 1][pos_y] == 'x' ||
            board_ipc->game_board[pos_x + 1][pos_y] == 'x' ||
            board_ipc->game_board[pos_x][pos_y - 1] == 'x'))
        return 1;
    }

    else if (!(board_ipc->game_board[pos_x - 1][pos_y] == 'x' ||
               board_ipc->game_board[pos_x + 1][pos_y] == 'x' ||
               board_ipc->game_board[pos_x][pos_y - 1] == 'x' ||
               board_ipc->game_board[pos_x][pos_y + 1] == 'x'))
      return 1;
  }
  return 0;
}

void add_piece(pid_t id, Board *board_ipc)
{

  if (id > 0)
  {
    board_ipc->game_board[player_1.x][player_1.y] = 'r';
    red_position.game_board[player_1.x][player_1.y] = 'r';
    player_1.history_x.push(player_1.x);
    player_1.history_y.push(player_1.y);
  }
  else if (id == 0)
  {
    board_ipc->game_board[player_2.x][player_2.y] = 'b';
    blue_position.game_board[player_2.x][player_2.y] = 'b';
    player_2.history_x.push(player_2.x);
    player_2.history_y.push(player_2.y);
  }
}

void print_board_ipc(Board *board_ipc)
{
  cout << "***************" << endl;
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {

      if (board_ipc->game_board[i][j] == 'r')
      {
        printf("\x1b[31m"
               "* "
               "\x1b[0m");
      }
      else if (board_ipc->game_board[i][j] == 'b')
      {
        printf("\x1b[34m"
               "* "
               "\x1b[0m");
      }
      else
      {
        printf("x ");
      }
    }
    cout << endl;
  }

  cout << "***************" << endl;
}

void random_choice_p1_ipc(Board *board_ipc)
{

  int rand_pos_j1 = 1 + rand() % 4;
  switch (rand_pos_j1)
  {
  case UP:
    if (player_1.x > 0 && board_ipc->game_board[player_1.x - 1][player_1.y] == 'x')
    {
      player_1.x -= 1;
      break;
    }
  case DOWN:
    if (player_1.x < HEIGHT - 1 && board_ipc->game_board[player_1.x + 1][player_1.y] == 'x')
    {
      player_1.x += 1;
      break;
    }
  case LEFT:
    if (player_1.y > 0 && board_ipc->game_board[player_1.x][player_1.y - 1] == 'x')
    {
      player_1.y -= 1;
      break;
    }
  case RIGHT:
    if (player_1.y < WIDTH - 1 && board_ipc->game_board[player_1.x][player_1.y + 1] == 'x')
    {
      player_1.y += 1;
      break;
    }

  default:
    break;
  }
}

void random_choice_p2_ipc(Board *board_ipc)
{
  int rand_pos_j2 = 1 + rand() % 4;
  switch (rand_pos_j2)
  {
  case UP:
    if (player_2.x > 0 && board_ipc->game_board[player_2.x - 1][player_2.y] == 'x')
    {
      player_2.x -= 1;
      break;
    }

  case DOWN:
    if (player_2.x < HEIGHT - 1 && board_ipc->game_board[player_2.x + 1][player_2.y] == 'x')
    {
      player_2.x += 1;
      break;
    }

  case LEFT:
    if (player_2.y > 0 && board_ipc->game_board[player_2.x][player_2.y - 1] == 'x')
    {
      player_2.y -= 1;
      break;
    }

  case RIGHT:
    if (player_2.y < WIDTH - 1 && board_ipc->game_board[player_2.x][player_2.y + 1] == 'x')
    {
      player_2.y += 1;
      break;
    }
  default:
    break;
  }
}

void print_blue_board()
{
  cout << "*****************************" << endl;
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {

      if (blue_position.game_board[i][j] == 'b')
      {
        printf("\x1b[34m"
               "* "
               "\x1b[0m");
      }
      else
      {
        printf("x ");
      }
    }
    cout << endl;
  }

  cout << "*****************************" << endl;
}

void print_red_board()
{
  cout << "*****************************" << endl;
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {

      if (red_position.game_board[i][j] == 'r')
      {
        printf("\x1b[31m"
               "* "
               "\x1b[0m");
      }
      else
      {
        printf("x ");
      }
    }
    cout << endl;
  }

  cout << "*****************************" << endl;
}