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

			Load(-1, file_name, false);
		}
	}
}


void RaceTrackHandler::Clear(int index)
{
	std::memset(RaceTracks[index].Grid, 0, kMaxTrackWidth * kMaxTrackHeight * sizeof(int));
}


bool RaceTrackHandler::Load(int index, const std::wstring file_name, bool allow_fail)
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

							rt.SetStartDirection(initialdirection);

							if (GeneratePoints(rt) || allow_fail)
							{
								if (index == -1)
								{
									RaceTracks.push_back(rt);
								}
								else
								{
                                    RaceTracks[index] = rt;
								}
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


bool RaceTrackHandler::Test(int index)
{
    return GeneratePoints(RaceTracks[index]);
}


bool RaceTrackHandler::GeneratePoints(RaceTrack &rt)
{
    rt.Points.clear();

	int x = rt.StartX;
	int y = rt.StartY;
	bool is_stage = false;  // track type loop or stage
	bool finished = false;
	bool valid = true;
	int tile = 0;

	TrackDirection direction = rt.StartDirection;

	while (!finished && valid)
	{
		tile = rt.Grid[y * kMaxTrackWidth + x];

		BasicTrackType btt = TrackHelper::GetBasicTrackType(tile);

		switch (btt)
		{
			case BasicTrackType::kCurveTL:
				if (direction == TrackDirection::kNorth)
				{
					AddPoints(true, tile, x, y, rt.Points);

					direction = TrackDirection::kEast;
				}
				else if (direction == TrackDirection::kWest)
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
				if (direction == TrackDirection::kEast)
				{
					AddPoints(true, tile, x, y, rt.Points);

					direction = TrackDirection::kSouth;
				}
				else if (direction == TrackDirection::kNorth)
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
				if (direction == TrackDirection::kSouth)
				{
					AddPoints(true, tile, x, y, rt.Points);

					direction = TrackDirection::kWest;
				}
				else if (direction == TrackDirection::kEast)
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
				if (direction == TrackDirection::kWest)
				{
					AddPoints(true, tile, x, y, rt.Points);

					direction = TrackDirection::kNorth;
				}
				else if (direction == TrackDirection::kSouth)
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
				if (direction == TrackDirection::kEast)
				{
					AddPoints(true, tile, x, y, rt.Points);
				}
				else if (direction == TrackDirection::kWest)
				{
					AddPoints(false, tile, x, y, rt.Points);
				}
				else
				{
					valid = false;
				}
				break;

			case BasicTrackType::kVertical:
				if (direction == TrackDirection::kSouth)
				{
					AddPoints(true, tile, x, y, rt.Points);
				}
				else if (direction == TrackDirection::kNorth)
				{
					AddPoints(false, tile, x, y, rt.Points);
				}
				else
				{
					valid = false;
				}
				break;

			case BasicTrackType::kCrossroads:
				if (direction == TrackDirection::kEast)
				{
					AddPoints(true, kTileStraightH, x, y, rt.Points);
				}
				else if (direction == TrackDirection::kWest)
				{
					AddPoints(false, kTileStraightH, x, y, rt.Points);
				}
				else if (direction == TrackDirection::kSouth)
				{
					AddPoints(true, kTileStraightV, x, y, rt.Points);
				}
				else if (direction == TrackDirection::kNorth)
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
				ShowMessage(L"Tile: " + IntToStr(tile) + L"; x:" + IntToStr(x) + L" y: " + IntToStr(y) + L"; " + TrackHelper::TrackDirectionToStr(direction).c_str());
				valid = false;
				break;
		}

		if (valid)
		{
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
		}

		if (x == rt.StartX && y == rt.StartY)
		{
			finished = true;
		}
	}

  //	#if DEBUG
	if (!valid)
	{
		ShowMessage(L"Invalid track " + IntToStr(tile) + L" at x:" + IntToStr(x) + L" y: " + IntToStr(y) + L"; " + TrackHelper::TrackDirectionToStr(direction).c_str());
	}
   //	#endif

	return valid;
}


void RaceTrackHandler::AddPoints(bool forwards, int tile, int &x, int &y, std::vector<RaceTrackPoint>& rtps)
{
	TrackCorner corner = TrackHelper::Corner(tile);

	for (int i = 0; i < 64; i++)
	{
		int xpos = x * 64;
		int ypos = y * 64;
		int deltax = 0;
		int deltay = 0;
		int h = i;

		if (forwards)
		{
			deltax = kTrackTileData[tile][i][0];
			deltay = kTrackTileData[tile][i][1];
		}
		else
		{
			deltax = kTrackTileData[tile][63 - i][0];
			deltay = kTrackTileData[tile][63 - i][1];
		}

		if (deltax != -1 && deltay != -1)
		{
			xpos += deltax;
			ypos += deltay;

			RaceTrackPoint rtp(xpos, ypos, TrackSurface::kNone, corner, kTrackTileHazard[tile][h]);

			rtps.push_back(rtp);
		}
	}
}


bool RaceTrackHandler::Save(int index, const std::wstring file_name)
{
	std::ofstream file(file_name);

	if (file)
	{
		file << Formatting::to_utf8(L"{\n");
		file << Formatting::to_utf8(L"d:" + RaceTracks[index].Name + L"\n");
		file << Formatting::to_utf8(L"u:" + std::to_wstring(RaceTracks[index].UniqueId) + L"\n");
		file << Formatting::to_utf8(L"z:" + std::to_wstring(RaceTracks[index].CountryId) + L"\n");
		file << Formatting::to_utf8(L"c:" + std::to_wstring(RaceTracks[index].Category) + L"\n");
		file << Formatting::to_utf8(L"s:" + std::to_wstring(RaceTracks[index].Scale) + L"\n");
		file << Formatting::to_utf8(L"x:" + std::to_wstring(RaceTracks[index].MapX) + L"\n");
		file << Formatting::to_utf8(L"y:" + std::to_wstring(RaceTracks[index].MapY) + L"\n");
		file << Formatting::to_utf8(L"v:" + std::to_wstring(RaceTracks[index].StartX) + L"\n");
		file << Formatting::to_utf8(L"w:" + std::to_wstring(RaceTracks[index].StartY) + L"\n");
		file << Formatting::to_utf8(L"l:" + std::to_wstring(LevelToInt(RaceTracks[index].Level)) + L"\n");
		file << Formatting::to_utf8(L"m:" + std::to_wstring(DirectionToInt(RaceTracks[index].StartDirection)) + L"\n");
		file << Formatting::to_utf8(L"t:" + std::to_wstring(TerrainToInt(RaceTracks[index].Terrain)) + L"\n");

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

		file << Formatting::to_utf8(L"}\n");

		file.close();

		return true;
	}

	return false;
}


int RaceTrackHandler::LevelToInt(TrackLevel tl)
{
	switch (tl)
	{
	case TrackLevel::kAmateur:
		return 0;
	case TrackLevel::kAmateurOffRoad:
		return 1;
	case TrackLevel::kAmateurEndurance:
		return 2;
	case TrackLevel::kClubSeries:
		return 3;
	case TrackLevel::kClubSeriesP2P:
		return 4;
	case TrackLevel::kClubSeriesOffRoad:
		return 5;
	case TrackLevel::kPro:
		return 6;
	case TrackLevel::kProP2P:
		return 7;
	case TrackLevel::kProOffRoad:
		return 8;
	case TrackLevel::kSuperLeague:
		return 9;
	case TrackLevel::kSuperLeagueP2P:
		return 10;
	case TrackLevel::kEndurance:
		return 11;
	}
}


int RaceTrackHandler::DirectionToInt(TrackDirection td)
{
	switch (td)
	{
	case TrackDirection::kNorth:
		return 0;
	case TrackDirection::kEast:
		return 1;
	case TrackDirection::kSouth:
		return 2;
	case TrackDirection::kWest:
		return 3;
	}
}


int RaceTrackHandler::TerrainToInt(TrackTerrain tt)
{
	switch (tt)
	{
	case TrackTerrain::kRoad:
		return 0;
	case TrackTerrain::kDirt:
		return 1;
	case TrackTerrain::kGrass:
		return 2;
	case TrackTerrain::kDesert:
		return 3;
	case TrackTerrain::kIce:
		return 4;
	}
}
