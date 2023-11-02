#include <iostream>
#include <vector>
#include "lab_m1/Tema1/game.h"

void game::checkProjectileEnemyCollisions(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies)
{
	for (auto& projectile : projectiles) {
		for (auto& enemy : enemies) {
			if (projectile.type == enemy.type && glm::distance(projectile.coordinates, enemy.coordinates) < 100) {
				enemy.health--;
				projectile.health--;
			}
		}
	}
}

bool enemyReachedEnd(game::Enemy enemy) {
    return enemy.coordinates.x < SEPARATION + END_WIDTH / 2;
}
bool projectileReachedEnd(game::Projectile projectile) {
    return projectile.coordinates.x > 1280;
}
bool isSafeToDelete(game::Enemy enemy) {
    return enemy.safeToDelete;
}
bool isProjectileDead(game::Projectile projectile) {
    return projectile.health < 1;
}
void game::removeInvalidPieces(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies)
{
    enemies.erase(
        remove_if(enemies.begin(), enemies.end(), enemyReachedEnd),
        enemies.end()
    );

    enemies.erase(
        remove_if(enemies.begin(), enemies.end(), isSafeToDelete),
        enemies.end()
    );

    projectiles.erase(
        remove_if(projectiles.begin(), projectiles.end(), isProjectileDead),
        projectiles.end()
    );

    projectiles.erase(
        remove_if(projectiles.begin(), projectiles.end(), projectileReachedEnd),
        projectiles.end()
    );
}
