/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

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

PtWidget_t *b1;
PtWidget_t *b2;
PtWidget_t *b3;

int
newGame( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ){	
	buildBlock(ABW_window);
//	startBGM();
	snakeDirection = 0;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	return( Pt_CONTINUE );

	}
