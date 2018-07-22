#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class cardFac :public cocos2d::Ref {
public:
	//获取单例工厂
	static cardFac* getInstance();
	Vector<Sprite*>& initCards();
	Sprite* replaceCard(Sprite* toReplace);
private:
	cardFac();
	std::string getSoldierType(int type);
	Sprite* getValidCard();
	Vector<Sprite*> cardsArr;
	static cardFac* factory;
};