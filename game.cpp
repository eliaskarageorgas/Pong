#include "game.h"
#include "graphics.h"
#include "config.h"

bool Game::checkCollision(Disk d1, Disk d2)
{
	// checking collision between two disks
	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
	{
		selfgoal = false; // erase selfgoal text
		return true;
	}
	else
		return false;
}

void Game::update()
{

	if (status == status_start)
	{
		updateStartScreen();
	} 
	else if (status == status_playing)
	{
		updatePlayingScreen();
	}
	else
	{
		graphics::stopMusic();
		updateWinnerScreen();
	}
	
}

void Game::updateStartScreen()
{
	if (!init_done)
	{
		init();
		init_done = true;
	}
	if (graphics::getKeyState(graphics::SCANCODE_RETURN))
	{
		status = status_playing;
		new_ball_delay = graphics::getGlobalTime();
		if (!mute)
		{
			graphics::stopMusic();
			graphics::playMusic(std::string(ASSET_PATH) + "first_stage.wav", 0.3f, true);
		}
		graphics::setFont(std::string(ASSET_PATH) + "jellies.ttf");
	}

	if (graphics::getKeyState(graphics::SCANCODE_M) && graphics::getGlobalTime() > last_mute_time + 500)
	{
		if (!mute)
		{
			graphics::stopMusic();
		}
		else
		{
			graphics::playMusic(std::string(ASSET_PATH) + "elevator_music.mp3", 0.3f, true);
		}
		mute = !mute;
		last_mute_time = graphics::getGlobalTime();
	}
}

void Game::updatePlayingScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_P) && graphics::getGlobalTime() > last_pause_time + 500)
	{
		pause = !pause;
		last_pause_time = graphics::getGlobalTime();
	}

	if (graphics::getKeyState(graphics::SCANCODE_M) && graphics::getGlobalTime() > last_mute_time + 500)
	{
		if (!mute)
		{
			graphics::stopMusic();
		}
		else
		{
			if (secondsong)
				graphics::playMusic(std::string(ASSET_PATH) + "second_stage.wav", 0.3f, true);
			else if (thirdsong)
				graphics::playMusic(std::string(ASSET_PATH) + "third_stage.wav", 0.3f, true);
			else
				graphics::playMusic(std::string(ASSET_PATH) + "first_stage.wav", 0.3f, true);
		}
		mute = !mute;
		last_mute_time = graphics::getGlobalTime();
	}

	if (!left_player_initialized && graphics::getGlobalTime() > 1000) {
		// Player(0) is left player
		left_player = new Player(0);
		left_player_initialized = true;
	}

	if (!right_player_initialized && graphics::getGlobalTime() > 1000) {
		// Player(1) is right player
		right_player = new Player(1);
		right_player_initialized = true;
	}

	if (!ball_initialized && graphics::getGlobalTime() > new_ball_delay + 2000) {
		ball = new Ball();
		ball_initialized = true;
	}

	if (left_player)
		left_player->update();
	if (right_player)
		right_player->update();
	if (ball && graphics::getGlobalTime() > new_ball_delay + 7000 && !pause)
	{
		ball->update();
		selfgoal = false;
	}
		

	// checking for collision between players and ball
	if (left_player && right_player && ball && graphics::getGlobalTime() > last_collision_time + 1000)
	{
		Disk ball_disk = ball->getStandardCollisionHull();
		// checking collision between left player's racket and ball
		if (checkCollision(left_player->getStandardCollisionHull(), ball_disk))
		{
			if (!mute)
				graphics::playSound(std::string(ASSET_PATH) + "hitting_ball.wav", 0.5f);
			ball->setCollisionState();
			last_collision_time = graphics::getGlobalTime();
			lasthit = 0;
			handle_hit = false;
		}
		// checking collision between left player's handle and ball
		else if (checkCollision(left_player->getHandleCollisionHullUpper(), ball_disk) ||
			checkCollision(left_player->getHandleCollisionHullLower(), ball_disk))
		{
			if (!mute)
			{
				graphics::playSound(std::string(ASSET_PATH) + "fire_whoosh.wav", 0.8f);
				graphics::playSound(std::string(ASSET_PATH) + "hitting_ball.wav", 0.5f);
			}
			ball->setCollisionState();
			last_collision_time = graphics::getGlobalTime();
			lasthit = 0;
			handle_hit = true;
		}
		// checking collision between right player's racket and ball
		else if (checkCollision(right_player->getStandardCollisionHull(), ball_disk))
		{
			if (!mute)
				graphics::playSound(std::string(ASSET_PATH) + "hitting_ball.wav", 0.5f);
			ball->setCollisionState();
			last_collision_time = graphics::getGlobalTime();
			lasthit = 1;
			handle_hit = false;
		}
		// checking collision between right player's handle and ball
		else if (checkCollision(right_player->getHandleCollisionHullUpper(), ball_disk) ||
			checkCollision(right_player->getHandleCollisionHullLower(), ball_disk))
		{
			if (!mute)
			{
				graphics::playSound(std::string(ASSET_PATH) + "fire_whoosh.wav", 0.8f);
				graphics::playSound(std::string(ASSET_PATH) + "hitting_ball.wav", 0.5f);
			}
			ball->setCollisionState();
			last_collision_time = graphics::getGlobalTime();
			lasthit = 1;
			handle_hit = true;
		}

	}

	// changing score and deleting ball
	if (ball_initialized && (ball->getPosX() >= 988 || ball->getPosX() <= 11))
	{
		if (last_collision_time != 0.0f)// if a collision has happened
		{
			if (lasthit == 0)
			{
				if (ball->getPosX() <= 11)
				{
					// self goal 
					right_player->setScore(1);
					selfgoal = true;
				}
				else if (handle_hit)
					left_player->setScore(2);
				else
					left_player->setScore(1);
			}
			else
			{
				if (ball->getPosX() >= 988)
				{
					// self goal 
					left_player->setScore(1);
					selfgoal = true;
				}
				else if (handle_hit)
					right_player->setScore(2);
				else
					right_player->setScore(1);
			}
		}

		leftscore = left_player->getScore();
		rightscore = right_player->getScore();
		handle_hit = false;

		if (!mute)
			graphics::playSound(std::string(ASSET_PATH) + "error.wav", 0.5f);

		if ((rightscore > 8 || leftscore > 8) && !thirdsong)
		{
			if (!mute)
			{
				graphics::stopMusic();
				graphics::playMusic(std::string(ASSET_PATH) + "third_stage.wav", 0.3f, true);
			}
			thirdsong = true;
		}
		else if ((rightscore > 5 || leftscore > 5) && !secondsong)
		{
			if (!mute)
			{
				graphics::stopMusic();
				graphics::playMusic(std::string(ASSET_PATH) + "second_stage.wav", 0.3f, true);
			}
			secondsong = true;
		}
		delete ball;
		ball = nullptr;
		ball_initialized = false;
		last_collision_time = 0.0f;
		new_ball_delay = graphics::getGlobalTime();
	}

	if (leftscore >= 11 && leftscore - rightscore >= 2 && leftscore >= rightscore)
	{
		status = status_winner;
		winner = 0;
	}
	else if (rightscore >= 11 && rightscore - leftscore <= 2 && rightscore >= leftscore)
	{
		status = status_winner;
		winner = 1;
	}
}

