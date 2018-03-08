#include "LayerGameOver.h"
#include "AppMacros.h"
#include "LayerGameMain.h"
#include "SimpleAudioEngine.h"
#include "KinectControl.h"

using namespace CocosDenshion;

CCScene * LayerGameOver::scene()
{
	
	CCScene * scene = CCScene::create();
	LayerGameOver * layer = LayerGameOver::create();
	scene->addChild(layer);
	return scene;
}
bool LayerGameOver::init()
{
	CCLayer::init();
	addGameOverPicture();
	showScore();
	OneMoreGame();
	schedule(schedule_selector(LayerControl::kinectUpdate), 0.016f);   //定时器进行鼠标控制
	return true;
}

void LayerGameOver::kinectUpdate(float dt)
{
	 KinectControl::getInstance()->Update ();
}

void LayerGameOver::MenuGameMainCallBack(CCObject *obj) //切换场景到游戏运行界面
{
//	SetCursorPos(1024, 955);  //就算这样 ， 飞机还是回不来了 O(∩_∩)O~
	CCScene * scene = LayerGameMain::scene();
	CCDirector::sharedDirector()->replaceScene(scene);
}

void LayerGameOver::addGameOverPicture()
{
	CCSprite * background = CCSprite::createWithSpriteFrameName("gameover.png");
	background->setAnchorPoint(ccp(0, 0));
	background->setPosition(ccp(0, winSize.height-background->getContentSize().height));
	addChild(background);
}

void LayerGameOver::addOneMoreGame()
{
	MoreGame = CCSprite::createWithSpriteFrameName("btn_finish.png");
	MoreGame->setPosition(ccp(winSize.width / 2 - 10, winSize.height / 3 - 20));
	addChild(MoreGame);

}
void LayerGameOver::showScore()
{ 
	int score = LayerGameMain::score;
	//----------------显示最终得分------------------------------//
	CCString *strScore = CCString::createWithFormat("%d", score);
	scoreFinalItem = CCLabelBMFont::create(strScore->getCString(), "font/font.fnt");
	scoreFinalItem->setColor(ccc3(10, 250, 10));
	scoreFinalItem->setAnchorPoint(ccp(0, 0));
	scoreFinalItem->setPosition(ccp(winSize.width / 2 - 30, winSize.height / 2));
	addChild(scoreFinalItem);
	//----------------------历史最佳成绩显示---------------------//
	int  BestScore= CCUserDefault::sharedUserDefault()->getIntegerForKey("HistoryHighScore", 0);
	if (BestScore < score)
	{
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/achievement.wav", true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("HistoryHighScore", score);
		CCUserDefault::sharedUserDefault()->flush(); //下次读取前先从内存写入硬盘
		BestScore = score;
	}
	CCString *strHistoryHighScore = CCString::createWithFormat("%d", BestScore);
	scoreBestItem = CCLabelBMFont::create(strHistoryHighScore->getCString() , "font/font.fnt");
	scoreBestItem->setColor(ccc3(160, 160, 164));
	scoreBestItem->setAnchorPoint(ccp(0, 0));
	scoreBestItem->setPosition(ccp(winSize.width / 2 - 50, winSize.height -70));
	addChild(scoreBestItem);

}
void LayerGameOver::QuitCallBack(CCObject * obj)
{
	KinectControl::getInstance()->KinectRemove();
	exit(1);
}
void LayerGameOver::OneMoreGame() //添加回到游戏按钮
{  
	CCSprite * GameOnceMore = CCSprite::createWithSpriteFrameName("btn_finish.png");
	CCSprite * OutGame = CCSprite::create("define/out.png");

	CCMenuItemSprite * Again = CCMenuItemSprite::create(GameOnceMore, GameOnceMore, this, menu_selector(LayerGameOver::MenuGameMainCallBack));
	CCMenuItemSprite * End =  CCMenuItemSprite::create(OutGame, OutGame , this, menu_selector(LayerGameOver::QuitCallBack ));

	CCMenu * backMenu = CCMenu::create(Again,End, NULL);
	backMenu->alignItemsVerticallyWithPadding(40);
	backMenu->setPosition(ccp(winSize.width / 2 - 10, winSize.height / 3 - 20));
	addChild(backMenu);
}