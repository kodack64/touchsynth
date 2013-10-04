
#include "b2Factory.h"

#define AMPL 50
#define M2P(x) (x*AMPL)
#define P2M(x) (x/AMPL)

#define MAX_BAR_LIFE 60


#pragma once
#include "Common.h"

void MoveMatrix(LPCUSTOMVERTEX v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, MoveMatrix;
    int i;
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&MoveMatrix);
    D3DXMatrixTranslation(&MoveMatrix, x, y, z);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= MoveMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
}

void ZoomMatrix(LPCUSTOMVERTEX v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, ZoomMatrix;
    float x1, y1;
    int i;
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrix(v, -x1, -y1, 0.0f);
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&ZoomMatrix);
    D3DXMatrixScaling(&ZoomMatrix, x, y, z);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= ZoomMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
    MoveMatrix(v, x1, y1, 0.0f);
}

void RotateMatrix(LPCUSTOMVERTEX v, float Rotate)
{
    D3DXMATRIX PosMatrix, RotateMatrix;
    float x1, y1;
    int i;
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrix(v, -x1, -y1, 0.0f);
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&RotateMatrix);
    D3DXMatrixRotationZ(&RotateMatrix, (float)Rotate);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= RotateMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
    MoveMatrix(v, x1, y1, 0.0f);
}

void MoveMatrixLU(LPCUSTOMVERTEX v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, MoveMatrix;
    int i;
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&MoveMatrix);
    D3DXMatrixTranslation(&MoveMatrix, x, y, z);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= MoveMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
}

void ZoomMatrixLU(LPCUSTOMVERTEX v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, ZoomMatrix;
    float x1, y1;
    int i;
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrixLU(v, -x1, -y1, 0.0f);
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&ZoomMatrix);
    D3DXMatrixScaling(&ZoomMatrix, x, y, z);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= ZoomMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
    MoveMatrixLU(v, x1*x, y1*y, 0.0f*z);
}

void RotateMatrixLU(LPCUSTOMVERTEX v, float Rotate)
{
    D3DXMATRIX PosMatrix, RotateMatrix;
    float x1, y1;
    int i;
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrixLU(v, -x1, -y1, 0.0f);
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&RotateMatrix);
    D3DXMatrixRotationZ(&RotateMatrix, (float)Rotate);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= RotateMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
    MoveMatrixLU(v, x1, y1, 0.0f);
}

b2Factory::b2Factory()
:gapi(NULL)
,world(NULL)
{
}

b2Factory::~b2Factory(){
	b2Body* body=world->GetBodyList();
	for(;;body=body->GetNext()){
		if(body==NULL){
			break;
		}else{
			if(body->GetUserData()!=NULL){
				delete body->GetUserData();
				body->SetUserData(NULL);
			}
		}
	}
	SAFE_DELETE(world);
}

int b2Factory::Init(GraphicApi* _gapi){
	gapi=_gapi;

	/*Load Graphic*/
	img_ball=gapi->CreateImageFromFile("Image\\piano\\particle0.png");
	img_bar=gapi->CreateImageFromFile("Image\\piano\\particle0.png");
	img_wall=gapi->CreateImageFromFile("Image\\piano\\particle0.png");
	img_particle=gapi->CreateImageFromFile("Image\\piano\\particle0.png");
	for(int i=0;i<10;i++){
		char buf[256];
		sprintf_s(buf,256,"Image\\game\\n%d.jpg",i);
		img_number[i]=gapi->CreateImageFromFile(buf,Color4f(0,0,0,255));
	}
	img_win=gapi->CreateImageFromFile("Image\\game\\win.jpg",Color4f(0,0,0,255));
	img_lose=gapi->CreateImageFromFile("Image\\game\\lose.jpg",Color4f(0,0,0,255));
	img_draw=gapi->CreateImageFromFile("Image\\game\\draw.jpg",Color4f(0,0,0,255));

	world=new b2World(b2Vec2(0.0f,0.0f),true);
	bodyCount=0;

	lpd3ddev=gapi->GetDirect3DDevice();
	if(FAILED(lpd3ddev->CreateVertexBuffer( sizeof(CUSTOMVERTEX)*4, D3DUSAGE_WRITEONLY, FVF_CUSTOM, D3DPOOL_MANAGED, &pVertex, NULL))){
		return -1;
	}

	return 0;
}

