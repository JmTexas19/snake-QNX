/* Define header for application - AppBuilder 2.03  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_snakeTimer;
#define ABW_snakeTimer                       AbGetABW( ABN_snakeTimer )
extern const int ABN_leftButton;
#define ABW_leftButton                       AbGetABW( ABN_leftButton )
extern const int ABN_newGameButton;
#define ABW_newGameButton                    AbGetABW( ABN_newGameButton )
extern const int ABN_upButton;
#define ABW_upButton                         AbGetABW( ABN_upButton )
extern const int ABN_rightButton;
#define ABW_rightButton                      AbGetABW( ABN_rightButton )
extern const int ABN_downButton;
#define ABW_downButton                       AbGetABW( ABN_downButton )
extern const int ABN_pauseButton;
#define ABW_pauseButton                      AbGetABW( ABN_pauseButton )
extern const int ABN_score;
#define ABW_score                            AbGetABW( ABN_score )
extern const int ABN_level;
#define ABW_level                            AbGetABW( ABN_level )
extern const int ABN_window;
#define ABW_window                           AbGetABW( ABN_window )
extern const int ABN_foodTimer;
#define ABW_foodTimer                        AbGetABW( ABN_foodTimer )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"
