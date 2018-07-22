#include"soldier.h"
#include <string>
USING_NS_CC;
using std::string;
Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}

std::string Factory::getSoldierType(int type){
	std::string soldier;
	if(type == 0)
		soldier="fire-short";
	else if(type == 1)
		soldier="water-short";
	else if(type == 2)
		soldier="solid-short";
	else if(type == 3)
		soldier="fire-remote";
	else if(type == 4)
		soldier="water-remote";
	else if(type == 5)
		soldier="solid-remote";
	return soldier;
}

void Factory::initSpriteFrame() {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("soldier.plist");
	int soldierType = 6, actionType = 3;
	for(int i = 0;i < soldierType; i++){
		std::string soldier = getSoldierType(i);
		for(int j = 0;j < actionType; j++){
			std::string action;
			int totalFrames;
			if(j==0){
				action="walk";
				totalFrames=8;
			}
			if(j==1){
				action="attack";
				totalFrames=13;
			}
			if(j==2){
				action="dead";
				totalFrames=34;
			}
			Animation* nextAnimation = Animation::create();
			for(int k=0;k<totalFrames;k++){
				char num[10];
				sprintf(num,"%03d",k+1);
				std::string frameName = soldier + "-" + action + "-" + num + ".png";
				nextAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
			}
			nextAnimation->setDelayPerUnit(0.1);
			std::string animationName = soldier + "-" + action;
			AnimationCache::getInstance()->addAnimation(nextAnimation, animationName.c_str());
		}
	}
	Animation* attackAnimation = Animation::create();
	for (int i = 0; i < 27; i++) {
		char num[10];
		sprintf(num, "%03d", i + 1);
		std::string frameName = "tower-";
		frameName = frameName + num + ".png";
		log(frameName.c_str());
		attackAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str()));
	}
	attackAnimation->setDelayPerUnit(0.03);
	AnimationCache::getInstance()->addAnimation(attackAnimation, "tower");
	log("create tower");
}

void Factory::moveSoldier(Sprite* toMove, Vec2 targetPos, float time){
	if (toMove->getTag() == 6)
		return;
	Vec2 Pos = toMove->getPosition();
	Vec2 dir = targetPos - Pos;
	dir.normalize();
	toMove->runAction(MoveBy::create(time, dir * 30));
}

void Factory::removeSoldier(Sprite* toRemove){
	int type = toRemove->getTag();
	if (type == 6) {
		log("remove tower");
		toRemove->removeFromParent();
		leftSoldier.eraseObject(toRemove);
		rightSoldier.eraseObject(toRemove);
		return;
	}
	std::string soldier = getSoldierType(type);
	Animation* dead = AnimationCache::getInstance()->getAnimation((soldier + "-dead").c_str());
	Animate* ani = Animate::create(dead);
	Sequence* seq = Sequence::create(ani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, toRemove)), NULL);
	toRemove->runAction(seq);
	leftSoldier.eraseObject(toRemove);
	rightSoldier.eraseObject(toRemove);
}

Sprite* Factory::canAttack(int side,Rect rect){
	//如果所处边为左边，则可以攻击右边的怪
	if(side == 0){
		for (auto i : rightSoldier) {
			if (rect.containsPoint(i->getPosition())) {
				return i;
			}
		}
	}
	//否则可以攻击左边的怪
	else{
		for (auto i : leftSoldier) {
			if (rect.containsPoint(i->getPosition())) {
				return i;
			}
		}
	}
	return NULL;
}

void Factory::startAttack(Sprite* attacker){
	attacker->stopAllActions();
	int type = attacker->getTag();
	//log("start attack: %d", type);
	if (type == 6) {
		log("tower start attack");
		Animation* attack = AnimationCache::getInstance()->getAnimation("tower");
		if (attack == nullptr)
			log("attack is null");
		Animate* ani = Animate::create(attack);
		attacker->runAction(ani);
		return;
	}
	string soldier = getSoldierType(type);
	Animation* attack = AnimationCache::getInstance()->getAnimation((soldier + "-attack").c_str());
	Animate* ani = Animate::create(attack);
	attacker->runAction(ani);
}

void Factory::endAttack(Sprite* attacker){
	attacker->stopAllActions();
	int type = attacker->getTag();
	log("end attack: %d", type);
	if (type == 6)
		return;
	string soldier = getSoldierType(type);
	Animation* walk = AnimationCache::getInstance()->getAnimation((soldier + "-walk").c_str());
	Animate* ani = Animate::create(walk);
	attacker->runAction(RepeatForever::create(ani));
}

Sprite* Factory::createSoldier(int side,int type){
	string soldier = getSoldierType(type);
	Sprite* newSoldier;
	string frameName;
	if (type == 6)
		frameName = "tower-001.png";
	else frameName = soldier + "-" + "walk-001.png";
	newSoldier = Sprite::createWithSpriteFrameName(frameName.c_str());
	if (side == 0)
		leftSoldier.pushBack(newSoldier);
	else rightSoldier.pushBack(newSoldier);
	Sprite* hpBar = Sprite::create("hpBar.png");
	hpBar->setAnchorPoint(Vec2(0, 0));
	hpBar->setPosition(Vec2(0,0));
	Sprite* hp;
	if(side==0)
		hp = Sprite::create("redBlood.png");
	else hp = Sprite::create("blueBlood.png");
	auto pT = ProgressTimer::create(hp);
	pT->setScaleX(60);
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(newSoldier->getContentSize().width/2+10,newSoldier->getContentSize().height-5));
	pT->setTag(100);
	//newSoldier->addChild(hpBar);
	newSoldier->addChild(pT,2);
	newSoldier->setTag(type);
	endAttack(newSoldier); //播放行走动画
	newSoldier->setScaleX(0.5);
	newSoldier->setScaleY(0.5);
	if (side == 1)
		newSoldier->setFlippedX(true);
	return newSoldier;
}

float calculateMul(int attackerType,int targetType){
	if(attackerType == targetType)
		return 1;
	//水克火
	if(attackerType == 0 && targetType == 1)
		return 0.75;
	else if(attackerType == 1 && targetType == 0)
		return 1.5;
	//火克木
	else if(attackerType == 0 && targetType == 2)
		return 1.5;
	else if(attackerType == 2 && targetType == 0)
		return 0.75;
	//木克水
	else if(attackerType == 1 && targetType == 2)
		return 0.75;
	else if(attackerType == 2 && targetType == 1)
		return 1.5;
	log("error");
	return 1;
}
bool Factory::beAttacked(Sprite* target, int type){
	int targetType = target->getTag();
	float multiple = calculateMul(type % 3,targetType % 3);
	ProgressTimer* pT =(ProgressTimer*)target->getChildByTag(100);
	auto progressMove = ProgressFromTo::create(0.5, pT->getPercentage(), pT->getPercentage() - 10*multiple);
	pT->runAction(progressMove);
	if (pT->getPercentage()-10*multiple <= 0) {
			return true;
	}
	return false;
}

Vector<Sprite*>& Factory::getRightSoldier(){
		return rightSoldier;
}
	
Vector<Sprite*>& Factory::getLeftSoldier(){
		return leftSoldier;
}
