//
//  FHLayor.cpp
//  StickArcher
//
//  Created by 邓子安 on 12-12-11.
//
//

#include "FHLayer.h"
#include "FHSprite.h"

double FHLayer::bulletDamage = 0;
FHLayer* FHLayer::m_sharedLayer = NULL;

CCScene* FHLayer::scene()
{
    CCScene * scene = NULL;
    do
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        
        // 'layer' is an autorelease object
        FHLayer *layer = FHLayer::create();
        CC_BREAK_IF(! layer);
		
        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);
    
    // return the scene
    return scene;
}

FHLayer* FHLayer::sharedLayer()
{
	return m_sharedLayer;
}

// on "init" you need to initialize your instance
bool FHLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! CCLayer::init());
		this->initWithColor(ccc4(255, 255, 255, 255));
		this->setTouchEnabled(true);
        
		CCSize viewSize = this->getContentSize();
		CCSprite* player = CCSprite::create("Player.png");
		playerSize = player->getContentSize();
		player->setPosition(ccp(player->getContentSize().width / 2, viewSize.height / 2));
		this->addChild(player);
        
		srand((unsigned)time(NULL));
		
		this->schedule(schedule_selector(FHLayer::GameLogic), 1.0);
		this->schedule(schedule_selector(FHLayer::CollisionDetection), 0.05f);
        
		CCTime::gettimeofdayCocos2d(&lastTime, NULL);
		attackDelay = 0.4;
		bulletDamage = 4;
		m_sharedLayer = this;
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void FHLayer::CreateEnemy()
{
	CCSize size = this->getContentSize();
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("Monster.png");
	int textureWidth = pTexture->getContentSize().width;
	int textureHeight = pTexture->getContentSize().height;
	CCLog("width=%d,height=%d",textureWidth, textureHeight);
	const int col = 4;
	int spriteWidth = textureWidth / col;
    
	CCSpriteFrame* enemyFrame = CCSpriteFrame::createWithTexture(pTexture,CCRectMake(0,0,spriteWidth,textureHeight));
	FHSprite* enemy = FHSprite::createWithSpriteFrame(enemyFrame);
	enemy->setScale(0.5);
	enemy->retain();
    
	int randomHeight = enemy->getContentSize().height / 2 + rand() % (int)(size.height - enemy->getContentSize().height);
	enemy->setPosition(ccp(size.width + enemy->getContentSize().width / 2, randomHeight));
	this->addChild(enemy);
	enemy->setTag(1);
	enemyArray.addObject(enemy);
    
	CCArray* pAnimFrames = new CCArray();
	for(int i = 0; i != col; ++i)
	{
		CCSpriteFrame* pSpriteFrame = CCSpriteFrame::createWithTexture(pTexture,CCRectMake(spriteWidth * i, 0, spriteWidth, textureHeight));
		pAnimFrames->addObject(pSpriteFrame);
	}
    
	//delayƒ¨»œ «0
	CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pAnimFrames, 2 / enemy->Speed());
	pAnimFrames->release();
	enemy->runAction(CCRepeatForever::create(CCAnimate::create(pAnimation)));
	
	CCFiniteTimeAction* enemyMove = CCMoveBy::create(enemy->Speed(), ccp(-size.width - enemy->getContentSize().width, 0));
	CCFiniteTimeAction* moveDone = CCCallFuncN::create(this, callfuncN_selector(FHLayer::ActionDone));
	enemy->runAction(CCSequence::create(enemyMove, moveDone, NULL));
    
}

void FHLayer::ActionDone(CCNode* node)
{
	this->removeChild(node, true);
	if (node->getTag() == 1)
	{
		enemyArray.removeObject(node);
	}
	if (node->getTag() == 2)
	{
		bulletArray.removeObject(node);
	}
}

void FHLayer::GameLogic(float time)
{
	this->CreateEnemy();
}

void FHLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	cc_timeval currentTime;
	CCTime::gettimeofdayCocos2d(&currentTime, NULL);
    CCTouch* touch = (CCTouch*)pTouches->anyObject();
	touchPosition = touch->getLocation();
	if (CCTime::timersubCocos2d(&lastTime, &currentTime) / 1000 >= attackDelay)
	{
		Shoot(1);
	}
	this->schedule(schedule_selector(FHLayer::Shoot), attackDelay);
}

void FHLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	touchPosition = touch->getLocation();
}

void FHLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	this->unschedule(schedule_selector(FHLayer::Shoot));
}

void FHLayer::CollisionDetection(float time)
{
	if (enemyArray.count() == 0 || bulletArray.count() == 0)
	{
		return;
	}
    
	for (unsigned int i = 0; i < enemyArray.count(); i++)
	{
		CCRect enemyRect = ((FHSprite*)enemyArray.objectAtIndex(i))->boundingBox();
		for (unsigned int j = 0; j < bulletArray.count(); j++)
		{
			CCRect bulletRect = ((FHSprite*)bulletArray.objectAtIndex(j))->boundingBox();
			if (enemyRect.intersectsRect(bulletRect))
			{
				bool isRemoved;
				((FHSprite*)enemyArray.objectAtIndex(i))->Hitted(bulletDamage, isRemoved);
				if (isRemoved)
				{
					enemyArray.removeObjectAtIndex(i);
				}
				this->removeChild((CCNode*)bulletArray.objectAtIndex(j), true);
				bulletArray.removeObjectAtIndex(j);
			}
		}
	}
}

void FHLayer::Shoot(float time)
{
	CCSize viewSize = this->getContentSize();
	CCSprite* bullet = CCSprite::create("Bullet.png");
	CCSize bulletSize = bullet->getContentSize();
	bullet->setPosition(ccp(bulletSize.width / 2 + playerSize.width, viewSize.height / 2));
	if (touchPosition.x < bullet->getPosition().x)
	{
		return;
	}
	this->addChild(bullet);
	bullet->setTag(2);
	bulletArray.addObject(bullet);
    
	float vectorX = touchPosition.x - bullet->getPosition().x;
	float vectorY = touchPosition.y - bullet->getPosition().y;
	float tanAngle = vectorY / vectorX;
	float cutOffVectorX = viewSize.width - bullet->getPosition().x;
	float cutOffVectorY = vectorY > 0 ? viewSize.height - bullet->getPosition().y : - bullet->getPosition().y;
	float tanCutOff = cutOffVectorY / cutOffVectorX;
	CCPoint shootVector;
	if (abs(tanAngle) > abs(tanCutOff))
	{
		shootVector.y = (viewSize.height / 2 + bulletSize.height / 2) * vectorY / abs(vectorY);
		shootVector.x = shootVector.y / tanAngle;
	}
	else
	{
		shootVector.x = viewSize.width - playerSize.width;
		shootVector.y = shootVector.x * tanAngle;
	}
	float distance = sqrt(shootVector.x * shootVector.x + shootVector.y * shootVector.y);
	CCFiniteTimeAction* shoot = CCMoveBy::create(distance / 500, shootVector);
	CCFiniteTimeAction* shootDone = CCCallFuncN::create(this, callfuncN_selector(FHLayer::ActionDone));
	bullet->runAction(CCSequence::create(shoot, shootDone, NULL));
    
	CCTime::gettimeofdayCocos2d(&lastTime, NULL);
}