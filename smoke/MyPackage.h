
#pragma once

#include "package.h"
#include "fluid2d.h"
#include "Equalizer.h"
#include "Recorder.h"

typedef struct{  
   float x, y, z;
   float rhw;
   DWORD dwColor;
   float u,v;
}CUSTOMVERTEX,*LPCUSTOMVERTEX;

#define FVF_CUSTOM ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

class MyPackage:public Package{
private:
	/*Graphic*/
	LPDIRECT3DDEVICE9 lpd3ddev;
	LPDIRECT3DVERTEXBUFFER9 pVertex;
	CUSTOMVERTEX* meshVertex;
	int vertexCount;
	virtual void UpdateVertex(float x,float y,float z,float rhw,float r,float g,float b);

	/*Fluid*/
	Fluid2D* fluid;
	int meshsize;
	float timestep;
	float viscosity;
	map <int,Color4f> touchColor;
	map <int,TouchCursor> touchCursor;

	/*Audio*/
	float phase;
	float freq;
	Equalizer* eq;
	long frameSum;
	TouchCursor at[MAX_TOUCH];
//	Recorder rec;
	int audioSwitch;
	AudioId snd_loop;
	
	/*Do Not Edit Below*/
private:
	static MyPackage* myInstance;
public:
	virtual int Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi);
	virtual int Reset();
	virtual int Close();
	virtual int End();
	virtual int Run();
	virtual int AudioCallback(float* in,float* out,int frames);
	virtual int GraphicCallback();
	virtual int InputCallback(TOUCH_EVENT ev,int id,int x,int y);

	static __declspec(dllexport) Package* CALLBACK CreateInstance();
	static __declspec(dllexport) void CALLBACK DestroyInstance();
	static __declspec(dllexport) void CALLBACK GetName(char* buf,int max);
};