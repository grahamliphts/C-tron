#include "Ctron.h"

#include "Client.h"
#include "Server.h"

USING_NS_CC;

Scene* Ctron::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Ctron::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Ctron::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Ctron::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2, origin.y + closeItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
	clientButton = cocos2d::ui::Button::create();
	clientButton->setTitleText("Join"); clientButton->setTitleFontSize(24); clientButton->setTitleFontName("fonts/Marker Felt.ttf");
	clientButton->setPosition(Vec2(origin.x + 150, origin.y + visibleSize.height - clientButton->getContentSize().height));
	clientButton->addClickEventListener(CC_CALLBACK_1(Ctron::menuClientCallback, this));
	this->addChild(clientButton);

	serverButton = ui::Button::create();
	serverButton->setTitleText("Host"); serverButton->setTitleFontSize(24); serverButton->setTitleFontName("fonts/Marker Felt.ttf");
	serverButton->setPosition(Vec2(origin.x + 100, origin.y + visibleSize.height - serverButton->getContentSize().height));
	serverButton->addClickEventListener(CC_CALLBACK_1(Ctron::menuServerCallback, this));

	this->addChild(serverButton);
	
    auto label = Label::createWithTTF("1st Build", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);

    auto sprite = Sprite::create("tron-sprite_blue_player.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);
    
    return true;
}


void Ctron::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void Ctron::menuServerCallback(Ref* pSender)
{
	clientButton->setVisible(false);
	serverButton->setVisible(false);

	Server *server = new Server();
	int value = server->createSocket();
	if (value == 0)
		CCLOG("Success");
}

void Ctron::menuClientCallback(Ref* pSender)
{
	clientButton->setVisible(false);
	serverButton->setVisible(false);

	Client *client = new Client();
	int value = client->createSocket();
	if (value == 0)
		CCLOG("Success");
	value = client->sendToServer("Tesssssst\0");
	if (value == 0)
		CCLOG("Success");
}
