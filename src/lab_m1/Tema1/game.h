#pragma once

#include <chrono>
#include "lab_m1/Tema1/constants.h";
#include "glm/glm.hpp"

using namespace std::chrono;

namespace game {
	struct Shooter
	{
		Shooter(int price, glm::vec3 color) : price(price), color(color) {}
		int price;
		glm::vec3 color;
	};

	struct Enemy
	{
		Enemy(int line, glm::vec3 coordinates, int type, glm::vec3 color) : line(line), coordinates(coordinates), type(type), color(color) {}
		int health = 3;
		int type;
		int line;
		glm::vec3 coordinates;
		glm::vec3 color;
		bool safeToDelete = false;
		float scale = 1.0f;
		float acceleration = START_SPEED;
	};

	struct Projectile
	{
		Projectile(int type, glm::vec3 coordinates, glm::vec3 color) : type(type), coordinates(coordinates), color(color) {}
		int type;
		int health = 1;
		float angularStep = 0;
		glm::vec3 coordinates;
		glm::vec3 color;
	};

	struct ItemBoxData
	{
		ItemBoxData(int x, int y, int length, Shooter* shooter) : x(x), y(y), length(length), shooter(shooter) {}
		int x; // x coordinate of bottom-left corner
		int y; // y coordinate of bottom-left corner
		int length;
		Shooter* shooter;
	};

	struct TableBoxData
	{
		TableBoxData(int x, int y, int length) : x(x), y(y), length(length) {}
		int x; // x coordinate of bottom-left corner
		int y; // y coordinate of bottom-left corner
		int length;
		milliseconds timeLastShot = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		Shooter *shooter = nullptr;
		bool isShooterDeleted = false;
		float shooterScale = 1.0f;
		float acceleration = START_SPEED;

		inline glm::vec3 getCenter() { return glm::vec3(x + SQUARE_LENGTH / 2, y + SQUARE_LENGTH / 2, 0); }
	};

	void checkProjectileEnemyCollisions(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);
	void checkShooterEnemyCollisions(game::TableBoxData tableCoordinates[3][3], std::vector<Enemy>& enemies);
	void removeInvalidPieces(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);
	void generateEnemies(std::vector<Enemy>& enemies, std::vector<Shooter>& shooters);
	void generateProjectiles(TableBoxData tableCoordinates[3][3], std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);
	void checkHasSelectedShooter(glm::vec3 mouseCoordinates, std::vector<ItemBoxData> itemCoordinates,Shooter* &selectedShooter);
	void checkHasDroppedShooter(glm::vec3 mouseCoordinates, TableBoxData tableCoordinates[3][3], Shooter*& selectedShooter);
	void checkHasClearedBox(glm::vec3 mouseCoordinates, TableBoxData tableCoordinates[3][3]);
}