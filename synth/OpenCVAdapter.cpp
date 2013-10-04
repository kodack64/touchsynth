
#include "OpenCVAdapter.h"
#include "Logger.h"
#include "ApplicationProperty.h"
#include "Labeling.h"

#define DEF_CAMERA_ID 0
#define DEF_CAMERA_WIDTH 320
#define DEF_CAMERA_HEIGHT 240
#define DEF_CAMERA_FPS 60
#define DEF_CAMERA_STEP_X 1
#define DEF_CAMERA_STEP_Y 1
#define DEF_CAMERA_THRESHOLD 0.5

OpenCVAdapter* OpenCVAdapter::myInstance=NULL;

OpenCVAdapter* OpenCVAdapter::CreateInstance(){
	if(myInstance!=NULL)return myInstance;
	else{
		myInstance=new OpenCVAdapter;
		return myInstance;
	}
}
void OpenCVAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

void OpenCVAdapter::SetInputHandler(InputHandler* s){
	inputHandler=s;
}

bool OpenCVAdapter::Init(){
	Logger::Println("[OpenCVAdapter]	Initialize");
	myHandle=NULL;
	f_init=true;
	f_loop=false;
	f_start=false;

	string mySection="Camera";

	cam_num=ApplicationProperty::ReadSetupInt(mySection,"CameraId",DEF_CAMERA_ID);
	width=ApplicationProperty::ReadSetupInt(mySection,"CameraWidth",DEF_CAMERA_WIDTH);
	height=ApplicationProperty::ReadSetupInt(mySection,"CameraHeight",DEF_CAMERA_HEIGHT);
	fps=ApplicationProperty::ReadSetupInt(mySection,"CameraFPS",DEF_CAMERA_FPS);
	xstep=ApplicationProperty::ReadSetupInt(mySection,"CameraStepX",DEF_CAMERA_STEP_X);
	ystep=ApplicationProperty::ReadSetupInt(mySection,"CameraStepY",DEF_CAMERA_STEP_Y);
	thr=ApplicationProperty::ReadSetupDouble(mySection,"CameraDevieThreshold",DEF_CAMERA_THRESHOLD);
	trackbarMaxValue=0xff*3+1;

	capture = cvCreateCameraCapture (cam_num);
	if(capture==NULL){
		Logger::Println("[OpenCVAdapter]	CreateCameraCapture Fail");
		f_init=false;
		return false;
	}
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,width);
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,height);

	Logger::Println("[OpenCVAdapter]	Launch OpenCV Thread");
	myHandle = (HANDLE)_beginthreadex(NULL,0,OpenCVAdapter::Launch,this,0,NULL);
	if(myHandle==NULL){
		f_init=false;
		Logger::Println("[OpenCVAdapter]	CreateThread Fail");
		return false;
	}

	xnum=width/xstep;
	ynum=height/ystep;
	winx=ApplicationProperty::ReadSetupInt("Graphic","WindowSizeX",640);
	winy=ApplicationProperty::ReadSetupInt("Graphic","WindowSizeX",480);

	trackbarPosition=50;
	trackbarPosition2=100;

	windowNameCap = "Capture";
	windowNameThr = "Threshold";
	trackbarName = "ThresholdBar";
	trackbarName2 = "MinimumConnect";
	fc=0;
	return true;
}

void OpenCVAdapter::Run(){
	Logger::Println("[OpenCVAdapter]	Run Capture Thread");
	f_start=true;
	return;
}

void OpenCVAdapter::Close(){
	Logger::Println("[OpenCVAdapter]	Close");

	if(myHandle==NULL)return;
	Logger::Println("[OpenCVAdapter]	Killing Capture Thread...");
	f_loop=false;
	WaitForSingleObject(myHandle,INFINITE);
	Logger::Println("[OpenCVAdapter]	End Capture Thread");
	cvReleaseCapture (&capture);
	CloseHandle(myHandle);
	return;
}

void OpenCVAdapter::Stop(){
	f_start=false;
}

