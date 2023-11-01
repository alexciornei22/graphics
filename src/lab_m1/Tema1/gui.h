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
}