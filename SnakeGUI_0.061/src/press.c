/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <gulliver.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/termio.h>
#include <sys/types.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/asoundlib.h>
#include <Pt.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"
#include "buttonTiming.h"
#include "press.h"

//functions for audio handling
//consider using and revising the playingWave function in wavePlayer.c

const char *RIFF_Id = "RIFF";	
const char *WAVE_Id = "WAVE";
int inFunction;
float progress = 0;
int intProgress, totalSize;
snd_pcm_t *pcm_handle;
snd_mixer_t *mixer_handle;
FILE *waveFile;

extern PtWidget_t *ProgressBar;

//Struct for Tag Information
typedef struct {
	char tag[4];
	long length;			//Length of song
} RIFF_Tag;

//Struct for RIFF Header Information
typedef struct {
	char Riff[4];
	long Size;	
	char Wave[4];
} RIFF_Header;

//Struc for Wave Header Information
typedef struct {
	short FormatTag;
	short Channels;
	long SamplesPerSec;
	long AvgBytesPerSec;
	short BlockAlign;
	short BitsPerSample;
} WAVE_Header;

//Error Message
int err(char *msg) {
	perror(msg);
	return -1;
}

//Get Tags
int FindTag(FILE * fp, const char *tag) {
	int retVal;
	RIFF_Tag tagBfr = { "", 0 };

	retVal = 0;

	// Keep reading until we find the tag or hit the EOF.
	while (fread((unsigned char *) &tagBfr, sizeof(tagBfr), 1, fp)) {

		// If this is our tag, set the length and break.
		if (strncmp(tag, tagBfr.tag, sizeof tagBfr.tag) == 0) {
			retVal = ENDIAN_LE32 (tagBfr.length);
			break;
		}
		// Skip ahead the specified number of bytes in the stream
		fseek(fp, tagBfr.length, SEEK_CUR);
	}

	totalSize = tagBfr.length;

	// Return the result of our operation
	return (retVal);
}

//Checks Song file header

int CheckFileHeader(FILE * fp) {
	RIFF_Header riffHdr = { "", 0 };

	// Read the header and, if successful, play
	// the WAVE file.
	if (fread((unsigned char *) &riffHdr, sizeof(RIFF_Header), 1, fp) == 0)
		return 0;
	
	//Compare two strings
	if (strncmp(riffHdr.Riff, RIFF_Id, strlen(RIFF_Id)) || strncmp(
			riffHdr.Wave, WAVE_Id, strlen(WAVE_Id)))
		return -1;

	return 0;
}

