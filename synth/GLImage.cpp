
#include "GLImage.h"
#include "Logger.h"

GLImage::GLImage(int id){
	myId=id;
	imageData=NULL;
}

GLImage::~GLImage(){
	SAFE_DELETE_ARRAY(imageData);
}

bool GLImage::Load(const char* filename){
	FILE* fp;
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
	fopen_s(&fp,filename,"rb");
	if(fp==NULL){
		Logger::Println("[GraphicAPI]	can't load bitmap : %s",filename);
		return false;
	}

	fread(&header, sizeof(BITMAPFILEHEADER), 1, fp);
	if ( header.bfType != 0x4d42 ){
		Logger::Println("[GraphicAPI]	this is not bitmap : %s",filename);
		fclose(fp);
		return false;
	}
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	width = bitmapInfoHeader.biWidth;
	height = bitmapInfoHeader.biHeight;
	if ( bitmapInfoHeader.biSizeImage == 0 ){
		bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * 3;
	}
	fseek(fp, header.bfOffBits, SEEK_SET);

	imageSize = bitmapInfoHeader.biSizeImage;
	imageData = new GLubyte [imageSize];

	if (!imageData){
		Logger::Println("[GraphicAPI]	memory error");
		delete [] imageData;
		fclose(fp);
		return false;
	}

	fread(imageData, 1, bitmapInfoHeader.biSizeImage, fp);

	GLubyte temp;
	for(int i=0;i<(int)imageSize;i+=3)
	{
		temp = imageData[i+0];
		imageData[i+0] = imageData[i+2];
		imageData[i+2] = temp;
	}
	fclose(fp);
	return true;
}

void GLImage::Draw(int x,int y){
	glBitmap(width,height,(float)x,(float)y,0,0,imageData);
}