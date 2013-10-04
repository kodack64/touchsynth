#pragma once

#include "Common.h"
#include "MyImage.h"

class GLImage:public Image{
private:
	GLuint imageSize;
    GLubyte *imageData;
    GLuint width;
    GLuint height;
public:
	GLImage(int id);
	virtual ~GLImage();
	virtual bool Load(const char* filename);
	virtual void Draw(int x,int y);
};