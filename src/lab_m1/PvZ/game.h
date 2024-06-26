#pragma once

#include <chrono>
#include "lab_m1/PvZ/constants.h"
#include "glm/glm.hpp"
#include <core/window/window_object.h>

using namespace std::chrono;

namespace game {
	struct ScaleEaseInOutAnimatable
	{
		float elapsedAnimationTime = 0;
		float scale = 1.0f;
		inline float easeInOutCubic(float x) {
			return x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2;
		}
		inline void animateScale(float deltaTime) {
			elapsedAnimationTime += deltaTime;
			scale = 1.0f - easeInOutCubic(elapsedAnimationTime);
		}
		virtual void animationDone() = 0;
	};

	struct Shooter
	{
		Shooter(int price, glm::vec3 color) : price(price), color(color) {}
		int price;
		glm::vec3 color;
	};

	struct Enemy : public ScaleEaseInOutAnimatable
	{
		Enemy(int line, glm::vec3 coordinates, int type, glm::vec3 color) : line(line), coordinates(coordinates), type(type), color(color) {}
		int health = 3;
		int type;
		int line;
		glm::vec3 coordinates;
		glm::vec3 color;
		bool safeToDelete = false;

		inline void animationDone() override {
			safeToDelete = true;
		}
	};

	struct Heart : public ScaleEaseInOutAnimatable
	{
		bool removed = false;
		bool safeToDelete = false;
		inline void animationDone() override {
			safeToDelete = true;
		}
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

	struct Star
	{
		Star(glm::vec3 coordinates) : coordinates(coordinates) {}
		glm::vec3 coordinates;
	};

	struct ItemBoxData
	{
		ItemBoxData(int x, int y, int length, Shooter* shooter) : x(x), y(y), length(length), shooter(shooter) {}
		int x; // x coordinate of bottom-left corner
		int y; // y coordinate of bottom-left corner
		int length;
		Shooter* shooter;
	};

	struct TableBoxData : public ScaleEaseInOutAnimatable
	{
		TableBoxData(int x, int y, int length) : x(x), y(y), length(length) {}
		int x; // x coordinate of bottom-left corner
		int y; // y coordinate of bottom-left corner
		int length;
		milliseconds timeLastShot = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		Shooter *shooter = nullptr;
		bool isShooterDeleted = false;

		inline void animationDone() override {
			scale = 1.0f;
			elapsedAnimationTime = 0;
			isShooterDeleted = false;
			shooter = nullptr;
		}
		inline glm::vec3 getCenter() { return glm::vec3(x + SQUARE_LENGTH / 2, y + SQUARE_LENGTH / 2, 0); }
	};

	void checkProjectileEnemyCollisions(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);
	void checkShooterEnemyCollisions(game::TableBoxData tableCoordinates[3][3], std::vector<Enemy>& enemies);
	void removeInvalidPieces(std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies, std::vector<Heart>& hearts);
	void generateEnemies(std::vector<Enemy>& enemies, std::vector<Shooter>& shooters, milliseconds& lastGeneratedEnemies);
	void generateProjectiles(TableBoxData tableCoordinates[3][3], std::vector<Projectile>& projectiles, std::vector<Enemy>& enemies);
	void generateStars(std::vector<Star>& stars, milliseconds& lastGeneratedStars);
	void checkEnemyReachedEnd(std::vector<Enemy>& enemies, std::vector<Heart>& hearts);
	void checkHasSelectedShooter(glm::vec3 mouseCoordinates, std::vector<ItemBoxData> itemCoordinates, Shooter* &selectedShooter, int& currentStars);
	void checkHasCollectedStar(glm::vec3 mouseCoordinates, std::vector<Star>& stars, int& currentStars);
	void checkHasDroppedShooter(glm::vec3 mouseCoordinates, TableBoxData tableCoordinates[3][3], Shooter*& selectedShooter, int& currentStars);
	void checkHasClearedBox(glm::vec3 mouseCoordinates, TableBoxData tableCoordinates[3][3]);
	void checkIfGameEnded(std::vector<Heart>& hearts, WindowObject* window);
}