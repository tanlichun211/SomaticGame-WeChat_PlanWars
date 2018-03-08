#include "Enemy.h"


Enemy::Enemy()
{
	_sprite = NULL;
	_life = 0;
}
Enemy::~Enemy()
{

}

Enemy * Enemy::create()
{
	Enemy * pRect = new Enemy();
	pRect->autorelease();  //接受内存管理
	return pRect;
}

void Enemy::bindEnemySprite(CCSprite *sp, int life)
{
	_sprite = sp;
	_life = life;
	addChild(_sprite); 
}

CCSprite * Enemy::getSprite()
{
	return _sprite;
}
int Enemy::getLife()
{
	return _life;
}
void Enemy::loseLife()
{
	_life--;
}

CCRect Enemy::getBoundingBox()
{
	CCRect rect = _sprite->boundingBox();
	CCPoint pos = this->convertToWorldSpace(rect.origin); 
	CCRect enenyRect = CCRect(pos.x, pos.y, rect.size.width, rect.size.height);
	return enenyRect;
}


