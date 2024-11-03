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

#include <fstream>

#include "Formatting.h"
#include "IOUtility.h"
#include "RaceTrackHandler.h"

RaceTrackHandler* GRaceTrackHandler;


RaceTrackHandler::RaceTrackHandler(bool autoload, const std::wstring path)
{
	if (autoload)
	{
		for (int t = 0; t < kTrackCount; t++)
		{
			std::wstring file_name = path + L"data\\system\\tracks\\racetrack" + IntToHex(t + 1, 3).c_str() + L".dat";

			LoadTrack(file_name);
		}
	}
}


void RaceTrackHandler::Clear(int index)
{
	std::memset(RaceTracks[index].Grid, 0, kMaxTrackWidth * kMaxTrackHeight * sizeof(int));
}


bool RaceTrackHandler::LoadTrack(const std::wstring file_name)
{
	std::wifstream file(file_name);

	if (file)
	{
		std::wstring s(L"");

		std::wstring description = L"";
		int id = -1;
		int level = -1;
		int countryid = -1;
		int mapx = -1;
		int mapy = -1;
		int terrain = -1;
		int category = -1;
		int scale = -1;
		int startx = -1;
		int starty = -1;
		int initialdirection = 1;
		std::vector<std::wstring> trackdata;

		while (std::getline(file, s))
		{
			if (!s.empty())
			{
				if (s[0] == L'/' || s[0] == L'#')
				{
					// comment, do nothing
				}
				else
				{
					std::wstring v = s;

					if (s.length() >= 2)
					{
                        v = s.substr(2);
					}

					switch (IOUtility::GetParameter(s))
					{
					case IOParameter::kStructBegin:
						break;
					case IOParameter::kStructEnd:
					{
						RaceTrack rt(id, description, countryid, category, level, scale, mapx, mapy, startx, starty);

						if (trackdata.size() != 0)
						{
							for (int t = 0; t < trackdata.size(); t++)
							{
								rt.AddTrackRow(t, trackdata[t]);
							}

							if (GeneratePoints(rt))
							{
								rt.SetStartDirection(initialdirection);

								RaceTracks.push_back(rt);
							}
						}
						else
						{
							ShowMessage(L"No trackdata for \"" + ExtractFileName(file_name.c_str()) + L"\"");
						}

						break;
					}
					case IOParameter::kC:
						category = stoi(v);
						break;
					case IOParameter::kD:
						description = v;
						break;
					case IOParameter::kL:
						level = stoi(v);
						break;
					case IOParameter::kM:
						initialdirection = stoi(v);
						break;
					case IOParameter::kR:
						trackdata.push_back(v);
						break;
					case IOParameter::kS:
						scale = stoi(v);
						break;
					case IOParameter::kT:
                        terrain = stoi(v);
						break;
					case IOParameter::kU:
                        id = stoi(v);
						break;
					case IOParameter::kV:
						startx = stoi(v);
						break;
					case IOParameter::kW:
						starty = stoi(v);
						break;
					case IOParameter::kX:
						mapx = stoi(v);
						break;
					case IOParameter::kY:
						mapy = stoi(v);
						break;
					case IOParameter::kZ:
						countryid = stoi(v);
						break;
					}
				}
			}
		}

		file.close();

		return true;
	}

	return false;
}


bool RaceTrackHandler::GeneratePoints(RaceTrack &rt)
{
	int x = rt.StartX;
	int y = rt.StartY;
	bool is_stage = false;  // track type loop or stage
	bool finished = false;
	bool valid = true;

	TrackDirection direction = rt.StartDirection;

	while (!finished && valid)
	{
		int tile = rt.Grid[y * kMaxTrackWidth + x];

		BasicTrackType btt = TrackHelper::GetBasicTrackType(tile);

		switch (btt)
		{
			case BasicTrackType::kCurveTL:
				if (direction == kNorth)
				{
					AddPoints(true, tile, x, y, rt.Points);

					direction = TrackDirection::kEast;
				}
				else if (direction == kWest)
				{
					AddPoints(false, tile, x, y, rt.Points);

					direction = TrackDirection::kSouth;
				}
				else
				{
					valid = false;
				}
				break;

			case BasicTrackType::kCurveTR:
				if (direction == kEast)
				{
					AddPoints(true, tile, x, y, rt.Points);

					direction = TrackDirection::kSouth;
				}
				else if (direction == kNorth)
				{
					AddPoints(false, tile, x, y, rt.Points);

					direction = TrackDirection::kWest;
				}
				else
				{
					valid = false;
				}
				break;

			case BasicTrackType::kCurveBR:
				if (direction == kSouth)
				{
					AddPoints(true, tile, x, y, rt.Points);

					direction = TrackDirection::kWest;
				}
				else if (direction == kEast)
				{
					AddPoints(false, tile, x, y, rt.Points);

					direction = TrackDirection::kNorth;
				}
				else
				{
					valid = false;
				}
				break;

			case BasicTrackType::kCurveBL:
				if (direction == kWest)
				{
					AddPoints(true, tile, x, y, rt.Points);

					direction = TrackDirection::kNorth;
				}
				else if (direction == kSouth)
				{
					AddPoints(false, tile, x, y, rt.Points);

					direction = TrackDirection::kEast;
				}
				else
				{
					valid = false;
				}
				break;

			case BasicTrackType::kHorizontal:
				if (direction == kEast)
				{
					AddPoints(true, tile, x, y, rt.Points);
				}
				else if (direction == kWest)
				{
					AddPoints(false, tile, x, y, rt.Points);
				}
				else
				{
					valid = false;
				}
				break;

			case BasicTrackType::kVertical:
				if (direction == kSouth)
				{
					AddPoints(true, tile, x, y, rt.Points);
				}
				else if (direction == kNorth)
				{
					AddPoints(false, tile, x, y, rt.Points);
				}
				else
				{
					valid = false;
				}
				break;

			case BasicTrackType::kCrossroads:
				if (direction == kEast)
				{
					AddPoints(true, kTileStraightH, x, y, rt.Points);
				}
				else if (direction == kWest)
				{
					AddPoints(false, kTileStraightH, x, y, rt.Points);
				}
				else if (direction == kSouth)
				{
					AddPoints(true, kTileStraightV, x, y, rt.Points);
				}
				else if (direction == kNorth)
				{
					AddPoints(false, kTileStraightV, x, y, rt.Points);
				}
				break;

			case BasicTrackType::kStart:
				AddPoints(true, tile, x, y, rt.Points);
				is_stage = true;
				break;

			case BasicTrackType::kEnd:
				AddPoints(true, tile, x, y, rt.Points);
				if (is_stage)
				{
					finished = true;
				}
				else
				{
					valid = false;
				}
				break;

			default:
				ShowMessage(L"Tile: " + IntToStr(tile) + L"; x:" + IntToStr(x) + L" y: " + IntToStr(y));
				valid = false;
				break;
		}

		switch (direction)
		{
		case TrackDirection::kNorth:
			y--;
			break;
		case TrackDirection::kEast:
			x++;
			break;
		case TrackDirection::kSouth:
			y++;
			break;
		case TrackDirection::kWest:
			x--;
			break;
		}

		if (x == rt.StartX && y == rt.StartY)
		{
			finished = true;
		}
	}

	#if DEBUG
	if (!valid)
	{
		ShowMessage(L"Invalid track at x:" + IntToStr(x) + L" y: " + IntToStr(y));
	}
	#endif

	return valid;
}


