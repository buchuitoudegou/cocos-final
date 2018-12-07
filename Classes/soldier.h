#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class Factory :public cocos2d::Ref {
public:
	//获取单例工厂
	static Factory* getInstance();
	/*生成一个士兵，并存储到容器中管理，并添加血条，type指定类型，side指定是左边还是右边
	类型按照0~6依次为火近，水近，木近，火远，水远，木远，防御塔，此类型也会作为tag添加到Sprite中，
	默认播放行走动画*/
	Sprite* createSoldier(int side,int type);
	//移动士兵
	void moveSoldier(Sprite* toMove, Vec2 targetPos, float time);
	//移除怪物
	void removeSoldier(Sprite*);
	//判断是否有对方的士兵在攻击范围内，如果有则返回攻击目标
	Sprite* canAttack(int side,Rect rect);
	//开始播放攻击动画
	void startAttack(Sprite* attacker);
	//结束攻击动画，继续播放行走动画
	void endAttack(Sprite* attacker);
	//怪物被攻击，给定目标和伤害类型，返回值确定是否死亡
	bool beAttacked(Sprite* target, int type);
	//初始化士兵的帧动画
	void initSpriteFrame();
	Vector<Sprite*>& getRightSoldier();
	Vector<Sprite*>& getLeftSoldier();
private:
	Factory();
	std::string getSoldierType(int type);
	Vector<Sprite*> leftSoldier;
	Vector<Sprite*> rightSoldier;
	//依次为火近的行走，攻击，死亡，根据以上type的类型类推
	static Factory* factory;
};
