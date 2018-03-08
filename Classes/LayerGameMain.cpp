#include "LayerGameMain.h"
#include "AppMacros.h"
#include "LayerGameOver.h"
#include "SimpleAudioEngine.h"
#include "KinectControl.h"

using namespace CocosDenshion;


int LayerGameMain::score = 0;   //������ʼ��

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
	// ���ð�׿���ؼ� setKeypadEnabled(true);

	//���ű�������
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/game_music.wav", true);
	score = 0;

	 bombInterval =false ;   //�����������ը�������ը
	 bombIntervalCount = 0;

	bigBoomCount = 0;
	scheduleUpdate();  // ����֡ѭ����ʱ�� ������ײ���

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	addBackGround(); //��������������
	addHero();       //�ӷɻ�

	addBulletLayer();
	addEnemyLayer();
	addCtrlLayer();
	addFoodLayer();
	return true;
}

//��������������
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

	//������ʱ�� ֡ѭ����һ֡��0.016
	schedule(schedule_selector(LayerGameMain::movingBackGround), 0.01f);

}
//����������
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

void LayerGameMain::addHero()  //��ӷɻ�
{
	Plane * hero = Plane::getInstance(); //������÷ɻ�
	hero->setPosition(ccp(winSize.width / 2, hero->getContentSize().height / 2)); //���ó�ʼʱ�ɻ���λ��
	addChild(hero);

}

