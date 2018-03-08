#include "LayerGameMain.h"
#include "AppMacros.h"
#include "LayerGameOver.h"
#include "SimpleAudioEngine.h"
#include "KinectControl.h"

using namespace CocosDenshion;


int LayerGameMain::score = 0;   //分数初始化

CCScene * LayerGameMain::scene()
{
	CCScene * scene = CCScene::create();
	LayerGameMain * layer = LayerGameMain::create();
	scene->addChild(layer);
	return scene;
}

bool LayerGameMain::init()
{
	CCLayer::init();
	// 设置安卓返回键 setKeypadEnabled(true);

	//播放背景音乐
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/game_music.wav", true);
	score = 0;

	 bombInterval =false ;   //处理多于两个炸弹间隔爆炸
	 bombIntervalCount = 0;

	bigBoomCount = 0;
	scheduleUpdate();  // 开启帧循环定时器 进行碰撞检测

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	addBackGround(); //加载主场景背景
	addHero();       //加飞机

	addBulletLayer();
	addEnemyLayer();
	addCtrlLayer();
	addFoodLayer();
	return true;
}

//加载主场景背景
void  LayerGameMain::addBackGround()
{
	CCSprite * bg1 = CCSprite::createWithSpriteFrameName("background.png");
	bg1->setTag(BACK1);
	bg1->setAnchorPoint(ccp(0, 0));
	bg1->setPosition(ccp(0, 0));
	addChild(bg1);

	CCSprite * bg2 = CCSprite::createWithSpriteFrameName("background.png");
	bg2->setTag(BACK2);
	bg2->setAnchorPoint(ccp(0, 0));
	bg2->setPosition(ccp(0, bg2->getContentSize().height - 2));
	addChild(bg2);

	//启动定时器 帧循环是一帧是0.016
	schedule(schedule_selector(LayerGameMain::movingBackGround), 0.01f);

}
//背景动起来
void LayerGameMain::movingBackGround(float dt)
{
	CCSprite * bg1 = (CCSprite*)getChildByTag(BACK1);
	CCSprite * bg2 = (CCSprite*)getChildByTag(BACK2);

	bg1->setPositionY(bg1->getPositionY() - 5);
	bg2->setPositionY(bg1->getPositionY() + bg2->getContentSize().height - 2);

	if (bg2->getPositionY() < 0)
	{
		bg1->setPositionY(0);
	}
}

void LayerGameMain::addHero()  //添加飞机
{
	Plane * hero = Plane::getInstance(); //单例获得飞机
	hero->setPosition(ccp(winSize.width / 2, hero->getContentSize().height / 2)); //设置初始时飞机的位置
	addChild(hero);

}

bool LayerGameMain::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCRect planeRect = Plane::getInstance()->boundingBox();
	planeRect.origin.x -= 30;  
	planeRect.origin.y -= 40;
	planeRect.size.width += 60;
	planeRect.size.height += 80;
	if (planeRect.containsPoint(pTouch->getLocation())){   //点中了则继续
		return true;
	}
	else{
		return false;
	}

}
void LayerGameMain::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	Plane *hero = Plane::getInstance();
	CCPoint positon = hero->getPosition() + pTouch->getDelta(); //getDelta 一帧中获得的

	//CCLog("%g", hero->boundingBox().size.width/2);
	//CCLog("%g", winSize.width - hero->boundingBox().size.width/2);
	//CCLog("%g", hero->boundingBox().size.height/2);
	//CCLog("%g", winSize.height - hero->boundingBox().size.height/2);
	if (positon.x < hero->boundingBox().size.width / 2 ||
		positon.x > winSize.width - hero->boundingBox().size.width / 2 ||
		positon.y <hero->boundingBox().size.height / 2 ||
		positon.y > winSize.height - hero->boundingBox().size.height / 2)
	{
		return; //出界了
	}
	else
	{
		Plane::getInstance()->setPosition(positon);
	}
}

void LayerGameMain::addBulletLayer()
{
	_bulletLayer = LayerBullet::create();
	addChild(_bulletLayer);
}

void LayerGameMain::addEnemyLayer()
{
	_enemyLayer = LayerEnemy::create();
	addChild(_enemyLayer);
}


