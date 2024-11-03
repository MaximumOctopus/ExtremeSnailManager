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

#pragma once

#include <vector>

#include "RaceTrackPoint.h"


class RaceTrack
{

public:

	std::vector<RaceTrackPoint> Points;

	std::wstring Name = L"";

	int UniqueId = 0;
	int CountryId = 0;
	int Category = 0;
	int Level = 0;
	int Scale = 1;
	TrackTerrain Terrain = TrackTerrain::kNone;

    TrackDirection StartDirection = TrackDirection::kEast;

	int MapX = 0;
	int MapY = 0;

	int StartX = 0;
	int StartY = 0;

	int MaxX = 0;
    int MaxY = 0;

	int Grid[kMaxTrackWidth * kMaxTrackHeight];

	RaceTrack(int, const std::wstring, int, int, int, int, int, int, int, int);

	void AddTrackRow(int, const std::wstring);

	void SetStartDirection(int);
};
