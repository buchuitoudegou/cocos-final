#pragma once
#include <stdio.h>
#include <string>
#include "cocos2d.h"
USING_NS_CC;

class UserScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(std::string username);

	virtual bool init();
	CREATE_FUNC(UserScene);
	//static UserScene* create(std::string username);
	void logoutButtonCallback(Ref *pSender);
	void showCardInfo(Ref*, int i);
	void initFight(Ref*);
private:
	static std::string name;

};
