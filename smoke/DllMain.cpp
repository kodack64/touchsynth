
#include "MyPackage.h"
#include "Common.h"
#pragma comment (lib,"RFFTW2dll.lib")

#define SAMPLE_RATE 44100
#define MESH_SIZE 50


int MyPackage::Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi){
	gapi=_gapi;
	iapi=_iapi;
	aapi=_aapi;
	lpd3ddev=gapi->GetDirect3DDevice();

	meshsize = MESH_SIZE;
	timestep = 0.5f;
	viscosity = 0.008f;
	fluid=NULL;

	if(FAILED(lpd3ddev->CreateVertexBuffer( sizeof(CUSTOMVERTEX)*MESH_SIZE*2, D3DUSAGE_WRITEONLY, FVF_CUSTOM, D3DPOOL_MANAGED, &pVertex, NULL))){
		assert(false);
		return -1;
	}
	meshVertex = new CUSTOMVERTEX[MESH_SIZE*(MESH_SIZE-1)*2];
	snd_loop=aapi->LoadWaveFile("Audio\\smoke\\SampleLoop.wav");

	eq=new Equalizer;
	eq->Init(MAX_EQ,SAMPLE_RATE);
	return 0;
}
int MyPackage::Reset(){
	touchCursor.clear();
	touchColor.clear();
	phase=0.0f;
	freq=440.0f*pow(2.0f, ( 69 - 12 - 69.0f )/12 );
	frameSum=0;
	audioSwitch=0;
	aapi->Rewind(snd_loop);
//	aapi->SetState(snd_loop,Audio_LoopPlay);

	if(fluid!=NULL)delete fluid;
	fluid = new Fluid2D(meshsize,timestep,viscosity);

	return 0;
}


int MyPackage::Close(){
	aapi->ReleaseWave(snd_loop);
	SAFE_RELEASE(pVertex);
	SAFE_DELETE(fluid);
	eq->Close();
	SAFE_DELETE(eq);
	SAFE_DELETE_ARRAY(meshVertex);
	return 0;
}
int MyPackage::End(){
//	aapi->SetState(snd_loop,Audio_FadeOut);
	return 0;
}
int MyPackage::Run(){
	fluid->Evolve();
	return 0;
}

int MyPackage::GraphicCallback(){

	int i, j, idx;
	const int n = fluid->getMeshSize();
	fftw_real  wn = (fftw_real)gapi->GetScreenWidth() / (fftw_real)(n - 1);
	fftw_real  hn = (fftw_real)gapi->GetScreenHeight() / (fftw_real)(n - 1);
	fftw_real px, py;

	const fftw_real* r = fluid->getDensityFieldR();
	const fftw_real* g = fluid->getDensityFieldG();
	const fftw_real* b = fluid->getDensityFieldB();
	const fftw_real* u = fluid->getVelocityFieldX();
	const fftw_real* v = fluid->getVelocityFieldY();

	vertexCount=0;
	int lastVertexCount=0;

	for (i = 0; i < n - 1; i++)
	{
		lastVertexCount=vertexCount;

		j = 0;
		px =  (fftw_real)i * wn;
		py = (fftw_real)j * hn;
		idx = (j * n) + i;
		UpdateVertex((float)px,(float)py,0.0f,1.0f,(float)r[idx],(float)g[idx],(float)b[idx]);

		for (j = 0; j < n - 1; j++) {
			px =(fftw_real)(i + 1) * wn;
			py =(fftw_real)j * hn;
			idx = (j * n) + (i + 1);
			UpdateVertex((float)px,(float)py,0.0f,1.0f,(float)r[idx],(float)g[idx],(float)b[idx]);

			px =(fftw_real)i * wn;
			py =(fftw_real)(j + 1) * hn;
			idx = ((j + 1) * n) + i;
			UpdateVertex((float)px,(float)py,0.0f,1.0f,(float)r[idx],(float)g[idx],(float)b[idx]);
		}

		px =(fftw_real)(i + 1) * wn;
		py =(fftw_real)(n - 1) * hn;
		idx = ((n - 1) * n) + (i + 1);
		UpdateVertex((float)px,(float)py,0.0f,1.0f,(float)r[idx],(float)g[idx],(float)b[idx]);

		int updated=vertexCount-lastVertexCount;
		void *pData;
		if(FAILED(pVertex->Lock(0, sizeof(CUSTOMVERTEX)*updated, (void**)&pData, 0))){
			return -1;
		}
		memcpy(pData,meshVertex+lastVertexCount, sizeof(CUSTOMVERTEX)*updated);
		pVertex->Unlock();
		lpd3ddev->SetTexture(0,NULL);
		lpd3ddev->SetStreamSource(0, pVertex, 0, sizeof(CUSTOMVERTEX));
		lpd3ddev->SetFVF(FVF_CUSTOM);
		lpd3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,updated-2);
	}
	return 0;
}

