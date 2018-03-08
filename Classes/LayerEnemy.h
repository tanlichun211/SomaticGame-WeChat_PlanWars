#ifndef __LayerEnemy_H__
#define __LayerEnemy_H__

#include "cocos2d.h"
#include "Enemy.h"

USING_NS_CC;

const int SMALL_MAXLIFE = 1;
const int MID_MAXLIFE = 3;
const int BIG_MAXLIFE = 6;  

struct PARAMETER
{
	int smallParameter;
	int midParameter;
	int bigParameter;
};

class LayerEnemy: public CCLayer
{
public:
	CREATE_FUNC(LayerEnemy);
	bool init();

	enum ENEMY_DIVISION
	{
		SMALL=1 , MID , BIG 
	};

//------------小敌机-------------------//
	void addSmallEnemy(float dt);
	void smallEnemyMovefinished(CCNode *node);
	void smallEnemyBlowUp(Enemy * smallEnemy);
	void removeSmallEnemy(CCNode * target, void * data);
	void removeAllSmallEnemy();

	void smallEnemyShow();

//-----------中敌机-------------------//
	void addMidEnemy(float dt);
	void midHitAnimate(Enemy *midEnemy);
	void midEnemyMovefinished(CCNode *node);
	void midEnemyBlowUp(Enemy * midEnemy);
	void removeMidEnemy(CCNode * target, void * data);
	void removeAllMidEnemy();

	void midEnemyShow();

//-----------大敌机-------------------//
	void addBigEnemy(float dt);
	void bigHitAnimate(Enemy *bigEnemy);
	void bigEnemyMovefinished(CCNode *node);
	void bigEnemyBlowUp(Enemy * bigEnemy);
	void removeBigEnemy(CCNode * target, void * data);
	void removeAllBigEnemy();

	void bigEnemyShow();
//---------------------------------//
    void removeAllEnemy(); //使用炸弹

	CCArray * smallArray;
	CCArray * midArray;
	CCArray * bigArray;

	void updateParameter(); //更新随着分数上涨而设置的参数
	void reSetframeCount(int degree);
private:
	int  curSmallFramCount;
	int  curMidFrameCount;
	int  curBigFrameCount;
	int  smallEnemyFrameCount;
	int  midEnemyFrameCount;
	int  bigEnemyFrameCount;
	PARAMETER  parameter;
};


#endif







