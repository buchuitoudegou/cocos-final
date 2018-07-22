#pragma once
#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include "network\HttpClient.h"
using namespace cocos2d::network;
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
	void onHttpPointRequestCompleted(HttpClient* sender, HttpResponse* response);
	void onHttpRecordRequestCompleted(HttpClient* sender, HttpResponse* response);
	void getBattleRecord();
private:
	static std::string name;
	int point;

};