void Game::updateWinnerScreen()
{
	if (!mute && !party_blower)
	{
		graphics::playSound(std::string(ASSET_PATH) + "party_blower.wav", 0.3f);
		party_blower = true;
	}

	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		status = status_start;
		party_blower = false;
	}

	else if (graphics::getKeyState(graphics::SCANCODE_RETURN))
		exit(0);

	// we want to call init again if they play again
	init_done = false;
}

void Game::draw()
{
	if (status == status_start)
	{
		drawStartScreen();
	}
	else if (status == status_playing)
	{
		drawPlayingScreen();
	}
	else
	{
		drawWinnerScreen();
	}
}

void Game::drawStartScreen()
{
	// drawing backround
	graphics::Brush br;
	br.fill_color[0] = 0.9f;
	br.fill_color[1] = 0.6f;
	br.fill_color[2] = 0.7f;
	graphics::setWindowBackground(br);

	br.fill_color[0] = 0.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	graphics::setFont(std::string(ASSET_PATH) + "Solicitous.ttf");
	graphics::drawText(CANVAS_WIDTH / 2 - 200, 50, 65, "Press Enter To Start", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480,  40, 40, "Pong", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480, 100, 40, "Game Rules", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480, 130, 35, "1. Left player moves using W and S keys", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480, 160, 35, "2. Right player moves using UP and DOWN arrows", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480, 190, 35, "3. The first to score 11 points wins", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480, 220, 35, "4. To win there must be a 2-points difference, otherwise the game continues", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480, 250, 35, "5. A hit with the racket's handle (known as flame throw) counts for two points", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480, 280, 35, "    but if the ball hits too low it's a selfgoal", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480, 310, 35, "6. Press key P to pause, key M to mute", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 480, 340, 35, "7. It's two rackets and a ball. You don't need any other rules", br);

	br.texture = std::string(ASSET_PATH) + "tworackets.png";
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 1.0f;
	br.outline_opacity = 0.0f;
	// draw backround
	graphics::drawRect(CANVAS_WIDTH / 2 , 425, 130, 130, br);
}

