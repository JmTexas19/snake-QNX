
/* abmain.c */

/* downButton.c */
int downButton ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* foodTimer.c */
int foodTimer ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* leftButton.c */
int leftButton ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* my_init.c */
int my_init ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* newGame.c */
int newGame ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* pauseButton.c */
int pauseButton ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* press.c */
int err ( char *msg );
int FindTag ( FILE *fp , const char *tag );
int CheckFileHeader ( FILE *fp );
void *playingWave ( void );
int press ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* rightButton.c */
int rightButton ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* snakeFunctions.c */
struct funcNode *createList ( void );
void addNode ( int x1 , int y1 , PgColor_t c1 );
void pauseGame ( void );
void buildBlock ( PtWidget_t *window );
void createFood ( void );
PtWidget_t *createBlock ( PtWidget_t *window , int posX , int posY , PgColor_t color );
void snakeMoveLeft ( void );
void snakeMoveUp ( void );
void snakeMoveRight ( void );
void snakeMoveDown ( void );
void startBGM ( void );
void loopSnake ( void );

/* snakeTimer.c */
int snakeTimer ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* start.c */
int start ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* upButton.c */
int upButton ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
