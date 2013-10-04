
#include "GLAPI.h"
#include "Logger.h"
#include "ApplicationProperty.h"
#include "PackageManager.h"

GLAPI* GLAPI::myInstance=NULL;

GLAPI* GLAPI::createInstance(){
	if(myInstance==NULL){
		myInstance=new GLAPI;
	}
	return myInstance;
}
void GLAPI::destroyInstance(){
	SAFE_DELETE(myInstance);
}

GLAPI::GLAPI(){
}

GLAPI::~GLAPI(){
}

bool GLAPI::Init(int width,int height,int winx,int winy,int _gamemode,string _name){
	Logger::Println("[GraphicAPI]	Initialize OpenGL");
	glutInit(ApplicationProperty::argc,ApplicationProperty::argv);

	glutInitWindowSize(width,height);
	glutInitWindowPosition(winx,winy);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	isEndFlag=false;
	wndid=-1;
	gamemode=(_gamemode!=0);
	name=_name;

	fwidth=640;
	fheight=480;
	fcolorBit=32;
	frefreshRate=60;

	return true;
}

bool GLAPI::Close(){
	for(int i=0;i<(signed)myImage.size();i++){
		SAFE_DELETE(myImage[i]);
	}
	Logger::Println("[GraphicAPI]	Close GraphicAPI");
	return true;
}

int tex;
bool GLAPI::Run(){
	Logger::Println("[GraphicAPI]	Run OpenGL MainLoop");

	tex=this->CreateImageFromFile("test.bmp");

	char gameModeString[256];
	sprintf_s(gameModeString,256,"%dx%d:%d@%d",fwidth,fheight,fcolorBit,frefreshRate);
	glutGameModeString(gameModeString);
	if(glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)==0){
		Logger::Println("[GraphicAPI]	Invalid Display Mode");
		return false;
	}
	Logger::Debugln("[GraphicAPI]	DisplayType Check OK");

	Logger::Debugln("[GraphicAPI]	Create Window");
	if(gamemode){
		glutEnterGameMode();
	}else{
		wndid=glutCreateWindow(name.c_str());
		hWnd = FindWindow("FREEGLUT",name.c_str());
		LONG windowStyle;
		windowStyle  = GetWindowLong( hWnd, GWL_STYLE );
		if(windowStyle==0){
			Logger::Println("[GraphicAPI]	Can't Get Window Style");
			return false;
		}
		windowStyle &= ~WS_THICKFRAME;
		windowStyle &= ~WS_MAXIMIZEBOX;
		windowStyle &= ~WS_MINIMIZEBOX;
		windowStyle  = SetWindowLong( hWnd, GWL_STYLE, windowStyle );
		SetWindowPos( hWnd, NULL, 0, 0, 0, 0, (SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED) );
	}

	Logger::Debugln("[GraphicAPI]	Register Event Function");
	glutDisplayFunc(GLAPI::LaunchDisplay);
	glutTimerFunc(0,GLAPI::LaunchTimer,0);
	glutCloseFunc(GLAPI::LaunchClose);
	glutKeyboardFunc(GLAPI::LaunchKeyboard);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	Logger::Println("[GraphicAPI]	Enter MainLoop");
	while(!myInstance->isEndFlag){
		glutMainLoopEvent();
	}
	Logger::Println("[GraphicAPI]	Exit MainLoop");
	return true;
}

void GLAPI::LaunchDisplay(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	PackageManager::GraphicCallbackProc(myInstance);
	glEnable(GL_BLEND);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINE_LOOP);
	glColor3d(0.5,0.5,0.5);
	glVertex2d(-0.9, -0.9);
	glVertex2d(0.9, -0.9);
	glVertex2d(0.9, 0.9);
	glVertex2d(-0.9, 0.9);
	glEnd();
	myInstance->DrawImage(0,0,tex);
	glDisable(GL_BLEND);
	glutSwapBuffers();
}

void GLAPI::LaunchTimer(int t){
	if(!myInstance->isEndFlag){
		PackageManager::RunCallbackProc();
		glutTimerFunc(1000/60,LaunchTimer,0);
		glutPostRedisplay();
	}
	return;
}
void GLAPI::LaunchKeyboard(unsigned char key,int,int){
	switch(key){
	case VK_ESCAPE:
		if(myInstance->gamemode){
			glutLeaveGameMode();
		}else{
			glutDestroyWindow(myInstance->wndid);
		}
		myInstance->isEndFlag=true;
		break;
	case VK_TAB:
		Logger::Debug("[GraphicAPI]	Change Window Type ");
		Logger::Debugln(myInstance->gamemode?"fullsc -> window":"window -> fullsc");
		glutCloseFunc(NULL);
		if(myInstance->gamemode){
			glutLeaveGameMode();
			myInstance->wndid=glutCreateWindow(myInstance->name.c_str());
			myInstance->hWnd = FindWindow("FREEGLUT",myInstance->name.c_str());
			LONG windowStyle;
			windowStyle  = GetWindowLong( myInstance->hWnd, GWL_STYLE );
			if(windowStyle==0){
				Logger::Println("[GraphicAPI]	Can't Get Window Style");
				myInstance->isEndFlag=true;
				return;
			}
			windowStyle &= ~WS_THICKFRAME;
			windowStyle &= ~WS_MAXIMIZEBOX;
			windowStyle &= ~WS_MINIMIZEBOX;
			windowStyle  = SetWindowLong( myInstance->hWnd, GWL_STYLE, windowStyle );
			SetWindowPos( myInstance->hWnd, NULL, 0, 0, 0, 0, (SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED) );
		}else{
			glutDestroyWindow(myInstance->wndid);
			glutEnterGameMode();
		}
		glutDisplayFunc(GLAPI::LaunchDisplay);
		glutCloseFunc(GLAPI::LaunchClose);
		glutKeyboardFunc(GLAPI::LaunchKeyboard);
		myInstance->gamemode=!myInstance->gamemode;
		break;
	default:
		break;
	}
}

void GLAPI::LaunchClose(){
	myInstance->isEndFlag=true;
	return;
}

ImageId GLAPI::CreateImageFromFile(const char* filename){
	Logger::Println("[GraphicAPI] Load Image : %s",filename);
	int i;
	int id=-1;
	for(i=0;i<(signed)myImage.size();i++){
		if(myImage[i]==NULL){
			id=i;
			myImage[i]=new GLImage(id);
			break;
		}
	}
	if(id==-1){
		id=myImage.size();
		myImage.push_back(new GLImage(id));
	}

	if(myImage[id]->Load(filename)==false){
		SAFE_DELETE(myImage[id]);
		return -1;
	}
	return id;
}

void GLAPI::DrawImage(int x,int y,ImageId id){
	if(0<=id && id < myImage.size()){
		if(myImage[id]!=NULL){
			myImage[id]->Draw(x,y);
		}
	}
	return;
}
void GLAPI::ReleaseImage(ImageId id){
	if(0<=id && id < myImage.size()){
		SAFE_DELETE(myImage[id]);
	}
	return;
}