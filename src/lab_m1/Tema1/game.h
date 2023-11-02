#pragma once

namespace game {
	struct Shooter
	{
		Shooter(int price, glm::vec3 color) : price(price), color(color) {}
		int price;
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
		Shooter *shooter = nullptr;
	};
}