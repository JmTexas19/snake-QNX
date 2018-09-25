/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"
#include "snakeFunctions.h"
#include "press.h"

PtWidget_t *food;
PtWidget_t *b2;
PtWidget_t *b1;
int Exists = 0;
int body;
int realized;
int headPosx;
int snakeDirection = 0;
int foodExists;
int foodX, foodY;
int deleteTail = 0;
int paused = 0;
int oldSnakeDirection;
char scoreString[100];
int score = 0;
char levelString[100];
int level = 1;
pthread_t thrdNom;
pthread_t thrdDeath;
pthread_t thrdBGM;

//Struct holding information for each task
typedef struct funcNode{
	PtWidget_t *b;
	PgColor_t c;
	int x;
	int y;
	struct funcNode *next;
} NODE;

NODE *head;
NODE *tail;
PtWidget_t *pane;

//Create Linked List
struct funcNode *createList(){
    tail = NULL;
	NODE *temp;		//For Traversal
	temp = malloc(sizeof(NODE*));		//Dynamic Allocation
	
	temp->next = NULL;		//Set last node to null
	head = temp;
	tail = head;
	return tail;
}

void addNode(int x1, int y1, PgColor_t c1){
	NODE* temp;
	temp = malloc(sizeof(NODE*));
	temp -> x = x1;
	temp -> y = y1;
	temp -> c = c1;
	
	if(tail == NULL){
		tail = createList();
		tail = temp;
		head = tail;
	
	}
	else{	
		head -> next = temp;		
		head = temp;
	}	
}

void pauseGame(){
	if(paused == 0){
		paused = 1;
		oldSnakeDirection = snakeDirection;
		snakeDirection = 0;
		
		
	}
	else{
		snakeDirection = oldSnakeDirection;
		paused = 0;
	}
	
}

void buildBlock(PtWidget_t *window){
	score = 0;
	level = 1;
	sprintf(scoreString, "Score: %d", score);
	sprintf(levelString, "Level: %d", level);
	PtSetResource(ABW_level, Pt_ARG_TEXT_STRING, levelString, 0);
	PtSetResource(ABW_score, Pt_ARG_TEXT_STRING, scoreString, 0);

	pane = window;
	//Clear Pane
	if(Exists == 1){
		PtUnrealizeWidget(food);
		while(tail->next != NULL){
			PtUnrealizeWidget(tail -> b);
			tail = tail->next;
		}
		PtUnrealizeWidget(tail -> b);
		head = NULL;
		tail = NULL;
	}

	
	//Randomly generate tail
	int randomX = (rand() % 24) * 15;
	int randomY = (rand() % 15) * 15;

	addNode(randomX, randomY, Pg_WHITE);
	
	tail->b = createBlock(window, tail->x, tail->y, tail->c);
	
	//Generate Body
	realized = 0;
	while(realized == 0){
		int randomNum = rand() % 4;
	
		//Left
		if(randomNum == 0){
			if(randomX > 15){
				addNode(randomX - 15, randomY, Pg_WHITE);
				
				head->b = createBlock(window, randomX - 15, randomY, Pg_WHITE);
				randomX -= 15;
				realized = 1;
				body = 0;
			}
			
			randomNum = 1;			
		}
		
		//Top
		else if(randomNum == 1){
			if(randomY > 15){
				addNode(randomX, randomY-15, Pg_WHITE);
			
				head->b = createBlock(window, randomX, randomY - 15, Pg_WHITE);
				randomY -= 15;
				realized = 1;
				body = 1;
			}
			
			randomNum = 2;
		}
		
		//Right
		else if(randomNum == 2){
			if(randomX < 330){
				addNode(randomX + 15, randomY, Pg_WHITE);
			    head->b = createBlock(window, randomX + 15, randomY, Pg_WHITE);
				randomX += 15;
				realized = 1;
				body = 2;
			}
			
			randomNum = 3;
		}
		
		//Bottom
		else if(randomNum == 3){
			if(randomY < 220){
				addNode(randomX, randomY + 15, Pg_WHITE);
				head->b = createBlock(window, randomX, randomY + 15, Pg_WHITE);
				randomY += 15;
				realized = 1;
				body = 3;
			}
			
			randomNum = 0;
		}
	}
	
	//Generate Head 
	realized = 0;
	while(realized == 0){
		int randomNum = rand() % 4;
	
		//Left
		if(randomNum == 0 && body != 2){
			if(randomX > 15){
				addNode(randomX - 15, randomY, Pg_GREEN);
				head -> b = createBlock(window, randomX - 15, randomY, Pg_GREEN);
				realized = 1;
				disableRight = 1;
			}
			
			randomNum = 1;
		}
		
		//Top
		else if(randomNum == 1 && body != 3){
			if(randomY > 15){
				addNode(randomX, randomY - 15, Pg_GREEN);
				head -> b = createBlock(window, randomX, randomY - 15, Pg_GREEN);
				realized = 1;
				disableDown = 1;
			}
			
			randomNum = 2;
		}
		
		//Right
		else if(randomNum == 2 && body != 0){
			if(randomX < 330){
				addNode(randomX + 15, randomY, Pg_GREEN);
				head -> b = createBlock(window, randomX + 15, randomY, Pg_GREEN);
				realized = 1;
				disableLeft = 1;
			}
			
			randomNum = 3;
		}
		
		//Bottom
		else if(randomNum == 3 && body != 1){
			if(randomY < 220){
				addNode(randomX, randomY + 15, Pg_GREEN);
				head -> b = createBlock(window, randomX, randomY + 15, Pg_GREEN);
				realized = 1;
				disableUp = 1;
			}
			
			randomNum = 0;
		}
	}	
	
	PtRealizeWidget(tail -> b);
	PtRealizeWidget(tail -> next -> b);
	PtRealizeWidget(head -> b);	
	
	createFood();
	
	//Prevents breaking
	Exists = 1;
}

