#include "Player.h"
#include "Engine.h"

void Player::Draw()
{
	engSetColor(0xDEADBEEF);
	engFillRect(x,y,32,32);
}