int b2Factory::Run(){


	if(frameCount==60*3){
		CreateBall(b2Vec2((float)gapi->GetScreenWidth()/2,(float)gapi->GetScreenHeight()/2),10);
	}
	if(frameCount==60*13){
		CreateBall(b2Vec2((float)gapi->GetScreenWidth()/2,(float)gapi->GetScreenHeight()/2),10);
	}
	if(frameCount==60*23){
		CreateBall(b2Vec2((float)gapi->GetScreenWidth()/2,(float)gapi->GetScreenHeight()/2),10);
	}
	if(frameCount==60*33){
		CreateBall(b2Vec2((float)gapi->GetScreenWidth()/2,(float)gapi->GetScreenHeight()/2),10);
	}
	if(frameCount==60*43){
		CreateBall(b2Vec2((float)gapi->GetScreenWidth()/2,(float)gapi->GetScreenHeight()/2),10);
	}
	if(frameCount==60*53){
		CreateBall(b2Vec2((float)gapi->GetScreenWidth()/2,(float)gapi->GetScreenHeight()/2),10);
	}
	if(frameCount==60*53+1){
		CreateBall(b2Vec2((float)gapi->GetScreenWidth()/2,(float)gapi->GetScreenHeight()/2),10);
	}
	if(frameCount==60*53+2){
		CreateBall(b2Vec2((float)gapi->GetScreenWidth()/2,(float)gapi->GetScreenHeight()/2),10);
	}
	int time=(63-frameCount/60);
	if(time<=0)DestroyAll();
	frameCount++;
	if(time<=-7){
		return 1;
	}

	world->Step(1.0f/60.0f,6,2);
	world->ClearForces();

	vector <b2Body*> dead;
	dead.clear();
	b2Body* body=world->GetBodyList();
	b2Body* lastBody=NULL;
	while(body!=NULL){
		OBJECT_TYPE kind=((Base*)body->GetUserData())->kind;
		if(kind==BALL){
			//ボールがシュートされた
			if(M2P(body->GetPosition().x)<0){
				dead.push_back(body);
				CreateBall(b2Vec2(gapi->GetScreenWidth()/2.0f,gapi->GetScreenHeight()/2.0f),10);
				rightScore++;
			}
			else if(M2P(body->GetPosition().x) > gapi->GetScreenWidth() ){
				dead.push_back(body);
				CreateBall(b2Vec2(gapi->GetScreenWidth()/2.0f,gapi->GetScreenHeight()/2.0f),10);
				leftScore++;
			}
		}
		else if(kind==BAR){
			CBar* bar=(CBar*)(body->GetUserData());
			bar->life--;
			if(bar->life<=0){
				dead.push_back(body);
				if(bar->flag){
					leftBarCount--;
				}else{
					rightBarCount--;
				}
			}
		}
		body=body->GetNext();
	}
	for(unsigned int i=0;i<dead.size();i++){
		if(dead[i]->GetUserData()!=NULL){
			delete dead[i]->GetUserData();
			dead[i]->SetUserData(NULL);
		}
		world->DestroyBody(dead[i]);
	}
	return 0;
}

