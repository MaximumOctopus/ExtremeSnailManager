//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------

#include "DrawTool.h"

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPaintBox *pbTrack;
	TLabel *lTileHex;
	TBitBtn *bNew;
	TBitBtn *bLoad;
	TBitBtn *bSave;
	TBitBtn *bTest;
	TImage *Image1;
	TImage *Image2;
	TImage *Image3;
	TImage *Image4;
	TImage *Image5;
	TImage *Image6;
	TImage *Image7;
	TImage *Image8;
	TImage *Image9;
	TImage *Image10;
	TImage *Image11;
	TImage *Image12;
	TImage *Image13;
	TImage *Image14;
	TImage *Image15;
	TImage *Image16;
	TImage *Image17;
	TImage *Image18;
	TImage *Image19;
	TImage *Image20;
	TBitBtn *bStart;
	TLabel *lStart;
	TSaveDialog *sdMain;
	void __fastcall pbTrackPaint(TObject *Sender);
	void __fastcall pbTrackMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall pbTrackMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall bLoadClick(TObject *Sender);
	void __fastcall bNewClick(TObject *Sender);
	void __fastcall bSaveClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall bTestClick(TObject *Sender);
	void __fastcall Image1Click(TObject *Sender);
	void __fastcall bStartClick(TObject *Sender);
	void __fastcall Image10Click(TObject *Sender);

private:

	DrawTool drawtool;

	void UpdateInfoPanel();

public:
	__fastcall TForm1(TComponent* Owner);
};

extern PACKAGE TForm1 *Form1;

#endif
