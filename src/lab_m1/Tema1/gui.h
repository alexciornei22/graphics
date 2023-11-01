#pragma once

namespace gui {
	struct ItemBoxData
	{
		ItemBoxData(int x, int y, int length, int itemType) : x(x), y(y), length(length), itemType(itemType) {}
		int x; // x coordinate of bottom-left corner
		int y; // y coordinate of bottom-left corner
		int length;
		int itemType;
	};

	struct TableBoxData
	{
		TableBoxData(int x, int y, int length) : x(x), y(y), length(length) {}
		int x; // x coordinate of bottom-left corner
		int y; // y coordinate of bottom-left corner
		int length;
		int shooter = -1;
	};
}