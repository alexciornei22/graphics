#include <iostream>
#include <vector>
#include <algorithm>
#include "lab_m1/PvZ/animate.h"

void game::checkProjectileEnemyCollisions(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies)
{
	for (auto& projectile : projectiles) {
		for (auto& enemy : enemies) {
			if (
                projectile.type == enemy.type && 
                enemy.health &&
                glm::distance(projectile.coordinates, enemy.coordinates) < (PROJECTILE_SIZE + ENEMY_SIZE) / 3
                ) {
				enemy.health--;
				projectile.health--;
			}
		}
	}
}

void game::checkShooterEnemyCollisions(game::TableBoxData tableCoordinates[3][3], std::vector<Enemy>& enemies)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game::TableBoxData& currentBox = tableCoordinates[i][j];
            for (auto& enemy : enemies) {
                if (glm::distance(enemy.coordinates, currentBox.getCenter()) < SQUARE_LENGTH * 0.8) {
                    currentBox.isShooterDeleted = true;
                }
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
bool isEnemySafeToDelete(game::Enemy enemy) {
    return enemy.safeToDelete;
}
bool isHeartSafeToDelete(game::Heart heart) {
    return heart.safeToDelete;
}
bool isProjectileDead(game::Projectile projectile) {
    return projectile.health < 1;
}
void game::removeInvalidPieces(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies, std::vector<Heart>& hearts)
{
    enemies.erase(
        remove_if(enemies.begin(), enemies.end(), enemyReachedEnd),
        enemies.end()
    );

    enemies.erase(
        remove_if(enemies.begin(), enemies.end(), isEnemySafeToDelete),
        enemies.end()
    );

    hearts.erase(
        remove_if(hearts.begin(), hearts.end(), isHeartSafeToDelete),
        hearts.end()
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

void game::generateEnemies(std::vector<Enemy>& enemies, std::vector<Shooter>& shooters, milliseconds& lastGeneratedEnemies)
{
    if (system_clock::now().time_since_epoch() - lastGeneratedEnemies < milliseconds(5000))
        return;

    lastGeneratedEnemies = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    int random = rand();
    bool linesGenerated[3] = {false, false, false};
    do {
        int line = rand() % 3;
        int type = rand() % 4;
        if (!linesGenerated[line]) {
            enemies.push_back(
                game::Enemy(line, glm::vec3(1400, SEPARATION + SQUARE_LENGTH / 2 + line * (SQUARE_LENGTH + SEPARATION), 1), type, shooters[type].color)
            );
            linesGenerated[line] = true;
        }

        random /= 3;
    } while (random && random % 3 == 0);
}

void game::generateProjectiles(TableBoxData tableCoordinates[3][3], std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies)
{
    for (auto& enemy : enemies) {
        for (int j = 0; j < 3; j++) {
            game::TableBoxData& currentBox = tableCoordinates[enemy.line][j];
            if (!currentBox.shooter) continue;

            if (currentBox.shooter->color == enemy.color) {
                if (system_clock::now().time_since_epoch() - currentBox.timeLastShot >= milliseconds(PROJECTILE_LAUNCH_INTERVAL)) {
                    currentBox.timeLastShot = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

                    projectiles.push_back(
                        game::Projectile(enemy.type, glm::vec3(currentBox.x + SQUARE_LENGTH, currentBox.y + SQUARE_LENGTH / 2, 0), enemy.color)
                    );
                }
            }
        }
    }
}

void game::generateStars(std::vector<Star>& stars, milliseconds& lastGeneratedStars)
{
    if (system_clock::now().time_since_epoch() - lastGeneratedStars >= milliseconds(STAR_GENERATION_INTERVAL)) {
        lastGeneratedStars = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

        for (int i = 0; i < 3; i++) {
            int random = rand();
            int x = random % 1280;
            int y = random % 720;
           
            stars.push_back(
                game::Star(glm::vec3(x, y, 10))
            );
        }
    }
}

void game::checkEnemyReachedEnd(std::vector<Enemy>& enemies, std::vector<Heart>& hearts)
{
    int enemiesAtEnd = std::count_if(enemies.begin(), enemies.end(), enemyReachedEnd);
    for (int i = 0; i < enemiesAtEnd; i++) {
        while (hearts[hearts.size() - i - 1].removed) i++;
        if (i < 0) break;
        hearts[hearts.size() - i - 1].removed = true;
    }
}

void game::checkHasSelectedShooter(glm::vec3 mouseCoordinates, std::vector<ItemBoxData> itemCoordinates, Shooter*& selectedShooter, int& currentStars)
{
    for (int i = 0; i < NR_SHOOTERS; i++) {
        game::ItemBoxData currentBox = itemCoordinates[i];
        if (currentBox.shooter->price > currentStars)
            break;

        if (
            mouseCoordinates.x > currentBox.x &&
            mouseCoordinates.x < currentBox.x + currentBox.length &&
            mouseCoordinates.y > currentBox.y &&
            mouseCoordinates.y < currentBox.y + currentBox.length
            ) {
            selectedShooter = currentBox.shooter;
            break;
        }
    }
}

void game::checkHasCollectedStar(glm::vec3 mouseCoordinates, std::vector<Star>& stars, int& currentStars)
{
    for (auto it = stars.begin(); it < stars.end(); it++) {
        Star star = *it;
        if (
            abs(mouseCoordinates.x - star.coordinates.x) < STAR_SIZE / 2 &&
            abs(mouseCoordinates.y - star.coordinates.y) < STAR_SIZE / 2
            ) {
            stars.erase(it);
            currentStars++;
            break;
        }
    }
}

void game::checkHasDroppedShooter(glm::vec3 mouseCoordinates, TableBoxData tableCoordinates[3][3], Shooter*& selectedShooter, int& currentStars)
{
    if (!selectedShooter) return;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game::TableBoxData& currentBox = tableCoordinates[i][j];
            if (
                mouseCoordinates.x > currentBox.x &&
                mouseCoordinates.x < currentBox.x + currentBox.length &&
                mouseCoordinates.y > currentBox.y &&
                mouseCoordinates.y < currentBox.y + currentBox.length &&
                currentBox.shooter == nullptr || currentBox.isShooterDeleted == true
                ) {
                currentBox.animationDone();
                currentBox.shooter = selectedShooter;
                currentStars -= selectedShooter->price;
                break;
            }
        }
    }

    selectedShooter = nullptr;
}

void game::checkHasClearedBox(glm::vec3 mouseCoordinates, TableBoxData tableCoordinates[3][3])
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game::TableBoxData& currentBox = tableCoordinates[i][j];
            if (
                mouseCoordinates.x > currentBox.x &&
                mouseCoordinates.x < currentBox.x + currentBox.length &&
                mouseCoordinates.y > currentBox.y &&
                mouseCoordinates.y < currentBox.y + currentBox.length &&
                currentBox.shooter != nullptr
                ) {
                currentBox.isShooterDeleted = true;
                break;
            }
        }
    }
}

void game::checkIfGameEnded(std::vector<Heart>& hearts, WindowObject* window)
{
    if (hearts.size() <= 0)
        window->Close();
}
