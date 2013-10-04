
/*Application*/
#include "Application.h"
#include "ApplicationProperty.h"
#include "Logger.h"

/*Audio&GraphicAdapter*/
#include "AudioAdapter.h"
#include "GraphicAdapter.h"
#include "InputManager.h"
#include "PackageManager.h"

#include <fcntl.h>
#include <io.h>

#define DEF_CONSOLE_LOG	1
#define DEF_TEXT_LOG	1

Application::Application(){
	audioAdapter=NULL;
	graphicAdapter=NULL;
	inputManager=NULL;
	packageManager=NULL;
}

Application::~Application(){
}

bool Application::Init(int* argc,char** argv,HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	/* ログ出力用のコンソール画面の作成 */
	AllocConsole();
	FILE* dfp;
	freopen_s(&dfp,"CONOUT$", "w", stdout);
	freopen_s(&dfp,"CONIN$", "r", stdin);

	/* コンフィグ管理の初期化 */
	ApplicationProperty::Init(argc,argv,hInstance,hPrevInstance,lpCmdLine,nCmdShow);
	string mySection = "Application";
	int ir=0;

	/* ログの初期化 */
	ir=ApplicationProperty::ReadSetupInt(mySection,"ConsoleLog",DEF_CONSOLE_LOG);
	Logger::SetConsoleLog(ir!=0);
	ir=ApplicationProperty::ReadSetupInt(mySection,"TextLog",DEF_TEXT_LOG);
	Logger::SetTextLog(ir!=0);

	Logger::Println("[Application]	Start");
	Logger::Println("[Application]	Initialize");

	/* グラフィックの初期化 */
	graphicAdapter = GraphicAdapter::CreateInstance();
	if(!graphicAdapter->Init()){
		Logger::Println("[Application]	Error : Graphic Initialize Fail");
		return false;
	}
	/* オーディオの初期化 */
	audioAdapter = AudioAdapter::CreateInstance();
	if(!audioAdapter->Init()){
		Logger::Println("[Application]	Error : Audio Initialize Fail");
		return false;
	}

	/* 入力アダプタ群の初期化 */
	inputManager = InputManager::CreateInstance();
	if(!inputManager->Init()){
		Logger::Println("[Application]	Error : Input Initialize Fail");
		return false;
	}

	/* パッケージ群の初期化 */
	packageManager = PackageManager::CreateInstance();
	if(!packageManager->Init(graphicAdapter->GetAPI(),audioAdapter->GetAPI(),inputManager->GetAPI())){
		Logger::Println("[Application]	Error : Package Initialize Fail");
		return false;
	}
	return true;
}

void Application::Close(){
	Logger::Println("[Application]	Close");

	//パッケージの終了
	if(packageManager!=NULL)packageManager->Close();
	if(audioAdapter!=NULL)audioAdapter->Close();
	if(graphicAdapter!=NULL)graphicAdapter->Close();
	if(inputManager!=NULL)inputManager->Close();

	PackageManager::DestroyInstance();
	AudioAdapter::DestroyInstance();
	GraphicAdapter::DestroyInstance();
	InputManager::DestroyInstance();

	Logger::Println("[Application]	End");
}

void Application::Run(){
	Logger::Println("[Application]	Run");

	inputManager->Run();
	audioAdapter->Run();
	graphicAdapter->Run();
}