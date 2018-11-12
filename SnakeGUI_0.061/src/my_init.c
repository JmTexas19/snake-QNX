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
#include "playerState.h"

PhImage_t *images[SIZE];
int media_index =0;

char *imageDir = "/mplayer/images/";
char *musicDir = "/mplayer/musics/";

char *songs[] = {"coffee.wav", "wolf.wav", "sample.wav"};
char *imgNames[] = {"coffee.jpg", "wolf.jpg", "sample.jpg"};


int
my_init( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;
	    
	int i=0;
	for (i=0; i<SIZE; i++){
		PhImage_t *p;
		char imgName[50];
		strcpy(imgName, imageDir);
		strcat(imgName, imgNames[i]);

		images[i] = p;
	}

	media_index = 0;

	return( Pt_CONTINUE );

	}

