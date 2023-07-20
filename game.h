#pragma once
#include "player.h"
#include "ball.h"

class Game
{
	typedef enum {status_start, status_playing, status_winner} status_t;
	Player* left_player = nullptr;
	bool left_player_initialized = false;
	Player* right_player = nullptr;
	bool right_player_initialized = false;
	Ball* ball = nullptr;
	bool ball_initialized = false;
	bool checkCollision(Disk d1, Disk d2);
	float last_collision_time = 0.0f; // variable which stores the time when the last collision happened
	float new_ball_delay; // variable used to delay the first ball movement
	bool handle_hit = false; // if the last hit was with racket handle, handle_hit = true
	bool lasthit = 0; 
	// if lasthit = 0, then left player touched last the ball
	// if lasthit = 1, then right player touched last the ball
	int rightscore = 0;
	int leftscore= 0;
	bool secondsong = false; // for the song change
	bool thirdsong = false;
	bool selfgoal = false;
	status_t status = status_start;
	bool pause = false;
	float last_pause_time;
	bool mute = false;
	float last_mute_time;
	bool winner;
	bool party_blower = false;
	bool init_done = false;

	void updateStartScreen();
	void updatePlayingScreen();
	void updateWinnerScreen();
	void drawStartScreen();
	void drawPlayingScreen();
	void drawWinnerScreen();

public:
	void update();
	void draw();
	void init();
	Game();
	~Game();
};