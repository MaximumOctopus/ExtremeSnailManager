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
	lTileHex->Caption = IntToHex(drawtool.Tile, 2);
}


void __fastcall TForm1::bNewClick(TObject *Sender)
{
	GRaceTrackHandler->Clear(0);

    pbTrack->Invalidate();
}


void __fastcall TForm1::bLoadClick(TObject *Sender)
{
//
}


void __fastcall TForm1::bSaveClick(TObject *Sender)
{
	if (sdMain->Execute())
	{
		GRaceTrackHandler->Save(0, sdMain->FileName.c_str());
	}
}


void __fastcall TForm1::bTestClick(TObject *Sender)
{
	//
}


void __fastcall TForm1::pbTrackPaint(TObject *Sender)
{
	for (int h = 0; h < 10; h++)
	{
		for (int w = 0; w < 18; w++)
		{
			int tile = GRaceTrackHandler->RaceTracks[0].Grid[h * 18 + w];

			GImageHandler->RaceTrackTiles[tile]->Draw(pbTrack->Canvas, Rect(w * 64, h * 64, w * 64 + 64, h * 64 + 64));
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

	pbTrack->Invalidate();
}


void __fastcall TForm1::pbTrackMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
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

		lStart->Caption = L"x: " + IntToStr(drawtool.x) + L", y: " + IntToStr(drawtool.y);

		break;
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
}


void __fastcall TForm1::Image10Click(TObject *Sender)
{
	TImage* image = (TImage*)Sender;

	drawtool.Mode = 2;
	drawtool.SquareTile1 = image->Tag;
	drawtool.SquareTile2 = image->Tag + 1;
	drawtool.SquareTile3 = image->Tag + 2;
	drawtool.SquareTile4 = image->Tag + 3;
}


void __fastcall TForm1::bStartClick(TObject *Sender)
{
    drawtool.Mode = 99;
}