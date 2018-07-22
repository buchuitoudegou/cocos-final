#include "UserScene.h"
#include "LoginRegisterScene.h"
#include "AppDelegate.h"

#include "fightScene.h"
USING_NS_CC;


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

	//背景图片
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto bg = Sprite::create("user.png");
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bg->setScale(2, 2.35);
	this->addChild(bg, 0);

	//用户名
	//AppDelegate* app = (AppDelegate *)Application::getInstance();
	auto username = Label::createWithSystemFont("Welcome,"+UserScene::name, "Arial", 25);
	username->setPosition(origin.x + 100, visibleSize.height - 50);
	username->setColor(Color3B(237, 216, 199));
	this->addChild(username, 1);

	//卡牌界面
	auto fireShort = MenuItemImage::create("fire-short.png", "fire-short.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 0));
	auto waterShort = MenuItemImage::create("water-short.png", "water-short.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 1));
	auto solidShort = MenuItemImage::create("solid-short.png", "water-short.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 2));
	auto fireRemote = MenuItemImage::create("fire-remote.png", "fire-remote.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 3));
	auto waterRemote = MenuItemImage::create("water-remote.png", "water-remote.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 4));
	auto solidRemote = MenuItemImage::create("solid-remote.png", "solid-remote.png", CC_CALLBACK_1(UserScene::showCardInfo, this, 5));

	//设置卡片的位置
	fireShort->setPosition(Vec2(origin.x-170, origin.y+30));
	waterShort->setPosition(Vec2(origin.x, origin.y+35));
	solidShort->setPosition(Vec2(origin.x +200, origin.y+30));
	fireRemote->setPosition(Vec2(origin.x-170, origin.y-130));
	waterRemote->setPosition(Vec2(origin.x, origin.y-130));
	solidRemote->setPosition(Vec2(origin.x +200, origin.y -130));

	auto cardMenu = Menu::create(fireShort, waterShort, solidShort, fireRemote, waterRemote, solidRemote, NULL);
	cardMenu->setAnchorPoint(Vec2(0.5, 0.5));
	cardMenu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2+100));
	this->addChild(cardMenu, 1);

	//六卡片信息
	auto fireShortInfo = Label::createWithSystemFont("attribute:fire\nattack range:50", "Arial", 15);
	fireShortInfo->setPosition(visibleSize.width/2-180,visibleSize.height/2+50);
	fireShortInfo->setColor(Color3B(237, 216, 199));
	this->addChild(fireShortInfo, 2);

	auto waterShortInfo = Label::createWithSystemFont("attribute:water\nattack range:50", "Arial", 15);
	waterShortInfo->setPosition(visibleSize.width / 2 - 10, visibleSize.height / 2 + 50);
	waterShortInfo->setColor(Color3B(237, 216, 199));
	this->addChild(waterShortInfo, 2);

	auto solidShortInfo = Label::createWithSystemFont("attribute:earth\nattack range:50", "Arial", 15);
	solidShortInfo->setPosition(visibleSize.width / 2 + 200, visibleSize.height / 2 + 50);
	solidShortInfo->setColor(Color3B(237, 216, 199));
	this->addChild(solidShortInfo, 2);

	auto fireRemoteInfo = Label::createWithSystemFont("attribute:fire\nattack range:90", "Arial", 15);
	fireRemoteInfo->setPosition(visibleSize.width / 2 - 180, visibleSize.height / 2-110);
	fireRemoteInfo->setColor(Color3B(237, 216, 199));
	this->addChild(fireRemoteInfo, 2);

	auto waterRemoteInfo = Label::createWithSystemFont("attribute:water\nattack range:90", "Arial", 15);
	waterRemoteInfo->setPosition(visibleSize.width / 2 - 10, visibleSize.height / 2 -110);
	waterRemoteInfo->setColor(Color3B(237, 216, 199));
	this->addChild(waterRemoteInfo, 2);

	auto solidRemoteInfo = Label::createWithSystemFont("attribute:earth\nattack range:90", "Arial", 15);
	solidRemoteInfo->setPosition(visibleSize.width / 2 + 200, visibleSize.height / 2-110);
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
	//转到斗争界面
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(0.6f, Fight::createScene(UserScene::name)));
}