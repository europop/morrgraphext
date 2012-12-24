#ifndef _VIDEO_PLAYBACK_H_
#define _VIDEO_PLAYBACK_H_

#include "d3d8header.h"

	struct sDSTexture {
		IUnknown *pGraph;
		IUnknown *pBuild;
		IUnknown *pVmr;
		IUnknown *pConfig;
		IUnknown *pAlloc;
		IUnknown *pControl;
		IUnknown *pMyAlloc;
		IUnknown *pSeek;
	};

	//VideoPlayback
	sDSTexture* FreeMovie(sDSTexture* info);
	sDSTexture* CreateDSGraph(char* path,IDirect3DTexture9** tex);
	void ResumeMovie(sDSTexture* movie);
	void PauseMovie(sDSTexture* movie);
	void StopMovie(sDSTexture* movie);
	void RewindMovie(sDSTexture* movie);
	void SeekMovie(sDSTexture* movie,DWORD shortTime);

#endif