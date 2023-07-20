#pragma once
#include "gameobject.h"
#include "config.h"
#include "graphics.h"

class Player : public GameObject, public Collidable{

	bool which_player;
	float speed = 10.0f;
	float pos_x; 
	float pos_y;
	graphics::Brush br;
	int score = 0;

public:
	void update() override;
	void draw() override;
	void init() override;
	float getPosX() { return pos_x; };
	float getPosY() { return pos_y; };
	int getScore() { return score; };
	void setScore(int i) { score += i; };
	Disk getStandardCollisionHull() const override;
	Disk getHandleCollisionHullUpper()const;
	Disk getHandleCollisionHullLower()const;
	Player(bool which_player);
};