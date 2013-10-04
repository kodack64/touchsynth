
#pragma once

#include "Common.h"

class TouchCursor{
private:
	int x;
	int y;
	int vx;
	int vy;
	bool active;
	int id;
public:
	TouchCursor(int _id,int _x,int _y){
		id=_id;
		x=_x;
		y=_y;
		active=true;
	}
	virtual ~TouchCursor(){}
	int GetX(){return x;};
	int GetY(){return y;};
	int GetId(){return id;};
	void SetX(int _x){x=_x;};
	void SetY(int _y){y=_y;};
	void SetActive(bool _active){active=_active;};
	bool IsActive(){return active;};
};

class InputApi{
private:
public:
	virtual void Lock()=0;
	virtual void UnLock()=0;

	virtual int GetCursorNum()=0;
	virtual void GetCursorList(vector<TouchCursor>*)=0;
};