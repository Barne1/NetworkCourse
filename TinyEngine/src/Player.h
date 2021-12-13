#pragma once
#define PLAYER_MAX 20
#define PLAYER_NAME_MAX 15
#define PLAYER_BOOST_DURATION 0.5f
#define PLAYER_BOOST_MULT 2

const float playerSpeed = 180.f;
const float playerErrorCorrectionStrength = 1.5f;
const float playerRadius = 16;

class Player
{
public:
	int id = -1;
	bool alive = false;
	float x;
	float y;

	int inputX = 0;
	int inputY = 0;

	float errorX = 0.f;
	float errorY = 0.f;

	float lastFireTime = 0.f;

	char name[PLAYER_NAME_MAX + 1];

	bool boosting = false;
	float boostTimer = 0;

	void netReceivePosition(float newX, float newY);

	void startBoost();
	void stopBoost();

	void spawn(int id, int spawnX, int spawnY);
	void destroy();

	bool hasControl();
	void update();

	void draw();
};

extern Player players[PLAYER_MAX];

#if CLIENT
extern int possessedPlayerId;
#endif