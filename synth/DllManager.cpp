
#include "Common.h"
#include "DllManager.h"
#include "Logger.h"
#include "Package.h"
#include "ApplicationProperty.h"

DllManager::DllManager(){
}

DllManager::~DllManager(){
}

int DllManager::Init(){
	f_init=true;
	dllFolderPath=ApplicationProperty::GetDirectory()+ApplicationProperty::ReadSetupString("DLL","Path","./");
	dllDirectory.clear();
	dllFileName.clear();
	dllProgramName.clear();
	dllAvoidCheck.push_back("freeglut.dll");
	dllAvoidCheck.push_back("portaudio_x86.dll");
	dllAvoidCheck.push_back("RFFTW2dll.dll");
	return 0;
}

int DllManager::Close(){
	return 0;
}

int DllManager::GetDllNum(){
	return dllDirectory.size();
}

int DllManager::Search(){
	dllDirectory.clear();
	dllFileName.clear();
	dllProgramName.clear();

	WIN32_FIND_DATA fileData;
	HANDLE fileSearchHandle;

	Logger::Println("[DllManager]	Search dll");
	string searchPath=dllFolderPath+"*.dll";
	fileSearchHandle=FindFirstFile(searchPath.c_str(),&fileData);
	if(fileSearchHandle!=INVALID_HANDLE_VALUE){
		for(;;){
			string fileName = fileData.cFileName;
			bool f_avoid=false;
			for(unsigned int i=0;i<dllAvoidCheck.size();i++){
				if(fileName==dllAvoidCheck[i]){
					f_avoid=true;
					break;
				}
			}
			if(!f_avoid){
				string filePath = dllFolderPath+fileData.cFileName;
				string nameBuf;
				Logger::Println("[DllManager]	Check %s",fileData.cFileName);
				if(Check(filePath,&nameBuf)){
					dllDirectory.push_back(filePath);
					dllFileName.push_back(fileData.cFileName);
					dllProgramName.push_back(nameBuf);
				}
			}
			if(FindNextFile(fileSearchHandle,&fileData)==0)break;
		}
		FindClose(fileSearchHandle);
	}
	dllHandle.resize(dllDirectory.size());
	return 0;
}

bool DllManager::Check(string dllpath,string* dllgetname){
	HINSTANCE hdll;
	bool flag=true;
	LPVOID lpMsgBuf;

	FUNC_CREATE_INSTANCE funcCreateInstance;
	FUNC_DESTROY_INSTANCE funcDestroyInstance;
	FUNC_GET_NAME	funcGetName;

	hdll=LoadLibrary(dllpath.c_str());

	if(hdll==NULL){
		Logger::Println("[DllManager]	 Invalid Dll - Can't Load Dll");
	}else{
		funcCreateInstance=(FUNC_CREATE_INSTANCE)GetProcAddress(hdll,"CreateInstance");
		if(funcCreateInstance==NULL){
			Logger::Println("[DllManager]	 Invalid Dll - Can't Load Function - createInstance");
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL,
				GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0,NULL );
//			Logger::Debug((LPCSTR)lpMsgBuf);
			LocalFree(lpMsgBuf);
			flag=false;
		}
		funcDestroyInstance=(FUNC_DESTROY_INSTANCE)GetProcAddress(hdll,"DestroyInstance");
		if(funcCreateInstance==NULL){
			Logger::Println("[DllManager]	 Invalid Dll - Can't Load Function - destroyInstance");
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL,
				GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0,NULL );
//			Logger::Debug((LPCSTR)lpMsgBuf);
			LocalFree(lpMsgBuf);
			flag=false;
		}
		funcGetName=(FUNC_GET_NAME)GetProcAddress(hdll,"GetName");
		if(funcGetName==NULL){
			Logger::Println("[DllManager]	 Invalid Dll - Can't Load Function - getName");
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL,
				GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0,NULL );
//			Logger::Debug((LPCSTR)lpMsgBuf);
			LocalFree(lpMsgBuf);
			flag=false;
		}
		if(flag){
			char buf[MAX_NAME];
			funcGetName(buf,MAX_NAME);
			(*dllgetname)=buf;
			Logger::Println("[DllManager]	 %s - OK",buf);
		}
	}
	FreeLibrary(hdll);
	return flag;
}

string DllManager::GetDllName(unsigned int i){
	if(f_init && 0<=i && i<dllProgramName.size()){
		return dllProgramName[i];
	}else{
		return "";
	}
}
string DllManager::GetDllPath(unsigned int i){
	if(f_init && 0<=i && i<dllDirectory.size()){
		return dllDirectory[i];
	}else{
		return "";
	}
}

Package* DllManager::CreatePackageInstance(unsigned int a){
	if(0<=a && a<dllDirectory.size()){
		dllHandle[a]=LoadLibrary(dllDirectory[a].c_str());
		FUNC_CREATE_INSTANCE CreateFunc=(FUNC_CREATE_INSTANCE)GetProcAddress(dllHandle[a],"CreateInstance");
		return CreateFunc();
	}
	return NULL;
}

void DllManager::DestroyPackageInstance(unsigned int a){
	if(0<=a && a<dllDirectory.size()){
		FUNC_DESTROY_INSTANCE DestroyFunc=(FUNC_DESTROY_INSTANCE)GetProcAddress(dllHandle[a],"DestroyInstance");
		DestroyFunc();
		FreeLibrary(dllHandle[a]);
	}
}