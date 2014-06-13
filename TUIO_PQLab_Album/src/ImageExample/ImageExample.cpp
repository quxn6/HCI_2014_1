#include "ImageExample/ImageExample.h"
#include <time.inl>

//--------------------------------------------------------------
void ImageExample::setup(){
	ofSetFrameRate(60);
	// Connect
	MyClient.connect(3333);

	ofAddListener(ofEvents().touchDown, this, &ImageExample::touchDown);
	ofAddListener(ofEvents().touchMoved, this, &ImageExample::touchMoved);
	ofAddListener(ofEvents().touchUp, this, &ImageExample::touchUp);
	ofAddListener(ofEvents().touchDoubleTap, this, &ImageExample::touchUp);

	srand(unsigned(time(NULL)));
	m_LastTouchTime = 0;
	m_LastFingerDistance = 0.0f;

	m_Cursor1 = nullptr;
	m_Cursor2 = nullptr;
	m_Cursor3 = nullptr;
	m_Cursor4 = nullptr;
	tmpCOunter = 0;

	for ( int i = 0 ; i < NUM_PICTURE ; ++i)
	{
		m_ImageList.push_back(PQImage());
	}
	m_ImageList[0].m_Image.loadImage("album.png");
	m_ImageList[1].m_Image.loadImage("data01.jpg");
	m_ImageList[2].m_Image.loadImage("data02.jpg");
	m_ImageList[3].m_Image.loadImage("data03.jpg");
	m_ImageList[4].m_Image.loadImage("data04.jpg");
	m_ImageList[5].m_Image.loadImage("data05.jpg");
	m_ImageList[6].m_Image.loadImage("data06.jpg");
	m_ImageList[7].m_Image.loadImage("data07.jpg");
	m_ImageList[8].m_Image.loadImage("data08.jpg");
	m_ImageList[9].m_Image.loadImage("data09.jpg");

	// 해제는 어디서 하죠?
	m_OpenGLRenderer = new ofGLRenderer();

	for ( int i = 0 ; i < m_ImageList.size() ; ++i )
	{
		m_ImageList[i].m_ImgWidth = 200.0;
		m_ImageList[i].m_ImgHeight = 200.0;
		m_ImageList[i].m_ImgRotateAngle = static_cast<float>( rand() % 360 );
		m_ImageList[i].m_ImgPositionX = rand() % static_cast<int>( WINDOW_HEIGHT - m_ImageList[i].m_ImgHeight ) ;
		m_ImageList[i].m_ImgPositionY = rand() % static_cast<int>( WINDOW_WIDTH - m_ImageList[i].m_ImgWidth ) ;
		m_ImageList[i].m_IsVisible = false;
	}
	m_ImageList[0].m_IsVisible = true;
}

//--------------------------------------------------------------
void ImageExample::update(){

}

//--------------------------------------------------------------
void ImageExample::draw(){

 	//MyClient.drawCursors();
 	//MyClient.drawObjects();
	for ( int i = m_ImageList.size()-1  ; i >= 0  ; --i )
	{
		// not visible일 때, 그리지 않고 통과
		if ( !m_ImageList[i].m_IsVisible )
			continue;

		m_OpenGLRenderer.pushMatrix();
		m_OpenGLRenderer.translate(m_ImageList[i].m_ImgPositionX, m_ImageList[i].m_ImgPositionY);
		m_OpenGLRenderer.rotateZ(m_ImageList[i].m_ImgRotateAngle);

		m_OpenGLRenderer.draw(m_ImageList[i].m_Image, -m_ImageList[i].m_ImgWidth / 2, -m_ImageList[i].m_ImgHeight / 2, 0.0,
			m_ImageList[i].m_ImgWidth, m_ImageList[i].m_ImgHeight,
			0.0, 0.0,
			m_ImageList[i].m_Image.getWidth(), m_ImageList[i].m_Image.getHeight());

		m_OpenGLRenderer.popMatrix();
	}
	
}

//--------------------------------------------------------------
void ImageExample::keyPressed(int key){

}

//--------------------------------------------------------------
void ImageExample::keyReleased(int key){

}

//--------------------------------------------------------------
void ImageExample::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ImageExample::mouseDragged(int x, int y, int button){
// 	m_ImgPositionX = x;
// 	m_ImgPositionY = y;
}

//--------------------------------------------------------------
void ImageExample::mousePressed(int x, int y, int button){
// 	for ( int i=0 ; i < m_ImageList.size() ; ++i )
// 	{
// 		m_ImageList[i].
// 
// 	}
// 	m_ImgPositionX = x;
// 	m_ImgPositionY = y;
}

