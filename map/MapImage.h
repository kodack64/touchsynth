
#pragma once

#include "Common.h"
#include "GraphicApi.h"

class MapImage{
private:
	int x;
	int y;
	int pr;
	float zoom;
	float rot;
	ImageId myImage;
	GraphicApi* gapi;
	vector <int> count;
public:
	MapImage(string,GraphicApi*);
	virtual ~MapImage();
	virtual int Init(int x,int y,float zoom,float rot,int pr);
	virtual int Draw();
	virtual int IsThere(int x,int y);
	virtual int GetPriority();
	virtual int SetPriority(int _p);
	virtual int Move(int dx,int dy);
	virtual int Zooming(float zm);
	virtual int Rotate(float rt);
	virtual int Add(int x);
	virtual int Del(int x);
	virtual int GetTouchedNum();
	virtual int GetAnother(int x);
};