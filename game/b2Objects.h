#pragma once

#include "Common.h"

#define MAX_BACK 20
#define MAX_PARTICLE 50
#define PARTICLE_VANISH_TIME 60
#define PARTICLE_VISCOSITY (0.99f)
#define MAX_BAR 4

typedef enum{
	BALL,
	BOX,
	BAR,
	PARTICLE
}OBJECT_TYPE;

class Base{
public:
	OBJECT_TYPE kind;
};

class CBox:public Base{
public:
	CBox(int _x0,int _y0,int _x1,int _y1){
		x0=_x0;
		y0=_y0;
		x1=_x1;
		y1=_y1;
		kind=BOX;
	}
	int x0;
	int x1;
	int y0;
	int y1;
};

class CBall:public Base{
public:
	CBall(int _x,int _y,int _r){
		kind=BALL;
		r=_r;
		flag=false;
		memset(bx,0,sizeof(bx));
		memset(by,0,sizeof(by));
		for(int i=0;i<MAX_BACK;i++){
			bx[i]=_x;
			by[i]=_y;
		}
	}
	int r;
	bool flag;
	int bx[MAX_BACK];
	int by[MAX_BACK];
};

class CBar:public Base{
public:
	CBar(int _x0,int _y0,int _x1,int _y1,bool _flag,int _life){
		kind=BAR;
		x0=_x0;
		y0=_y0;
		x1=_x1;
		y1=_y1;
		flag=_flag;
		life=_life;
	}
	double x0;
	double x1;
	double y0;
	double y1;
	bool flag;
	int life;
};

class CParticle:public Base{
public:
	CParticle(double _vx,double _vy,double _rot,double _zoom){
		kind=PARTICLE;
		x=y=q=0;
		vx=_vx;
		vy=_vy;
		rot=_rot;
		zoom=_zoom;

		vis=PARTICLE_VISCOSITY;
		rest=PARTICLE_VANISH_TIME;
		life=PARTICLE_VANISH_TIME;
	}
	double x;
	double y;
	double vx;
	double vy;
	double vis;
	double q;
	double rot;
	double zoom;
	int rest;
	int life;
};
