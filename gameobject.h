#pragma once
#include "util.h"

class GameObject
{

public:
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void init() = 0;
};

class Collidable
{
public:
	virtual Disk getStandardCollisionHull() const = 0;
};
