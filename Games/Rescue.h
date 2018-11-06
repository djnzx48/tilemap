#pragma once
#include "SpectrumGame.h"

class Rescue_Game : public SpectrumGame
{
public:
	Rescue_Game(std::shared_ptr<TileView> pTileView, bool cheat_pokes);

private:
	void InstallHooks(std::vector<uint8_t> &mem);
	std::vector<MapRoom> BuildMap(const std::vector<uint8_t> &mem);

	static constexpr int MAP_WIDTH = 8;
	static constexpr int MAP_HEIGHT = 8;
	static constexpr int PANEL_HEIGHT = 48;
	static constexpr int TILE_WIDTH = SPECTRUM_WIDTH_PIXELS - 16;
	static constexpr int TILE_HEIGHT = SPECTRUM_HEIGHT_PIXELS - PANEL_HEIGHT;
	static constexpr int TILE_OFFSET_X = 8;
	static constexpr int TILE_OFFSET_Y = PANEL_HEIGHT;

	static constexpr int LEFT_UP_ONE_ROOM = 54;
	static constexpr int START_ROOM = 63;
};