void LayerGameMain::update(float dt)
{


	//bullet vs enemysmall 
	CCObject * bt = NULL;  //子弹
	CCObject * et = NULL;  //敌机
	CCArray  *bulletsToDel = CCArray::create();

	CCARRAY_FOREACH(_bulletLayer->_bulletArray, bt) //遍历子弹
	{
		CCSprite * bullet = (CCSprite*)bt;
		CCArray  * smallEnemyToDel = CCArray::create();

		CCARRAY_FOREACH(_enemyLayer->smallArray, et) //取得小敌机
		{
			Enemy * smallEnemy = (Enemy *)et;
			if (bullet->boundingBox().intersectsRect(smallEnemy->getBoundingBox()))
			{
				CCLog("%d", smallEnemy->getLife());
				if (smallEnemy->getLife() == 1)
				{
					smallEnemy->loseLife();
					bulletsToDel->addObject(bullet);
					smallEnemyToDel->addObject(smallEnemy);
					score += SMALL_SCORE;
					_ctrlLayer->updataScore(score); //更新分数
				}
			}
		}
		CCARRAY_FOREACH(smallEnemyToDel, et) //遍历一遍再删除
		{
			Enemy * smallEnemy = (Enemy*)et;
			_enemyLayer->smallEnemyBlowUp(smallEnemy);
		}
		smallEnemyToDel->release();
	}


	CCARRAY_FOREACH(bulletsToDel, bt)
	{
		CCSprite * bullet = (CCSprite*)bt;
		_bulletLayer->removeBullet(bullet);
	}

	
	bulletsToDel->removeAllObjects();
	
	//----------------bullet vs midplane 
	CCARRAY_FOREACH(_bulletLayer->_bulletArray, bt) //遍历子弹
	{
		CCSprite * bullet = (CCSprite*)bt;
		CCArray  * midEnemyToDel = CCArray::create();

		CCARRAY_FOREACH(_enemyLayer->midArray, et) //取得中敌机
		
		{
			Enemy * midEnemy = (Enemy *)et;
			if (bullet->boundingBox().intersectsRect(midEnemy->getBoundingBox()))
			{
				//CCLog("%d", midEnemy->getLife());
				if (midEnemy->getLife() > 1)
				{
					midEnemy->loseLife();
					_enemyLayer->midHitAnimate(midEnemy); //显示击中效果
					bulletsToDel->addObject(bullet);
				}
				else if (midEnemy->getLife() == 1)
				{
					midEnemy->loseLife();
					bulletsToDel->addObject(bullet);
					midEnemyToDel->addObject(midEnemy);
					score += MID_SCORE;
					_ctrlLayer->updataScore(score); //更新分数
				}
			}
		}
		CCARRAY_FOREACH(midEnemyToDel, et) //遍历一遍再删除
		{
			Enemy * midEnemy = (Enemy*)et;
			_enemyLayer->midEnemyBlowUp(midEnemy);
		}
		midEnemyToDel->release();
	}

	CCARRAY_FOREACH(bulletsToDel, bt)
	{
		CCSprite * bullet = (CCSprite*)bt;
		_bulletLayer->removeBullet(bullet);
	}
	bulletsToDel->removeAllObjects();

	
	////----------------------------------bullet VS bigplane 
	
	CCARRAY_FOREACH(_bulletLayer->_bulletArray, bt) //遍历子弹
	{
		CCSprite * bullet = (CCSprite*)bt;
		CCArray  * bigEnemyToDel = CCArray::create();

		CCARRAY_FOREACH(_enemyLayer->bigArray, et) //取得大敌机

		{
			Enemy * bigEnemy = (Enemy *)et;
			if (bullet->boundingBox().intersectsRect(bigEnemy->getBoundingBox()))
			{
				//CCLog("%d", bigEnemy->getLife());
				if (bigEnemy->getLife() > 1)
				{
					bigEnemy->loseLife();
					_enemyLayer->bigHitAnimate(bigEnemy); //显示击中效果
					bulletsToDel->addObject(bullet);
				}
				else if (bigEnemy->getLife() == 1)
				{
					bigEnemy->loseLife();
					bulletsToDel->addObject(bullet);
					bigEnemyToDel->addObject(bigEnemy);
					score += BIG_SCORE;
					_ctrlLayer->updataScore(score); //更新分数
				}
			}
		}
		CCARRAY_FOREACH(bigEnemyToDel, et) //遍历一遍再删除
		{
			Enemy * bigEnemy = (Enemy*)et;
			_enemyLayer->bigEnemyBlowUp(bigEnemy);
		}
		bigEnemyToDel->release();
	}

	CCARRAY_FOREACH(bulletsToDel, bt)
	{
		CCSprite * bullet = (CCSprite*)bt;
		_bulletLayer->removeBullet(bullet);
	}
	bulletsToDel->removeAllObjects();




//------------------------------------hero vs enemySmall
	CCRect planeRect = Plane::getInstance()->boundingBox();  //获得飞机单例
	planeRect.origin.x += 30;
	planeRect.origin.y += 20;
	planeRect.size.width -= 60;
	planeRect.size.height -= 40;

	//-----------------------//
	CCARRAY_FOREACH(_enemyLayer->smallArray,et)
	{
		Enemy  * smallEnemy = (Enemy  *)et;
		if (planeRect.intersectsRect(smallEnemy->getBoundingBox()))
		{
		 	((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(false);  //关闭触摸
			_bulletLayer->stopShoot();
			unscheduleAllSelectors(); // 关闭所有定时器
			Plane::getInstance()->blowUp();
			Plane::getInstance()->removePlane();   //如果加了场景切换  这里就不需要REMOVE
			_enemyLayer->smallEnemyBlowUp(smallEnemy); //把碰撞的小敌机也销毁
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/game_over.wav", true);
			CCScene *over = LayerGameOver::scene(); 
			CCDirector::sharedDirector()->replaceScene(over);
			
		}
	}

	//------------------------------------hero vs enemyMid
	CCARRAY_FOREACH(_enemyLayer->midArray, et)
	{
		Enemy  * midEnemy = (Enemy  *)et;
		if (planeRect.intersectsRect(midEnemy->getBoundingBox()))
		{
			((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(false);  //关闭触摸
			_bulletLayer->stopShoot();
			unscheduleAllSelectors(); // 关闭所有定时器
			Plane::getInstance()->blowUp();
			Plane::getInstance()->removePlane();   //如果加了场景切换  这里就不需要REMOVE	
			_enemyLayer->midEnemyBlowUp(midEnemy); //把碰撞的中敌机也销毁
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/game_over.wav", true);
			CCScene *over = LayerGameOver::scene();
			CCDirector::sharedDirector()->replaceScene(over);

		}
	}
	//------------------------------------hero vs enemyBig
	CCARRAY_FOREACH(_enemyLayer->bigArray, et)
	{
		Enemy  * bigEnemy = (Enemy  *)et;
		if (planeRect.intersectsRect(bigEnemy->getBoundingBox()))
		{
			((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(false);  //关闭触摸
			_bulletLayer->stopShoot();
			unscheduleAllSelectors(); // 关闭所有定时器
			Plane::getInstance()->blowUp();
			Plane::getInstance()->removePlane();   //如果加了场景切换  这里就不需要REMOVE	
			_enemyLayer->midEnemyBlowUp(bigEnemy); //把碰撞的大敌机也销毁
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/game_over.wav", true);
			CCScene *over = LayerGameOver::scene();
			CCDirector::sharedDirector()->replaceScene(over);
		}
	}

	//------------------------------------food vs hero

	CCObject * ut;

	CCARRAY_FOREACH(_foodLayer->bigBoomArray, ut)
	{
		CCSprite * bigBoom = (CCSprite*)ut;
		if (bigBoom->boundingBox().intersectsRect(Plane::getInstance()->boundingBox())  )
		{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/get_bomb.wav", true); //得到炸弹的音效
			_foodLayer->removeBigBoom(bigBoom);
			bigBoomCount++;  //有几个炸弹
			updateBigBoomCount(bigBoomCount);
		}

	}
	//-------------------------gesture bigboom ---------------------
	if (bombInterval)
	{
		bombIntervalCount++;
	}
	if (bombIntervalCount > 40) //比较合理的参数 
	{
		bombInterval =false ;
		bombIntervalCount = 0;
	}

	if (KinectControl::getInstance()->useBomb && bigBoomCount > 0 && !bombInterval )
	{
		bombInterval = true;  //处于间隔时间

		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/use_bomb.wav", true);
		bigBoomCount--;
		CCString *strBoomCount = CCString::createWithFormat("X%d", bigBoomCount);
		((CCLabelBMFont*)(getChildByTag(TAG_BIGBOOMCOUNT)))->setCString(strBoomCount->getCString());

		score += _enemyLayer->smallArray->count() * SMALL_SCORE;
		score += _enemyLayer->midArray->count() * MID_SCORE;
		score += _enemyLayer->bigArray->count() * BIG_SCORE;

		_ctrlLayer->updataScore(score);
		_enemyLayer->removeAllEnemy(); //干掉所有敌机
		if (bigBoomCount == 0)
		{
			removeChildByTag(TAG_BIGBOOM);
			removeChildByTag(TAG_BIGBOOMCOUNT);
		}
		KinectControl::getInstance()->useBomb = false;
	}

	//----------------------------------foodMultiBullet vs hero 
	CCObject * mbt;

	CCARRAY_FOREACH(_foodLayer->multiBulletArray, mbt)
	{
		CCSprite * multiBullets = (CCSprite*)mbt;
		if (multiBullets->boundingBox().intersectsRect(Plane::getInstance()->boundingBox()))
		{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/get_double_laser.wav", true); //得到双排子弹的音效
			_foodLayer->removeMultiBullets(multiBullets);
			_bulletLayer->multiBulletEffet();  //立即使用
		}
	}

}


void  LayerGameMain::updateBigBoomCount(int bigBoomCount)
{
	CCString strBoomCount;
	if (bigBoomCount < 0)
	{
		return;
	}
	else if (bigBoomCount == 0)
	{
		if (getChildByTag(TAG_BIGBOOM))  //炸弹 图显示
		{
			removeChildByTag(TAG_BIGBOOM, true);
		}
		if (getChildByTag(TAG_BIGBOOMCOUNT)) //炸弹  数目的显示
		{
			removeChildByTag(TAG_BIGBOOMCOUNT, true);
		}
	}
	else if (bigBoomCount == 1)
	{
		CCSprite * norBoom = CCSprite::createWithSpriteFrameName("bomb.png");
		if (!getChildByTag(TAG_BIGBOOM))
		{
			CCSprite * selBoom = CCSprite::createWithSpriteFrameName("bomb.png");
			CCMenuItemSprite * boomItem = CCMenuItemSprite::create(norBoom, selBoom, this, menu_selector(LayerGameMain::boomMenuCallBack));
			boomItem->setPosition(ccp(norBoom->getContentSize().width / 2, norBoom->getContentSize().height / 2));
			CCMenu * boomMenu = CCMenu::create(boomItem, NULL);
			boomMenu->setPosition(CCPointZero);
			addChild(boomMenu, 100, TAG_BIGBOOM);
		}
		if (!getChildByTag(TAG_BIGBOOMCOUNT))
		{
			strBoomCount.initWithFormat("X%d", bigBoomCount);
			CCLabelBMFont * labelBoomCount = CCLabelBMFont::create(strBoomCount.getCString(), "font/font.fnt");
			labelBoomCount->setAnchorPoint(ccp(0, 0.5));
			labelBoomCount->setPosition(ccp(norBoom->getContentSize().width + 15, norBoom->getContentSize().height / 2));
			addChild(labelBoomCount, 200, TAG_BIGBOOMCOUNT);
		}
	}
	else if (bigBoomCount > 1)
	{
		strBoomCount.initWithFormat("X%d", bigBoomCount);
		((CCLabelBMFont*)(getChildByTag(TAG_BIGBOOMCOUNT)))->setCString(strBoomCount.getCString());
	}
}

void LayerGameMain::boomMenuCallBack(CCObject * obj) //炸弹炸
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/use_bomb.wav", true);
	bigBoomCount--;
	CCString *strBoomCount = CCString::createWithFormat("X%d", bigBoomCount);
	((CCLabelBMFont*)(getChildByTag(TAG_BIGBOOMCOUNT)))->setCString(strBoomCount->getCString());

	score += _enemyLayer->smallArray->count() * SMALL_SCORE;
	score += _enemyLayer->midArray->count() * MID_SCORE;
	score += _enemyLayer->bigArray->count() * BIG_SCORE;

	_ctrlLayer->updataScore(score);
	_enemyLayer->removeAllEnemy(); //干掉所有敌机
	if (bigBoomCount == 0)
	{
		removeChildByTag(TAG_BIGBOOM);
		removeChildByTag(TAG_BIGBOOMCOUNT);
	}
}

void LayerGameMain::addCtrlLayer()
{
	_ctrlLayer = LayerControl::create();
	addChild(_ctrlLayer);
}

void LayerGameMain::addFoodLayer()
{
	_foodLayer = LayerFood::create();
	addChild(_foodLayer);
}