int b2Factory::Draw(){
	CBall* ball;
	const float par=0.1f;

	//秒数
	int time=(63-frameCount/60);
	Color4f ctm(150,150,150,150);
	Color4f csc(150,150,150,150);
	Color4f cscs(196,196,196,255);
	if(0<=time && time<=63){
		if(time>60)time=time%60;
		gapi->DrawImageLU(290*4,220*4,0,0,0,0,0.25f,0.0f,ctm,img_number[time/10%10]);
		gapi->DrawImageLU(330*4,220*4,0,0,0,0,0.25f,0.0f,ctm,img_number[time%10]);
	}
	if(0>time){
		gapi->DrawImageLU(290*4,220*4,0,0,0,0,0.25f,0.0f,ctm,img_number[0]);
		gapi->DrawImageLU(330*4,220*4,0,0,0,0,0.25f,0.0f,ctm,img_number[0]);
	}
	//スコア
	int rsc[3];
	int lsc[3];
	int rscore;
	int lscore;
	rscore=rightScore;
	lscore=leftScore;
	for(int i=0;i<3;i++){
		rsc[i]=rscore%10;
		lsc[i]=lscore%10;
		rscore/=10;
		lscore/=10;
	}
	if(time>=0){
		for(int i=0;i<3;i++){
		gapi->DrawImageLU(-30,60+i*140,0,0,0,0,1.0f,PI/2,csc,img_number[rsc[2-i]]);
		gapi->DrawImageLU(420,-120+i*140,0,0,0,0,1.0f,-PI/2,csc,img_number[lsc[i]]);
		}
	}else{
		if((-time)%2==0){
			for(int i=0;i<3;i++){
				gapi->DrawImageLU(-30,60+i*140,0,0,0,0,1.0f,PI/2,cscs,img_number[rsc[2-i]]);
				gapi->DrawImageLU(420,-120+i*140,0,0,0,0,1.0f,-PI/2,cscs,img_number[lsc[i]]);
			}
		}else{
			ImageId rid,lid;
			if(rightScore==leftScore){
				rid=lid=img_draw;
			}else if(rightScore<leftScore){
				rid=img_win;
				lid=img_lose;
			}else{
				rid=img_lose;
				lid=img_win;
			}
			gapi->DrawImageLU(-330,80,0,0,0,0,1.0f,PI/2,cscs,rid);
			gapi->DrawImageLU(450,-100,0,0,0,0,1.0f,-PI/2,cscs,lid);
		}
	}


	b2Body* body=world->GetBodyList();
	while(body!=NULL){
		OBJECT_TYPE kind=((Base*)body->GetUserData())->kind;
		if(kind==BALL){
			ball=(CBall*)body->GetUserData();

			for(int i=0;i+1<MAX_BACK;i++){
				ball->bx[i]=ball->bx[i+1];
				ball->by[i]=ball->by[i+1];
			}
			ball->bx[MAX_BACK-1]=(int)M2P(body->GetPosition().x);
			ball->by[MAX_BACK-1]=(int)M2P(body->GetPosition().y);

			for(int i=MAX_BACK-1;i>=0;i--){
				Color4f ballColor(255,0,255,(int)(255.0*i/MAX_BACK));
				ballColor.blue=(int)(255*cos(PI/2*(ball->bx[i])/gapi->GetScreenWidth()));
				ballColor.red=(int)(255*sin(PI/2*(ball->bx[i])/gapi->GetScreenWidth()));
				gapi->DrawImage((int)(ball->bx[i]),(int)(ball->by[i]),
					0,0,0,0,
					0.05f+0.05f*i/MAX_BACK
					,0.0f,ballColor,img_ball);
			}
		}else if(kind==BOX){
		}else if(kind==BAR){
			float x0,y0,x1,y1;
			bool flag;
			int divide;
			divide=8;
			x0=(float)((CBar*)body->GetUserData())->x0;
			y0=(float)((CBar*)body->GetUserData())->y0;
			x1=(float)((CBar*)body->GetUserData())->x1;
			y1=(float)((CBar*)body->GetUserData())->y1;
			flag=((CBar*)body->GetUserData())->flag;
			for(int j=0;j<=divide;j++){
				gapi->DrawImage(
					(int)((x0*j+x1*(divide-j))/divide),
					(int)((y0*j+y1*(divide-j))/divide),
					0,
					0,
					0,
					0,
					par,
					0,
					Color4f(flag?0:255,0,flag?255:0,(int)(((CBar*)(body->GetUserData()))->life*255.0f/MAX_BAR_LIFE)),
					img_bar);
			}
		}

		body=body->GetNext();
	}
	return 0;
}

int b2Factory::CreateBar(int x0,int y0,int x1,int y1,bool flag){
	if(flag){
		if(leftBarCount>=MAX_BAR){
			return 0;
		}else{
			leftBarCount++;
		}
	}else{
		if(rightBarCount>=MAX_BAR){
			return 0;
		}else{
			rightBarCount++;
		}
	}
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	b2Body* body;
	b2CircleShape circleShape;
	b2PolygonShape polygon;
	b2Fixture* fixture;

	bodyDef.type = b2_kinematicBody;
	bodyDef.userData=(void*)(new CBar(x0,y0,x1,y1,flag,MAX_BAR_LIFE));
	body=world->CreateBody(&bodyDef);

	float dist=(float)hypot(x1-x0,y1-y0);
	polygon.SetAsBox(
			P2M((float)dist/2),
			P2M((float)5),
			b2Vec2(P2M(((float)(x0+x1))/2), P2M(((float)(y0+y1))/2)),
			atan2((float)y1-y0,(float)x1-x0)
		);

	fixtureDef.shape=&polygon;
	fixtureDef.restitution=1.1f;
	fixture=body->CreateFixture(&fixtureDef);
	return bodyCount;
}

