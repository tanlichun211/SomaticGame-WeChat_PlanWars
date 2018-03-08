#include "AppDelegate.h"
#include "CCEGLView.h"
#include "KinectControl.h"
#include "Plane.h"
#include <math.h>

int KinectControl::lassoCount = 0;
int KinectControl::useBombCount = 0;

KinectControl* KinectControl::m_Kinect = NULL; //初始化


KinectControl* KinectControl::getInstance()
{
	if (!m_Kinect)
	{  
		m_Kinect = new KinectControl;
		if (m_Kinect && m_Kinect->init())
		{
		}
	}
	return m_Kinect;
}
bool KinectControl::init()
{
	mouseSensitivity = MOUSE_SENSITIVITY;
	cursorSmoothing =  CURSOR_SMOOTHING;

	alreadyTrackedPos = false;
	useBomb = false;
	lasso = false;

	lastCurPos = { 0, 0 };
	m_pSensor = nullptr;
	m_pBodySource = nullptr;
	m_pBodyReader = nullptr;
	m_pBodyFrame = nullptr;
	click_pause = CLICK_PAUSE;
//  get screen with and height
//	screenWidth = GetSystemMetrics (SM_CXSCREEN);
//	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	HWND h = cocos2d::CCEGLView::sharedOpenGLView()->getHWnd(); //获得cocos2dx窗口的句柄
	rt = { 0 };
	::GetWindowRect(h, &rt);  //得到的既是相对于屏幕左上角的窗口矩形区域
	Plane * plane = Plane::getInstance();

	rtNew.left = rt.left + plane->boundingBox().size.width / 3 ;
	rtNew.right = rt.right - plane->boundingBox().size.width / 2;
	rtNew.top = rt.top + 30; //加上30为菜单栏的大小
	rtNew.bottom = rt.bottom - plane->boundingBox().size.height / 2 - 5;

	TopLeftCorner = { rtNew.left, rtNew.top };  //屏幕左上角
	lWidth = rtNew.right - rtNew.left;
	lHeight = rtNew.bottom - rtNew.top;

	SetCurPositonInPlane();
	ClipCursor(&rtNew);          //限制光标在矩形区域内

	InitializeDefaultSensor(); 

	return true;
}
void KinectControl::SetCurPositonInPlane()
{
	SetCursorPos(rt.left + (int)((rt.right - rt.left) / 2), rt.bottom - Plane::getInstance()->boundingBox().size.width / 2); //进入时固定光标在飞机附近
	MouseLeftDown(); //点中飞机
}

HRESULT KinectControl::InitializeDefaultSensor()
{
	HRESULT hr;
	hr = GetDefaultKinectSensor(&m_pSensor);
	//**********************Body帧的准备，直接开好Reader********************
	if (FAILED(hr))
	{
		return hr;
	}
	if (m_pSensor)
	{
		hr = m_pSensor->Open();
		m_pSensor->get_BodyFrameSource(&m_pBodySource);
		m_pBodySource->OpenReader(&m_pBodyReader);
	}
	return hr;
}