//Play the music file
void *playingWave() {
	int card = -1;
	int dev = 0;
	WAVE_Header wavHdr1;
	int mSamples;
	int mSampleRate;
	int mSampleChannels;
	int mSampleBits;
	char *mSampleBfr1;
	int fragsize = -1;

	int rtn;
	snd_pcm_channel_info_t pi;
	snd_mixer_group_t group;
	snd_pcm_channel_params_t pp;
	snd_pcm_channel_setup_t setup;
	int bsize, sizeRtn, N = 0;
	fd_set rfds, wfds;

	if (card == -1) {
		if ((rtn = snd_pcm_open_preferred(&pcm_handle, &card, &dev,
				SND_PCM_OPEN_PLAYBACK)) < 0)
			return err("device open");
	} else {
		if ((rtn = snd_pcm_open(&pcm_handle, card, dev, SND_PCM_OPEN_PLAYBACK))
				< 0)
			return err("device open error");
	}

	char songName[50];
	char *musicDir = "/mplayer/musics/";		//Music Directory 
	
	//Create string directory
	strcpy(songName, musicDir);				
	strcat(songName, soundFile);
	printf("Song Directory: %s\n", songName);

	//If song is not found, return error
	if ((waveFile = fopen(songName, "r")) == 0)
		return err("file open #1");

	if (CheckFileHeader(waveFile) == -1)
		return err("CheckHdr #1");

	mSamples = FindTag(waveFile, "fmt ");
	fread(&wavHdr1, sizeof(wavHdr1), 1, waveFile);
	//fseek(waveFile, (mSamples - sizeof(WAVE_Header)), SEEK_CUR);

	mSampleRate = ENDIAN_LE32 (wavHdr1.SamplesPerSec);
	mSampleChannels = ENDIAN_LE16 (wavHdr1.Channels);
	mSampleBits = ENDIAN_LE16 (wavHdr1.BitsPerSample);

	printf("SampleRate = %d, Channels = %d, SampleBits = %d, Samples = %d\n",
			mSampleRate, mSampleChannels, mSampleBits, mSamples);

	/* disabling mmap is not actually required in this example but it is included to
	 * demonstrate how it is used when it is required.
	 */
	if ((rtn = snd_pcm_plugin_set_disable(pcm_handle, PLUGIN_DISABLE_MMAP)) < 0) {
		fprintf(stderr, "snd_pcm_plugin_set_disable failed: %s\n",
				snd_strerror(rtn));
		return -1;
	}

	memset(&pi, 0, sizeof(pi));
	pi.channel = SND_PCM_CHANNEL_PLAYBACK;
	if ((rtn = snd_pcm_plugin_info(pcm_handle, &pi)) < 0) {
		fprintf(stderr, "snd_pcm_plugin_info failed: %s\n", snd_strerror(rtn));
		return -1;
	}

	memset(&pp, 0, sizeof(pp));

	pp.mode = SND_PCM_MODE_BLOCK;
	pp.channel = SND_PCM_CHANNEL_PLAYBACK;
	pp.start_mode = SND_PCM_START_FULL;
	pp.stop_mode = SND_PCM_STOP_STOP;

	pp.buf.block.frag_size = pi.max_fragment_size;
	if (fragsize != -1)
	pp.buf.block.frag_size = fragsize;
	pp.buf.block.frags_max = 1;
	pp.buf.block.frags_min = 1;

	pp.format.interleave = 1;
	pp.format.rate = mSampleRate;
	pp.format.voices = mSampleChannels;

	if (ENDIAN_LE16(wavHdr1.FormatTag) == 6)
		pp.format.format = SND_PCM_SFMT_A_LAW;
	else if (ENDIAN_LE16(wavHdr1.FormatTag) == 7)
		pp.format.format = SND_PCM_SFMT_MU_LAW;
	else if (mSampleBits == 8)
		pp.format.format = SND_PCM_SFMT_U8;
	else if (mSampleBits == 24)
		pp.format.format = SND_PCM_SFMT_S24;
	else
		pp.format.format = SND_PCM_SFMT_S16_LE;

	if ((rtn = snd_pcm_plugin_params(pcm_handle, &pp)) < 0) {
		fprintf(stderr, "snd_pcm_plugin_params failed: %s\n", snd_strerror(rtn));
		return -1;
	}

	if ((rtn = snd_pcm_plugin_prepare(pcm_handle, SND_PCM_CHANNEL_PLAYBACK))
			< 0)
		fprintf(stderr, "snd_pcm_plugin_prepare failed: %s\n",
				snd_strerror(rtn));

	memset(&setup, 0, sizeof(setup));
	memset(&group, 0, sizeof(group));
	setup.channel = SND_PCM_CHANNEL_PLAYBACK;
	setup.mixer_gid = &group.gid;
	if ((rtn = snd_pcm_plugin_setup(pcm_handle, &setup)) < 0) {
		fprintf(stderr, "snd_pcm_plugin_setup failed: %s\n", snd_strerror(rtn));
		return -1;
	}
	printf("Format %s \n", snd_pcm_get_format_name(setup.format.format));
	printf("Frag Size %d \n", setup.buf.block.frag_size);
	printf("Rate %d \n", setup.format.rate);
	bsize = setup.buf.block.frag_size;

	if (group.gid.name[0] == 0) {
		printf("Mixer Pcm Group [%s] Not Set \n", group.gid.name);
		exit(-1);
	}
	printf("Mixer Pcm Group [%s]\n", group.gid.name);
	if ((rtn = snd_mixer_open(&mixer_handle, card, setup.mixer_device)) < 0) {
		fprintf(stderr, "snd_mixer_open failed: %s\n", snd_strerror(rtn));
		return -1;
	}

mSamples = FindTag(waveFile, "data");

	mSampleBfr1 = malloc(bsize);
	FD_ZERO (&rfds);
	FD_ZERO (&wfds);
	sizeRtn = 1;
	while (N < mSamples && sizeRtn > 0) {
	
	//Not needed for piano, no stop function
	/*
		while (paused == 1) {
			usleep(1000);
			if (stopped == 1) {
				onStopped();
				return;
			}
		}
	*/



		if (tcgetpgrp(0) == getpid())
			FD_SET (STDIN_FILENO, &rfds);
		FD_SET (snd_mixer_file_descriptor (mixer_handle), &rfds);
		FD_SET (snd_pcm_file_descriptor (pcm_handle, SND_PCM_CHANNEL_PLAYBACK), &wfds);

		rtn = max (snd_mixer_file_descriptor (mixer_handle),
				snd_pcm_file_descriptor (pcm_handle, SND_PCM_CHANNEL_PLAYBACK));

		if (select(rtn + 1, &rfds, &wfds, NULL, NULL) == -1)
			return err("select");

		if (FD_ISSET (snd_mixer_file_descriptor (mixer_handle), &rfds)) {
			snd_mixer_callbacks_t callbacks = { 0, 0, 0, 0 };

			snd_mixer_read(mixer_handle, &callbacks);
		}

		if (FD_ISSET (snd_pcm_file_descriptor (pcm_handle, SND_PCM_CHANNEL_PLAYBACK), &wfds)) {
			snd_pcm_channel_status_t status;
			int written = 0;

			if ((sizeRtn = fread(mSampleBfr1, 1, min (mSamples - N, bsize),
					waveFile)) <= 0)
				continue;
			written = snd_pcm_plugin_write(pcm_handle, mSampleBfr1, sizeRtn);
			if (written < sizeRtn) {
				memset(&status, 0, sizeof(status));
				status.channel = SND_PCM_CHANNEL_PLAYBACK;
				if (snd_pcm_plugin_status(pcm_handle, &status) < 0) {
					fprintf(stderr, "underrun: playback channel status error\n");
					exit(1);
				}

				if (status.status == SND_PCM_STATUS_READY || status.status
						== SND_PCM_STATUS_UNDERRUN) {
					if (snd_pcm_plugin_prepare(pcm_handle,
							SND_PCM_CHANNEL_PLAYBACK) < 0) {
						fprintf(stderr,
								"underrun: playback channel prepare error\n");
						exit(1);
					}
				}
				if (written < 0)
					written = 0;
				written += snd_pcm_plugin_write(pcm_handle, mSampleBfr1
						+ written, sizeRtn - written);
			}
			
			N += written;
			
//Not needed for piano, no progress bar
/*	
		progress = ((float) N / (float) totalSize) * 100;
		intProgress = (int) progress;

		PtEnter(Pt_EVENT_PROCESS_ALLOW);
		PtSetResource(ABW_ProgressBar, Pt_ARG_GAUGE_VALUE, intProgress, 0);
		PtLeave(Pt_EVENT_PROCESS_ALLOW);
*/

		}
	} // END WHILE LOOP

	sizeRtn = snd_pcm_plugin_flush(pcm_handle, SND_PCM_CHANNEL_PLAYBACK);

	rtn = snd_mixer_close(mixer_handle);
	rtn = snd_pcm_close(pcm_handle);
	fclose(waveFile);
	inFunction = 0;

}

int press( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	//consider start a new thread here to play the wave of the pressed button

	return ( Pt_CONTINUE );
}