int b2Factory::CreateWall(int width,int height,int r){
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	b2Body* body;
	b2CircleShape circleShape;
	b2PolygonShape polygon;
	b2Fixture* fixture;

	/*make box*/
	bodyDef.type = b2_staticBody;
	fixtureDef.friction=0.0f;
	fixtureDef.restitution=1.0f;
	fixtureDef.shape=&polygon;

	bodyDef.userData=(void*)(new CBox(0,height-r,width,height+r));
	bodyDef.position.Set(P2M((float)width/2),P2M((float)height));
	body=world->CreateBody(&bodyDef);
	polygon.SetAsBox(P2M((float)width/2),P2M((float)r));
	fixture=body->CreateFixture(&fixtureDef);

	bodyDef.userData=(void*)(new CBox(0,-r,width,r));
	bodyDef.position.Set(P2M((float)width/2),P2M(0));
	body=world->CreateBody(&bodyDef);
	polygon.SetAsBox(P2M((float)width/2),P2M((float)r));
	fixture=body->CreateFixture(&fixtureDef);

/*	bodyDef.userData=(void*)(new CBox(width-r,0,width+r,height/3));
	bodyDef.position.Set(P2M((float)width),P2M((float)height/6));
	body=world->CreateBody(&bodyDef);
	polygon.SetAsBox(P2M((float)r),P2M((float)height/6));
	fixture=body->CreateFixture(&fixtureDef);

	bodyDef.userData=(void*)(new CBox(width-r,height/3*2,width+r,height));
	bodyDef.position.Set(P2M((float)width),P2M((float)height/6*5));
	body=world->CreateBody(&bodyDef);
	polygon.SetAsBox(P2M((float)r),P2M((float)height/6));
	fixture=body->CreateFixture(&fixtureDef);

	bodyDef.userData=(void*)(new CBox(-r,0,r,height/3));
	bodyDef.position.Set(P2M(0),P2M((float)height/6));
	body=world->CreateBody(&bodyDef);
	polygon.SetAsBox(P2M((float)r),P2M((float)height/6));
	fixture=body->CreateFixture(&fixtureDef);

	bodyDef.userData=(void*)(new CBox(-r,height/3*2,r,height));
	bodyDef.position.Set(P2M(0),P2M((float)height/6*5));
	body=world->CreateBody(&bodyDef);
	polygon.SetAsBox(P2M((float)r),P2M((float)height/6));
	fixture=body->CreateFixture(&fixtureDef);*/
	return 0;
}

int b2Factory::CreateBall(b2Vec2 pos,int r){
	CreateBall(pos,b2Vec2((rand()%2==0?1:-1)*((float)(200+rand()%50)),(rand()%2==0?1:-1)*((float)(rand()%200))),r);
	return 0;
}
int b2Factory::CreateBall(b2Vec2 pos,b2Vec2 vec,int r){
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	b2Body* body;
	b2CircleShape circleShape;
	b2PolygonShape polygon;
	b2Fixture* fixture;

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(P2M((float)pos.x),P2M((float)pos.y));
	bodyDef.linearVelocity=b2Vec2(P2M(vec.x),P2M(vec.y));
	bodyDef.userData=(void*)(new CBall((int)pos.x,(int)pos.y,r));
	body=world->CreateBody(&bodyDef);
	circleShape.m_radius=P2M((float)r);
	fixtureDef.shape=&circleShape;
	fixtureDef.density=1.0f;
	fixtureDef.restitution=1.01f;
	fixture=body->CreateFixture(&fixtureDef);
	return 0;
}

int b2Factory::DestroyAll(){
	b2Body* body=world->GetBodyList();
	for(;body!=NULL;){
		b2Body* sub=body->GetNext();
		if(body->GetUserData()!=NULL){
			delete body->GetUserData();
			body->SetUserData(NULL);
		}
		world->DestroyBody(body);
		body=sub;
	}
	leftBarCount=0;
	rightBarCount=0;
	return 0;
}

int b2Factory::Reset(){
	leftScore=0;
	rightScore=0;
	DestroyAll();
	bodyCount=0;
	frameCount=0;
	leftBarCount=0;
	rightBarCount=0;
	return 0;
}