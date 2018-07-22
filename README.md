# cocos-final
cocos2d 期末项目

## 项目管理
1. 每个人开一个分支，把更新的代码放到分支里面，然后请求合并到test分支
2. 每个分支里面只放resource和classes，自己在本地创建项目
3. <strong>不要直接修改test和master</strong>
4. 分支名feature/{实现的功能}。如feature/server
5. 服务端运行, 在根目录: node server/bin/www

## 基本架构
### 前端用cocos2d 3.x，用物理引擎渲染画面
  + 一共四个画面：登陆，用户界面，对战界面，结算界面
  + 对战匹配：（有待商榷）
  + 对战界面：
    1. 放出卡牌：向服务端发出一个事件请求，事件请求加入缓冲池
    2. 怪物在攻击范围内会对敌方怪物进行攻击，有一定的攻击优先级：怪->塔（有待商榷）
    3. 怪物死亡：向服务端发出一个怪物死亡事件声明。（塔的死亡同理）
    4. 对战胜利：向服务端发出对战胜利声明。
    5. 倒计时
    6. 圣水数量和恢复速度
  + 登陆界面：参考上次的用户登陆
    1. 有注册和登陆两个功能。
    2. cookie的保存，保持登陆。
  + 用户界面：
    1. 卡牌管理界面
    2. 积分查看和对战记录查看（有待商榷）
    3. 发起对战
    4. 登出
  + 结算界面：
    1. 胜利方攻破塔数量，失败方攻破塔数量
    2. 积分结算
### 后端用node搭建
  + 用户信息管理：用户卡牌，用户积分，用户对战记录，用户基本信息
  + 对战信息管理：对战信息缓冲池，对战事件缓冲池，每隔一定时长的对战信息更新，对战结果记录
    1. 对战开始时，初始化缓冲池，并初始化LSN=0 
    2. 当接收到一个对战事件请求时，将其放入对战事件缓冲池，LSN=LSN+1
    3. 每隔一定时长的对战信息更新：向两个客户端发送事件缓冲池中的事件。
      - 每次发送更新信息时：有LSN和更新信息（事件和同步信息（不一定需要））
      - 客户端发送更新请求的时候：发送的数据中要包含上次更新收到的LSN，当前对战信息（怪物位置，血量，状态，塔的血量）
      - 当两个客户更新请求的LSN不同时，同步信息为LSN较大值的客户端的对战信息；相同时，同步信息为空
    4. 当接收到怪物死亡声明时，加入到对战事件缓冲池。
    5. 当接收到游戏结束声明时，加入到对战事件缓冲池。当该事件发出后，记录到对战结果中，同步到用户对战记录中。
### API设计
  + 注册POST:api/register, request-data: {name: {用户名}, password: {密码}}, 返回信息: {status: ok/dulplicated username}
  + 登陆POST:api/login, request-data: {name: {用户名}, password: {密码}}, 返回信息: {status: ok/username error/password error}
  + 对战申请POST: api/battle/waiting/{用户名}, 返回信息: {status: ok}
  + 对战开始轮询GET: api/battle/begin/{用户名}, 返回信息: {status: begin/waiting, id: {对战id}}
  + 发送对战事件POST: api/battle/event, request-data: {id: 对战id, event: {eventType: {monster | end-battle }, monsterType: {turret | monster | null}, master: {用户名}, action: {die | create | null}, positionX: {位置的x坐标}, positionY: {位置的y坐标}}}, 返回信息: {status: ok}
  + 对战事件轮询GET: api/battle/event/battle-id/{对战id}/LSN/{当前LSN}, 返回信息: {status: ok, event: [{eventType: {monster | end-battle }, monsterType: {turret | monster | null}, master: {用户名}, action: {die | create | null}, positionX: {位置的x坐标}, positionY: {位置的y坐标}}}]