void Game::drawPlayingScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "field.png";
	br.outline_opacity = 0.0f;

	// draw backround
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br);

	br.texture = std::string(ASSET_PATH) + "net.png";
	graphics::setOrientation(+90.0f);

	//draw net
	graphics::drawRect(CANVAS_WIDTH / 2 - 2, CANVAS_HEIGHT / 2 + 3, CANVAS_HEIGHT, CANVAS_WIDTH / 20, br);

	graphics::setOrientation(-90.0f);

	// draw players
	if (left_player)
		left_player->draw();
	if (right_player)
		right_player->draw();

	// draw ball
	if (ball)
		ball->draw();

	// draw score and fire if handle hit
	if (left_player && right_player)
	{
		graphics::resetPose();
		br.texture = "";
		br.outline_opacity = 1.0f;
		graphics::drawText(CANVAS_WIDTH / 2 - 90, 50, 40, std::to_string((left_player)->getScore()), br);
		graphics::drawText(CANVAS_WIDTH / 2 + 50, 50, 40, std::to_string((right_player)->getScore()), br);
		if (handle_hit)
		{
			br.texture = std::string(ASSET_PATH) + "flame.png";
			br.outline_opacity = 0.0f;
			graphics::drawRect(CANVAS_WIDTH / 2, 30, 60, 60, br);
			br.texture = "";
		}
	}

	// draw self goal text
	if (left_player && right_player && ball)
	{
		if (selfgoal)
		{
			graphics::resetPose();
			br.texture = "";
			br.outline_opacity = 1.0f;
			graphics::drawText(CANVAS_WIDTH / 2 - 125, CANVAS_HEIGHT / 2 - 20, 40, "SELF GOAL", br);
		}
		
	}

	// draw pause, mute
	if (left_player && right_player) 
	{
		if (pause)
		{
			br.texture = "";
			br.outline_opacity = 1.0f;
			graphics::drawText(CANVAS_WIDTH / 2 - 125, CANVAS_HEIGHT / 2 - 60, 60, "PAUSE", br);
		}
		if (mute)
		{
			br.texture = "";
			br.outline_opacity = 1.0f;
			graphics::drawText(CANVAS_WIDTH / 2 - 300, 40, 20, "MUTE", br);
		}
	}
}

void Game::drawWinnerScreen()
{
	// drawing backround
	graphics::Brush br;
	br.fill_color[0] = 0.9f;
	br.fill_color[1] = 0.6f;
	br.fill_color[2] = 0.7f;
	graphics::setWindowBackground(br);

	br.fill_color[0] = 0.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	graphics::setFont(std::string(ASSET_PATH) + "Solicitous.ttf");
	if(winner)
		graphics::drawText(CANVAS_WIDTH / 2 - 130, 50, 65, "Blue Player Won", br);
	else
		graphics::drawText(CANVAS_WIDTH / 2 - 130, 50, 65, "Red Player Won", br);

	graphics::drawText(CANVAS_WIDTH / 2 - 130, 100, 65, "Congratulations", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 130, 300, 45, "Press SPACE to play again", br);
	graphics::drawText(CANVAS_WIDTH / 2 - 130, 350, 45, "Press ENTER to exit", br);
}

void Game::init()
{
	if (!mute)
		graphics::playMusic(std::string(ASSET_PATH) + "elevator_music.mp3", 0.3f, true);
	left_player = nullptr;
	left_player_initialized = false;
	right_player = nullptr;
	right_player_initialized = false;
	ball = nullptr;
	ball_initialized = false;
	last_collision_time = 0.0f; // variable which stores the time when the last collision happened
	new_ball_delay; // variable used to delay the first ball movement
	handle_hit = false; // if the last hit was with racket handle, handle_hit = true
	lasthit = 0;
	// if lasthit = 0, then left player touched last the ball
	// if lasthit = 1, then right player touched last the ball
	rightscore = 0;
	leftscore = 0;
	secondsong = false; // for the song change
	thirdsong = false;
	selfgoal = false;
	pause = false;
	last_pause_time = 0.0f;
	winner = 1;
	party_blower = false;

}

Game::Game()
{
}

Game::~Game()
{
	if (left_player){
		delete left_player;
	}
	if (right_player) {
		delete right_player;
	}
}