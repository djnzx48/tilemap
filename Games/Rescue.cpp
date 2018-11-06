// Rescue (64 rooms)
//

#include "stdafx.h"
#include "Rescue.h"

Rescue_Game::Rescue_Game(std::shared_ptr<TileView> pTileView, bool cheat_pokes)
	: SpectrumGame(pTileView, 0)
{
	auto &mem = load_snapshot(L"rescue.sna");

	if (cheat_pokes)
	{
		mem[0xbd50] = 0xc9;		// infinite strength
		mem[0xbfb1] = 0xc9;		// infinite ammo
	}

	InstallHooks(mem);
	SetMap(BuildMap(mem));
	m_pTileView->SetPanels({ { 0, 0, SPECTRUM_WIDTH_PIXELS, PANEL_HEIGHT, PanelAlignment::Top } });
}

void Rescue_Game::InstallHooks(std::vector<uint8_t> &mem)
{
	// Start of game
	Hook(mem, 0xc12a, 0x31 /*LD SP, nn*/, [&](Tile &tile)
	{
		if (IsActiveTile(tile)) {
			CloneToAllTiles(tile);

			auto &tile_down = FindRoomTile(LEFT_UP_ONE_ROOM);
			m_pTileView->CentreViewOnTile(tile_down);

			auto &tile_start = FindRoomTile(START_ROOM);
			ActivateTile(tile_start);
		}
	});

	// Room change
	Hook(mem, 0xca22, 0x2a /*LD HL, nn*/, [&](Tile &tile)
	{
		int new_room = tile.mem[0xd170];

		if (IsActiveTile(tile) && new_room != tile.room) {
			auto &tile_new = FindRoomTile(new_room);
			auto lock = tile_new.Lock();

			CloneTile(tile, tile_new);

			tile_new.mem[0xd170] = new_room; // player room number

			int grid_x = new_room & 7;
			int grid_y = new_room >> 3;

			constexpr int base_address = 0xa080;
			int map_address = base_address + 6*10*MAP_WIDTH*grid_y + 10*grid_x;

			tile_new.mem[0xd18b] = map_address & 0xff;
			tile_new.mem[0xd18c] = map_address >> 8;

			int thingy = 0x08d8 + 1024*grid_y + 4*grid_x;

			tile_new.mem[0xfd39] = thingy & 0xff;
			tile_new.mem[0xfd3a] = thingy >> 8;

			ActivateTile(tile_new);
			tile_new.drawing = false;
		}

		{
			tile.drawing = false;
			tile.mem[0xd170] = tile.room; // player room number

			int grid_x = tile.room & 7;
			int grid_y = tile.room >> 3;

			constexpr int base_address = 0xa080;
			int map_address = base_address + 6 * 10 * MAP_WIDTH*grid_y + 10 * grid_x;

			tile.mem[0xd18b] = map_address & 0xff;
			tile.mem[0xd18c] = map_address >> 8;

			int thingy = 0x08d8 + 1024 * grid_y + 4 * grid_x;

			tile.mem[0xfd39] = thingy & 0xff;
			tile.mem[0xfd3a] = thingy >> 8;
		}
	});

	// Room drawing complete
	Hook(mem, 0xca50, 0x3a /*LD A, (nn)*/, [&](Tile &tile)
	{
		tile.drawing = true;
	});


	// Call-if-active helper
	auto call_if_active = [&](Tile &tile)
	{
		if (!IsActiveTile(tile))
			Ret(tile);
	};

	// Skip player sprite drawing in inactive rooms
	Hook(mem, 0xf582, 0xdd /*LD IX, nn*/, call_if_active);
}

std::vector<MapRoom> Rescue_Game::BuildMap(const std::vector<uint8_t> &mem)
{
	return BuildRegularMap(MAP_WIDTH, MAP_HEIGHT, TILE_WIDTH, TILE_HEIGHT,
		0, 0, TILE_OFFSET_X, TILE_OFFSET_Y);
}