//--------------------------------------------------------------
void ImageExample::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ImageExample::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ImageExample::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ImageExample::dragEvent(ofDragInfo dragInfo){ 
	
}

void ImageExample::touchDown( ofTouchEventArgs & touch )
{
	unsigned long long curTime = ofGetElapsedTimeMillis();
	// 최소 터치 인터벌을 만족시키지 못하면(부벼져서 두번 터치되는거) 리턴
	if ( curTime - m_LastTouchTime < TOUCH_INTEVAL)
	{
		m_LastTouchTime = curTime;
		return ; 
	}

	bool isDoubleTap = false;

	// double touch인지 확인
	if ( curTime - m_LastTouchTime < LAST_TOUCH_TIME )
	{
		isDoubleTap = true;
	}

	m_LastTouchTime = curTime;

	// move image
	if ( MyClient.client->getTuioCursors().size() == 1 )
	{		
		// 앨범을 double tap하면 이미지 숨기기
		if ( isDoubleTap && tapAlbum(touch) )
		{
			hideImages(touch);
		}
		else // 더블 탭이 아닐 경우는, 앨범을 탭하면 show 앨범이 아니면 글로 이동
		{
			if ( tapAlbum(touch) )	
			{
				showImages(touch);								
			}
			else
			{
				moveAlbum(touch);
			}
		}
	}

	// up scale / down scale
	if ( MyClient.client->getTuioCursors().size() == 2 )
	{
		resizeAlbum(touch);
	}

	// calc rotate
	if ( MyClient.client->getTuioCursors().size() == 3 )
	{
		rotateImages(touch);
	}
}

void ImageExample::touchMoved( ofTouchEventArgs & touch )
{

	if ( MyClient.client->getTuioCursors().size() == 1 )
	{
		dragAlbum(touch);
	}

	if ( MyClient.client->getTuioCursors().size() == 2 )
	{
		resizeAlbum(touch);
	}

	// rotate
	if ( MyClient.client->getTuioCursors().size() == 3 )
	{
		rotateImages(touch);
	}

	if ( MyClient.client->getTuioCursors().size() == 4 )
	{
		controlImages(touch);
	}
}

void ImageExample::touchUp( ofTouchEventArgs & touch )
{

}

// 더블탭은 작동을 안함, 수동으로 만들겠음..
void ImageExample::touchDoubleTap(ofTouchEventArgs & touch)
{
	if ( tapAlbum(touch) )
		hideImages(touch);
}

// album을 탭했는지 판정, 점대 사각형 충돌체크
bool ImageExample::tapAlbum(ofTouchEventArgs & touch)
{	
	// 터치한 포인트를 이미지에서의 좌표로 변경 
	// 이때, 이미지의 width, height의 1/2만큼을 보정
	// (가운데가 이미지의 원점인 것처럼 하기위해서)
	float touchPointXOnImage = touch.x * WINDOW_WIDTH + m_ImageList[0].m_ImgWidth / 2 - m_ImageList[0].m_ImgPositionX;
	if ( touchPointXOnImage < m_ImageList[0].m_ImgWidth && touchPointXOnImage > 0 )
	{
		float touchPointYOnImage = touch.y * WINDOW_HEIGHT + m_ImageList[0].m_ImgHeight / 2 - m_ImageList[0].m_ImgPositionY;
		if ( touchPointYOnImage < m_ImageList[0].m_ImgHeight && touchPointYOnImage > 0 )
		{
			return true;
		}
	}
	return false;

// 	auto cursor = MyClient.client->getTuioCursors().front();
// 
// 	float xDist = cursor->getPosition().getX() - m_ImageList[0].m_ImgPositionX;
// 	if ( xDist < m_ImageList[0].m_ImgWidth && xDist > 0 )
// 	{
// 		float yDist = cursor->getPosition().getY() - m_ImageList[0].m_ImgPositionY;
// 		if ( yDist < m_ImageList[0].m_ImgHeight && yDist > 0 )
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
}


void ImageExample::moveAlbum(ofTouchEventArgs & touch)
{
	float deltaX = touch.x * WINDOW_WIDTH - m_ImageList[0].m_ImgPositionX;
	float deltaY = touch.y * WINDOW_HEIGHT - m_ImageList[0].m_ImgPositionY;

	for ( int i=0 ; i < m_ImageList.size() ; ++i )
	{
		m_ImageList[i].m_ImgPositionX += deltaX;
		m_ImageList[i].m_ImgPositionY += deltaY;
	}

// 	m_ImageList[0].m_ImgPositionX = touch.x * WINDOW_WIDTH;
// 	m_ImageList[0].m_ImgPositionY = touch.y * WINDOW_HEIGHT;
}


