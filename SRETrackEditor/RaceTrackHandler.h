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

#include "RaceTrack.h"
#include "RaceTrackPoint.h"


class RaceTrackHandler
{
    const static int kTrackCount = 1;

	bool LoadTrack(const std::wstring);
	bool GeneratePoints(RaceTrack&);

	void AddPoints(bool, int, int&, int&, std::vector<RaceTrackPoint>&);

public:

	std::vector<RaceTrack> RaceTracks;

	RaceTrackHandler(bool, const std::wstring);

	void Clear(int);

    bool Save(int, const std::wstring);
};
