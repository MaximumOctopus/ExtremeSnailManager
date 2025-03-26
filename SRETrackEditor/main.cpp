//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"

#include "ConstantsImages.h"
#include "ImageHandler.h"
#include "RaceTrack.h"
#include "RaceTrackHandler.h"
#include "TileUtility.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

extern ImageHandler* GImageHandler;
extern RaceTrackHandler* GRaceTrackHandler;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	std::wstring path = ExtractFilePath(Application->ExeName).c_str();

	GImageHandler = new ImageHandler(path);

	GRaceTrackHandler = new RaceTrackHandler(false, path);

	RaceTrack rt(0, L"Default", 0, 0, 0, 0, 0, 0, 1, 1);

	GRaceTrackHandler->RaceTracks.push_back(rt);
}


void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == 0x52)
	{
		switch (drawtool.Mode)
		{
		case 1:
			drawtool.Tile = TileUtility::RotateCW(drawtool.Tile);

            UpdateInfoPanel();

			pbTrack->Invalidate();
			break;

		case 2:
			drawtool.SquareTile1 = TileUtility::RotateQuadCW(drawtool. SquareTile1);
			drawtool.SquareTile2 = drawtool.SquareTile1 + 1;
			drawtool.SquareTile3 = drawtool.SquareTile1 + 2;
			drawtool.SquareTile4 = drawtool.SquareTile1 + 3;

			pbTrack->Invalidate();
            break;
		}
	}
}


void TForm1::UpdateInfoPanel()
{
	lStart->Caption = L"x: " + IntToStr(GRaceTrackHandler->RaceTracks[0].StartX) + L", y: " + IntToStr(GRaceTrackHandler->RaceTracks[0].StartY);

	switch (drawtool.Mode)
	{
	case 1:
		lTileHex->Caption = IntToHex(drawtool.Tile, 2);
		break;
	case 2:
		lTileHex->Caption = L"2x2; " + IntToHex(drawtool.SquareTile1, 2) + L"/" +
									   IntToHex(drawtool.SquareTile2, 2) + L"/" +
									   IntToHex(drawtool.SquareTile3, 2) + L"/" +
									   IntToHex(drawtool.SquareTile4, 2);
		break;
	}
}


void TForm1::UpdateTrackFileName(const std::wstring file_name)
{
	TrackFileName = file_name;

	if (TrackFileName.empty())
	{
		Caption = L"SRETrackEditor v0.4 - November 9th 2024";
	}
	else
	{
		std::wstring output = L"SRETrackEditor v0.4 - November 9th 2024 [" + TrackFileName + L"]";

		Caption = output.c_str();
	}
}


void __fastcall TForm1::bNewClick(TObject *Sender)
{
	GRaceTrackHandler->Clear(0);

	UpdateTrackFileName(L"");

	cbStartDirection->ItemIndex = 1;
	cbStartDirectionChange(nullptr);

	cbLevel->ItemIndex = 0;
    cbLevelChange(nullptr);

	pbTrack->Invalidate();
}


void __fastcall TForm1::bLoadClick(TObject *Sender)
{
	if (odMain->Execute())
	{
		GRaceTrackHandler->Load(0, odMain->FileName.c_str(), true);

		switch (GRaceTrackHandler->RaceTracks[0].StartDirection)
		{
		case TrackDirection::kNorth:
			cbStartDirection->ItemIndex = 0;
			break;
		case TrackDirection::kEast:
			cbStartDirection->ItemIndex = 1;
			break;
		case TrackDirection::kSouth:
			cbStartDirection->ItemIndex = 2;
			break;
		case TrackDirection::kWest:
			cbStartDirection->ItemIndex = 3;
			break;
		}

		switch (GRaceTrackHandler->RaceTracks[0].Terrain)
		{
		case TrackTerrain::kRoad:
			cbTrackTerrain->ItemIndex = 0;
			break;
		case TrackTerrain::kDirt:
			cbTrackTerrain->ItemIndex = 1;
			break;
		case TrackTerrain::kGrass:
			cbTrackTerrain->ItemIndex = 2;
			break;
		case TrackTerrain::kDesert:
			cbTrackTerrain->ItemIndex = 3;
			break;
		case TrackTerrain::kIce:
			cbTrackTerrain->ItemIndex = 4;
			break;

		default:
			cbTrackTerrain->ItemIndex = 0;
		}

		switch (GRaceTrackHandler->RaceTracks[0].Level)
		{
		case TrackLevel::kAmateur:
			cbLevel->ItemIndex = 0;
			break;
		case TrackLevel::kAmateurOffRoad:
			cbLevel->ItemIndex = 1;
			break;
		case TrackLevel::kClubSeries:
			cbLevel->ItemIndex = 2;
			break;
		case TrackLevel::kClubSeriesOffRoad:
			cbLevel->ItemIndex = 3;
			break;
		case TrackLevel::kPro:
			cbLevel->ItemIndex = 4;
			break;
		case TrackLevel::kProOffRoad:
			cbLevel->ItemIndex = 5;
			break;
		case TrackLevel::kSuperLeague:
			cbLevel->ItemIndex = 6;
			break;
		case TrackLevel::kEndurance:
			cbLevel->ItemIndex = 7;
			break;
		}

		UpdateTrackFileName(odMain->FileName.c_str());

		UpdateInfoPanel();
	}
}


