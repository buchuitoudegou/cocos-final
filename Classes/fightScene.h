#pragma once
#include "cocos2d.h"
#include "soldier.h"
#include "card.h"
extern "C"
using namespace cocos2d;

class Fight : public cocos2d::Scene
{
public:
  static cocos2d::Scene* createScene(std::string name);
  virtual bool init();
  void update(float dt) override;
  void hitSoldier(float dt);
  void move();
  bool createSoldier(Vec2 pos);
  void preloadMusic();
  void playBgm();
  //���������ļ�
  void getEnemyInfo();//��ȡ������Ϣ
  void setSelectedCard(Ref* pSender, Sprite* select);
 void getSide();//��ȡ�ҷ��ı�
 bool isOver();//�ж���Ϸ�Ƿ����
 void stopAction(); //��Ϸ��������¼�
 void endGame(); //��������
    // implement the "static create()" method manually
  CREATE_FUNC(Fight);
private:
  int side;//��¼���ζ�ս�ҷ���λ��
  bool onTouchBegan(Touch *touch, Event *event); //����ʿ���������¼�
  // void onTouchMoved(Touch *touch, Event *event);
  bool onTouchEnded(Touch *touch, Event *event);
  Vec2 findNearest(Sprite* soldier, Vector<Sprite*>& arr);
  void updateMenu();
  cocos2d::Size visibleSize;
  cocos2d::Label* time;
  Vector<Sprite*> bridges;
  Vec2 origin;
  Sprite* clickCard;//ѡ��Ƭ
  int water;//ʥˮ
  int waterSpeed;//ʥˮ�ָ��ٶ�
  int dtime;
  int LSN; //����������µı�ʶ
  bool winSide;//��־����һ��Ӯ
  cocos2d::ProgressTimer* pT;
  Vector<Sprite*> attackers;
  Vector<Sprite*> targets;
  Vector<Sprite*> cards;
  Vector<Sprite*> leftTowers;
  Vector<Sprite*> rightTowers;
  Menu* buttonMenu;
  static std::string name;
};