int MyPackage::AudioCallback(float* in,float* out,int frames){
	int i;
	float left,right;
	frameSum+=frames;

	if(frameSum>SAMPLE_RATE/10){
		audioSwitch=0;
		frameSum=0;
		int num;
		iapi->GetCursorList(at,&num);
		int check[MAX_EQ];
		memset(check,0,sizeof(check));
		for(i=0;i<num && audioSwitch<MAX_EQ;i++){
			if(at[i].IsActive() && at[i].GetId()<MAX_EQ){
				eq->SetParam(
					at[i].GetId(),
					100+(at[i].GetX()*1.0f/gapi->GetScreenWidth())*10000,
					2.0f/sqrt(2.0f),
					(1.0f-at[i].GetY()*1.0f/gapi->GetScreenHeight())*4-2.0f);
				audioSwitch++;
				check[at[i].GetId()]=1;
			}
		}
		for(i=0;i<MAX_EQ;i++){
			if(check[i]==0)eq->SetDefault(i);
		}
	}

	for(i=0;i<frames;i++){
		left=right=0.0f;
		aapi->GetWaveDataAll(&left,&right);
		if(freq>0)phase+=2.0f*PI*freq/SAMPLE_RATE;
		if(phase>2.0f*PI)phase-=2.0f*PI;
		if(audioSwitch>0){
//			left=(phase-PI)/PI/1.5f;
//			right=(phase-PI)/PI/1.5f;
		}
//		left=sin(phase)/1.5f;
//		right=sin(phase)/1.5f;
		left*=0.5f;
		right*=0.5f;

		if(eq!=NULL)eq->Run(&left,&right);

		*out++ = left;
		*out++ = right;
//		rec.Next(left,right);
	}
	return 0;
}

int MyPackage::InputCallback(TOUCH_EVENT ev,int id,int x,int y){
	int nx=touchCursor[id].GetX();
	int ny=touchCursor[id].GetY();
	switch(ev){
	case TOUCH_ON:
		touchCursor[id].SetX(x);
		touchCursor[id].SetY(y);
		touchColor[id].alpha=255;
		touchColor[id].red=rand()%256;
		touchColor[id].green=rand()%256;
		touchColor[id].blue=rand()%256;
		break;
	case TOUCH_CONTINUE:
//		if((nx-x)!=0 || (ny-y)!=0){
			fluid->Drag(nx*1.0f/gapi->GetScreenWidth(),ny*1.0f/gapi->GetScreenHeight()
				,(x-nx)*1.0f/gapi->GetScreenWidth(),(y-ny)*1.0f/gapi->GetScreenHeight()
				,touchColor[id].red/255.0f*10.0f,touchColor[id].green/255.0f*10.0f,touchColor[id].blue/255.0f*10.0f);
			touchCursor[id].SetX(x);
			touchCursor[id].SetY(y);
//		}
		break;
	case TOUCH_OFF:
		touchCursor.erase(id);
		touchColor.erase(id);
		break;
	}
	return 0;
}

void MyPackage::UpdateVertex(float _x,float _y,float _z,float _rhw,float _r,float _g,float _b){
	meshVertex[vertexCount].x=_x;
	meshVertex[vertexCount].y=_y;
	meshVertex[vertexCount].z=_z;
	int ir,ig,ib;
	ir=(int)(sqrt(_r)/10*255);
	ig=(int)(sqrt(_g)/10*255);
	ib=(int)(sqrt(_b)/10*255);
	meshVertex[vertexCount].dwColor=D3DCOLOR_RGBA(ir,ig,ib,255);
	meshVertex[vertexCount].rhw=1.0f;
	meshVertex[vertexCount].u=_x/640;
	meshVertex[vertexCount].v=_y/480;
	vertexCount++;
}
