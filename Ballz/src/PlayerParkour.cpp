#include "stdafx.h"
#include "PlayerParkour.h"
#include "Player.h"

PlayerParkour::PlayerParkour(Player* player) : p(player), body(player->body)
{
}

bool PlayerParkour::spacePressed()
{

}