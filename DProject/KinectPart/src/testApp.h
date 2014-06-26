#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ofxUDPManager.h"
#include <array>
#include "Sound3D.h"

const int MAX_DEVICES = 2;
const int HAND_POSITION_BUFFER_SIZE = 6;
const int OLDEST_VALUE_INDEX	=	HAND_POSITION_BUFFER_SIZE-1;
const int Z_DIFFERENCE = 170;
const int DEFAULT_SLICE_RADIUS = 20;
const int DEFAULT_SLICE_HEIGHT = 10;
const int DEFAULT_SLICE_CAP = 2;
const double DEFAULT_VIB_TIME = 0.3;
const float DEFAULT_ENEMY_HP =  2000.0f;
const int NUMBER_OF_SOUND_EFFECTS = 4;
static const char* LEFT_PUNCH = "lp";
static const char* RIGHT_PUNCH = "rp";
static const char* NO_PUNCH = "l";
const int IDLE_STATE_COUNTER = 500;
const int RANDOM_DIRECTION_STEP = 10;
const float DEAD_TIME = 2.0f;


class testApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

private:
	void updateEnemy();
	void drawEnemy();
	void drawDeadEnemy();
	bool isEnemyDead() { return m_EnemyHP < 0.0f; }
    
	// system 包访
	ofxOpenNI openNIDevice;
	ofxUDPManager udpConnectionTx;
	Sound3D m_SoundEffect[NUMBER_OF_SOUND_EFFECTS];
	ofTrueTypeFont verdana;
	double  m_DTime;
	
	// player 包访
	int		m_NumUsers;
	std::array<ofVec3f, HAND_POSITION_BUFFER_SIZE> m_LeftHandBuffer;
	std::array<ofVec3f, HAND_POSITION_BUFFER_SIZE> m_RightHandBuffer;
	int		m_BufferIterator;	
	float	m_PunchVelocity;
	float	m_PunchAngle;
	
	// GameManager 包访
	int		m_Score;
	bool	m_FlagLeftPunch;	
	bool	m_FlagRightPunch;	
	int		m_FrameCounter;
	
	// enemy 包访
	std::shared_ptr<ofCylinderPrimitive>	m_Enemy;	
	ofImage					m_Image;
	float	m_EnemyHP;
	float	m_EnemyPositionX;
	float	m_EnemyPositionY;
	ofVec3f m_Direction;
	float	m_EnemyRadius;
	float	m_EnemyHeight;
	
};

#endif
