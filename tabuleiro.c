#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define UP      1
#define DOWN    2
#define LEFT    3
#define RIGHT   4

typedef struct
{
    int posx;
    int posy;

} player;

player player_1;
player player_2;
char tabuleiro[8][8];

void *player_one_play(void*);
void *player_two_play(void*);

int main(){

    long long int i, j;
    pthread_t thread1, thread2;
    int iret1, iret2;

    for (i = 0; i < 8; i++){
        for (j = 0; j < 8; j++){
            tabuleiro[i][j] = 'x';
        }
    }
    tabuleiro[0][3] = 'r';
    tabuleiro[7][3] = 'b';

    player_1.posx = 0;
    player_1.posy = 3;

    player_2.posx = 7;
    player_2.posy = 3;

    int rand_pos_j1, rand_pos_j2 = 0;
        
    while(1){
        rand_pos_j1 = 1 + rand()%4;
        rand_pos_j2 = 1 + rand()%4;

        iret1 = pthread_create(&thread1, NULL, player_one_play, &rand_pos_j1);
        iret2 = pthread_create(&thread2, NULL, player_two_play, &rand_pos_j2);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                printf(" %c ",tabuleiro[i][j]);
            }
            printf("\n");
        }
        printf("%d %d *******************************\n", iret1, iret2);
        sleep(1);
        system("clear");
    }
    return 0;
}

void *player_one_play(void *ptr){
    int *rand_pos_j1 = (int*)ptr;
    switch (*rand_pos_j1)
    {
        case UP:
            if(player_1.posx > 0){
                player_1.posx -= 1;
                if(tabuleiro[player_1.posx][player_1.posy] == 'r'){ 
                    player_1.posx += 1;

                    if(player_1.posy > 0 && tabuleiro[player_1.posx][player_1.posy-1] == 'x'){
                        player_1.posy -=1;
                    }
                    else if(player_1.posy < 7 && tabuleiro[player_1.posx][player_1.posy+1] == 'x'){
                        player_1.posy +=1;    
                    }
                    else if(player_1.posx < 7 && tabuleiro[player_1.posx+1][player_1.posy] == 'x'){
                        player_1.posx +=1;    
                    }
                }
                if(tabuleiro[player_1.posx][player_1.posy] != 'b' && tabuleiro[player_1.posx][player_1.posy] == 'x' )
                    tabuleiro[player_1.posx][player_1.posy] = 'r';             
            }
            break;
        case DOWN:
            if(player_1.posx < 7){
                player_1.posx += 1;
                if(tabuleiro[player_1.posx][player_1.posy] == 'r'){ 
                    player_1.posx -= 1;
                    if(player_1.posy > 0 && tabuleiro[player_1.posx][player_1.posy-1] == 'x'){
                        player_1.posy -=1;
                    }
                    else if(player_1.posy < 7 && tabuleiro[player_1.posx][player_1.posy+1] == 'x'){
                        player_1.posy +=1;    
                    }
                    else if(player_1.posx > 0 && tabuleiro[player_1.posx-1][player_1.posy] == 'x'){
                        player_1.posx -=1;    
                    }
                } 
                if(tabuleiro[player_1.posx][player_1.posy] != 'b' && tabuleiro[player_1.posx][player_1.posy] == 'x' )
                    tabuleiro[player_1.posx][player_1.posy] = 'r';    
            }
            break;
        case LEFT:
            if(player_1.posy > 0){
                player_1.posy -= 1;
                if(tabuleiro[player_1.posx][player_1.posy] == 'r'){ 
                    player_1.posy += 1;
                    if(player_1.posx < 7 && tabuleiro[player_1.posx+1][player_1.posy] == 'x' ){
                        player_1.posx +=1;
                    }
                    else if(player_1.posy < 8 && tabuleiro[player_1.posx][player_1.posy+1] == 'x'){
                        player_1.posy +=1;    
                    }
                    else if(player_1.posx > 0 && tabuleiro[player_1.posx-1][player_1.posy] == 'x'){
                        player_1.posx -=1;    
                    }
                } 
                if(tabuleiro[player_1.posx][player_1.posy] != 'b' && tabuleiro[player_1.posx][player_1.posy] == 'x' )
                    tabuleiro[player_1.posx][player_1.posy] = 'r';   
            }
            break;
        case RIGHT:
            if(player_1.posy < 7){
                player_1.posy += 1;
                if(tabuleiro[player_1.posx][player_1.posy] == 'r'){ 
                    player_1.posx += 1;
                    if(player_1.posy > 0 && tabuleiro[player_1.posx][player_1.posy-1] == 'x'){
                        player_1.posy -=1;
                    }
                    else if(player_1.posx < 7 && tabuleiro[player_1.posx+1][player_1.posy] == 'x'){
                        player_1.posx +=1;    
                    }
                    else if(player_1.posx > 0 && tabuleiro[player_1.posx-1][player_1.posy] == 'x'){
                        player_1.posx -=1;    
                    }
                } 
                if(tabuleiro[player_1.posx][player_1.posy] != 'b' && tabuleiro[player_1.posx][player_1.posy] == 'x' )
                    tabuleiro[player_1.posx][player_1.posy] = 'r';    
            }
            break;        
        default:
            break;
    }
}

