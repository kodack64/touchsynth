
#pragma once

#include "Common.h"
#include "b2Objects.h"
#include "GraphicApi.h"
#include "InputApi.h"
#include <Box2D\Box2D.h>

typedef struct{  
   float x, y, z;
   float rhw;
   DWORD dwColor;
   float u,v;
}CUSTOMVERTEX,*LPCUSTOMVERTEX;

#define FVF_CUSTOM ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )


class b2Factory{
private:
	b2World* world;
	GraphicApi* gapi;
	IDirect3DVertexBuffer9* pVertex;
	LPDIRECT3DDEVICE9 lpd3ddev;
	ImageId img_ball;
	ImageId img_bar;
	ImageId img_wall;
	ImageId img_particle;
	ImageId img_number[10];
	ImageId img_win;
	ImageId img_lose;
	ImageId img_draw;
	int bodyCount;
	int rightScore;
	int leftScore;
	int frameCount;
	int rightBarCount;
	int leftBarCount;
public:
	b2Factory();
	virtual ~b2Factory();
	virtual int Init(GraphicApi*);
	virtual int Run();
	virtual int Draw();
	virtual int CreateWall(int width,int height,int r);
	virtual int CreateBall(b2Vec2 pos,int r);
	virtual int CreateBall(b2Vec2 pos,b2Vec2 vec,int r);
	virtual int CreateBar(int x0,int y0,int x1,int y1,bool flag);
	virtual int Reset();
	virtual int DestroyAll();
};