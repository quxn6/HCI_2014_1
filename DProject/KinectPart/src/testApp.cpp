#include "testApp.h"

#define RES_X	1920
#define RES_Y	1080
#define SPEED	5
#define HP_DECREASE 10

//--------------------------------------------------------------
void testApp::setup() {

	srand((unsigned)time(NULL));

	// 게임 설정
	m_BufferIterator = 10;
	m_DTime = DEFAULT_VIB_TIME;
	m_PunchVelocity = 0.0f;
	m_PunchAngle = 0.0f;
	m_FrameCounter = 0;
	
	m_Enemy.reset(new ofCylinderPrimitive());	
	m_EnemyPositionX = ofGetWidth()/2;
	m_EnemyPositionY = ofGetHeight()/3;
	m_EnemyRadius = 150.0f;
	m_EnemyHeight = ofGetHeight() / 3.0f * 2.0f;
	m_EnemyHP = DEFAULT_ENEMY_HP;

	// network 설정
	udpConnectionTx.Create();
	udpConnectionTx.Connect("192.168.0.3", 2390);
	udpConnectionTx.SetNonBlocking(true);

	// sound 설정
	m_SoundEffect[0].loadSound(ofToDataPath("p1.wav"));
	m_SoundEffect[1].loadSound(ofToDataPath("p2.wav"));
	m_SoundEffect[2].loadSound(ofToDataPath("p3.wav"));
	m_SoundEffect[3].loadSound(ofToDataPath("p4.wav"));
	for ( int i = 0 ; i < NUMBER_OF_SOUND_EFFECTS ; ++i)
	{
		m_SoundEffect[i].setVolume(1.0);
		m_SoundEffect[i].setMultiPlay(true);
	}

	openNIDevice.setup();
	openNIDevice.setSkeletonProfile(XN_SKEL_PROFILE_UPPER);
	openNIDevice.setResolution(RES_Y,RES_X,60);
	openNIDevice.addDepthGenerator();
	openNIDevice.addImageGenerator();
	openNIDevice.setRegister(true);
	openNIDevice.setMirror(true);
	openNIDevice.setMaxNumUsers(1);
	openNIDevice.addUserGenerator();

	openNIDevice.start();
}

//--------------------------------------------------------------
void testApp::update(){
	m_DTime += ofGetLastFrameTime();
	openNIDevice.update();
	for ( int i = 0 ; i < NUMBER_OF_SOUND_EFFECTS; ++i)
	{
		m_SoundEffect[i].update();
	}
	updateEnemy();	
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofClear(50,30,30);
	ofSetColor(150, 100, 120);
	if ( !isEnemyDead() )
		drawEnemy();	
	else
		drawDeadEnemy();
}


void testApp::updateEnemy()
{
	m_NumUsers = openNIDevice.getNumTrackedUsers();
	int currentIndex = m_BufferIterator%HAND_POSITION_BUFFER_SIZE;
	int tmp = OLDEST_VALUE_INDEX;
	int oldestIndex = (m_BufferIterator-OLDEST_VALUE_INDEX)%HAND_POSITION_BUFFER_SIZE;
	int halfIndex = (m_BufferIterator - HAND_POSITION_BUFFER_SIZE/2) % HAND_POSITION_BUFFER_SIZE;
	m_FlagLeftPunch = false;
	m_FlagRightPunch = false;
	m_PunchVelocity = 0.0f;

	++m_BufferIterator;
	if ( m_BufferIterator == 100)
		m_BufferIterator = 10;

	//for ( int i = 0 ; i < m_NumUsers; ++i )
	if ( m_NumUsers != 0 ) //&& openNIDevice.getNumTrackedHands() == 2
	{
		ofxOpenNIUser &user = openNIDevice.getTrackedUser(0);
		ofxOpenNIJoint leftHand = user.getJoint(JOINT_LEFT_HAND);		
		ofxOpenNIJoint rightHand = user.getJoint(JOINT_RIGHT_HAND);				
		m_LeftHandBuffer[currentIndex] = leftHand.getProjectivePosition();
		m_RightHandBuffer[currentIndex] = rightHand.getProjectivePosition();

		if ( m_LeftHandBuffer[oldestIndex].z - m_LeftHandBuffer[currentIndex].z > Z_DIFFERENCE && m_DTime >= DEFAULT_VIB_TIME)
		{
			m_FlagLeftPunch = true;
			m_PunchVelocity = m_LeftHandBuffer[halfIndex].z - m_LeftHandBuffer[currentIndex].z;
			m_PunchAngle = m_LeftHandBuffer[halfIndex].x - m_LeftHandBuffer[currentIndex].x;
			printf("%f\n", m_PunchVelocity);
			// lp 발송
			udpConnectionTx.Send(LEFT_PUNCH,strlen(LEFT_PUNCH));
		}
		if ( m_RightHandBuffer[oldestIndex].z - m_RightHandBuffer[currentIndex].z > Z_DIFFERENCE && m_DTime >= DEFAULT_VIB_TIME)
		{
			m_FlagRightPunch = true;
			m_PunchVelocity = m_RightHandBuffer[halfIndex].z - m_RightHandBuffer[currentIndex].z;
			m_PunchAngle = m_RightHandBuffer[halfIndex].x - m_RightHandBuffer[currentIndex].x;
			printf("%f\n", m_PunchVelocity);
			// rp 발송
			udpConnectionTx.Send(RIGHT_PUNCH,strlen(RIGHT_PUNCH));
		}
		printf( "%f\t",m_LeftHandBuffer[oldestIndex].z - m_LeftHandBuffer[currentIndex].z);
		printf( "%f\n",m_RightHandBuffer[oldestIndex].z - m_RightHandBuffer[currentIndex].z);
	}
	if (!m_FlagLeftPunch && !m_FlagRightPunch && m_BufferIterator % 10 == 0 )
	{
		// led 발송2
		udpConnectionTx.Send(NO_PUNCH,strlen(NO_PUNCH));
	}
}

