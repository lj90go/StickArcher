#include "FHSprite.h"
#include "FHLayer.h"

FHSprite::FHSprite()
{
}

FHSprite::~FHSprite(void)
{
}

FHSprite* FHSprite::create(const char *pszFileName)
{
	FHSprite *pobSprite = new FHSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName) && pobSprite->FHInit())
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

FHSprite* FHSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
	FHSprite *pobSprite = new FHSprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame) && pobSprite->FHInit())
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

bool FHSprite::FHInit()
{
	hpLeft = hp = 10;
	speed = 10;
	CCSprite* hpBar = CCSprite::create("HpBar.png");
	CCSprite* hpBG = CCSprite::create("HpBarBg.png");
	hpBar->setAnchorPoint(ccp(0, 0));
	hpBar->setPosition(ccp(this->getContentSize().width / 2 - hpBar->getContentSize().width / 2
		, this->getContentSize().height));
	hpBG->setPosition(ccp(this->getContentSize().width / 2, 
		this->getContentSize().height + hpBar->getContentSize().height / 2));
	this->addChild(hpBar, 1, 1);
	this->addChild(hpBG, 0, 0);
	return true;
}

void FHSprite::Hitted(double damage, bool &isRemoved)
{
	if ((hpLeft -= damage) <= 0)
	{
		FHLayer::sharedLayer()->removeChild(this, true);
		isRemoved = true;
	}
	else
	{
		this->getChildByTag(1)->setScaleX(hpLeft / hp);
		isRemoved = false;
	}	
}

double FHSprite::Speed()
{
	return speed;
}


