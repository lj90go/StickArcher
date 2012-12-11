//
//  FHLayor.h
//  StickArcher
//
//  Created by 邓子安 on 12-12-11.
//
//

#ifndef __StickArcher__FHLayor__
#define __StickArcher__FHLayor__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

class FHLayer : public cocos2d::CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
	static FHLayer* sharedLayer();
    
	void CreateEnemy();
	void ActionDone(CCNode*);
	void GameLogic(float time);
	void CollisionDetection(float time);
	void Shoot(float time);
    
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    CREATE_FUNC(FHLayer);
    
public:
	static double bulletDamage;
    
private:
	CCSize playerSize;
	CCArray bulletArray;
	CCArray enemyArray;
	cc_timeval lastTime;
	double attackDelay;
	CCPoint touchPosition;
	static FHLayer* m_sharedLayer;
};


#endif /* defined(__StickArcher__FHLayor__) */
