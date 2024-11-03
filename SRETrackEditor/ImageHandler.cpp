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

#include "ConstantsImages.h"
#include "ImageHandler.h"

ImageHandler* GImageHandler;


ImageHandler::ImageHandler(const std::wstring path)
{
	const std::wstring terrain_folders[] = { L"road" };

	std::wstring tiles = L"data\\images\\track\\" + terrain_folders[0] + L"\\64x64\\";

	LoadTrackTiles(tiles);
}


bool ImageHandler::LoadTrackTiles(const std::wstring path)
{
	for (int t = 0; t < kRaceTrackTileCount; t++)
	{
		TPngImage* image = new TPngImage();

		image->LoadFromFile(path.c_str() + IntToHex(t, 2) + L".png");

        RaceTrackTiles.push_back(image);
	}

	return true;
}
