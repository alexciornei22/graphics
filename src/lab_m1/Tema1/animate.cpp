#include "lab_m1/Tema1/animate.h"

void animate::scaleDownShooters(game::TableBoxData tableCoordinates[3][3], float deltaTime)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game::TableBoxData& currentBox = tableCoordinates[i][j];
            if (currentBox.isShooterDeleted) {
                currentBox.shooterScale -= currentBox.acceleration * deltaTime / 2;
                currentBox.acceleration *= ACCELERATION;
            }
            if (currentBox.shooterScale < 0) {
                currentBox.shooterScale = 1.0f;
                currentBox.isShooterDeleted = false;
                currentBox.shooter = nullptr;
                currentBox.acceleration = START_SPEED;
            }
        }
    }
}

void animate::scaleDownEnemies(std::vector<game::Enemy>& enemies, float deltaTime)
{
    for (auto& enemy : enemies) {
        if (enemy.health < 1) {
            enemy.scale -= enemy.acceleration * deltaTime / 2;
            enemy.acceleration *= ACCELERATION;
        }

        if (enemy.scale < 0) {
            enemy.safeToDelete = true;
        }
    }
}

void animate::moveEnemiesLeft(std::vector<game::Enemy>& enemies, float deltaTime)
{
    for (auto& enemy : enemies) {
        if (enemy.health < 1) continue;

        enemy.coordinates -= glm::vec3(100 * deltaTime, 0, 0);
    }
}

void animate::moveProjectilesRight(std::vector<game::Projectile>& projectiles, float deltaTime)
{
    for (auto& projectile : projectiles) {
        projectile.coordinates += glm::vec3(200 * deltaTime, 0, 0);
        projectile.angularStep -= 2 * deltaTime;
    }
}
