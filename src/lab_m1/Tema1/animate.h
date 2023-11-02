#pragma once

#include <iostream>
#include <vector>
#include "game.h"

namespace animate {
	void scaleDownShooters(game::TableBoxData tableCoordinates[3][3], float deltaTime);
	void moveEnemiesLeft(std::vector<game::Enemy>& enemies, float deltaTime);
	void moveProjectilesRight(std::vector<game::Projectile>& projectiles, float deltaTime);
}