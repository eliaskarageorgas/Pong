#pragma once
#include "gameobject.h"
#include "config.h"
#include "graphics.h"

class Ball : public GameObject, public Collidable {

	float speed = 0.8f;
	float pos_x = CANVAS_WIDTH / 2;
	float pos_y = CANVAS_HEIGHT / 2;
	bool collision_state = false;
	float angle_x;
	float angle_y;
	graphics::Brush br;
	float last_collision_time = 0.0f;

public:
	void update() override;
	void draw() override;
	void init() override;
	float getPosX() { return pos_x; };
	float getPosY() { return pos_y; };
	void getRandomAngles();
	Disk getStandardCollisionHull() const override;
	void setCollisionState() { collision_state = true; }
	Ball();
	~Ball();
};