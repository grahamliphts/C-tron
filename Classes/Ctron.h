#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <ui\UIButton.h>

class Ctron : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuServerCallback(cocos2d::Ref* pSender);
	void menuClientCallback(cocos2d::Ref* pSender);
private:
	cocos2d::ui::Button *clientButton, *serverButton;
    // implement the "static create()" method manually
    CREATE_FUNC(Ctron);
	
};

#endif // __HELLOWORLD_SCENE_H__
