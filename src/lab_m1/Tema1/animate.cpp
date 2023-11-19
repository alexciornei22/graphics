#include "lab_m1/Tema1/animate.h"

void animate::scaleDownShooters(game::TableBoxData tableCoordinates[3][3], float deltaTime)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game::TableBoxData& currentBox = tableCoordinates[i][j];
            if (currentBox.isShooterDeleted) {
                currentBox.animateScale(deltaTime);
            }
            if (currentBox.elapsedAnimationTime > 1.0f) {
                currentBox.animationDone();
            }
        }
    }
}

void animate::scaleDownEnemies(std::vector<game::Enemy>& enemies, float deltaTime)
{
    for (auto& enemy : enemies) {
        if (enemy.health < 1) {
            enemy.animateScale(deltaTime);
        }

        if (enemy.scale < 0) {
            enemy.animationDone();
        }
    }
}

void animate::scaleDownHearts(std::vector<game::Heart>& hearts, float deltaTime)
{
    for (auto& heart : hearts) {
        if (heart.removed) {
            heart.animateScale(deltaTime);
        }

        if (heart.scale < 0) {
            heart.animationDone();
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
        projectile.coordinates += glm::vec3(PROJECTILE_SPEED * deltaTime, 0, 0);
        projectile.angularStep -= PROJECTILE_ROTATION * deltaTime;
    }
}