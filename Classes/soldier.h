#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class Factory :public cocos2d::Ref {
public:
	//��ȡ��������
	static Factory* getInstance();
	/*����һ��ʿ�������洢�������й��������Ѫ����typeָ�����ͣ�sideָ������߻����ұ�
	���Ͱ���0~6����Ϊ�����ˮ����ľ������Զ��ˮԶ��ľԶ����������������Ҳ����Ϊtag��ӵ�Sprite�У�
	Ĭ�ϲ������߶���*/
	Sprite* createSoldier(int side,int type);
	//�ƶ�ʿ��
	void moveSoldier(Sprite* toMove, Vec2 targetPos, float time);
	//�Ƴ�����
	void removeSoldier(Sprite*);
	//�ж��Ƿ��жԷ���ʿ���ڹ�����Χ�ڣ�������򷵻ع���Ŀ��
	Sprite* canAttack(int side,Rect rect);
	//��ʼ���Ź�������
	void startAttack(Sprite* attacker);
	//�������������������������߶���
	void endAttack(Sprite* attacker);
	//���ﱻ����������Ŀ����˺����ͣ�����ֵȷ���Ƿ�����
	bool beAttacked(Sprite* target, int type);
	//��ʼ��ʿ����֡����
	void initSpriteFrame();
	Vector<Sprite*>& getRightSoldier();
	Vector<Sprite*>& getLeftSoldier();
private:
	Factory();
	std::string getSoldierType(int type);
	Vector<Sprite*> leftSoldier;
	Vector<Sprite*> rightSoldier;
	//����Ϊ��������ߣ���������������������type����������
	static Factory* factory;
};
