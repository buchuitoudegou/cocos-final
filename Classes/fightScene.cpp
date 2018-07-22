#include "FightScene.h"
#include "SimpleAudioEngine.h"
#include <string>
#pragma execution_character_set("utf-8")
USING_NS_CC;
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
using namespace std;
std::string  Fight::name = "";
Scene* Fight::createScene(string username)
{
	Fight::name = username;
    return Fight::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in FightScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Fight::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//背景的背景
	auto bgbg = Sprite::create("bgbg.png");
	bgbg->setAnchorPoint(Vec2(0.5, 0.5));
	bgbg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bgbg->setScale(1.5, 1.5);
	this->addChild(bgbg, -1);


	//创建背景
	auto bg = Sprite::create("background.png");
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2+70));
	bg->setScale(1.8, 1.3);
	this->addChild(bg, 0);

	//获取我方的边
	// side = Fight::getSide();
	side = 0;
	waterSpeed = 1;//设置初始圣水恢复速度


	preloadMusic();     // 预加载音乐
	playBgm();          // 播放背景音乐

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar 
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(200, 10));
	addChild(pT, 2);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y));
	//addChild(sp0, 1);

	log("start create tower");
	//创建双方防御塔
	auto fac = Factory::getInstance();
	auto leftTower_1 = fac->createSoldier(0, 6);
	
	auto leftTower_2 = fac->createSoldier(0, 6);

	auto leftTower_3 = fac->createSoldier(0, 6);
	leftTower_1->setScale(0.8, 0.8);
	leftTower_2->setScale(0.8, 0.8);
	leftTower_3->setScale(0.8, 0.8);
	leftTower_1->setPosition(65, origin.y + visibleSize.height / 2+80);
	leftTower_2->setPosition(leftTower_1->getPosition().x + 110, leftTower_1->getPosition().y + 150);
	leftTower_3->setPosition(leftTower_1->getPosition().x + 110, leftTower_1->getPosition().y - 150);
	auto rightTower_1 = fac->createSoldier(1, 6);
	auto rightTower_2 = fac->createSoldier(1, 6);
	auto rightTower_3 = fac->createSoldier(1, 6);
	rightTower_1->setScale(0.8, 0.8);
	rightTower_2->setScale(0.8, 0.8);
	rightTower_3->setScale(0.8, 0.8);
	rightTower_1->setPosition(visibleSize.width - 65, leftTower_1->getPosition().y);
	rightTower_2->setPosition(rightTower_1->getPosition().x - 110, rightTower_1->getPosition().y + 150);
	rightTower_3->setPosition(rightTower_1->getPosition().x - 110, rightTower_1->getPosition().y - 150);
	log("rightTower location: %f,%f", rightTower_2->getPosition().x, rightTower_2->getPosition().y);
	leftTowers.pushBack(leftTower_1);
	leftTowers.pushBack(leftTower_2);
	leftTowers.pushBack(leftTower_3);
	rightTowers.pushBack(rightTower_1);
	rightTowers.pushBack(rightTower_2);
	rightTowers.pushBack(rightTower_3);
	for (int i = 0; i < 3; i++) {
		addChild(leftTowers.at(i));
		addChild(rightTowers.at(i));
	}
	log("start create bridge");
	//创建桥头
	for (int i = 0; i < 2; ++i)
	{
		auto bridge = Sprite::create("bridge.png");
		bridge->setScale(0.6, 0.6);
		if(i == 0)
			bridge->setPosition(visibleSize.width / 2, leftTower_2->getPosition().y-10);
		else bridge->setPosition(visibleSize.width / 2, leftTower_3->getPosition().y-10);
		bridges.pushBack(bridge);
		addChild(bridge, 1);
	}

	log("create card table");
	//添加卡片信息 设置回调事件
	auto cardFac = cardFac::getInstance();
	cards = cardFac->initCards();
	updateMenu();


	///倒计时部分
	time = Label::createWithTTF("200", "fonts/arial.ttf", 36);
	//schedule(schedule_selector(Fight::update), 1.0f, kRepeatForever, 0);
	dtime = 200;
	time->setPosition(Vec2(origin.x + visibleSize.width - 50,
		origin.y + 30));
	time->setColor(Color3B(255, 122, 179));
	this->addChild(time, 1);

	//调度器
	schedule(schedule_selector(Fight::update), 1.0f, kRepeatForever, 0);
	schedule(schedule_selector(Fight::hitSoldier), 1.0f, kRepeatForever,0);
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Fight::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Fight::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListener,1);
	log("end init");
	return true;
}	
  //获取敌方的信息
 void Fight::getEnemyInfo(){
  
}