void createFood(){
	if(foodExists == 1){
		PtUnrealizeWidget(food);
	}
		
	foodX = (rand() % 24) * 15;
	foodY = (rand() % 17) * 15;
	
	//Cycle through each body of snake	
	NODE* temp;
	temp = tail;
	
	//Snake Body Collision
	while(temp->next != NULL){
		if (temp->x == foodX && temp->y == foodY){	
			foodX = (rand() % 24) * 15;
			foodY = (rand() % 17) * 15;
			temp = tail;
		}
		else{
			temp = temp->next;
		}
	}
	
	food = createBlock(ABW_window, foodX, foodY, Pg_RED);
	PtRealizeWidget(food);	
	foodExists = 1;
}

PtWidget_t *createBlock(PtWidget_t *window, int posX, int posY, PgColor_t color){
	
	PhPoint_t pos;
	PhArea_t area;
	PhDim_t dim;
	PtArg_t argt[5];
	
	//top left
	pos.x = posX;
	pos.y = posY;
	area.pos = pos;
	dim.h = 15;
	dim.w = 15;
	area.size = dim;
	PtSetArg(&argt[0], Pt_ARG_AREA, &area, 0);
	PtSetArg(&argt[1], Pt_ARG_FILL_COLOR, color, 0);
	return(PtCreateWidget(PtPane, window, 2, argt));
}

void snakeMoveLeft(){
	PtUnrealizeWidget(head -> b);
	
	//Add Block if snake eats food
	if(deleteTail == 1){
		deleteTail = 0;
	}
	else{
		PtUnrealizeWidget(tail -> b);
		tail = tail->next;	
	}
		
	head -> b = createBlock(pane, head->x, head->y, Pg_WHITE);
	PtRealizeWidget(head->b);	
	addNode(head->x - 15, head->y, Pg_GREEN);
	
	head -> b = createBlock(pane, head->x, head->y, head->c);
	PtRealizeWidget(head->b);	
	
	//If Snake hits border, end game
	if(head->x < 0){
		//Play Sound
		strcpy(soundFile, "Death.WAV");
		pthread_cancel(thrdNom);
		pthread_cancel(thrdDeath);
		pthread_create(&thrdDeath, NULL, (void*) playingWave, NULL);	
		buildBlock(ABW_window);
		snakeDirection = 0;
		sleep(3);
		pthread_cancel(thrdDeath);
	}
}

void snakeMoveUp(){
	PtUnrealizeWidget(head -> b);
	
	//Add Block if snake eats food
	if(deleteTail == 1){
		deleteTail = 0;
	}
	else{
		PtUnrealizeWidget(tail -> b);
		tail = tail->next;	
	}	
	
	head -> b = createBlock(pane, head->x, head->y, Pg_WHITE);
	PtRealizeWidget(head->b);	
	addNode(head->x, head->y-15, Pg_GREEN);
	
	head -> b = createBlock(pane, head->x, head->y, head->c);
	PtRealizeWidget(head->b);	
		
	if(head->y < 0){
		//Play Sound
		strcpy(soundFile, "Death.WAV");
		pthread_cancel(thrdNom);
		pthread_cancel(thrdDeath);
		pthread_create(&thrdDeath, NULL, (void*) playingWave, NULL);	
		buildBlock(ABW_window);
		snakeDirection = 0;
		sleep(3);
		pthread_cancel(thrdDeath);
	}
}

