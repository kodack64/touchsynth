
#pragma once


typedef int ImageId;

class Color4f{
public:
	Color4f(){
		red=green=blue=alpha=0;
	}
	Color4f(int r,int g,int b,int a){
		red=r;
		green=g;
		blue=b;
		alpha=a;
	}
	int red;
	int green;
	int blue;
	int alpha;
};

class Image{
private:
protected:
	ImageId myId;
public:
	virtual bool Load(const char* filename,Color4f colorKey)=0;
	virtual void Draw(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color)=0;
};
