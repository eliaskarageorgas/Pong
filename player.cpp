#include "player.h"
#include "graphics.h"

void Player::update()
{
	if (!which_player) {
		if (graphics::getKeyState(graphics::SCANCODE_W))
			pos_y -= speed * graphics::getDeltaTime() / 30.0f;
		if (graphics::getKeyState(graphics::SCANCODE_S))
			pos_y += speed * graphics::getDeltaTime() / 30.0f;
	}
	else {
		if (graphics::getKeyState(graphics::SCANCODE_UP))
			pos_y -= speed * graphics::getDeltaTime() / 30.0f;
		if (graphics::getKeyState(graphics::SCANCODE_DOWN))
			pos_y += speed * graphics::getDeltaTime() / 30.0f;
	}
	
	if (pos_y > 490) pos_y = 490;
	if (pos_y < 30) pos_y = 30;
}


void Player::draw()
{
	br.outline_opacity = 0.0f;

	if (!which_player) {
		graphics::setOrientation(+25.0f);

		br.texture = std::string(ASSET_PATH) + "racket_shadow.png";
		br.fill_opacity = 0.5f;
		graphics::drawRect(pos_x + 10, pos_y - 10, 60, 60, br);

		br.fill_opacity = 1.0f;
		br.texture = std::string(ASSET_PATH) + "red_racket.png";
		graphics::drawRect(pos_x, pos_y, 60, 60, br);
	}
	else {
		graphics::setOrientation(+70.0f);

		br.texture = std::string(ASSET_PATH) + "racket_shadow.png";
		br.fill_opacity = 0.5f;
		graphics::drawRect(pos_x + 10, pos_y - 10, 60, 60, br);

		br.fill_opacity = 1.0f;
		br.texture = std::string(ASSET_PATH) + "blue_racket.png";
		graphics::drawRect(pos_x, pos_y, 60, 60, br);
	}
	
	graphics::resetPose();

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
	hull = getHandleCollisionHullUpper();
	graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
	hull = getHandleCollisionHullLower();
	graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);

#endif // DEBUG_MODE

}

void Player::init()
{
	if (!which_player) {
		pos_x = 30.0f;
		pos_y = CANVAS_HEIGHT / 2;
	}
	else {
		pos_x = CANVAS_WIDTH - 30;
		pos_y = CANVAS_HEIGHT / 2;
	}
}

Disk Player::getStandardCollisionHull() const
{
	Disk disk;
	disk.radius = 20.0f;
	if (!which_player) {
		disk.cx = pos_x + 4;
		disk.cy = pos_y - 10;
	}
	else {
		disk.cx = pos_x - 2.3f;
		disk.cy = pos_y - 10;
	}
	
	
	return disk;
}

Disk Player::getHandleCollisionHullUpper() const
{
	Disk disk;
	disk.radius = 7.0f;
	if (!which_player) {
		disk.cx = pos_x - 8;
		disk.cy = pos_y + 15;
	}
	else {
		disk.cx = pos_x + 10;
		disk.cy = pos_y + 15;
	}
	return disk;
}

Disk Player::getHandleCollisionHullLower() const
{
	Disk disk;
	disk.radius = 5.0f;
	if (!which_player) {
		disk.cx = pos_x - 10;
		disk.cy = pos_y + 27;
	}
	else {
		disk.cx = pos_x + 12;
		disk.cy = pos_y + 27;
	}
	return disk;
}

Player::Player(bool which_player)
{
	this->which_player = which_player;
	init();
}