void __fastcall TForm1::bSaveClick(TObject *Sender)
{
	if (TrackFileName.empty())
	{
		bSaveAsClick(nullptr);
	}
	else
	{
		GRaceTrackHandler->Save(0, TrackFileName);
	}
}


void __fastcall TForm1::bSaveAsClick(TObject *Sender)
{
	if (sdMain->Execute())
	{
		GRaceTrackHandler->Save(0, sdMain->FileName.c_str());

		UpdateTrackFileName(sdMain->FileName.c_str());
	}
}


void __fastcall TForm1::bTestClick(TObject *Sender)
{
	if (GRaceTrackHandler->Test(0))
	{
		std::wstring l = std::to_wstring(GRaceTrackHandler->RaceTracks[0].Points.size()) + L" units";

		lLength->Caption = l.c_str();
	}
	else
	{
		lLength->Caption = L"Fail :(";
	}

	if (GRaceTrackHandler->RaceTracks[0].Points.size() == 0)
	{
		cbShowRoute->Checked = false;
		cbShowRoute->Enabled = false;
	}
	else
	{
		cbShowRoute->Enabled = true;
	}
}


void __fastcall TForm1::bStartClick(TObject *Sender)
{
	drawtool.Mode = 99;
}


void __fastcall TForm1::cbShowRouteClick(TObject *Sender)
{
    pbTrack->Invalidate();
}


void __fastcall TForm1::pbTrackPaint(TObject *Sender)
{
	for (int h = 0; h < kMaxTrackHeight; h++)
	{
		for (int w = 0; w < kMaxTrackWidth; w++)
		{
			int tile = GRaceTrackHandler->RaceTracks[0].Grid[h * kMaxTrackWidth + w];

			GImageHandler->RaceTrackTiles[tile]->Draw(pbTrack->Canvas, Rect(w * 64, h * 64, w * 64 + 64, h * 64 + 64));
		}
	}

	if (cbShowRoute->Checked)
	{
		for (int t = 0; t < GRaceTrackHandler->RaceTracks[0].Points.size(); t++)
		{
			pbTrack->Canvas->Rectangle(GRaceTrackHandler->RaceTracks[0].Points[t].x,
									   GRaceTrackHandler->RaceTracks[0].Points[t].y,
									   GRaceTrackHandler->RaceTracks[0].Points[t].x + 3,
									   GRaceTrackHandler->RaceTracks[0].Points[t].y + 3);
		}
	}

	switch (drawtool.Mode)
	{
	case 1:
		GImageHandler->RaceTrackTiles[drawtool.Tile]->Draw(pbTrack->Canvas, Rect(drawtool.x * 64, drawtool.y * 64, drawtool.x * 64 + 64, drawtool.y * 64 + 64));
		break;

	case 2:
		GImageHandler->RaceTrackTiles[drawtool.SquareTile1]->Draw(pbTrack->Canvas, Rect(drawtool.x * 64,       drawtool.y * 64, drawtool.x * 64 + 64, drawtool.y * 64 + 64));
		GImageHandler->RaceTrackTiles[drawtool.SquareTile2]->Draw(pbTrack->Canvas, Rect((drawtool.x + 1) * 64, drawtool.y * 64, (drawtool.x + 1) * 64 + 64, drawtool.y * 64 + 64));
		GImageHandler->RaceTrackTiles[drawtool.SquareTile3]->Draw(pbTrack->Canvas, Rect(drawtool.x * 64,       (drawtool.y + 1)  * 64, drawtool.x * 64 + 64, (drawtool.y + 1) * 64 + 64));
		GImageHandler->RaceTrackTiles[drawtool.SquareTile4]->Draw(pbTrack->Canvas, Rect((drawtool.x + 1) * 64, (drawtool.y + 1)  * 64, (drawtool.x + 1) * 64 + 64, (drawtool.y + 1) * 64 + 64));
		break;
	}
}


