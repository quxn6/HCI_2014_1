#include "testApp.h"

#define RES_X	640
#define RES_Y	480
#define SPEED	20
#define HP_DECREASE 10

//--------------------------------------------------------------
void testApp::setup() {
	m_GuardSize = 30;
	m_CoreSize = 20;
	m_HP = 40;
	m_MissileOrder = 0;
	m_DTime = 0;
	m_NumUsers = 0;
	m_HeadPos.x = RES_X / 2.0f;
	m_HeadPos.y = RES_Y / 2.0f;
	m_LHPos.x = RES_X / 2.0f;
	m_LHPos.y = RES_Y / 2.0f;
	m_RHPos.x = RES_X / 2.0f;
	m_RHPos.y = RES_Y / 2.0f;
	m_Missiles.fill( Missle(-10.0f, 0));

	srand((unsigned)time(NULL));

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
	openNIDevice.update();
	++m_DTime;
	if ( m_DTime > 10 )
	{
		setupMissiles();
		m_DTime = 0;
	}
	updatePlayers();	
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofClear(m_HP,30,30);
	drawPlayer();
	drawMissiles();
	checkCollision();
}

void testApp::setupMissiles()
{
	// Set Position
	ofVec2f pos;
	int randX = rand();
	int randY = rand();

	if ( (randX % 2) && (randY % 2) )
	{
		pos.y = RES_Y;
		pos.x = randX % RES_X;
	}
	else if ( (randX % 2) && !(randY % 2) )
	{
		pos.y = 0;
		pos.x = randX % RES_X;
	}
	else if ( !(randX % 2) && !(randY % 2) )
	{
		pos.y = randY % RES_Y;
		pos.x = RES_X;
	}
	else 
	{
		pos.y = randY % RES_Y;
		pos.x = 0;
	}

	// Set Velocity
	randX = rand();
	randY = rand();

	ofVec2f vel;
	vel.x = SPEED * (randX % RES_X - pos.x) / RES_X;
	vel.y = SPEED * (randY % RES_Y - pos.y) / RES_Y;

	// Set Penetration
	bool pen = false;
	if ( 1 == randX % 10 )
		pen = true;

	// Set Missile
	if ( m_MissileOrder >= MAX_NUMBER_OF_MISSILE ) m_MissileOrder = 0;
	m_Missiles[m_MissileOrder].m_Pos = pos;
	m_Missiles[m_MissileOrder].m_Vel = vel;
	m_Missiles[m_MissileOrder].penetration = pen;
	++m_MissileOrder;
}

void testApp::drawMissiles()
{	
	for ( int i=0; i < MAX_NUMBER_OF_MISSILE ; ++i)
	{
		m_Missiles[i].m_Pos += m_Missiles[i].m_Vel;
		m_MissileSize = m_CoreSize;
		( m_Missiles[i].penetration )? ofSetColor( rand() % 255, rand() % 255, rand() % 255 ) : ofSetColor(200,200,200);
		ofEllipse( m_Missiles[i].m_Pos.x, m_Missiles[i].m_Pos.y, m_MissileSize, m_MissileSize);
	}
}

void testApp::updatePlayers()
{
	m_NumUsers = openNIDevice.getNumTrackedUsers();

	for ( int i = 0 ; i < m_NumUsers; ++i )
	{
		//openNIDevice.drawSkeleton(i);
		ofxOpenNIUser &user = openNIDevice.getTrackedUser(i);

		ofxOpenNIJoint leftHand = user.getJoint(JOINT_LEFT_HAND);		
		ofxOpenNIJoint rightHand = user.getJoint(JOINT_RIGHT_HAND);
		ofxOpenNIJoint head = user.getJoint(JOINT_HEAD);		
		m_HeadPos.x = head.getProjectivePosition().x;
		m_HeadPos.y = head.getProjectivePosition().y + 100;
		m_LHPos = leftHand.getProjectivePosition();
		m_RHPos = rightHand.getProjectivePosition();
		//if ( (m_LHPos - m_HeadPos).x < m_GuardSize )
	}
}

void testApp::drawPlayer()
{
	ofSetColor(50, 50, 190);
	ofEllipse(m_LHPos.x, m_LHPos.y,m_GuardSize,m_GuardSize);
	ofEllipse(m_RHPos.x, m_RHPos.y,m_GuardSize,m_GuardSize);
	ofSetColor(230, m_HP , m_HP);
	ofEllipse(m_HeadPos.x, m_HeadPos.y,m_CoreSize,m_CoreSize);
}


void testApp::checkCollision()
{
	for ( int i = 0 ; i < MAX_NUMBER_OF_MISSILE ; ++i )
	{
		// 화면 밖에 있거나, 속도 셋팅이 되지 않은 미사일은 충돌체크 패스
		if ( m_Missiles[i].m_Vel.x == 0 || m_Missiles[i].m_Pos.x < 0 || m_Missiles[i].m_Pos.y < 0 || m_Missiles[i].m_Pos.x > RES_X || m_Missiles[i].m_Pos.y > RES_Y ) 
			continue;

		float lhDist = sqrt( pow(m_Missiles[i].m_Pos.x - m_LHPos.x , 2) + pow(m_Missiles[i].m_Pos.y - m_LHPos.y , 2) );
		float rhDist = sqrt( pow(m_Missiles[i].m_Pos.x - m_RHPos.x , 2) + pow(m_Missiles[i].m_Pos.y - m_RHPos.y , 2) );
		float hDist =  sqrt( pow(m_Missiles[i].m_Pos.x - m_HeadPos.x , 2) + pow(m_Missiles[i].m_Pos.y - m_HeadPos.y , 2) );
		if ( !m_Missiles[i].penetration && (lhDist < (m_MissileSize + m_GuardSize) / 2 || rhDist < (m_MissileSize + m_GuardSize) / 2) )
		{
			if ( m_GuardSize < 50 )
				m_GuardSize += 1;
			m_Missiles[i] = Missle(-10.0f, 0.0f);
		}
		if ( hDist < ( m_MissileSize + m_CoreSize ) / 2 )
		{
			m_HP += HP_DECREASE;
		}
	}
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
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
