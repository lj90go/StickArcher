#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class FHSprite : public CCSprite
{
public:
	FHSprite();
	~FHSprite(void);

	static FHSprite* create(const char *pszFileName);
	static FHSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
	bool FHInit();
	void Hitted(double damage, bool &isRemoved);

	double Speed();

private:
	double hp;
	double hpLeft;
	double speed;
};

