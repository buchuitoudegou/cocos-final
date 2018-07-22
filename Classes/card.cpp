#include"card.h"
#include <vector>
#include <string>
USING_NS_CC;
cardFac* cardFac::factory = NULL;
cardFac::cardFac(){
	cardsArr.reserve(4);
}
cardFac* cardFac::getInstance(){
	if (factory == NULL) {
		factory = new cardFac();
	}
	return factory;
}

std::string cardFac::getSoldierType(int type){
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
Vector<Sprite*>& cardFac::initCards(){
	for(int i = 0; i < 4; i++){
		cardsArr.pushBack(getValidCard());
	}
	return cardsArr;
}

Sprite* cardFac::replaceCard(Sprite* toReplace){
	cardsArr.eraseObject(toReplace);
	Sprite* next = getValidCard();
	cardsArr.pushBack(next);
	return next;
}

Sprite* cardFac::getValidCard(){
	int arr[6] = {1,1,1,1,1,1};
	for(auto i : cardsArr){
		arr[i->getTag()] = 0;
	}
	std::vector<int> validType;
	for(int i = 0; i < 6; i++){
		if(arr[i])
			validType.push_back(i);
	}
	auto rand = RandomHelper();
	int selectedType =validType[rand.random_int(0, (int)validType.size() - 1)];
	std::string soldier = getSoldierType(selectedType);
	std::string fileName = soldier + ".png";
	auto next = Sprite::create(fileName.c_str());
	next->setTag(selectedType);
	return next;
}