/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 12 ];


#ifdef __cplusplus
extern "C" {
#endif
int newGame( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int pauseButton( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int start( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int upButton( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int rightButton( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int downButton( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int leftButton( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int snakeTimer( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int foodTimer( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif
