#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include <array>

#define MAX_DEVICES 2
#define MAX_NUMBER_OF_MISSILE 30

struct Missle
{	
	Missle() : penetration(false) {}
	Missle(float pos, float vel) : penetration(false) { m_Pos = ofVec2f(pos,pos); m_Vel = ofVec2f(vel, vel); }
	ofVec2f m_Pos;
	ofVec2f m_Vel;
	bool penetration;
};

class testApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

private:
    
    void handEvent(ofxOpenNIHandEvent & event);
	void updatePlayers();
	void setupMissiles();
	void drawMissiles();
	void checkCollision();
	void drawPlayer();
    
	ofxOpenNI openNIDevice;
	ofTrueTypeFont verdana;
	int m_NumUsers;

//	std::array<DetectingEffect*, MAX_NUMBER_OF_HAND> detectingEffectList;
//	unsigned int detectingEffectIdx;
	std::array<Missle, MAX_NUMBER_OF_MISSILE> m_Missiles;
	int		m_MissileOrder;
	float	m_MissileSize;

	ofVec3f m_HeadPos;
	ofVec3f m_LHPos;
	ofVec3f m_RHPos;
	
	int m_GuardSize;
	int m_CoreSize;
	int m_HP;
	float m_DTime;
};

#endif
