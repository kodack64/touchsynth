
#include "MyPackage.h"
#include "Common.h"

#ifdef _DEBUG
#pragma comment(lib,"Box2D_d.lib")
#else
#pragma comment(lib,"Box2D.lib")
#endif

#define SAMPLE_RATE 44100

int MyPackage::Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi){
	gapi=_gapi;
	iapi=_iapi;
	aapi=_aapi;
	FILE* dfp;
	freopen_s(&dfp,"CONOUT$", "w", stdout);
	freopen_s(&dfp,"CONIN$", "r", stdin);

	srand((unsigned)time(NULL));

	/*Field Setting*/
	ballSize=10;
	merg=ballSize;
	half=gapi->GetScreenWidth()/2;
	maxBarLength=gapi->GetScreenHeight()/4;

	touchMutex=CreateMutex(NULL,false,NULL);

	b2m=new b2Factory;
	b2m->Init(gapi);

	img_back=gapi->CreateImageFromFile("Image\\game\\back.jpg",Color4f(0,0,0,0));
	return 0;
}

int MyPackage::Reset(){
	/*TouchSetting*/
	memset(tcUse,0,sizeof(tcUse));
	psl.clear();
	psr.clear();
	pslid.clear();
	psrid.clear();
	b2m->Reset();
	b2m->CreateWall(gapi->GetScreenWidth(),gapi->GetScreenHeight(),10);
	return 0;
}
int MyPackage::Close(){
	CloseHandle(touchMutex);
	b2m->DestroyAll();
	SAFE_DELETE(b2m);
	gapi->ReleaseImage(img_back);
	return 0;
}
int MyPackage::End(){
	return 0;
}
int MyPackage::Run(){
	//タッチによるバーの生成消滅処理
	WaitForSingleObject(touchMutex,INFINITE);
	if(b2m->Run()==1)Reset();
	ReleaseMutex(touchMutex);
	return 0;
}

int MyPackage::GraphicCallback(){
	WaitForSingleObject(touchMutex,INFINITE);
	gapi->DrawImageLU(0,0,img_back);
	b2m->Draw();
	ReleaseMutex(touchMutex);
	return 0;
}

int MyPackage::AudioCallback(float* in,float* out,int frames){
	int i;
	float left,right;
	for(i=0;i<frames;i++){
		left=right=0.0f;
//		aapi->GetWaveDataAll(&left,&right);
		*out++ = left;
		*out++ = right;
	}
	return 0;
}

int MyPackage::InputCallback(TOUCH_EVENT ev,int id,int x,int y){
	WaitForSingleObject(touchMutex,INFINITE);
	int pa=-1;
	long life=-1;
	assert(id<MAX_TOUCH);
	switch(ev){
	case TOUCH_ON:
		tc[id].SetActive(true);
		tc[id].SetX(x);
		tc[id].SetY(y);
		for(int i=0;i<MAX_TOUCH;i++){
			if(!tc[id].IsActive())continue;
			if(id==i)continue;
			if(abs(half-x)<merg)continue;
			if(abs(half-tc[i].GetX())<merg)continue;
			if(x<half-merg)if(!(tc[i].GetX()<half-merg))continue;
			if(x>half+merg)if(!(tc[i].GetX()>half+merg))continue;
			if(tcUse[i]!=1)continue;

			if(hypot(tc[i].GetX()-x,tc[i].GetY()-y)<maxBarLength){
				if(life==-1 || life>tc[i].GetLife()){
					life=tc[i].GetLife();
					pa=i;
				}
			}
		}
		if(pa==-1){
			tcUse[id]=1;
		}else{
			if(x<half-merg){
				b2m->CreateBar(tc[id].GetX(),tc[id].GetY(),tc[pa].GetX(),tc[pa].GetY(),true);
				tcUse[id]=2;
				tcUse[pa]=2;
			}
			else if(x>half+merg){
				b2m->CreateBar(tc[id].GetX(),tc[id].GetY(),tc[pa].GetX(),tc[pa].GetY(),false);
				tcUse[id]=2;
				tcUse[pa]=2;
			}else{
				tcUse[id]=1;
			}
		}
		break;
	case TOUCH_CONTINUE:
		tc[id].SetX(x);
		tc[id].SetY(y);
		if(tcUse[id]==1){
			int pa=-1;
			int dist=-1;
			for(int i=0;i<MAX_TOUCH;i++){
				if(!tc[i].IsActive())continue;
				if(tcUse[i]!=1)continue;
				if(id==i)continue;
				if(abs(half-x)<merg)continue;
				if(abs(half-tc[i].GetX())<merg)continue;
				if(x<half-merg)if(!(tc[i].GetX()<half-merg))continue;
				if(x>half+merg)if(!(tc[i].GetX()>half+merg))continue;
				if(hypot(tc[i].GetX()-x,tc[i].GetY()-y)>maxBarLength)continue;
				if(dist==-1 || dist>hypot(tc[i].GetX()-x,tc[i].GetY()-y)){
					dist=(int)hypot(tc[i].GetX()-x,tc[i].GetY()-y);
					pa=i;
				}
			}
			if(pa!=-1){
				if(x<half-merg){
					b2m->CreateBar(tc[id].GetX(),tc[id].GetY(),tc[pa].GetX(),tc[pa].GetY(),true);
					tcUse[id]=2;
					tcUse[pa]=2;
				}else if(x>half+merg){
					b2m->CreateBar(tc[id].GetX(),tc[id].GetY(),tc[pa].GetX(),tc[pa].GetY(),false);
					tcUse[id]=2;
					tcUse[pa]=2;
				}
			}
		}
		break;
	case TOUCH_OFF:
		tc[id].SetActive(false);
		tcUse[id]=0;
		break;
	}
	ReleaseMutex(touchMutex);
	return 0;
}