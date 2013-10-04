#ifndef _DLLMANAGER_H
#define _DLLMANAGER_H

#include "common.h"
#include "Package.h"

#define MAX_NAME 256

typedef Package* (CALLBACK* FUNC_CREATE_INSTANCE)(void);
typedef void (CALLBACK* FUNC_DESTROY_INSTANCE)(void);
typedef void (CALLBACK* FUNC_GET_NAME)(char*,int);

class DllManager{
private:
	bool f_init;
	vector <string> dllDirectory;
	vector <string> dllFileName;
	vector <string> dllProgramName;
	vector <string> dllAvoidCheck;
	vector <HINSTANCE> dllHandle;
	string dllFolderPath;
	virtual bool Check(string str,string* dllgetname);
public:
	DllManager();
	virtual ~DllManager();
	virtual int Init();
	virtual int Close();
	virtual int Search();
	virtual int GetDllNum();
	virtual string GetDllName(unsigned int i);
	virtual string GetDllPath(unsigned int i);
	virtual Package* CreatePackageInstance(unsigned int i);
	virtual void DestroyPackageInstance(unsigned int i);
};

#endif _DLLMANAGER_H