bool LayerGameMain::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCRect planeRect = Plane::getInstance()->boundingBox();
	planeRect.origin.x -= 30;  
	planeRect.origin.y -= 40;
	planeRect.size.width += 60;
	planeRect.size.height += 80;
	if (planeRect.containsPoint(pTouch->getLocation())){   //�����������
		return true;
	}
	else{
		return false;
	}

}
void LayerGameMain::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	Plane *hero = Plane::getInstance();
	CCPoint positon = hero->getPosition() + pTouch->getDelta(); //getDelta һ֡�л�õ�

	//CCLog("%g", hero->boundingBox().size.width/2);
	//CCLog("%g", winSize.width - hero->boundingBox().size.width/2);
	//CCLog("%g", hero->boundingBox().size.height/2);
	//CCLog("%g", winSize.height - hero->boundingBox().size.height/2);
	if (positon.x < hero->boundingBox().size.width / 2 ||
		positon.x > winSize.width - hero->boundingBox().size.width / 2 ||
		positon.y <hero->boundingBox().size.height / 2 ||
		positon.y > winSize.height - hero->boundingBox().size.height / 2)
	{
		return; //������
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
	CCObject * bt = NULL;  //�ӵ�
	CCObject * et = NULL;  //�л�
	CCArray  *bulletsToDel = CCArray::create();

	CCARRAY_FOREACH(_bulletLayer->_bulletArray, bt) //�����ӵ�
	{
		CCSprite * bullet = (CCSprite*)bt;
		CCArray  * smallEnemyToDel = CCArray::create();

		CCARRAY_FOREACH(_enemyLayer->smallArray, et) //ȡ��С�л�
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
					_ctrlLayer->updataScore(score); //���·���
				}
			}
		}
		CCARRAY_FOREACH(smallEnemyToDel, et) //����һ����ɾ��
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
	CCARRAY_FOREACH(_bulletLayer->_bulletArray, bt) //�����ӵ�
	{
		CCSprite * bullet = (CCSprite*)bt;
		CCArray  * midEnemyToDel = CCArray::create();

		CCARRAY_FOREACH(_enemyLayer->midArray, et) //ȡ���ел�
		
		{
			Enemy * midEnemy = (Enemy *)et;
			if (bullet->boundingBox().intersectsRect(midEnemy->getBoundingBox()))
			{
				//CCLog("%d", midEnemy->getLife());
				if (midEnemy->getLife() > 1)
				{
					midEnemy->loseLife();
					_enemyLayer->midHitAnimate(midEnemy); //��ʾ����Ч��
					bulletsToDel->addObject(bullet);
				}
				else if (midEnemy->getLife() == 1)
				{
					midEnemy->loseLife();
					bulletsToDel->addObject(bullet);
					midEnemyToDel->addObject(midEnemy);
					score += MID_SCORE;
					_ctrlLayer->updataScore(score); //���·���
				}
			}
		}
		CCARRAY_FOREACH(midEnemyToDel, et) //����һ����ɾ��
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
	
	CCARRAY_FOREACH(_bulletLayer->_bulletArray, bt) //�����ӵ�
	{
		CCSprite * bullet = (CCSprite*)bt;
		CCArray  * bigEnemyToDel = CCArray::create();

		CCARRAY_FOREACH(_enemyLayer->bigArray, et) //ȡ�ô�л�

		{
			Enemy * bigEnemy = (Enemy *)et;
			if (bullet->boundingBox().intersectsRect(bigEnemy->getBoundingBox()))
			{
				//CCLog("%d", bigEnemy->getLife());
				if (bigEnemy->getLife() > 1)
				{
					bigEnemy->loseLife();
					_enemyLayer->bigHitAnimate(bigEnemy); //��ʾ����Ч��
					bulletsToDel->addObject(bullet);
				}
				else if (bigEnemy->getLife() == 1)
				{
					bigEnemy->loseLife();
					bulletsToDel->addObject(bullet);
					bigEnemyToDel->addObject(bigEnemy);
					score += BIG_SCORE;
					_ctrlLayer->updataScore(score); //���·���
				}
			}
		}
		CCARRAY_FOREACH(bigEnemyToDel, et) //����һ����ɾ��
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
	CCRect planeRect = Plane::getInstance()->boundingBox();  //��÷ɻ�����
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
		 	((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(false);  //�رմ���
			_bulletLayer->stopShoot();
			unscheduleAllSelectors(); // �ر����ж�ʱ��
			Plane::getInstance()->blowUp();
			Plane::getInstance()->removePlane();   //������˳����л�  ����Ͳ���ҪREMOVE
			_enemyLayer->smallEnemyBlowUp(smallEnemy); //����ײ��С�л�Ҳ����
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
			((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(false);  //�رմ���
			_bulletLayer->stopShoot();
			unscheduleAllSelectors(); // �ر����ж�ʱ��
			Plane::getInstance()->blowUp();
			Plane::getInstance()->removePlane();   //������˳����л�  ����Ͳ���ҪREMOVE	
			_enemyLayer->midEnemyBlowUp(midEnemy); //����ײ���ел�Ҳ����
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
			((CCLayer*)(Plane::getInstance()->getParent()))->setTouchEnabled(false);  //�رմ���
			_bulletLayer->stopShoot();
			unscheduleAllSelectors(); // �ر����ж�ʱ��
			Plane::getInstance()->blowUp();
			Plane::getInstance()->removePlane();   //������˳����л�  ����Ͳ���ҪREMOVE	
			_enemyLayer->midEnemyBlowUp(bigEnemy); //����ײ�Ĵ�л�Ҳ����
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
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/get_bomb.wav", true); //�õ�ը������Ч
			_foodLayer->removeBigBoom(bigBoom);
			bigBoomCount++;  //�м���ը��
			updateBigBoomCount(bigBoomCount);
		}

	}
	//-------------------------gesture bigboom ---------------------
	if (bombInterval)
	{
		bombIntervalCount++;
	}
	if (bombIntervalCount > 40) //�ȽϺ���Ĳ��� 
	{
		bombInterval =false ;
		bombIntervalCount = 0;
	}

	if (KinectControl::getInstance()->useBomb && bigBoomCount > 0 && !bombInterval )
	{
		bombInterval = true;  //���ڼ��ʱ��

		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/use_bomb.wav", true);
		bigBoomCount--;
		CCString *strBoomCount = CCString::createWithFormat("X%d", bigBoomCount);
		((CCLabelBMFont*)(getChildByTag(TAG_BIGBOOMCOUNT)))->setCString(strBoomCount->getCString());

		score += _enemyLayer->smallArray->count() * SMALL_SCORE;
		score += _enemyLayer->midArray->count() * MID_SCORE;
		score += _enemyLayer->bigArray->count() * BIG_SCORE;

		_ctrlLayer->updataScore(score);
		_enemyLayer->removeAllEnemy(); //�ɵ����ел�
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
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/get_double_laser.wav", true); //�õ�˫���ӵ�����Ч
			_foodLayer->removeMultiBullets(multiBullets);
			_bulletLayer->multiBulletEffet();  //����ʹ��
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
		if (getChildByTag(TAG_BIGBOOM))  //ը�� ͼ��ʾ
		{
			removeChildByTag(TAG_BIGBOOM, true);
		}
		if (getChildByTag(TAG_BIGBOOMCOUNT)) //ը��  ��Ŀ����ʾ
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

void LayerGameMain::boomMenuCallBack(CCObject * obj) //ը��ը
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/use_bomb.wav", true);
	bigBoomCount--;
	CCString *strBoomCount = CCString::createWithFormat("X%d", bigBoomCount);
	((CCLabelBMFont*)(getChildByTag(TAG_BIGBOOMCOUNT)))->setCString(strBoomCount->getCString());

	score += _enemyLayer->smallArray->count() * SMALL_SCORE;
	score += _enemyLayer->midArray->count() * MID_SCORE;
	score += _enemyLayer->bigArray->count() * BIG_SCORE;

	_ctrlLayer->updataScore(score);
	_enemyLayer->removeAllEnemy(); //�ɵ����ел�
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