void  KinectControl::Update()
{

	if (lasso) lassoCount++;
	if (lassoCount > click_pause )
	 {
		 lassoCount = 0;
		 lasso = false;
	 }
	if (useBomb)  useBombCount++;
	if (useBombCount > 20)
	{
		useBombCount = 0;
		useBomb = false;
	}

	while (m_pBodyReader->AcquireLatestFrame(&m_pBodyFrame) != S_OK);		//读取Body数据

		int	bodyBufferSize = 0;
		m_pBodySource->get_BodyCount(&bodyBufferSize);
		IBody	** bodyArray = new IBody *[bodyBufferSize];
		for (int i = 0; i < bodyBufferSize; i++)
		{
			bodyArray[i] = nullptr;
		}
		m_pBodyFrame->GetAndRefreshBodyData(bodyBufferSize, bodyArray);

		for (int i = 0; i < bodyBufferSize; i++)  //遍历6个人
		{
			BOOLEAN result = false;
			if (bodyArray[i]->get_IsTracked(&result) == S_OK && result)
			{
				Joint	jointArray[JointType_Count];				//将关节点输出，正式开始处理
				bodyArray[i]->GetJoints(JointType_Count, jointArray);
				if (jointArray[JointType_Neck].TrackingState != TrackingState_Tracked ||
					jointArray[JointType_Head].TrackingState != TrackingState_Tracked)
					continue;

				CameraSpacePoint handLeft = jointArray[JointType_HandLeft].Position;    //左手掌
				CameraSpacePoint spineBase = jointArray[JointType_SpineBase].Position;  //脊椎基部
				CameraSpacePoint handRight = jointArray[JointType_HandRight].Position;  //右手掌


				if (handRight.Z - spineBase.Z < -0.15f) // if right hand lift forward
				{
					
					float x = handRight.X - spineBase.X + 0.05f;
					float y = spineBase.Y - handRight.Y + 0.51f;
					// get current cursor position
					POINT curPos;
					GetCursorPos(&curPos);

					if (curPos.y < rt.top) 	  curPos.y = rt.top;
					if (curPos.y > rt.bottom) curPos.y > rt.bottom;
					if (curPos.x< rt.left)    curPos.x = rt.left;
					if (curPos.x > rt.right)  curPos.x > rt.right;
					

					// smoothing for using should be 0 - 0.95f. The way we smooth the cusor is: oldPos + (newPos - oldPos) * smoothValue
					float smoothing = 1 - cursorSmoothing;
					int  xNow = (int)(curPos.x + (x *mouseSensitivity*lWidth -( curPos.x - rt.left))*smoothing);
					int  yNow = (int)(curPos.y + ((y + 0.25f) * mouseSensitivity *lHeight - (curPos.y-rt.top)) * smoothing);

					if (yNow< rt.top)	    yNow= rt.top;
					if (yNow > rt.bottom)	yNow > rt.bottom;
					if (xNow< rt.left) 	    xNow = rt.left;
					if (xNow > rt.right)	xNow > rt.right;

					SetCursorPos(xNow, yNow);
					alreadyTrackedPos = true;
					//---------------- Grip gesture Right-------------------//
					HandState HandRightState = HandState::HandState_Unknown;
					HRESULT hr;
					hr = bodyArray[i]->get_HandRightState(&HandRightState);
					if (HandRightState == HandState_Open )  //布     先拳头再布 实现点击
		 				{
		    					useBomb = false ;
			    				MouseLeftDown();
						}
					else if (HandRightState == HandState_Closed ) // 拳头 
						{
							
        						useBomb = true ;  //使用炸弹 
								MouseLeftUp();
						}
						else if (HandRightState == HandState_Lasso && !lasso)
						{
							useBomb = false;
							DoMouseClick();
							lasso = true;
						}
				}


				else if (handLeft.Z - spineBase.Z < -0.15f) // if left hand lift forward
				{
					float x = handLeft.X - spineBase.X + 0.3f;
					float y = spineBase.Y - handLeft.Y + 0.51f;
					POINT curPos;
					GetCursorPos(&curPos);

					if (curPos.y < rt.top) 	  curPos.y = rt.top;
					if (curPos.y > rt.bottom) curPos.y > rt.bottom;
					if (curPos.x < rt.left)    curPos.x = rt.left;
					if (curPos.x > rt.right)  curPos.x > rt.right;

					float smoothing = 1 - cursorSmoothing;
					int  xNow = (int)(curPos.x + (x *mouseSensitivity*lWidth - (curPos.x - rt.left))*smoothing);
					int  yNow = (int)(curPos.y + ((y + 0.25f) * mouseSensitivity *lHeight - (curPos.y - rt.top)) * smoothing);

					if (yNow < rt.top)	    yNow = rt.top;
					if (yNow > rt.bottom)	yNow > rt.bottom;
					if (xNow < rt.left) 	    xNow = rt.left;
					if (xNow > rt.right)	xNow > rt.right;
					
					SetCursorPos(xNow, yNow);
					
					alreadyTrackedPos = true;

					//---------------- Grip gesture left-------------------//
					HandState HandLeftState = HandState::HandState_Unknown;
					HRESULT hr;
					hr = bodyArray[i]->get_HandLeftState(&HandLeftState);

					if (HandLeftState == HandState_Open)  //布     先拳头再布 实现点击
					{
						useBomb = false;
						MouseLeftDown();
					}
					else if (HandLeftState == HandState_Closed) // 拳头 
					{

						useBomb = true;  //使用炸弹 
						MouseLeftUp();
					}
					else if (HandLeftState == HandState_Lasso && !lasso)
					{
						useBomb = false;
						DoMouseClick();
						lasso = true;
					}
				}
				else
				{
					alreadyTrackedPos = false;
				}
				break;
			}
		}
		SafeRelease(m_pBodyFrame);
		delete[] bodyArray;
}

void KinectControl::KinectRemove()
{
	SafeRelease(m_pBodyReader);
	SafeRelease(m_pBodyReader);
	if (m_pSensor)
	{
		m_pSensor->Close();
	}
	SafeRelease(m_pSensor);
	ClipCursor(NULL);//释放光标
	m_Kinect = NULL; 
}

float KinectControl::PointDistanceCalc(POINT & p1, POINT & p2)
{
	float OutCome = sqrt(pow((float)(p1.x - p2.x), 2) + pow(float(p1.y - p2.y), 2));
	return OutCome;
}

void KinectControl::MouseLeftDown()
{
	mouse_event(LeftDown, 0, 0, 0, 0);
}

void KinectControl::MouseLeftUp()
{
	mouse_event(LeftUp, 0, 0, 0, 0);
}

void KinectControl::DoMouseClick()
{
	mouse_event(LeftDown | LeftUp, 0, 0, 0, 0);
	mouse_event(LeftDown | LeftUp, 0, 0, 0, 0);
}