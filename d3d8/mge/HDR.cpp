#define _HDR
#include "Configuration.h"
#include "d3d8header.h"


#define CHAINLENGTH 9

IDirect3DSurface* rend[CHAINLENGTH];
//float ReactionSpeed;
bool HDR_Initialized=false;
DWORD LastTime;

RECT rChain;

void HDR_Init() {
	HDR_Initialized=true;
	HDR.x=HDR.y=HDR.z=HDR.w=0;
	LastTime=GetTickCount();
	//Create rects
	D3DSURFACE_DESC BackbufferDesc;
	BackBuffer->GetDesc(&BackbufferDesc);
    rChain.left=BackbufferDesc.Width/4;
    rChain.right=BackbufferDesc.Width-rChain.left;
    rChain.top=BackbufferDesc.Height/4;
    rChain.bottom=BackbufferDesc.Height-rChain.top;
    HRESULT hr=0;
    for(int i=0;i<CHAINLENGTH-1;i++) {
        UINT size=(UINT)pow((float)2,(CHAINLENGTH-(i+1)));
        hr|=RealDevice->CreateRenderTarget(size,size,BackbufferDesc.Format,D3DMULTISAMPLE_NONE,0,false,&rend[i],0);
    }
	hr|=RealDevice->CreateRenderTarget(1,1,BackbufferDesc.Format,D3DMULTISAMPLE_NONE,0,true,&rend[CHAINLENGTH-1],0);
	if(hr!=D3D_OK) ERRORMSG("HDR: Stretch chain creation failed");
}
void HDR_Update(DWORD Time) {
	float TimePassed=(float)(Time-LastTime)/1000.0f;
	LastTime=Time;
	if(!HDR_Initialized) HDR_Init();
	else {
		// HDR calculation moved here for better performance
		float r,g,b;
		D3DLOCKED_RECT rect;
		rend[CHAINLENGTH-1]->LockRect(&rect,0,D3DLOCK_READONLY);
		BYTE* data=(BYTE*)rect.pBits;
		r=((float)data[0]/255.0f);
		g=((float)data[1]/255.0f);
		b=((float)data[2]/255.0f);
		rend[CHAINLENGTH-1]->UnlockRect();
		HDR.w=(0.27f*r)+(0.67f*g)+(0.06f*b);
	}
	RealDevice->StretchRect(BackBuffer,&rChain,rend[0],0,D3DTEXF_LINEAR);
	for(int i=0;i<CHAINLENGTH-1;i++) RealDevice->StretchRect(rend[i],0,rend[i+1],0,D3DTEXF_LINEAR);
	//HDR.x+=(HDR.w-HDR.x)*ReactionSpeed*TimePassed;
	HDR.x=HDR.w + (HDR.x-HDR.w)*exp(-ReactionSpeed*TimePassed);
	HDR.y=HDR.z=HDR.x;
}


