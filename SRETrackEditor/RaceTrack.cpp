// =======================================================================
//
// Extreme Snail Manager
//
// (c) Paul Alan Freshney 1995-2024
// paul@freshney.org
//
// https://github.com/MaximumOctopus/ExtremeSnailManager
//
// =======================================================================

#include "ConvertUtility.h"
#include "RaceTrack.h"


RaceTrack::RaceTrack(int id, std::wstring description, int countryid, int cat, int level, int scale, int mapx, int mapy, int startx, int starty)
{
	Name = description;
	UniqueId = id;
	CountryId = countryid;
	Category = cat;
	Level = level;
	Scale = scale;

	MapX = mapy;
	MapY = mapy;

	StartX = startx;
	StartY = starty;

	std::memset(Grid, 0, kMaxTrackWidth * kMaxTrackHeight * sizeof(int));
}


void RaceTrack::SetStartDirection(int direction)
{
	int tile = Grid[StartY * kMaxTrackWidth + StartX];

	switch (direction)
	{
	case 0:
		StartDirection = TrackDirection::kNorth;
		break;
	case 1:
		StartDirection = TrackDirection::kEast;
		break;
	case 2:
		StartDirection = TrackDirection::kSouth;
		break;
	case 3:
		StartDirection = TrackDirection::kWest;
		break;
	}

	// override the track direction if we can set from the start tile

	if (tile != 0)
	{
		switch (tile)
		{
		case kTileStartDown:
			StartDirection = TrackDirection::kSouth;
			break;
		case kTileStartLeft:
			StartDirection = TrackDirection::kWest;
			break;
		case kTileStartUp:
			StartDirection = TrackDirection::kNorth;
			break;
		case kTileStartRight:
			StartDirection = TrackDirection::kEast;
			break;
		}
	}
}


void RaceTrack::AddTrackRow(int row, const std::wstring data)
{
	std::wstring tile(L"");
	int x = 0;

	for (int i = 0; i < data.length(); i++)
	{
		if (data[i] == ',')
		{
			int tileid = ConvertUtility::HexToInt(tile);

			Grid[row * kMaxTrackWidth + x] = tileid;

			x++;

			tile = L"";
		}
		else if (i == data.length() - 1)
		{
            tile += data[i];

			int tileid = ConvertUtility::HexToInt(tile);

			Grid[row * kMaxTrackWidth + x] = tileid;

			x++;

			tile = L"";
		}
		else
		{
			tile += data[i];
		}
	}
}
