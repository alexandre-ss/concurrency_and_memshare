#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define UP      1
#define DOWN    2
#define LEFT    3
#define RIGHT   4

int main(){

    char tabuleiro[8][8];
    long long int i, j;
    for (i = 0; i < 8; i++){
        for (j = 0; j < 8; j++){
            tabuleiro[i][j] = 'x';
        }
    }
    tabuleiro[0][0] = 'r';
    tabuleiro[7][7] = 'b';

    int jogador1_posx, jogador1_posy = 0;
    int jogador2_posx = 7, jogador2_posy = 7;
    int randj1, randj2 = 0;
        
    while(1){
        randj1 = 1 + rand()%4;
        randj2 = 1 + rand()%4;

        switch (randj1)
        {
        case UP:
            if(jogador1_posx > 0){
                jogador1_posx -= 1;
                tabuleiro[jogador1_posx][jogador1_posy] = 'r';
            }
            break;
        case DOWN:
            if(jogador1_posx < 7){
                jogador1_posx += 1;
                tabuleiro[jogador1_posx][jogador1_posy] = 'r';
            }
            break;
        case LEFT:
            if(jogador1_posy > 0){
                jogador1_posy -= 1;
                tabuleiro[jogador1_posx][jogador1_posy] = 'r';
            }
            break;
        case RIGHT:
            if(jogador1_posy < 7){
                jogador1_posy += 1;
                tabuleiro[jogador1_posx][jogador1_posy] = 'r';
            }
            break;        
        default:
            break;
        }

        switch (randj2)
        {
        case UP:
            if(jogador2_posx > 0){
                jogador2_posx -= 1;
                tabuleiro[jogador2_posx][jogador2_posy] = 'b';
            }
            break;
        case DOWN:
            if(jogador2_posx < 7){
                jogador2_posx += 1;
                tabuleiro[jogador2_posx][jogador2_posy] = 'b';
            }
            break;
        case LEFT:
            if(jogador2_posy > 0){
                jogador2_posy -= 1;
                tabuleiro[jogador2_posx][jogador2_posy] = 'b';
            }
            break;
        case RIGHT:
            if(jogador2_posy < 7){
                jogador2_posy += 1;
                tabuleiro[jogador2_posx][jogador2_posy] = 'b';
            }
            break;        
        default:
            break;
        }

        for (i = 0; i < 8; i++){
            for (j = 0; j < 8; j++){
                printf(" %c ",tabuleiro[i][j]);
            }
            printf("\n");
        }
        sleep(1);
        system("clear");
    }


    return 0;
}
