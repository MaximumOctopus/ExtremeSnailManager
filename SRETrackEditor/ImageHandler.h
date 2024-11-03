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

#include <Vcl.Imaging.pngimage.hpp>
#include <vector>


class ImageHandler
{
	bool LoadTrackTiles(const std::wstring);

public:

	std::vector<TPngImage*> RaceTrackTiles;

	ImageHandler(const std::wstring);
};
