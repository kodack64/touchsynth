
#pragma once

#include "package.h"
#include "MapImage.h"
#include "MapTouch.h"

#define IMG_NUM 6

class MyPackage:public Package{
private:
	ImageId img_map;
	int x;
	int y;
	float zoom;
	float rot;
	MapImage* mapImage[IMG_NUM];
	map<int,MapTouch>tc;
	HANDLE hMutex;

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