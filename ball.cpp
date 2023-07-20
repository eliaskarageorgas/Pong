#include "ball.h"
#include "graphics.h"
#include <cstdlib>
#include <ctime>

void Ball::update()
{
	
	pos_x += cos(angle_x) * speed * graphics::getDeltaTime() / 2;
	pos_y += sin(angle_y) * speed * graphics::getDeltaTime() / 2;

	if ((pos_y >= 490 || pos_y <= 11) && graphics::getGlobalTime() > last_collision_time + 500) {
		angle_x = - angle_x;
		angle_y = - angle_y;
		last_collision_time = graphics::getGlobalTime();
	} 
	else if (collision_state)
	{
		collision_state = false;
		angle_x = 3 - angle_x;
		angle_y = 3 - angle_y;
	}
	/*
	if (graphics::getKeyState(graphics::SCANCODE_UP))
		pos_y -= speed * graphics::getDeltaTime() / 30.0f;
	if (graphics::getKeyState(graphics::SCANCODE_DOWN))
		pos_y += speed * graphics::getDeltaTime() / 30.0f;
	if (graphics::getKeyState(graphics::SCANCODE_LEFT))
		pos_x -= speed * graphics::getDeltaTime() / 30.0f;
	if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
		pos_x += speed * graphics::getDeltaTime() / 30.0f;
		
	char info[30];
	sprintf_s(info, "(%1.0f)", getPosY());
	graphics::drawText(CANVAS_WIDTH / 4, CANVAS_HEIGHT / 2 - 30, 30, info, br);
	sprintf_s(info, "(%1.0f)", getPosX());
	graphics::drawText(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 - 30, 30, info, br);
	*/

}

void Ball::draw()
{
	/*br.outline_color[0] = 0.0f;
	br.outline_color[1] = 0.0f;
	br.outline_color[2] = 0.0f;
	br.outline_opacity = 1.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 1.0f;
	br.fill_opacity = 0.7f;
	graphics::drawDisk(pos_x, pos_y, 10, br);*/

	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + "ball_shadow.png";
	br.fill_opacity = 0.5f;
	graphics::drawRect(pos_x + 10, pos_y - 10, 30, 30, br);

	br.fill_opacity = 1.0f;
	br.texture = std::string(ASSET_PATH) + "ball.png";
	graphics::drawRect(pos_x, pos_y, 30, 30, br);

#ifdef DEBUG_MODE
	br.outline_opacity = 1.0f;
	br.texture = "";
	br.fill_color[0] = 0.3f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 0.3f;
	br.fill_opacity = 0.3f;
	br.gradient = false;
	Disk hull = getStandardCollisionHull();
	graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);

#endif // DEBUG_MODE

}

void Ball::init()
{

}

void Ball::getRandomAngles() 
{
	// function that returns random angle for the first ball movement
	srand(time(0));
	int random_number = rand()%4;

	if (random_number == 0) 
	{
		// this makes ball go up and right
		angle_x = -0.3f;
		angle_y = -0.7f;
	}
	else if (random_number == 1)
	{
		// this makes ball go up and left
		angle_x = 3.3f;
		angle_y = 3.7f;
	}
	else if (random_number == 2)
	{
		// this makes ball go down and left
		angle_x = -3.3f;
		angle_y = -3.7f;
	}
	else if (random_number == 3)
	{
		// this makes ball go down and right
		angle_x = 0.3f;
		angle_y = 0.7f;
	}
}

Ball::Ball()
{
	getRandomAngles();
}


Ball::~Ball()
{
}

Disk Ball::getStandardCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = 10.0f;
	return disk;
}
