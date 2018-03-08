#include "LayerGameStart.h"
#include "AppMacros.h"
#include "LayerGameMain.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;  //声音的外部库
#define  loadMusic()  CocosDenshion::SimpleAudioEngine::sharedEngine()
CCScene * LayerGameStart::scene()
{
	CCScene * scene = CCScene::create();
	LayerGameStart * layer = LayerGameStart::create();
	scene->addChild(layer);
	return scene;
}
bool LayerGameStart::init()
{
	CCLayer::init();
	addPreLoadMusic(); //预加载音乐
	addStartGamePicture();//预加载图片
	return true;
}


void LayerGameStart::addStartGamePicture()
{
	// 设置安卓返回键 setKeypadEnabled(true);

	//加载全局图片信息
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot_background.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot.plist");

	
	//加载当前gamestart页面所需要的图片,动画

	CCSprite * background = CCSprite::createWithSpriteFrameName("background.png");
	background->setAnchorPoint(ccp(0, 0));
	background->setPosition(ccp(0, 0));
	addChild(background);

	CCSprite *copyright = CCSprite::createWithSpriteFrameName("shoot_copyright.png");
	copyright->setAnchorPoint(ccp(0.5, 0));
	copyright->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	addChild(copyright);

	CCSprite *loading = CCSprite::createWithSpriteFrameName("game_loading1.png");
	loading->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 40));
	addChild(loading);

	CCAnimation * animation = CCAnimation::create();
	animation->setDelayPerUnit(0.2f);
	char nameBuf[100];
	for (int i = 0; i < 4; i++)
	{
		memset(nameBuf, 0, sizeof(nameBuf));
		sprintf(nameBuf, "game_loading%d.png", i + 1);
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(nameBuf));
	}
	CCAnimate *animate = CCAnimate::create(animation);
	CCRepeat *repeat = CCRepeat::create(animate, 4); //动画播放四次
	CCCallFunc * loadingDone = CCCallFunc::create(this, callfunc_selector(LayerGameStart::toMainGameCallback));
	//动画播放完毕回主场景

	CCSequence * sequence = CCSequence::create(repeat, loadingDone, NULL);
	loading->runAction(sequence);



}
void LayerGameStart::addPreLoadMusic()
{
	//加载全局音乐信息
	loadMusic()->preloadBackgroundMusic("sound/game_music.wav");
	loadMusic()->preloadEffect("sound/achievement.wav");
	loadMusic()->preloadEffect("sound/big_spaceship_flying.wav");
	loadMusic()->preloadEffect("sound/bullet.wav");
	loadMusic()->preloadEffect("sound/button.wav");
	loadMusic()->preloadEffect("sound/enemy1_down.wav");
	loadMusic()->preloadEffect("sound/enemy2_down.wav");
	loadMusic()->preloadEffect("sound/enemy3_down.wav");
	loadMusic()->preloadEffect("sound/game_over.wav");
	loadMusic()->preloadEffect("sound/get_bomb.wav");
	loadMusic()->preloadEffect("sound/get_double_laser.wav");
	loadMusic()->preloadEffect("sound/out_porp.wav");
	loadMusic()->preloadEffect("sound/use_bomb.wav");
}
void LayerGameStart::toMainGameCallback()
{
	CCScene * scene = LayerGameMain::scene();
	CCDirector::sharedDirector()->replaceScene(scene);
}