#pragma once

#include "Common.h"
#include "InputAdapter.h"

#include <cv.h>
#include <highgui.h>
#include <ctype.h>

class OpenCVAdapter:public InputAdapter{
private:
	static OpenCVAdapter* myInstance;
	
	CvCapture *capture;
	IplImage *frame;
	int cam_num;
	int width;
	int height;
	int fps;
	int xstep;
	int ystep;
	double thr;
	int trackbarMaxValue;

	int xnum;
	int ynum;
	int winx;
	int winy;

	int trackbarPosition;
	int trackbarPosition2;

	string windowNameCap;
	string windowNameThr;
	string trackbarName;
	string trackbarName2;

	unsigned char* label;
	short* labeled;

	int fc;

	vector < pair<int,int> > plist;

	HANDLE myHandle;
	bool f_init;
	bool f_loop;
	bool f_start;
	virtual void GetFrame();
	virtual void LabelFrame();
	virtual void ProcFrame();
	virtual void DebugFrame();
public:
	static OpenCVAdapter* CreateInstance();
	static void DestroyInstance();
	OpenCVAdapter(){};
	~OpenCVAdapter(){};

	virtual bool Init();
	virtual void Run();
	virtual void Stop();
	virtual void Close();
	static unsigned int CALLBACK Launch(void*);
	virtual void Loop();
	virtual void SetInputHandler(InputHandler*);
};
