/**
 * Author: FNU Richard
 * GBA Game - Invaders
 */

#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
#include "text.h"
#include "splashScn.h"
#include "orion.h"
#include "player.h"
#include "blueInvader.h"
#include "greenInvader.h"
#include "redInvader.h"
#include "winscrn.h"
#include "gameoverscrn.h"
#include "moon.h"

//Prototype
int splash();
int game();
int win();
int lose();
void restart();

int life;

typedef struct 
{
	int row;
	int col;
	int oldRow;
	int oldCol;
	int size;
	int speed;
}PLAYER PLAYER1;

typedef struct 
{
	int row;
	int col;
	int height;
	int width;
	int speed;
	int oldRow;
}INVADER;

INVADER invaderB;
INVADER invaderG;
INVADER invaderR;
//INVADER invaderBPrev;

enum{SPLASH, GAME, WIN, LOSE};
int main() {
	int state = SPLASH;
	while(1) {
		switch(state) {
			case SPLASH:
				state = splash();
				break;
			case  GAME:
				state = game();
				break;
			case WIN:
				state = win();
				break;
			case LOSE:
				state = lose();
				break; 
		} // switch
	}// while
	return 0;
} // main

int splash() {
	life = 3;
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	drawImage3(0, 0, 240, 160, splashScn);
	drawString(120, 65, "Press Start to Play", GREEN);
	while(1) {
		if(KEY_DOWN_NOW(BUTTON_START)){
			return GAME;
		}
		if(KEY_DOWN_NOW(BUTTON_SELECT)) {
			return SPLASH;
		}
	} // while
} //splash