void OpenCVAdapter::Loop(){
	f_loop=true;
	cvNamedWindow (windowNameCap.c_str(), CV_WINDOW_AUTOSIZE);
	cvNamedWindow (windowNameThr.c_str(), CV_WINDOW_AUTOSIZE);
	HWND hWndCap=(HWND)cvGetWindowHandle(windowNameCap.c_str());
	HWND hWndThr=(HWND)cvGetWindowHandle(windowNameThr.c_str());
	cvCreateTrackbar(trackbarName.c_str(),windowNameThr.c_str(),&trackbarPosition,100,NULL);
	cvCreateTrackbar(trackbarName2.c_str(),windowNameThr.c_str(),&trackbarPosition2,200,NULL);

	label = new unsigned char[xnum*ynum];
	labeled = new short[xnum*ynum];

	int fc=0;

	for(;f_loop;){
		if(f_start){
			GetFrame();
			LabelFrame();
			ProcFrame();
			cvShowImage (windowNameThr.c_str(), frame);
		}
		cvWaitKey (1000/fps);

	}
	SAFE_DELETE_ARRAY(label);
	SAFE_DELETE_ARRAY(labeled);

	cvDestroyWindow (windowNameCap.c_str());
	cvDestroyWindow (windowNameThr.c_str());
}
unsigned int CALLBACK OpenCVAdapter::Launch(void* arg){
	OpenCVAdapter* me = (OpenCVAdapter*)arg;
	me->Loop();
	return 0;
}

void OpenCVAdapter::GetFrame(){
	frame = cvQueryFrame (capture);
	cvShowImage (windowNameCap.c_str(), frame);

	int x,y;

	for(y=0;y<ynum;y++){
		for(x=0;x<xnum;x++){
			int pos = y*xnum+x;
			int od = frame->widthStep*y*ystep + x*xstep*3;
			int r,g,b;
			b = frame->imageData[od]>=0?frame->imageData[od]:256+frame->imageData[od];
			g = frame->imageData[od+1]>=0?frame->imageData[od+1]:256+frame->imageData[od+1];
			r = frame->imageData[od+2]>=0?frame->imageData[od+2]:256+frame->imageData[od+2];
			int a = r+g+b;
			if(a<trackbarPosition){
				frame->imageData[od] = 0;
				frame->imageData[od+1] = 0;
				frame->imageData[od+2] = 0;
				label[pos]=0;
			}else{
				frame->imageData[od] = -1;
				frame->imageData[od+1] = -1;
				frame->imageData[od+2] = -1;
				label[pos]=1;
			}
		}
	}
}

void OpenCVAdapter::LabelFrame(){
	LabelingBS	labeling;
	labeling.Exec(label,labeled,xnum,ynum,true,trackbarPosition2);
	int n = labeling.GetNumOfResultRegions();
	printf("%d region found\n",n);
	for(int i=0;i<n;i++){
		RegionInfoBS *ri;
		ri=labeling.GetResultRegionInfo(i);
		float x,y;
		ri->GetCenterOfGravity(x,y);
		int px=(int)x*winx/width;
		int py=(int)y*winy/height;
		plist.push_back(make_pair((int)px,(int)py));
	}
}

void OpenCVAdapter::ProcFrame(){
	printf("[Camera] %d Point Found\n",plist.size());
	for(unsigned int i=0;i<plist.size();i++){
		inputHandler->CreatePosition(plist[i].first,plist[i].second);
	}
}

void OpenCVAdapter::DebugFrame(){
	FILE* fp;
	FILE* fpd;
	char buf[256];
	int x,y;
	sprintf_s(buf,256,"camres%d.txt",fc);
	fopen_s(&fp,buf,"w");
	sprintf_s(buf,256,"labres%d.txt",fc);
	fopen_s(&fpd,buf,"w");
	for(y=0;y<ynum;y++){
		for(x=0;x<xnum;x++){
			fprintf(fp,"%2d",label[y*xnum+x]);
			fprintf(fpd,"%2d",labeled[y*xnum+x]);
		}
		fprintf(fp,"\n");
		fprintf(fpd,"\n");
	}
	fclose(fp);
	fclose(fpd);
	fc++;
}
