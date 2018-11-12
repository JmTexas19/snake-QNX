#ifndef __BTIMING_INCLUDED__
#define __BTIMING_INCLUDED__
#include <time.h>

double getButtonDiff();

//vars to store the time difference between button presses.

struct timespec btnBegin;
struct timespec btnEnd;

char key;


#endif
