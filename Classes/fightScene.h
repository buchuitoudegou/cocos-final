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
  //加载音乐文件
  void getEnemyInfo();//获取敌人信息
  void setSelectedCard(Ref* pSender, Sprite* select);
 void getSide();//获取我方的边
 bool isOver();//判断游戏是否结束
 void stopAction(); //游戏结束解绑事件
 void endGame(); //结束画面
    // implement the "static create()" method manually
  CREATE_FUNC(Fight);
private:
  int side;//记录本次对战我方的位置
  bool onTouchBegan(Touch *touch, Event *event); //放置士兵监听的事件
  // void onTouchMoved(Touch *touch, Event *event);
  bool onTouchEnded(Touch *touch, Event *event);
  Vec2 findNearest(Sprite* soldier, Vector<Sprite*>& arr);
  void updateMenu();
  cocos2d::Size visibleSize;
  cocos2d::Label* time;
  Vector<Sprite*> bridges;
  Vec2 origin;
  Sprite* clickCard;//选择卡片
  int water;//圣水
  int waterSpeed;//圣水恢复速度
  int dtime;
  int LSN; //与服务器更新的标识
  bool winSide;//标志是哪一边赢
  cocos2d::ProgressTimer* pT;
  Vector<Sprite*> attackers;
  Vector<Sprite*> targets;
  Vector<Sprite*> cards;
  Vector<Sprite*> leftTowers;
  Vector<Sprite*> rightTowers;
  Menu* buttonMenu;
  static std::string name;
};