void ImageExample::resizeAlbum(ofTouchEventArgs & touch)
{
	for ( auto *iter : MyClient.client->getTuioCursors() )
	{
		switch ( iter->getCursorID() )
		{
		case 0:
			m_Cursor1 = iter;
			break;
		case 1:
			m_Cursor2 = iter;
			break;
		default:
			break;
		}
	}

	// 방어코드
	if (m_Cursor1 == nullptr || m_Cursor2 == nullptr)
	{
		return;
	}

	m_ImageList[0].m_ImgWidth = abs( m_Cursor1->getX() - m_Cursor2->getX() ) * WINDOW_WIDTH * 2;
	m_ImageList[0].m_ImgHeight = abs( m_Cursor1->getY() - m_Cursor2->getY() ) * WINDOW_HEIGHT * 2;
}


void ImageExample::rotateImages(ofTouchEventArgs & touch)
{
	for ( auto *iter : MyClient.client->getTuioCursors() )
	{
		switch ( iter->getCursorID() )
		{
		case 0:
			m_Cursor1 = iter;
			break;
		case 1:
			m_Cursor2 = iter;
			break;
		default:
			break;
		}
	}

	// 방어코드
	if (m_Cursor1 == nullptr || m_Cursor2 == nullptr)
	{
		return;
	}
	// 포인트 1과 2의 기울기를 기준으로 회전시킨다.
	double tempAngle = ( m_Cursor1->getY() - m_Cursor2->getY() ) / ( m_Cursor1->getX() - m_Cursor2->getX() );
	
	float rotateAngle = 180.0 * atan(tempAngle) / PI;
	for (int i=0 ; i<m_ImageList.size() ; ++i )
	{
		m_ImageList[i].m_ImgRotateAngle = rotateAngle;
	}
	
}


void ImageExample::showImages(ofTouchEventArgs & touch)
{
	for ( int i = 0 ; i < m_ImageList.size() ; ++i)
	{
		m_ImageList[i].m_IsVisible = true;
	}
}

void ImageExample::hideImages(ofTouchEventArgs & touch)
{
	for ( int i = 0 ; i < m_ImageList.size() ; ++i)
	{
		m_ImageList[i].m_IsVisible = false;
	}
	m_ImageList[0].m_IsVisible = true;
}

void ImageExample::dragAlbum(ofTouchEventArgs & touch)
{
	for ( auto *iter : MyClient.client->getTuioCursors() )
	{
		if (iter->getCursorID() == 0)
		{
			m_Cursor1 = iter;
		}
	}

	if (m_Cursor1 == nullptr)
	{
		return;
	}

	int movedPositionX = m_Cursor1->getX() * WINDOW_WIDTH * 2;
	int movedPositionY = m_Cursor1->getY() * WINDOW_HEIGHT * 2;

	// if imaged moved too far, return;
	if ( (m_ImageList[0].m_ImgPositionX - movedPositionX) > 10.0 || m_ImageList[0].m_ImgPositionY - movedPositionY > 10.0 )
	{
		return;
	}

	moveAlbum(touch);
}

void ImageExample::controlImages(ofTouchEventArgs & touch)
{
	for ( auto *iter : MyClient.client->getTuioCursors() )
	{
		switch ( iter->getCursorID() )
		{
		case 0:
			m_Cursor1 = iter;
			break;
		case 1:
			m_Cursor2 = iter;
			break;
		case 2:
			m_Cursor3 = iter;
			break;
		case 3:
			m_Cursor4 = iter;
			break;
		default:
			break;
		}
	}

	float curFingerDist = ( m_Cursor1->getDistance(m_Cursor2) + m_Cursor1->getDistance(m_Cursor3) + m_Cursor1->getDistance(m_Cursor4 ) ) * WINDOW_WIDTH;
// 	if ( m_LastFingerDistance == 0.0f) 
// 	{
// 		m_LastFingerDistance = curFingerDist;
// 		return ;
// 	}
	float dist = curFingerDist - m_LastFingerDistance;
	if ( abs(dist) < 10 ) 
		return;

	float correctionValue = ( dist > 0.0f )? 1.0f : -1.0f;
	for ( int i=0 ; i<m_ImageList.size() ; ++i )
	{
		m_ImageList[i].m_ImgPositionX += correctionValue * ( m_ImageList[i].m_ImgPositionX - m_ImageList[0].m_ImgPositionX )/ 10;
		m_ImageList[i].m_ImgPositionY += correctionValue * ( m_ImageList[i].m_ImgPositionY - m_ImageList[0].m_ImgPositionY )/ 10;
	}
	
	tmpCOunter++;

	if (tmpCOunter == 100)
	{
		tmpCOunter = 0;
	}
	m_LastFingerDistance = curFingerDist;
}
