/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"


int
start( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){

	srand(time(NULL));
	PtRealizeWidget(ABW_snakeTimer);
	
	//Set Score
	char scoreString[100];
	int score = 0;
	sprintf(scoreString, "Score: %d", score);
	PtSetResource(ABW_score, Pt_ARG_TEXT_STRING, scoreString, 0);
	
	//Set Level
	char levelString[100];
	int level = 1;
	sprintf(levelString, "Level: %d", level);
	PtSetResource(ABW_level, Pt_ARG_TEXT_STRING, levelString, 0);
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	return( Pt_CONTINUE );

	}

