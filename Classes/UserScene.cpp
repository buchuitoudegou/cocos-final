#include "UserScene.h"
#include "LoginRegisterScene.h"
#include "AppDelegate.h"
#include "json\document.h"
#include <string>
#include "Utils.h"
#include "fightScene.h"
USING_NS_CC;
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

std::string UserScene::name = "";
Scene* UserScene::createScene(std::string username)
{
	UserScene::name = username;
	return UserScene::create();
}

// on "init" you need to initialize your instance
bool UserScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	//查询积分
	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::GET);
	request->setUrl("http://127.0.0.1:3000/api/point/username/" + UserScene::name);
	request->setResponseCallback(CC_CALLBACK_2(UserScene::onHttpPointRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();


	//获取对战记录
	UserScene::getBattleRecord();



	//背景图片
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto bg = Sprite::create("user.png");
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bg->setScale(2, 2.35);
	this->addChild(bg, 0);

	preloadMusic();     // 预加载音乐
	playBgm();          // 播放背景音乐


	//卡牌界面
	auto fireShort = MenuItemImage::create("fire-short.png", "fire-short.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 0));
	auto waterShort = MenuItemImage::create("water-short.png", "water-short.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 1));
	auto solidShort = MenuItemImage::create("solid-short.png", "water-short.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 2));
	auto fireRemote = MenuItemImage::create("fire-remote.png", "fire-remote.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 3));
	auto waterRemote = MenuItemImage::create("water-remote.png", "water-remote.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 4));
	auto solidRemote = MenuItemImage::create("solid-remote.png", "solid-remote.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 5));

	//设置卡片的位置
	fireShort->setPosition(Vec2(origin.x-170+50, origin.y+30));
	waterShort->setPosition(Vec2(origin.x + 50, origin.y+35));
	solidShort->setPosition(Vec2(origin.x +200 + 50, origin.y+30));
	fireRemote->setPosition(Vec2(origin.x-170 + 50, origin.y-130));
	waterRemote->setPosition(Vec2(origin.x + 50, origin.y-130));
	solidRemote->setPosition(Vec2(origin.x +200 + 50, origin.y -130));

	auto cardMenu = Menu::create(fireShort, waterShort, solidShort, fireRemote, waterRemote, solidRemote, NULL);
	cardMenu->setAnchorPoint(Vec2(0.5, 0.5));
	cardMenu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2+100));
	this->addChild(cardMenu, 1);

	//六卡片信息
	auto fireShortInfo = Label::createWithSystemFont("attribute:fire\nattack range:50", "Arial", 15);
	fireShortInfo->setPosition(visibleSize.width/2-180 + 50,visibleSize.height/2+50);
	fireShortInfo->setColor(Color3B(237, 216, 199));
	this->addChild(fireShortInfo, 2);

	auto waterShortInfo = Label::createWithSystemFont("attribute:water\nattack range:50", "Arial", 15);
	waterShortInfo->setPosition(visibleSize.width / 2 - 10 + 50, visibleSize.height / 2 + 50);
	waterShortInfo->setColor(Color3B(237, 216, 199));
	this->addChild(waterShortInfo, 2);

	auto solidShortInfo = Label::createWithSystemFont("attribute:wood\nattack range:50", "Arial", 15);
	solidShortInfo->setPosition(visibleSize.width / 2 + 200 + 50, visibleSize.height / 2 + 50);
	solidShortInfo->setColor(Color3B(237, 216, 199));
	this->addChild(solidShortInfo, 2);

	auto fireRemoteInfo = Label::createWithSystemFont("attribute:fire\nattack range:90", "Arial", 15);
	fireRemoteInfo->setPosition(visibleSize.width / 2 - 180 + 50, visibleSize.height / 2-110);
	fireRemoteInfo->setColor(Color3B(237, 216, 199));
	this->addChild(fireRemoteInfo, 2);

	auto waterRemoteInfo = Label::createWithSystemFont("attribute:water\nattack range:90", "Arial", 15);
	waterRemoteInfo->setPosition(visibleSize.width / 2 - 10 + 50, visibleSize.height / 2 -110);
	waterRemoteInfo->setColor(Color3B(237, 216, 199));
	this->addChild(waterRemoteInfo, 2);

	auto solidRemoteInfo = Label::createWithSystemFont("attribute:wood\nattack range:90", "Arial", 15);
	solidRemoteInfo->setPosition(visibleSize.width / 2 + 200 + 50, visibleSize.height / 2-110);
	solidRemoteInfo->setColor(Color3B(237, 216, 199));
	this->addChild(solidRemoteInfo, 2);


	//退出按钮 回到登录界面
	auto logout = MenuItemImage::create("userbtn-down.png", "userbt.png", [&](Object *sender) {
		Director::getInstance()->replaceScene(
			TransitionSlideInT::create(0.6f, LoginRegisterScene::createScene()));
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  
		exit(0);
	#endif  
	});
	logout->setScale(1.5,1.5);
	auto logoutButton = Label::createWithSystemFont("Logout", "Arial", 25);
	if (logoutButton) {
		logoutButton->setColor(Color3B(234, 229, 194));
		float x = origin.x +visibleSize.width- 100;
		float y = origin.y +visibleSize.height+ logoutButton->getContentSize().height / 2 -60;
		logout->setPosition(Vec2(x, y));
		logoutButton->setPosition(Vec2(x, y));
	}

	//切换到斗争场景
	auto fight = MenuItemImage::create("userbtn-down.png", "userbtn.png", CC_CALLBACK_1(UserScene::initFight, this));
	fight->setScale(1.8, 1.8);
	auto fightButton = Label::createWithSystemFont("Fight", "Arial", 25);
	if (fightButton) {
		fightButton->setColor(Color3B(234, 229, 194));
		float x = origin.x +visibleSize.width/2;
		float y = origin.y + fightButton->getContentSize().height / 2 + 100;
		fight->setPosition(Vec2(x, y));
		fightButton->setPosition(Vec2(x, y));
	}
	auto menu = Menu::create(logout,fight, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	this->addChild(logoutButton, 1);
	this->addChild(fightButton, 1);
	return true;
}




//显示对应卡片的信息
void UserScene::showCardInfo(Ref*, int i) {
}

void UserScene::initFight(Ref*) {
	//停止背景音乐，转到斗争界面 
	SimpleAudioEngine::getInstance()->stopAllEffects();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(0.6f, Fight::createScene(UserScene::name)));
}




void UserScene::onHttpPointRequestCompleted(HttpClient* sender, HttpResponse* response) {
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer %s", response->getErrorBuffer());
	}
	auto buffer = response->getResponseData();
	rapidjson::Document doc;
	doc.Parse(buffer->data(), buffer->size());
	if (doc["status"] == "ok") {
		point = doc["point"].GetInt();
		//添加信息
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto info = Label::createWithSystemFont("Welcome," + UserScene::name + "\nyour points:" + std::to_string(point).c_str(), "Arial", 25);
		info->setPosition(origin.x + 100, visibleSize.height - 50);
		info->setColor(Color3B(237, 216, 199));
		this->addChild(info, 1);
	}
}

void UserScene::getBattleRecord() {
	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::GET);
	request->setUrl("http://127.0.0.1:3000/api/battlelog/username/" + UserScene::name);
	request->setResponseCallback(CC_CALLBACK_2(UserScene::onHttpRecordRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

}

void UserScene::onHttpRecordRequestCompleted(HttpClient* sender, HttpResponse* response) {
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer %s", response->getErrorBuffer());
	}
	auto buffer = response->getResponseData();
	rapidjson::Document doc;
	doc.Parse(buffer->data(), buffer->size());
	if (doc["status"] == "ok") {
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto records = doc["log"].GetArray();
		//添加信息
		std::string record = "Battle records:\nenemy    result\n  ";
		//if (records.Size == 0)
		//	record += "  empty";
		for (auto& v : records) {
			auto enemy= UserScene::name==v["user1"].GetString()? v["user2"].GetString(): v["user1"].GetString();
			std::string isWin = UserScene::name == v["winner"].GetString() ? "          win" : "          lose";
			//record += std::to_string(v["id"].GetInt64());
			record += enemy + isWin + "\n  ";
		}
		//记录边框
		auto frame = Sprite::create("frame1.png");
		frame->setAnchorPoint(Vec2(0.5, 0.5));
		frame->setPosition(Vec2(visibleSize.width / 2 - 300, visibleSize.height - 280	));
		frame->setScale(3, 6);
		this->addChild(frame, 1);

		auto recordLabel = Label::createWithSystemFont(record, "Arial", 25);
		recordLabel->setPosition(visibleSize.width/2-300, visibleSize.height - 200);
		recordLabel->setColor(Color3B(237, 216, 199));
		this->addChild(recordLabel, 1);
	}
}

//预加载音乐文件
void UserScene::preloadMusic() {
	auto music = SimpleAudioEngine::getInstance();
	music->preloadBackgroundMusic("music/game.mp3");

}
//播放背景音乐
void UserScene::playBgm() {
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/game.mp3", true);
}