void RaceTrackHandler::AddPoints(bool forwards, int tile, int &x, int &y, std::vector<RaceTrackPoint>& rtps)
{
	bool is_corner = TrackHelper::IsCorner(tile);

	for (int i = 0; i < 64; i++)
	{
		int xpos = x * 64;
		int ypos = y * 64;
		int h = i;

		if (forwards)
		{
			xpos += kTrackTileData[tile][i][0];
			ypos += kTrackTileData[tile][i][1];
		}
		else
		{
			xpos += kTrackTileData[tile][63 - i][0];
			ypos += kTrackTileData[tile][63 - i][1];

			h = 63 - i;
		}

		RaceTrackPoint rtp(xpos, ypos, TrackSurface::kNone, is_corner, kTrackTileHazard[tile][h]);

		rtps.push_back(rtp);
	}
}


bool RaceTrackHandler::Save(int index, const std::wstring file_name)
{
	std::ofstream file(file_name);

	if (file)
	{
		file << Formatting::to_utf8(L"[\n");

		file << Formatting::to_utf8(L"d:" + RaceTracks[index].Name + L"\n");
		file << Formatting::to_utf8(L"u:" + std::to_wstring(RaceTracks[index].UniqueId) + L"\n");
		file << Formatting::to_utf8(L"z:" + std::to_wstring(RaceTracks[index].CountryId) + L"\n");
		file << Formatting::to_utf8(L"c:" + std::to_wstring(RaceTracks[index].Category) + L"\n");
		file << Formatting::to_utf8(L"s:" + std::to_wstring(RaceTracks[index].Scale) + L"\n");
		file << Formatting::to_utf8(L"l:" + std::to_wstring(RaceTracks[index].Level) + L"\n");
		file << Formatting::to_utf8(L"x:" + std::to_wstring(RaceTracks[index].MapX) + L"\n");
		file << Formatting::to_utf8(L"y:" + std::to_wstring(RaceTracks[index].MapY) + L"\n");
		file << Formatting::to_utf8(L"v:" + std::to_wstring(RaceTracks[index].StartX) + L"\n");
		file << Formatting::to_utf8(L"w:" + std::to_wstring(RaceTracks[index].StartY) + L"\n");

		switch (RaceTracks[index].StartDirection)
		{
		case TrackDirection::kNorth:
			file << Formatting::to_utf8(L"m:0\n");
			break;
		case TrackDirection::kEast:
			file << Formatting::to_utf8(L"m:1\n");
			break;
		case TrackDirection::kSouth:
			file << Formatting::to_utf8(L"m:2\n");
			break;
		case TrackDirection::kWest:
			file << Formatting::to_utf8(L"m:3\n");
			break;
		}

		file << Formatting::to_utf8(L"t:0\n");

		for (int y = 0; y < kMaxTrackHeight; y++)
		{
			std::wstring row(L"");

			for (int x = 0; x < kMaxTrackWidth; x++)
			{
				if (x == kMaxTrackWidth - 1)
				{
					row += IntToHex(RaceTracks[index].Grid[y * kMaxTrackWidth + x], 2);
				}
				else
				{
					row += IntToHex(RaceTracks[index].Grid[y * kMaxTrackWidth + x], 2) + L",";
				}
			}

			file << Formatting::to_utf8(L"r:" + row + L"\n");
		}

		file.close();

		return true;
	}

    return false;
}
