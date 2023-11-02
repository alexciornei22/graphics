#pragma once

#include "lab_m1/Tema1/constants.h";
#include "glm/glm.hpp"

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
		std::time_t timeLastShot = time(nullptr);
		Shooter *shooter = nullptr;
		bool isShooterDeleted = false;
		float shooterScale = 1.0f;
		float acceleration = START_SPEED;
	};

	void checkProjectileEnemyCollisions(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);
	void removeInvalidPieces(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);
}