void Fight::move() {
	//log("move");
	auto fac = Factory::getInstance();
	auto leftSoldiers = fac->getLeftSoldier();
	auto rightSoldiers = fac->getRightSoldier();
	//log("left: %d, right: %d", leftSoldiers.size(), rightSoldiers.size());
	attackers.clear();
	targets.clear();
	for (auto i : leftSoldiers) {
		int soldierType = i->getTag();
		int type = soldierType < 3 && soldierType != 6 ? 0 : 1;//分是近程还是远程 0 表示近程
		Rect sRect = i->getBoundingBox();
		Rect attackRect = Rect(sRect.getMinX() - 10 - 40 * (type+1), sRect.getMinY() - 10 - 40 * (type + 1), sRect.getMaxX() - sRect.getMinX() + 20 + 80 * (type+1), sRect.getMaxY() - sRect.getMinY() + 20 + 80 * (type + 1));
		//log("attackers location:%f, %f", i->getPosition().x, i->getPosition().y);
		Sprite* target = fac->canAttack(0, attackRect);
		if (target) {
			attackers.pushBack(i);
			targets.pushBack(target);
		}
		else {
			if (i->getPosition().x < bridges.at(0)->getPosition().x) {
				fac->moveSoldier(i, findNearest(i, bridges), 3.0f);
			}
			else {
				fac->moveSoldier(i, findNearest(i, rightTowers), 3.0f);  //否则朝对方的塔移动
				//log("move to tower");
			}
		}
	}
	for (auto i : rightSoldiers) {
		int soldierType = i->getTag();
		int type = soldierType < 3 && soldierType != 6 ? 0 : 1;//分是近程还是远程 0 表示近程
		Rect sRect = i->getBoundingBox();
		Rect attackRect = Rect(sRect.getMinX() - 10 - 40 * (type + 1), sRect.getMinY() - 10 - 40 * (type + 1), sRect.getMaxX() - sRect.getMinX() + 20 + 80 * (type + 1), sRect.getMaxY() - sRect.getMinY() + 20 + 80 * (type + 1));
		Sprite* target = fac->canAttack(1, attackRect);
		if (target) {
			attackers.pushBack(i);
			targets.pushBack(target);
		}
		else {
			if (i->getPosition().x > bridges.at(0)->getPosition().x) {
				fac->moveSoldier(i, findNearest(i,bridges), 3.0f);
			}
			else {
				fac->moveSoldier(i, findNearest(i, leftTowers), 3.0f);  //否则朝对方的塔移动 先写第一个塔 待完善
			}
		}
	}
}
//更新倒计时、圣水、怪物位置等信息
void Fight::update(float dt) {
  //更新倒计时
  if (dtime > 0)
    dtime--;
  char str[5];
  sprintf(str, "%d", dtime); // 将int类型转化为字符串char*类型
  time->setString(str);
  if(dtime<30) //圣水加倍
    waterSpeed*=2;
  //恢复圣水
  pT->runAction(CCProgressTo::create(0.1f, pT->getPercentage() + waterSpeed * 10));

  //移动soldiers
  Fight::move();
  if(isOver())
    stopAction();
}

void Fight::hitSoldier(float dt) {
	log("hit");
	log("attackers: %d,targets: %d", attackers.size(), targets.size());
	auto fac = Factory::getInstance();
	Vector<Sprite*> soldiers = side==0?fac->getLeftSoldier():fac->getRightSoldier();
	for (int i = 0; i < attackers.size(); i++){
		fac->startAttack(attackers.at(i));
		bool killed = fac->beAttacked(targets.at(i), attackers.at(i)->getTag());
		if (killed) {
			auto tmp = targets.at(i);
			fac->endAttack(attackers.at(i));
			fac->removeSoldier(targets.at(i));
			if (tmp == leftTowers.at(0) || tmp == rightTowers.at(0))
				endGame();
			leftTowers.eraseObject(targets.at(i));
			rightTowers.eraseObject(targets.at(i));
		}
	}
}

//判断游戏是否结束,时间到||一方塔全被攻破
bool Fight::isOver(){
  if(dtime<=0) return true; 
  auto fac = Factory::getInstance();
  //判断塔是否被推翻
  return false;
}

void Fight::stopAction(){
  unschedule(schedule_selector(Fight::hitSoldier));
  unschedule(schedule_selector(Fight::update));
  Fight::endGame();//游戏结束画面
}


bool Fight::onTouchBegan(Touch *touch, Event *event) {
	return true;
}

