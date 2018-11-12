/*
 * playerState.h
 *
 */

#ifndef PLAYERSTATE_H_
#define PLAYERSTATE_H_

#define SIZE 3

int paused;
int stopped;
int inFunction;


extern PhImage_t *images[];
extern int media_index;

extern char *imageDir;
extern char *musicDir;

extern char *songs[];
extern char *imgNames[];

void *playingWave();

#endif
