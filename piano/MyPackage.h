
#pragma once

#include "package.h"

#define PF_NUM 50

class MyPackage:public Package{
private:
	int count;
	ImageId img_piano;
	ImageId img_touch;
	AudioId sndPiano[PF_NUM];
	int posPiano[PF_NUM][5];
	int touchPitch[MAX_TOUCH];
	int pressCount[PF_NUM];
	virtual int GetPianoPitch(int x,int y);
	vector <TouchCursor> touch;


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