int game() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	PLAYER p1;
	p1.row = 50;
	p1.col = 20;
	p1.oldRow = p1.row;
	p1.oldCol = p1.col;
	p1.size = 5;
	p1.speed = 1;

	invaderB.row = 20;
	invaderB.col = 60;
	invaderB.width = BLUEINVADER_WIDTH;
	invaderB.height = BLUEINVADER_HEIGHT;
	invaderB.speed = 3;
	invaderB.oldRow = invaderB.row;

	invaderG.row = 20;
	invaderG.col = 110;
	invaderG.width = GREENINVADER_WIDTH;
	invaderG.height = GREENINVADER_HEIGHT;
	invaderG.speed = 4;
	invaderG.oldRow = invaderG.row;

	invaderR.row = 20;
	invaderR.col = 160;
	invaderR.width = REDINVADER_WIDTH;
	invaderR.height = REDINVADER_HEIGHT;
	invaderR.speed = 2;
	invaderR.oldRow = invaderR.row;

	while(1) {
		//Draw Backgrounds
		drawImage3(0, 0, ORION_WIDTH, ORION_HEIGHT, orion);
		drawRect(20, 0, 100, 240, BLACK);
		drawImage3(55, 225, MOON_WIDTH, MOON_HEIGHT, moon);
		//drawRect(60, 230, 10, 10, BLUE);

		//Controls
		while (!KEY_DOWN_NOW(BUTTON_SELECT)) {

			if(KEY_DOWN_NOW(BUTTON_UP)) {
				p1.row -= p1.speed;
			}

			if(KEY_DOWN_NOW(BUTTON_DOWN)) {
				p1.row += p1.speed;
			}

			if(KEY_DOWN_NOW(BUTTON_LEFT)) {
				p1.col -= p1.speed;
			}

			if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
				p1.col += p1.speed;
			}

			//Win conditions

			if((p1.col + p1.size -1 >= invaderR.col) && (p1.col <= invaderR.col + invaderR.width -1) 
				&& (p1.row + p1.size -1 >= invaderR.row) && (p1.row <= invaderR.row + invaderR.height))
			{
				life--;
				return GAME;
			}

			if((p1.col + p1.size >= 225) && (p1.row + p1.size >= 55) && (p1.row + p1.size < 70)) {
				return WIN;
			}

			//Lose conditions
			if(life == 0) {
				return LOSE;
			}

			
			//Object collisions conditions
			
			//BLUE Invader
			if((p1.col + p1.size -1 >= invaderB.col) && (p1.col <= invaderB.col + invaderB.width -1) 
				&& (p1.row + p1.size -1 >= invaderB.row) && (p1.row <= invaderB.row + invaderB.height))
			{
				life--;
				return GAME;
			}

			//Green Invader
			if((p1.col + p1.size -1 >= invaderG.col) && (p1.col <= invaderG.col + invaderG.width -1) 
				&& (p1.row + p1.size -1 >= invaderG.row) && (p1.row <= invaderG.row + invaderG.height))
			{
				life--;
				return GAME;
			}
			
			//Red Invader
			if((p1.col + p1.size -1 >= invaderR.col) && (p1.col <= invaderR.col + invaderR.width -1) 
				&& (p1.row + p1.size -1 >= invaderR.row) && (p1.row <= invaderR.row + invaderR.height))
			{
				life--;
				return GAME;
			}

			//Border check for player so they dont go out of the game frame	
			if(p1.row < 20) {
				p1.row = 20;
			}	
				
			if(p1.row > 119-p1.size) {
				p1.row = 119-p1.size;
			}
			if(p1.col < 0) {
				p1.col = 0;
			}
			if(p1.col > 239-p1.size) {
				p1.col = 239-p1.size;
			}

			//Bounds check for all the invaders so that they can bounce
			boundsCheck(&invaderB.row, 20, 119, &invaderB.speed, invaderB.height);
			boundsCheck(&invaderG.row, 20, 119, &invaderG.speed, invaderG.height);
			boundsCheck(&invaderR.row, 20, 119, &invaderR.speed, invaderR.height);

			//Draw all entities
			

			//drawLives(life);
			char buffer[41];
		 	sprintf(buffer, "Oxygen Tanks: %i", life);
		 	waitForVblank();
		 	drawRect(150, 40, 36, 20, BLACK);
			drawString(150, 5, buffer, WHITE);


			//Player / astronaut
			drawRect(p1.oldRow, p1.oldCol, p1.size, p1.size, BLACK);
			drawImage3(p1.row, p1.col, p1.size, p1.size, player);
			p1.oldRow = p1.row;
			p1.oldCol = p1.col;

			//Blue Invader
			drawImage3(invaderB.oldRow, invaderB.col, invaderB.width, invaderB.height, BLACK);
			drawImage3(invaderB.row, invaderB.col, invaderB.width, invaderB.height, blueInvader);
			invaderB.oldRow = invaderB.row;
			invaderB.row += invaderB.speed;

			//Green Invader
			drawImage3(invaderG.oldRow, invaderG.col, invaderG.width, invaderG.height, BLACK);
			drawImage3(invaderG.row, invaderG.col, invaderG.width, invaderG.height, greenInvader);
			invaderG.oldRow = invaderG.row;
			invaderG.row += invaderG.speed;

			//Red Invader
			drawImage3(invaderR.oldRow, invaderR.col, invaderR.width, invaderR.height, BLACK);
			drawImage3(invaderR.row, invaderR.col, invaderR.width, invaderR.height, redInvader);
			invaderR.oldRow = invaderR.row;
			invaderR.row += invaderR.speed;
			
		}//while
		return 0;
	}//while
} // Game

int win() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	while(!KEY_DOWN_NOW(BUTTON_SELECT)) {
		waitForVblank();
		drawImage3(0, 0, WINSCRN_WIDTH, WINSCRN_HEIGHT, winscrn);
	}
	return SPLASH;
}

int lose() {
	life = 3;
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	while(1){
		waitForVblank();
		drawImage3(0, 0, GAMEOVERSCRN_WIDTH, GAMEOVERSCRN_HEIGHT, gameOverScrn);
		while(!KEY_DOWN_NOW(BUTTON_SELECT)) {
			if(KEY_DOWN_NOW(BUTTON_START)) {
				return GAME;
			}
		}
		return SPLASH;
	}

}