void __fastcall TForm1::pbTrackMouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	drawtool.x = int((double)X / 64);
	drawtool.y = int((double)Y / 64);

	lCursor->Caption = IntToHex(GRaceTrackHandler->RaceTracks[0].Grid[drawtool.y * kMaxTrackWidth + drawtool.x], 2) + L"; " + IntToStr(drawtool.x) + L", " + IntToStr(drawtool.y);

	pbTrack->Invalidate();
}


void __fastcall TForm1::pbTrackMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	if (Shift.Contains(ssLeft))
	{
		switch (drawtool.Mode)
		{
		case 1:
			GRaceTrackHandler->RaceTracks[0].Grid[drawtool.y * kMaxTrackWidth + drawtool.x] = drawtool.Tile;

			pbTrack->Invalidate();
			break;

		case 2:
			GRaceTrackHandler->RaceTracks[0].Grid[drawtool.y * kMaxTrackWidth + drawtool.x] = drawtool.SquareTile1;
			GRaceTrackHandler->RaceTracks[0].Grid[drawtool.y * kMaxTrackWidth + drawtool.x + 1] = drawtool.SquareTile2;
			GRaceTrackHandler->RaceTracks[0].Grid[(drawtool.y + 1) * kMaxTrackWidth + drawtool.x] = drawtool.SquareTile3;
			GRaceTrackHandler->RaceTracks[0].Grid[(drawtool.y + 1) * kMaxTrackWidth + drawtool.x + 1] = drawtool.SquareTile4;

			pbTrack->Invalidate();
			break;

		case 99:
			GRaceTrackHandler->RaceTracks[0].StartX = drawtool.x;
			GRaceTrackHandler->RaceTracks[0].StartY = drawtool.y;

			UpdateInfoPanel();
			break;
		}
	}
	else if (Shift.Contains(ssRight))
	{
		GRaceTrackHandler->RaceTracks[0].Grid[drawtool.y * kMaxTrackWidth + drawtool.x] = 0;

		pbTrack->Invalidate();
	}
}


void __fastcall TForm1::FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled)
{
	if (drawtool.Tile < kRaceTrackTileCount - 1)
	{
		drawtool.Tile++;

		UpdateInfoPanel();

		pbTrack->Invalidate();

		Handled = true;
	}
}


void __fastcall TForm1::FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled)
{
	if (drawtool.Mode == 1)
	{
		if (drawtool.Tile != 0)
		{
			drawtool.Tile--;

			UpdateInfoPanel();

			pbTrack->Invalidate();

            Handled = true;
		}
	}
}


void __fastcall TForm1::Image1Click(TObject *Sender)
{
	TImage* image = (TImage*)Sender;

	drawtool.Mode = 1;
	drawtool.Tile = image->Tag;

	pbTrack->Invalidate();

	UpdateInfoPanel();
}


void __fastcall TForm1::Image10Click(TObject *Sender)
{
	TImage* image = (TImage*)Sender;

	drawtool.Mode = 2;
	drawtool.SquareTile1 = image->Tag;
	drawtool.SquareTile2 = image->Tag + 1;
	drawtool.SquareTile3 = image->Tag + 2;
	drawtool.SquareTile4 = image->Tag + 3;

	pbTrack->Invalidate();

	UpdateInfoPanel();
}


void __fastcall TForm1::cbStartDirectionChange(TObject *Sender)
{
	GRaceTrackHandler->RaceTracks[0].SetStartDirection(cbStartDirection->ItemIndex);
}


void __fastcall TForm1::cbTrackTerrainChange(TObject *Sender)
{
	GRaceTrackHandler->RaceTracks[0].SetTrackTerrain(cbTrackTerrain->ItemIndex);
}


void __fastcall TForm1::cbLevelChange(TObject *Sender)
{
	GRaceTrackHandler->RaceTracks[0].SetLevel(cbLevel->ItemIndex);
}
