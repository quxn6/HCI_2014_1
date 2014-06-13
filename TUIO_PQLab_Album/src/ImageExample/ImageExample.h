#pragma once

#include "ofMain.h"
#include "ofxTuioClient.h"
// for openGL
#include "ofGLRenderer.h"
#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define NUM_PICTURE 10
#define LAST_TOUCH_TIME 300
#define TOUCH_INTEVAL 100

struct PQImage
{
	PQImage() : m_ImgHeight(0.0f), m_ImgWidth(0.0f), m_ImgPositionX(0.0f), m_ImgPositionY(0.0f),m_ImgRotateAngle(0.0f), m_IsVisible(false) {}
	ofImage m_Image;
	float m_ImgPositionX;
	float m_ImgPositionY;
	float m_ImgWidth;
	float m_ImgHeight;
	float m_ImgRotateAngle;
	float m_IsVisible;
};

class ImageExample : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	// added to Example
		ofxTuioClient MyClient;

		void touchDown(ofTouchEventArgs & touch);
		void touchMoved(ofTouchEventArgs & touch);
		void touchUp(ofTouchEventArgs & touch);
		void touchDoubleTap(ofTouchEventArgs & touch);

		void moveAlbum(ofTouchEventArgs & touch);
		void resizeAlbum(ofTouchEventArgs & touch);
		void rotateImages(ofTouchEventArgs & touch);
		void showImages(ofTouchEventArgs & touch);
		void hideImages(ofTouchEventArgs & touch);
		void dragAlbum(ofTouchEventArgs & touch);
		void controlImages(ofTouchEventArgs & touch);
		bool tapAlbum(ofTouchEventArgs & touch);
		
		


private:
	ofGLRenderer m_OpenGLRenderer;
	unsigned long long m_LastTouchTime;
	std::vector<PQImage> m_ImageList;
	float m_LastFingerDistance;

	int tmpCOunter;

	TUIO::TuioCursor* m_Cursor1;
	TUIO::TuioCursor* m_Cursor2;
	TUIO::TuioCursor* m_Cursor3;
	TUIO::TuioCursor* m_Cursor4;
};
