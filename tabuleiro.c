#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define UP      1
#define DOWN    2
#define LEFT    3
#define RIGHT   4

typedef struct
{
    int posx;
    int posy;
    int blocked;

} player;

typedef struct
{
    int red;
    int blue;
    int board[8][8];

} board;
typedef struct 
{
    /* data */
} history;


player player_1;
player player_2;
board  game_board;

void *player_one_play();
void *player_two_play();
void counter();
int is_blocked(int,int, char);

int main(){

    long long int i, j;
    pthread_t thread1, thread2;
    int iret1, iret2;
    struct timespec ts;
    ts.tv_sec = 0.5;
    ts.tv_nsec = (500%1000)*1000000;

    for (i = 0; i < 8; i++){
        for (j = 0; j < 8; j++){
            game_board.board[i][j] = 'x';
        }
    }
    game_board.board[0][3] = 'r';
    game_board.board[7][3] = 'b';
    game_board.red = 0;
    game_board.blue = 0;

    player_1.posx = 0;
    player_1.posy = 3;

    player_2.posx = 7;
    player_2.posy = 3;

    int rand_pos_j1, rand_pos_j2 = 0;
    int valid_numbers[4] = {1,2,3,4};
        
    while(1){

        iret1 = pthread_create(&thread1, NULL, player_one_play, NULL);
        iret2 = pthread_create(&thread2, NULL, player_two_play, NULL);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                printf(" %c ",game_board.board[i][j]);
            }
            printf("\n");
        }
        if(player_1.blocked && player_2.blocked){
            counter();
            char* winner = game_board.red > game_board.blue ? "red" : "blue";
            printf("the winner is %s!\n", winner);
            exit(0);
        }
        printf("*******************************\n");
        nanosleep(&ts, NULL);
        system("clear");
    }
    return 0;
}

void *player_one_play(){
    srand(time(NULL));
    int rand_pos_j1  = 1 + rand()%4;

    switch (rand_pos_j1)
    {
        case UP:
            if(player_1.posx > 0 && game_board.board[player_1.posx-1][player_1.posy]  == 'x'){
                player_1.posx -= 1;
            }
            break;
        case DOWN:
            if(player_1.posx < 7 && game_board.board[player_1.posx+1][player_1.posy] == 'x'){
                player_1.posx += 1;  
            }
            break;
        case LEFT:
            if(player_1.posy > 0 && game_board.board[player_1.posx][player_1.posy-1]  == 'x'){
                player_1.posy -= 1;
            }
            break;
        case RIGHT:
            if(player_1.posy < 7 && game_board.board[player_1.posx][player_1.posy+1]  == 'x'){
                player_1.posy += 1;
            }
            break;        
        default:
            break;
    }
    if(game_board.board[player_1.posx][player_1.posy] == 'x' )
        game_board.board[player_1.posx][player_1.posy] = 'r';
    else if(is_blocked(player_1.posx, player_1.posy, 'r'))
        player_1.blocked = 1;   
}

void *player_two_play(){
    srand(time(NULL));
    int rand_pos_j2 = 1 + rand()%4;
    printf("%d\n",rand_pos_j2);
    switch (rand_pos_j2)
    {
        case UP:
            if(player_2.posx > 0 && game_board.board[player_2.posx-1][player_2.posy]  == 'x'){
                player_2.posx -= 1;
            }
            break;
        case DOWN:
            if(player_2.posx < 7  && game_board.board[player_2.posx+1][player_2.posy]  == 'x'){
                player_2.posx += 1;
            }
            break;
        case LEFT:
            if(player_2.posy > 0  && game_board.board[player_2.posx][player_2.posy-1]  == 'x'){
                player_2.posy -= 1;
            }
            break;
        case RIGHT:
            if(player_2.posy < 7  && game_board.board[player_2.posx][player_2.posy+1] == 'x'){
                player_2.posy += 1; 
            }
            break;        
        default:
            break;
    }
    if(game_board.board[player_2.posx][player_2.posy] == 'x' )
        game_board.board[player_2.posx][player_2.posy] = 'b'; 
    else if(is_blocked(player_2.posx, player_2.posy, 'b'))
        player_2.blocked = 1;      
}

void counter(){
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8;j++)
                switch(game_board.board[i][j]){
                    case 'r':
                        game_board.red += 1;
                    break;
                    case 'b':
                        game_board.blue += 1;
                    break;
                    default:
                    break;
                }
}

int is_blocked(int pos_x,int pos_y, char value){
    if(game_board.board[pos_x][pos_y] == value){
        if( game_board.board[pos_x-1][pos_y] == 'x' ||
            game_board.board[pos_x+1][pos_y] == 'x' ||
            game_board.board[pos_x][pos_y-1] == 'x' ||
            game_board.board[pos_x][pos_y+1] == 'x'
        ) return 0;
        else return 1;
    }
}