void *player_two_play(void *ptr){
    int *rand_pos_j2 = (int*)ptr;
    switch (*rand_pos_j2)
    {
        case UP:
            if(player_2.posx > 0){
                player_2.posx -= 1;
                if(tabuleiro[player_2.posx][player_2.posy] == 'b'){ 
                    player_2.posx += 1;

                    if(player_2.posy > 0 && tabuleiro[player_2.posx][player_2.posy-1] == 'x'){
                        player_2.posy -=1;
                    }
                    else if(player_2.posy < 7 && tabuleiro[player_2.posx][player_2.posy+1] == 'x'){
                        player_2.posy +=1;    
                    }
                    else if(player_2.posx < 7 && tabuleiro[player_2.posx+1][player_2.posy] == 'x'){
                        player_2.posx +=1;    
                    }
                }
                if(tabuleiro[player_2.posx][player_2.posy] != 'r' && tabuleiro[player_2.posx][player_2.posy] == 'x' )
                    tabuleiro[player_2.posx][player_2.posy] = 'b';             
            }
            break;
        case DOWN:
            if(player_2.posx < 7){
                player_2.posx += 1;
                if(tabuleiro[player_2.posx][player_2.posy] == 'b'){ 
                    player_2.posx -= 1;
                    if(player_2.posy > 0 && tabuleiro[player_2.posx][player_2.posy-1] == 'x'){
                        player_2.posy -=1;
                    }
                    else if(player_2.posy < 7 && tabuleiro[player_2.posx][player_2.posy+1] == 'x'){
                        player_2.posy +=1;    
                    }
                    else if(player_2.posx > 0 && tabuleiro[player_2.posx-1][player_2.posy] == 'x'){
                        player_2.posx -=1;    
                    }
                } 
                if(tabuleiro[player_2.posx][player_2.posy] != 'r' && tabuleiro[player_2.posx][player_2.posy] == 'x' )
                    tabuleiro[player_2.posx][player_2.posy] = 'b';    
            }
            break;
        case LEFT:
            if(player_2.posy > 0){
                player_2.posy -= 1;
                if(tabuleiro[player_2.posx][player_2.posy] == 'b'){ 
                    player_2.posy += 1;
                    if(player_2.posx < 7 && tabuleiro[player_2.posx+1][player_2.posy] == 'x' ){
                        player_2.posx +=1;
                    }
                    else if(player_2.posy < 8 && tabuleiro[player_2.posx][player_2.posy+1] == 'x'){
                        player_2.posy +=1;    
                    }
                    else if(player_2.posx > 0 && tabuleiro[player_2.posx-1][player_2.posy] == 'x'){
                        player_2.posx -=1;    
                    }
                } 
                if(tabuleiro[player_2.posx][player_2.posy] != 'r' && tabuleiro[player_2.posx][player_2.posy] == 'x' )
                    tabuleiro[player_2.posx][player_2.posy] = 'b';   
            }
            break;
        case RIGHT:
            if(player_2.posy < 7){
                player_2.posy += 1;
                if(tabuleiro[player_2.posx][player_2.posy] == 'b'){ 
                    player_2.posx += 1;
                    if(player_2.posy > 0 && tabuleiro[player_2.posx][player_2.posy-1] == 'x'){
                        player_2.posy -=1;
                    }
                    else if(player_2.posx < 7 && tabuleiro[player_2.posx+1][player_2.posy] == 'x'){
                        player_2.posx +=1;    
                    }
                    else if(player_2.posx > 0 && tabuleiro[player_2.posx-1][player_2.posy] == 'x'){
                        player_2.posx -=1;    
                    }
                } 
                if(tabuleiro[player_2.posx][player_2.posy] != 'r' && tabuleiro[player_2.posx][player_2.posy] == 'x' )
                    tabuleiro[player_2.posx][player_2.posy] = 'b';    
            }
            break;        
        default:
            break;
    }
}