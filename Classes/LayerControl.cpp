#include "LayerControl.h"
#include "Plane.h"
#include "KinectControl.h"
#include "AppMacros.h"
//#define  winSize   CCDirector::sharedDirector()->getWinSize()

int LayerControl::m_score = 0;

bool LayerControl::init()
{
	CCSprite * nor = CCSprite::createWithSpriteFrameName("game_pause_nor.png");
	CCSprite * press = CCSprite::createWithSpriteFrameName("game_pause_pressed.png");
	pauseMenuItem = CCMenuItemSprite::create(nor, press, this, menu_selector(LayerControl::menuCallBack));
	CCPoint menuBirth = ccp(pauseMenuItem->getContentSize().width / 2 + 10, winSize.height - pauseMenuItem->getContentSize().height / 2 - 10);
	pauseMenuItem->setPosition(menuBirth);
	CCMenu *pauseMenu = CCMenu::create(pauseMenuItem, NULL);
	pauseMenu->setPosition(CCPointZero);
	addChild(pauseMenu);

	scoreItem = CCLabelBMFont::create("0", "font/font.fnt");
	scoreItem->setColor(ccc3(143, 146, 147));
	scoreItem->setAnchorPoint(ccp(0, 0.5));
	scoreItem->setPosition(ccp(pauseMenuItem->getPositionX() + nor->getContentSize().width / 2 + 5, pauseMenuItem->getPositionY()));

	addChild(scoreItem);
	schedule(schedule_selector(LayerControl::kinectUpdate ), 0.016f);   //定时器进行鼠标控制
	return true;
}

void LayerControl::kinectUpdate(float dt)
{
	KinectControl * Kinect= KinectControl::getInstance();
	Kinect->Update();
}

void LayerControl::menuCallBack(CCObject *obj)
{
	if (!CCDirector::sharedDirector()->isPaused())
	{
		pauseMenuItem->setNormalImage(CCSprite::createWithSpriteFrameName("game_resume_nor.png"));
		pauseMenuItem->setSelectedImage(CCSprite::createWithSpriteFrameName("game_resume_pressed.png"));

		CCDirector::sharedDirector()->pause();
		((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(false);  //关闭触摸
	}
	else
	{
		pauseMenuItem->setNormalImage(CCSprite::createWithSpriteFrameName("game_pause_nor.png"));
		pauseMenuItem->setSelectedImage(CCSprite::createWithSpriteFrameName("game_pause_pressed.png"));
		CCDirector::sharedDirector()->resume();
		((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(true);
	}
}


void LayerControl::updataScore(int score) //更新分数
{
	m_score = score; 
	CCString *strScore = CCString::createWithFormat("%d", score);
	scoreItem->setCString(strScore->getCString());
}

int LayerControl::getScore()  //返回成绩
{
	return m_score;
}