bool Fight::onTouchEnded(Touch *touch, Event *event) {
	log("touch end");
	/*
	if (buttonMenu->getBoundingBox().containsPoint(touch->getLocation()))
		return false;
  //放置士兵
	log("not in menu");
	*/
  Vec2 pos = touch->getLocation();
  //只能放在自己方的一边 需再清楚界定范围 
  if (side == 0 && pos.x<visibleSize.width / 2 || side == 1 && pos.x>visibleSize.width / 2) {
	  createSoldier(touch->getLocation());
	  return true;
  }
  return false;
}


bool Fight::createSoldier(Vec2 pos){
  //判断圣水是否足够 
  if(pT->getPercentage() < 30 || clickCard == nullptr)
    return false;
  //创建soldier并添加
  auto fac = Factory::getInstance();
  auto s = fac->createSoldier(side,clickCard->getTag());
  s->setPosition(pos);
  addChild(s, 3);
  //消耗圣水
  pT->runAction(CCProgressTo::create(0.1f, pT->getPercentage() - 30));
  int index = cards.getIndex(clickCard);
  cards.replace(index, cardFac::getInstance()->replaceCard(clickCard));
  buttonMenu->removeFromParentAndCleanup(true);
  updateMenu();
  clickCard = nullptr;
  return true;
}

void Fight::setSelectedCard(Ref* pSender,Sprite* select) {
	log("set card succeed");
	clickCard = select;
}



void Fight::getSide() {
	side = 0;
}
void Fight::endGame() {
	SimpleAudioEngine::getInstance()->playEffect("music/endgame.wav", false);
}
Vec2 Fight::findNearest(Sprite* soldier, Vector<Sprite*>& arr) {
	Vec2 nearest = arr.at(0)->getPosition();
	float minDistance = soldier->getPosition().distance(arr.at(0)->getPosition());
	for (int i = 1; i < arr.size(); i++) {
		float dis = arr.at(i)->getPosition().distance(soldier->getPosition());
		if (dis < minDistance) {
			minDistance = dis;
			nearest = arr.at(i)->getPosition();
		}
	}
	return nearest;
}

void Fight::updateMenu() {
	auto card_1 = MenuItemSprite::create(cards.at(0), cards.at(0), CC_CALLBACK_1(Fight::setSelectedCard, this, cards.at(0)));
	auto card_2 = MenuItemSprite::create(cards.at(1), cards.at(1), CC_CALLBACK_1(Fight::setSelectedCard, this, cards.at(1)));
	auto card_3 = MenuItemSprite::create(cards.at(2), cards.at(2), CC_CALLBACK_1(Fight::setSelectedCard, this, cards.at(2)));
	auto card_4 = MenuItemSprite::create(cards.at(3), cards.at(3), CC_CALLBACK_1(Fight::setSelectedCard, this, cards.at(3)));
	card_1->setScaleX(0.5);
	card_1->setScaleY(0.5);
	card_2->setScaleX(0.5);
	card_2->setScaleY(0.5);
	card_3->setScaleX(0.5);
	card_3->setScaleY(0.5);
	card_4->setScaleX(0.5);
	card_4->setScaleY(0.5);
	//设置卡片的位置
	card_1->setPosition(Vec2(100 + visibleSize.width / 6, 80));
	card_2->setPosition(Vec2(100 + 2 * visibleSize.width / 6, 80));
	card_3->setPosition(Vec2(100 + 3 * visibleSize.width / 6, 80));
	card_4->setPosition(Vec2(100 + 4 * visibleSize.width / 6, 80));
	//设置大小
	card_1->setScale(0.8, 0.8);
	card_2->setScale(0.8, 0.8);
	card_3->setScale(0.8, 0.8);
	card_4->setScale(0.8, 0.8);

	//设置卡片边框
	for (int i = 0; i < 4; i++) {
		auto frame = Sprite::create("frame.png");
		frame->setPosition(Vec2(100 + (i+1) * visibleSize.width / 6, 80));
		frame->setScale(1.6, 1.1);
		addChild(frame, 1);
	}

	buttonMenu = Menu::create(card_1, card_2, card_3, card_4, NULL);
	buttonMenu->setPosition(Vec2(0 + side * origin.x / 2, 0));
	this->addChild(buttonMenu, 1);
}

//预加载音乐文件
void Fight::preloadMusic() {
	auto music = SimpleAudioEngine::getInstance();
	music->preloadBackgroundMusic("music/fight.mp3");
	music->preloadEffect("music/endgame.mp3");


}
//播放背景音乐
void Fight::playBgm() {
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/fight.mp3", true);
}