void testApp::drawEnemy()
{
	int scaleRange = 0.0f;
	float angleRange = 0.0f;

	// 펀치 이벤트 발생
	if ( m_PunchVelocity != 0.0f && m_DTime >= DEFAULT_VIB_TIME)
	{				
		m_Enemy->setOrientation(ofVec3f());
		m_EnemyHP -= m_PunchVelocity;
		m_DTime = 0.0f;		
		m_FrameCounter = 10;
		m_PunchAngle = m_PunchAngle * 3.0f / 100.0f;		
		angleRange = m_PunchAngle * m_FrameCounter;
		if ( isEnemyDead() )
		{
			m_Direction = ofVec3f(rand()%RANDOM_DIRECTION_STEP, rand()%RANDOM_DIRECTION_STEP, -RANDOM_DIRECTION_STEP);
			m_SoundEffect[NUMBER_OF_SOUND_EFFECTS-1].play();
			m_FrameCounter = 0;
			return;
		}
		int randNum = rand() % (NUMBER_OF_SOUND_EFFECTS-1);
		m_SoundEffect[randNum].stop();
		m_SoundEffect[randNum].play();
		
	}
	
	// 펀치 이펙트 처리
	if ( m_DTime < DEFAULT_VIB_TIME )
	{
		scaleRange = rand() % static_cast<int>(abs(m_PunchVelocity) / 10 + 1 ) + 10;				
	}
	else if ( m_FrameCounter > 0 )
	{
		--m_FrameCounter;
		angleRange = -m_PunchAngle;
	}
		
	// 그리기
	m_Enemy->set(m_EnemyRadius - scaleRange, m_EnemyHeight * 2 + scaleRange, DEFAULT_SLICE_HEIGHT, DEFAULT_SLICE_RADIUS, DEFAULT_SLICE_CAP, true);
	m_Enemy->setPosition(m_EnemyPositionX, 0, 0);	
	m_Enemy->rotate(angleRange, 0,0,1);
	m_Enemy->draw();
}

void testApp::drawDeadEnemy()
{
	m_Enemy->set(m_EnemyRadius, m_EnemyHeight, DEFAULT_SLICE_HEIGHT, DEFAULT_SLICE_RADIUS, DEFAULT_SLICE_CAP, true);
	ofVec3f pos = m_Enemy->getPosition() + m_Direction;
	m_Enemy->setPosition(pos);	
	m_Enemy->rotate(RANDOM_DIRECTION_STEP*2, 0,0,1);
	m_Enemy->draw();
	if ( m_DTime > DEAD_TIME )
	{
		m_EnemyHP = DEFAULT_ENEMY_HP;
		m_Enemy->setOrientation(ofVec3f());
	}

}


//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
	udpConnectionTx.Close();
	ofSoundShutdown();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

