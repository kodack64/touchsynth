#pragma once

#include "Common.h"
#include "InputAPI.h"

class InputHandler:public InputApi{
private:
	static InputHandler* myInstance;
	static HANDLE hMutex;
	vector <TouchCursor> touchCursorList;
//	TouchCursor touchCursorList[MAX_TOUCH];
//	int tcCursor;
	vector <unsigned int> deadCursorList;
	int count;
	bool isLock;
public:

	static InputHandler* CreateInstance();
	static void DestroyInstance();
	InputHandler();
	virtual ~InputHandler();

	virtual bool Init();
	virtual void Close();

	virtual void Lock();
	virtual void UnLock();

	virtual int GetCursorNum();
	virtual void GetCursorList(vector<TouchCursor>*);
	virtual void GetCursorList(TouchCursor*,int *num);

	virtual unsigned int CreatePosition(int x,int y);
	virtual void DestroyPosition(unsigned int id);
	virtual void UpdatePosition(unsigned int id,int x,int y);
	virtual void UpdatePosition(unsigned int id,int x,int y,float velocity,float accel,long life);
};