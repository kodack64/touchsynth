
#include "MyPackage.h"
#include "Common.h"

#define SAMPLE_RATE 44100

int MyPackage::Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi){
	gapi=_gapi;
	iapi=_iapi;
	aapi=_aapi;
	mapImage[0]=(new MapImage("Image\\map\\logos.png",gapi));
	mapImage[1]=(new MapImage("Image\\map\\posters.png",gapi));
	mapImage[2]=(new MapImage("Image\\map\\posterrs.png",gapi));
	mapImage[3]=(new MapImage("Image\\map\\masks.png",gapi));
	mapImage[4]=(new MapImage("Image\\map\\battings.png",gapi));
	mapImage[5]=(new MapImage("Image\\map\\maps.jpg",gapi));
	hMutex=(HANDLE)CreateMutex(NULL,false,NULL);
	return 0;
}

int MyPackage::Reset(){
	for(int i=0;i<IMG_NUM;i++){
		mapImage[i]->Init(320+rand()%240-120,240+rand()%200-100,0.2f,1.0f*(rand()%180)/180*PI,i);
	}
	return 0;
}
int MyPackage::Close(){
	for(unsigned int i=0;i<IMG_NUM;i++){
		SAFE_DELETE(mapImage[i]);
	}
	return 0;
}
int MyPackage::End(){
	return 0;
}
int MyPackage::Run(){
	return 0;
}

int MyPackage::GraphicCallback(){
	WaitForSingleObject(hMutex,INFINITE);
	for(int i=IMG_NUM-1;i>=0;i--){
		for(UINT j=0;j<IMG_NUM;j++){
			if(mapImage[j]->GetPriority()==i){
				mapImage[j]->Draw();
				break;
			}
		}
	}
	ReleaseMutex(hMutex);
	return 0;
}

int MyPackage::AudioCallback(float* in,float* out,int frames){
	int i;
	float left,right;
	for(i=0;i<frames;i++){
		left=right=0.0f;
		aapi->GetWaveDataAll(&left,&right);
		*out++ = left;
		*out++ = right;
	}
	return 0;
}

int MyPackage::InputCallback(TOUCH_EVENT ev,int id,int x,int y){
	WaitForSingleObject(hMutex,INFINITE);
	switch(ev){
	case TOUCH_ON:
		bool flag;
		flag=true;
		tc[id].x=x;
		tc[id].y=y;
		tc[id].point=-1;
		for(int i=0;i<IMG_NUM && flag;i++){
			for(UINT j=0;j<IMG_NUM;j++){
				if(mapImage[j]->GetPriority()==i){
					if(mapImage[j]->IsThere(x,y)==1){
						tc[id].point=j;
						mapImage[j]->Add(id);
						if(i!=0){
							for(UINT k=0;k<IMG_NUM;k++){
								if(mapImage[k]->GetPriority()<i){
									mapImage[k]->SetPriority(mapImage[k]->GetPriority()+1);
								}
							}
						}
						mapImage[j]->SetPriority(0);
						flag=false;
					}
					break;
				}
			}
		}
		break;
	case TOUCH_CONTINUE:
		int dx,dy,i;
		dx=x-tc[id].x;
		dy=y-tc[id].y;
		if(dx==0 && dy==0)break;
		i=tc[id].point;
		if(i!=-1){
			if(mapImage[i]->GetTouchedNum()==1){
				//ˆÚ“®
				if(hypot(dx,dy)<100)
					mapImage[i]->Move(dx,dy);
			}else if(mapImage[i]->GetTouchedNum()==2){
				//‰ñ“]AŠgk
				float vx,vy;
				float dv,rt;
				int pa;
				pa=mapImage[i]->GetAnother(id);
				if(pa!=-1){
					vx=(float)((x-tc[pa].x)/hypot(x-tc[pa].x,y-tc[pa].y));
					vy=(float)((y-tc[pa].y)/hypot(x-tc[pa].x,y-tc[pa].y));
					dv=vx*dx+vy*dy;
					rt=vy*dx-vx*dy;
					mapImage[i]->Zooming(dv);
					mapImage[i]->Rotate((float)(rt/hypot(x-tc[pa].x,y-tc[pa].y)));
				}
			}
		}
		tc[id].x=x;
		tc[id].y=y;
		break;
	case TOUCH_OFF:
		if(tc[id].point!=-1){
			mapImage[tc[id].point]->Del(id);
		}
		tc.erase(id);
		break;
	}
	ReleaseMutex(hMutex);
	return 0;
}
