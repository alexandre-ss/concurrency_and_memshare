#include "game.hpp"
#include <semaphore.h>
#include <signal.h>
#include <wait.h>

using namespace std;
sem_t smutex;
int k = 0;
int main()
{
  sem_init(&smutex, 0, 1);
  key_t key = ftok("shmfile", 65);                         // gerando uma chave única
  int shm_id = shmget(key, sizeof(int), 0666 | IPC_CREAT); // identificador para a memória compartilhada
  int *i = (int *)shmat(shm_id, NULL, 0);
  *i = 0;
  pid_t p = fork();
  for (int j = 0; j < 3; j++)
  {
    if (p == 0)
    {
      cout << *i << endl;
    }
    else if (p > 0)
    {
      //waitpid(p, NULL, 0);
      //if(j == 2) k = 1;
      cout << *i << endl;
    }
    sem_wait(&smutex);
    *i += 1;
    sem_post(&smutex);
    sleep(1);
  }
  if(k == 1) cout << "saiu" << endl;
  sem_destroy(&smutex);
  shmdt((int *)i);
  return 0;
}