void snakeMoveRight(){
	PtUnrealizeWidget(head -> b);
	
	//Add Block if snake eats food
	if(deleteTail == 1){
		deleteTail = 0;
	}
	else{
		PtUnrealizeWidget(tail -> b);
		tail = tail->next;	
	}	
	
	head -> b = createBlock(pane, head->x, head->y, Pg_WHITE);
	PtRealizeWidget(head->b);	
	addNode(head->x + 15, head->y, Pg_GREEN);
	
	head -> b = createBlock(pane, head->x, head->y, head->c);
	PtRealizeWidget(head->b);	
	
	if(head->x > 345){
		//Play Sound
		strcpy(soundFile, "Death.WAV");
		pthread_cancel(thrdNom);
		pthread_cancel(thrdDeath);
		pthread_create(&thrdDeath, NULL, (void*) playingWave, NULL);	
		buildBlock(ABW_window);
		snakeDirection = 0;
		sleep(3);
		pthread_cancel(thrdDeath);
	}
}

void snakeMoveDown(){
	PtUnrealizeWidget(head -> b);
	
	//Add Block if snake eats food
	if(deleteTail == 1){
		deleteTail = 0;
	}
	else{
		PtUnrealizeWidget(tail -> b);
		tail = tail->next;	
	}	
	
	head -> b = createBlock(pane, head->x, head->y, Pg_WHITE);
	PtRealizeWidget(head->b);	
	addNode(head->x, head->y+15, Pg_GREEN);
	
	head -> b = createBlock(pane, head->x, head->y, head->c);
	PtRealizeWidget(head->b);	

	if(head->y > 240){
		//Play Sound
		strcpy(soundFile, "Death.WAV");
		pthread_cancel(thrdNom);
		pthread_cancel(thrdDeath);
		pthread_create(&thrdDeath, NULL, (void*) playingWave, NULL);	
		buildBlock(ABW_window);
		snakeDirection = 0;
		sleep(3);
		pthread_cancel(thrdDeath);
	}
}

void startBGM(){
//	strcpy(soundFile, "RivBGM.wav");
//	pthread_create(&thrdBGM, NULL, (void*) playingWave, NULL);
}


//Control Snake Direction
void loopSnake(){	
	if(snakeDirection == 0){
		//DO NOTHING
	}
	else if(snakeDirection == 1 && disableLeft == 0){
		paused = 0;
		disableLeft = 0;
		disableUp = 0;
		disableRight = 0;
		disableDown = 0;

		snakeMoveLeft();		
	}
	else if(snakeDirection == 2 && disableUp == 0){
		paused = 0;
		disableLeft = 0;
		disableUp = 0;
		disableRight = 0;
		disableDown = 0;
	
		snakeMoveUp();
	}
	else if(snakeDirection == 3 && disableRight == 0){
		paused = 0;
		disableLeft = 0;
		disableUp = 0;
		disableRight = 0;
		disableDown = 0;
		
		snakeMoveRight();
	}
	else if(snakeDirection == 4 && disableDown == 0){
		paused = 0;
		disableLeft = 0;
		disableUp = 0;
		disableRight = 0;
		disableDown = 0;
	
		snakeMoveDown();
	}
	
	//Collision 
	//Cycle through each body of snake
	if(head != NULL){
		
		//Collision of Food AKA Snake eats food
		if(head->x == foodX && head->y == foodY){
			//Sound File
			int randomNum = rand() % 3;
			
			if(randomNum == 0){
				strcpy(soundFile, "nom1.wav");
				pthread_cancel(thrdNom);
				pthread_create(&thrdNom, NULL, (void*) playingWave, NULL);	
			}
			if( randomNum == 1){
				strcpy(soundFile, "nom2.wav");
				pthread_cancel(thrdNom);
				pthread_create(&thrdNom, NULL, (void*) playingWave, NULL);
			}
			if( randomNum == 2){
				strcpy(soundFile, "nom3.wav");
				pthread_cancel(thrdNom);
				pthread_create(&thrdNom, NULL, (void*) playingWave, NULL);
			}
		
			deleteTail = 1;
			
			//Set Score
			score = score +10;
			sprintf(scoreString, "Score: %d", score);
			PtSetResource(ABW_score, Pt_ARG_TEXT_STRING, scoreString, 0);
			
			//Set Level
			level++;
			sprintf(levelString, "Level: %d", level);
			PtSetResource(ABW_level, Pt_ARG_TEXT_STRING, levelString, 0);
			
			//Create new food and reset food timer
			createFood();
			PtUnrealizeWidget(ABW_foodTimer);
			PtRealizeWidget(ABW_foodTimer);
		}
		
		NODE* temp;
		temp = tail;
	
		//Snake Body Collision
		while(temp->next != NULL){
			if (temp->x == head->x && temp->y == head->y){	
				//New Game
				//Play Sound
				strcpy(soundFile, "Death.WAV");
				pthread_cancel(thrdNom);
				pthread_cancel(thrdDeath);
				pthread_create(&thrdDeath, NULL, (void*) playingWave, NULL);	
				buildBlock(ABW_window);
				snakeDirection = 0;
				sleep(3);
				pthread_cancel(thrdDeath);
			
				temp->next = NULL;
			}
			else{
				temp = temp->next;
			}
		}
	}
}