#include "Plane.h"

Plane* Plane::_splane = NULL;

Plane* Plane::getInstance()
{
	if (!_splane)
	{
		_splane = new Plane;
		if (_splane && _splane->init())
		{
		}
	}
	return _splane;
}

bool  Plane::init()
{
	CCSprite::init();
	// ��֡������֡����������
   this->initWithSpriteFrameName("hero2.png");
//	this->initWithFile("define/hh1.png");
	////һ����������
	CCBlink *blink = CCBlink::create(1, 3);

	//�������Ժ󣬿�ʼִ�ж���
	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(Plane::animatePlane));
	CCSequence *sequence = CCSequence::create(blink, callFunc, NULL);  // 
	//CCsequence ���ܴ��ccrepeatforever�Ķ���
	runAction(sequence);

	//��ը��������

	CCAnimation * animation = CCAnimation::create();
	animation->setDelayPerUnit(0.2f); //ʱ����
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n1.png"));
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n2.png"));
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n3.png"));
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "PlaneBlowUp"); 
	return true;
}

void  Plane::animatePlane() //�ɻ�����ʱ�Ķ���
{
	CCAnimation *Animation = CCAnimation::create();
	Animation->setDelayPerUnit(0.2f);  //����֡�����ʱ��
	Animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero1.png"));
    Animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero2.png"));
//Animation->addSpriteFrameWithFileName("define/hh1.png");
//Animation->addSpriteFrameWithFileName("define/hh2.png");
	
	CCAnimate *animate = CCAnimate::create(Animation);
	
	this->runAction(CCRepeatForever::create(animate));

}

void  Plane::blowUp()  //�ɻ���ըЧ��
{
	CCAnimate * animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("PlaneBlowUp"));
	CCCallFunc * actionDone = CCCallFunc::create(this, callfunc_selector(Plane::removePlane));
	CCSequence *sequence = CCSequence::create(animate, actionDone, NULL);

	this->runAction(sequence);
}
void Plane::removePlane()
{
	removeFromParent(); //�˴��и����
	_splane = NULL;    //û��NULL������Ч
}
