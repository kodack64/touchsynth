#pragma once

#include "GraphicApi.h"
#include "GLImage.h"

class GLAPI:public GraphicApi{
private:
	static GLAPI* myInstance;
	bool isEndFlag;

	int fwidth;
	int fheight;
	int fcolorBit;
	int frefreshRate;

	HWND hWnd;
	bool gamemode;
	int wndid;
	vector <GLImage*> myImage;
	string name;
public:
	GLAPI();
	virtual ~GLAPI();
	static GLAPI* createInstance();
	static void destroyInstance();
	virtual bool Init(int width,int height,int winx,int winy,int gamemode,string name);
	virtual bool Close();
	virtual bool Run();
	static void LaunchDisplay(void);
	static void LaunchTimer(int);
	static void LaunchClose();
	static void LaunchKeyboard(unsigned char,int,int);

	virtual ImageId CreateImageFromFile(const char*);
	virtual void DrawImage(int x,int y,ImageId id);
	virtual void ReleaseImage(ImageId id);
};