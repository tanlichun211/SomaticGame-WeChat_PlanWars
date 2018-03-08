#ifndef __KINECTCONTROL_H__
#define __KINECTCONTROL_H__

#include <windows.h>
#include <tchar.h>
// C RunTime Header Files
#include "CCStdC.h"
#include <Kinect.h>

template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

 const float  MOUSE_SENSITIVITY = 3.34f;
 const float  CURSOR_SMOOTHING = 0.68f;
 const int    CLICK_PAUSE = 20;

class KinectControl
{
public:
  
   static KinectControl* getInstance();//单例飞机
   static KinectControl* m_Kinect;
   void   KinectRemove();
   bool   init();
	IKinectSensor       * m_pSensor;  //默认的Kinect传感器

	IBodyFrameSource	* m_pBodySource;
	IBodyFrameReader	* m_pBodyReader;
	IBodyFrame		    * m_pBodyFrame;
	
public : 
	
	enum MouseEventFlag
	{
		Move = 0x0001,
		LeftDown = 0x0002,
		LeftUp = 0x0004,
		RightDown = 0x0008,
		RightUp = 0x0010,
		MiddleDown = 0x0020,
		MiddleUp = 0x0040,
		XDown = 0x0080,
		XUp = 0x0100,
		Wheel = 0x0800,
		VirtualDesk = 0x4000,
		Absolute = 0x8000
	};
	void Update() ;  // 主要更新区域
	
	HRESULT InitializeDefaultSensor();
	float PointDistanceCalc(POINT & P1, POINT & p2);
	void MouseLeftDown();
	void MouseLeftUp();
	void DoMouseClick();
	void SetCurPositonInPlane();
	
	bool useBomb;
	static int useBombCount;

private:
//	int screenWidth, screenHeight;
	float  mouseSensitivity;
	float  cursorSmoothing;
	int    click_pause; 

	bool alreadyTrackedPos;
	POINT lastCurPos;    /// For storing last cursor position
	bool lasso;  /// If true, user did a lasso click Grip gesture
	static  int lassoCount;

	RECT rt;
	RECT rtNew;//经过限制的区域
	POINT TopLeftCorner;
	long lWidth ;
	long lHeight;
};

#endif