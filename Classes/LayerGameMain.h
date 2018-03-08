#ifndef __LayerGameMain_H__
#define __LayerGameMain_H__ 
#include "cocos2d.h"
#include "Plane.h"
#include "LayerBullet.h"
#include "LayerEnemy.h"
#include "LayerControl.h"
#include "LayerFood.h"
USING_NS_CC;

const int  SMALL_SCORE = 10;
const int  MID_SCORE = 60;
const int  BIG_SCORE = 300;

const int MAX_BIGBOOM_COUNT = 100000;  //tag
const int TAG_BIGBOOM = 1000;
const int TAG_BIGBOOMCOUNT = 2000;

class LayerGameMain:public CCLayer
{
public:
	CREATE_FUNC(LayerGameMain);
	bool init();
	static CCScene * scene();

	//��������������
	void addBackGround();  //������Դ�Ϳ�����ʱ��
	//����������
	void movingBackGround(float dt);
	
	void addHero();
	void addBulletLayer();
	void addEnemyLayer();
	void addCtrlLayer();
	void addFoodLayer();

	void updateBigBoomCount(int bigBoomCount);

	void boomMenuCallBack(CCObject * obj);
	

	enum BACKGROUND
	{
		BACK1, BACK2
	};

	void update(float dt);  //��ײ���

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	LayerBullet  * _bulletLayer;
	LayerEnemy   * _enemyLayer;
	LayerControl * _ctrlLayer;
	LayerFood    * _foodLayer;


	static int score;  //����
	int bigBoomCount;
	bool bombInterval;   //�����������ը�������ը
	int  bombIntervalCount ;


};


#endif


