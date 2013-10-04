
#include "MapImage.h"

MapImage::MapImage(string str,GraphicApi* _gapi){
	gapi=_gapi;
	myImage=gapi->CreateImageFromFile(str.c_str());
}

MapImage::~MapImage(){
	gapi->ReleaseImage(myImage);
}

int MapImage::Init(int _x,int _y,float _zoom,float _rot,int _pr){
	x=_x;
	y=_y;
	zoom=_zoom;
	rot=_rot;
	pr=_pr;
	count.clear();
	return 0;
}

int MapImage::IsThere(int tx,int ty){
	int mh=min(gapi->GetImageWidth(myImage),gapi->GetImageHeight(myImage))/2;
	if(hypot(tx-x,ty-y)<mh*zoom)return 1;
	else return 0;
}

int MapImage::Draw(){
	gapi->DrawImage(x,y,0,0,0,0,zoom,rot,myImage);
	return 0;
}

int MapImage::Move(int dx,int dy){
	x+=dx;
	y+=dy;
	return 0;
}
int MapImage::Zooming(float zm){
	zoom+=zm/min(gapi->GetScreenHeight(),gapi->GetScreenWidth());
	if(zoom<0.10f)zoom=0.10f;
	if(zoom>0.4f)zoom=0.4f;
	return 0;
}
int MapImage::Rotate(float rt){
	rot-=rt;
	if(rot>2*PI)rot-=2*PI;
	if(rot<-2*PI)rot+=2*PI;
	return 0;
}
int MapImage::Add(int x){
	for(UINT i=0;i<count.size();i++){
		if(count[i]==x)return 0;
	}
	count.push_back(x);
	return 0;
}
int MapImage::Del(int x){
	for(UINT i=0;i<count.size();i++){
		if(count[i]==x){
			count.erase(count.begin()+i);
			break;
		}
	}
	return 0;
}
int MapImage::GetTouchedNum(){
	return count.size();
}
int MapImage::GetAnother(int x){
	if(count.size()!=2)return -1;
	if(count[0]==x || count[1]==x){
		if(count[0]==x)return count[1];
		else return count[0];
	}else{
		return -1;
	}
}
int MapImage::GetPriority(){
	return pr;
}
int MapImage::SetPriority(int _p){
	pr=_p;return 0;
}


