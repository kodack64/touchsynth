#pragma once

#include "Package.h"

class MasterPackage:public Package{
private:
	static MasterPackage* myInstance;
	ImageId img_point;
	ImageId img_icon_piano;
	ImageId img_icon_map;
	ImageId img_icon_smoke;
	ImageId img_icon_game;
	HANDLE hMutex;
	int choice;

	map <int,int> touch;
	int GetChoiceFromVector(int x,int y);

	int activeCount[5];

public:
	virtual int Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi);
	virtual int Reset();
	virtual int Close();
	virtual int End();
	virtual int Run();
	virtual int AudioCallback(float* in,float* out,int frames);
	virtual int GraphicCallback();
	virtual int InputCallback(TOUCH_EVENT ev,int id,int x,int y);

	static MasterPackage* CreateInstance(){
		if(myInstance==NULL){
			myInstance=new MasterPackage;
		}
		return myInstance;
	};
	static void DestroyInstance(){
		SAFE_DELETE(myInstance);
	}
};