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

	//�����ı���
	auto bgbg = Sprite::create("bgbg.png");
	bgbg->setAnchorPoint(Vec2(0.5, 0.5));
	bgbg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bgbg->setScale(1.5, 1.5);
	this->addChild(bgbg, -1);


	//��������
	auto bg = Sprite::create("background.png");
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2+70));
	bg->setScale(1.8, 1.3);
	this->addChild(bg, 0);

	//��ȡ�ҷ��ı�
	// side = Fight::getSide();
	side = 0;
	waterSpeed = 1;//���ó�ʼʥˮ�ָ��ٶ�


	preloadMusic();     // Ԥ��������
	playBgm();          // ���ű�������

	//hp��
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//ʹ��hp������progressBar 
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
	//����˫��������
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
	//������ͷ
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
	//��ӿ�Ƭ��Ϣ ���ûص��¼�
	auto cardFac = cardFac::getInstance();
	cards = cardFac->initCards();
	updateMenu();


	///����ʱ����
	time = Label::createWithTTF("200", "fonts/arial.ttf", 36);
	//schedule(schedule_selector(Fight::update), 1.0f, kRepeatForever, 0);
	dtime = 200;
	time->setPosition(Vec2(origin.x + visibleSize.width - 50,
		origin.y + 30));
	time->setColor(Color3B(255, 122, 179));
	this->addChild(time, 1);

	//������
	schedule(schedule_selector(Fight::update), 1.0f, kRepeatForever, 0);
	schedule(schedule_selector(Fight::hitSoldier), 1.0f, kRepeatForever,0);
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Fight::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Fight::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListener,1);
	log("end init");
	return true;
}	
  //��ȡ�з�����Ϣ
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
		int type = soldierType < 3 && soldierType != 6 ? 0 : 1;//���ǽ��̻���Զ�� 0 ��ʾ����
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
				fac->moveSoldier(i, findNearest(i, rightTowers), 3.0f);  //���򳯶Է������ƶ�
				//log("move to tower");
			}
		}
	}
	for (auto i : rightSoldiers) {
		int soldierType = i->getTag();
		int type = soldierType < 3 && soldierType != 6 ? 0 : 1;//���ǽ��̻���Զ�� 0 ��ʾ����
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
				fac->moveSoldier(i, findNearest(i, leftTowers), 3.0f);  //���򳯶Է������ƶ� ��д��һ���� ������
			}
		}
	}
}
//���µ���ʱ��ʥˮ������λ�õ���Ϣ
void Fight::update(float dt) {
  //���µ���ʱ
  if (dtime > 0)
    dtime--;
  char str[5];
  sprintf(str, "%d", dtime); // ��int����ת��Ϊ�ַ���char*����
  time->setString(str);
  if(dtime<30) //ʥˮ�ӱ�
    waterSpeed*=2;
  //�ָ�ʥˮ
  pT->runAction(CCProgressTo::create(0.1f, pT->getPercentage() + waterSpeed * 10));

  //�ƶ�soldiers
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

//�ж���Ϸ�Ƿ����,ʱ�䵽||һ����ȫ������
bool Fight::isOver(){
  if(dtime<=0) return true; 
  auto fac = Factory::getInstance();
  //�ж����Ƿ��Ʒ�
  return false;
}

void Fight::stopAction(){
  unschedule(schedule_selector(Fight::hitSoldier));
  unschedule(schedule_selector(Fight::update));
  Fight::endGame();//��Ϸ��������
}


bool Fight::onTouchBegan(Touch *touch, Event *event) {
	return true;
}

bool Fight::onTouchEnded(Touch *touch, Event *event) {
	log("touch end");
	/*
	if (buttonMenu->getBoundingBox().containsPoint(touch->getLocation()))
		return false;
  //����ʿ��
	log("not in menu");
	*/
  Vec2 pos = touch->getLocation();
  //ֻ�ܷ����Լ�����һ�� ��������綨��Χ 
  if (side == 0 && pos.x<visibleSize.width / 2 || side == 1 && pos.x>visibleSize.width / 2) {
	  createSoldier(touch->getLocation());
	  return true;
  }
  return false;
}


bool Fight::createSoldier(Vec2 pos){
  //�ж�ʥˮ�Ƿ��㹻 
  if(pT->getPercentage() < 30 || clickCard == nullptr)
    return false;
  //����soldier�����
  auto fac = Factory::getInstance();
  auto s = fac->createSoldier(side,clickCard->getTag());
  s->setPosition(pos);
  addChild(s, 3);
  //����ʥˮ
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
	//���ÿ�Ƭ��λ��
	card_1->setPosition(Vec2(100 + visibleSize.width / 6, 80));
	card_2->setPosition(Vec2(100 + 2 * visibleSize.width / 6, 80));
	card_3->setPosition(Vec2(100 + 3 * visibleSize.width / 6, 80));
	card_4->setPosition(Vec2(100 + 4 * visibleSize.width / 6, 80));
	//���ô�С
	card_1->setScale(0.8, 0.8);
	card_2->setScale(0.8, 0.8);
	card_3->setScale(0.8, 0.8);
	card_4->setScale(0.8, 0.8);

	//���ÿ�Ƭ�߿�
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

//Ԥ���������ļ�
void Fight::preloadMusic() {
	auto music = SimpleAudioEngine::getInstance();
	music->preloadBackgroundMusic("music/fight.mp3");
	music->preloadEffect("music/endgame.mp3");


}
//���ű�������
void Fight::playBgm() {
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/fight.mp3", true);
}
