#ifndef __LayerGameStart_H__
#define __LayerGameStart_H__
#include "cocos2d.h"
USING_NS_CC;
class LayerGameStart :public CCLayer
{
public:
	static CCScene * scene();
	CREATE_FUNC(LayerGameStart);
	bool init();
	void addStartGamePicture();//Ԥ������Ϸ����ͼƬ
	void addPreLoadMusic();   //Ԥ��������
	void toMainGameCallback(); //���ص���
};


#endif
