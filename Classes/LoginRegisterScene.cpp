#include "LoginRegisterScene.h"
#include "ui\CocosGUI.h"
#include "json\document.h"
#include "Utils.h"
#include "network\HttpClient.h"
#include "json/document.h"
#include "UserScene.h"

USING_NS_CC;
using namespace cocos2d::network;
using namespace cocos2d::ui;

cocos2d::Scene * LoginRegisterScene::createScene() {
  return LoginRegisterScene::create();
}

bool LoginRegisterScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();


  //设置背景
  auto bg = Sprite::create("loginbg.png");
  bg->setAnchorPoint(Vec2(0.5, 0.5));
  bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
  bg->setScale(2.5,2.5);
  this->addChild(bg, 0);


//注册和登录按钮的背景
  auto login = MenuItemImage::create("loginbtn.png", "loginbtn-down.png", CC_CALLBACK_1(LoginRegisterScene::loginButtonCallback, this));
  auto res = MenuItemImage::create("loginbtn.png", "loginbtn-down.png", CC_CALLBACK_1(LoginRegisterScene::registerButtonCallback, this));
  MenuItemFont::setFontSize(25);
  auto loginButton = Label::createWithSystemFont("Login", "Arial", 25);
  //auto loginButton = MenuItemFont::create("Login", CC_CALLBACK_1(LoginRegisterScene::loginButtonCallback, this));
  if (loginButton) {
	  loginButton->setColor(Color3B(234, 229, 194));
    float x = origin.x + visibleSize.width / 2-120;
    float y = origin.y + loginButton->getContentSize().height / 2+100;
    loginButton->setPosition(Vec2(x, y));
	login->setPosition(Vec2(x, y));
  }


  auto registerButton = Label::createWithSystemFont("Register", "Arial", 25);
  if (registerButton) {
	  registerButton->setColor(Color3B(234, 229, 194));

	  float x = origin.x + visibleSize.width / 2+120;
	  float y = origin.y + registerButton->getContentSize().height / 2+100;
	  registerButton->setPosition(Vec2(x, y));
	  res->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(login,res,NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);
  this->addChild(loginButton, 2);
  this->addChild(registerButton, 2);
  
  //输入框背景 


  usernameInput = TextField::create("username", "arial", 24);
  if (usernameInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    usernameInput->setPosition(Vec2(x, y));
    this->addChild(usernameInput, 2);
	auto input = Sprite::create("input.png");
	input->setAnchorPoint(Vec2(0.5, 0.5));
	input->setPosition(Vec2(x, y));
	this->addChild(input, 1);
  }

  passwordInput = TextField::create("password", "arial", 24);
  if (passwordInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 160.0f;
    passwordInput->setPosition(Vec2(x, y));
    this->addChild(passwordInput, 2);
	auto input = Sprite::create("input.png");
	input->setAnchorPoint(Vec2(0.5, 0.5));
	input->setPosition(Vec2(x, y));
	this->addChild(input, 1);
  }

  messageBox = Label::create("", "arial", 30);
  if (messageBox) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 300.0f;
    messageBox->setPosition(Vec2(x, y));
	messageBox->setColor(Color3B(116, 163, 94));
    this->addChild(messageBox, 1);
  }

  return true;
}

void LoginRegisterScene::loginButtonCallback(cocos2d::Ref * pSender) {
	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setUrl("http://127.0.0.1:3000/api/login");
	request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::onHttpRequestCompleted, this));
	//std::string  postData = "{ \"name\":\"" + usernameInput->getString() + "\"," + "\"password\":\"" + passwordInput->getString() + "\"}";
	std::string  postData = "{name:" + usernameInput->getString() + "," + "password:" + passwordInput->getString() + "}";
  request->setRequestData(postData.c_str(), postData.length());
  // HttpClient::getInstance()->enableCookies("cookie");
  cocos2d::network::HttpClient::getInstance()->send(request);
  request->release();

}

void LoginRegisterScene::registerButtonCallback(Ref * pSender) {
  HttpRequest* request = new HttpRequest();
  request->setRequestType(HttpRequest::Type::POST);  
  request->setUrl("http://127.0.0.1:3000/api/register");
  request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::onHttpRegisterRequestCompleted, this)
  );
  //std::string  postData = "{ \"name\":\""+ usernameInput->getString()+"\","+"\"password\":\""+ passwordInput->getString()+"\"}";
  std::string  postData = "{name:" + usernameInput->getString() + "," + "password:" + passwordInput->getString() + "}";

  request->setRequestData(postData.c_str(), postData.length());
  cocos2d::network::HttpClient::getInstance()->send(request);
  request->release();
}
void LoginRegisterScene::onHttpRequestCompleted(HttpClient* sender, HttpResponse* response) {
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
    this->messageBox->setString("Login successfully.");
	//登录成功跳转界面
	//Director::getInstance()->replaceScene(
	//	TransitionSlideInT::create(0.6f, UserScene::createScene()));
  }
  else {
    this->messageBox->setString(std::string("Login fail.\n") + doc["status"].GetString());
  }
}

void LoginRegisterScene::onHttpRegisterRequestCompleted(HttpClient* sender, HttpResponse* response) {
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
    this->messageBox->setString("Register successfully");
  }
  else {
    this->messageBox->setString(std::string("Register fail.\n") + doc["status"].GetString());
  }
}
