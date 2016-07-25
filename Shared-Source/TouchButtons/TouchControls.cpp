//---------------------------------------------------------------------------
#pragma hdrstop

#include "TouchControls.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// this is what we arbitrarly set the number of distinct hues there are! (must be a multiple of 6)
#define HUENUM 510

// given number above this is 1*HPE6/6, 2*HPE6/6, 3*HPE6/6 etc.
#define HPE1 85
#define HPE2 170
#define HPE3 255
#define HPE4 340
#define HPE5 425
#define HPE6 510

// Hues range (0 - HUENUM-1)
#define HUEMAX 509

// max value for rgb components (also max for luminance)
#define RGBMAX 255
//
// given the color, this function calculates hue, cromma, luminance
//
//---------------------------------------------------------------------------
__fastcall TCustomTouchButtonControl::TCustomTouchButtonControl(TComponent* Owner) : TCustomPanel(Owner),
		FImageBuffer(new Graphics::TBitmap), DesignMode(false)
{
	FImageBuffer->HandleType	= bmDIB;
	FImageBuffer->PixelFormat	= pf32bit;

	FLatchingType					= ltNone;
	FFontShadow						= false;
	FDisabledFontShadow			= true;
	FDefaultBtnColor				= Color;
	FDefaultBtnLatchedColor		= Color;
	ControlStyle					= ControlStyle << csOpaque;
	BevelInner						= bvNone;
	BevelOuter						= bvNone;
	FIndicatorDelay				= 300;
	LatchingProperties			= TLatchingProperties() << lkStayDown << lkLatchColor;

	FButtonBuffer					= NULL;
}
//---------------------------------------------------------------------------
__fastcall TCustomTouchButtonControl::~TCustomTouchButtonControl()
{
	try
	{
		delete [] FButtonBuffer;
		for (unsigned i=0; i<FButtons.size(); i++)
		{
			delete FButtons[i];
		}
	}
	catch(...)
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall TCustomTouchButtonControl::WMEraseBkgnd(TMessage& Message)
{
  Message.Result = 1; // Don't do it!
}
//---------------------------------------------------------------------------
void __fastcall TCustomTouchButtonControl::WMSize(TWMSize& Message)
{
	TCustomPanel::Dispatch(&Message);
}
//---------------------------------------------------------------------------
void __fastcall TCustomTouchButtonControl::CMFontChanged(TMessage& Message)
{
	TCustomPanel::Dispatch(&Message);

	for (unsigned i=0; i<FButtons.size(); i++)
	{
		if (FButtons[i]->ParentFont)
		{
			FButtons[i]->Font							= Font;
			FButtons[i]->ParentFont					= true;
		}
		if (FButtons[i]->ParentFontColor)
		{
			FButtons[i]->FontColor					= Font->Color;
			FButtons[i]->ParentFontColor			= true;
		}
		if (FButtons[i]->ParentLatchedFont)
		{
			FButtons[i]->LatchedFont				= Font;
			FButtons[i]->ParentLatchedFont		= true;
		}
		if (FButtons[i]->ParentLatchedFontColor)
		{
			FButtons[i]->LatchedFontColor			= Font->Color;
			FButtons[i]->ParentLatchedFontColor	= true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TCustomTouchButtonControl::CMColorChanged(TMessage& Message)
{
	TCustomPanel::Dispatch(&Message);
}
//---------------------------------------------------------------------------
inline void __fastcall TCustomTouchButtonControl::SetFontShadow(bool Enabled)
{
	FFontShadow = Enabled;
	Invalidate();
}
//---------------------------------------------------------------------------
inline void __fastcall TCustomTouchButtonControl::SetDisabledFontShadow(bool Enabled)
{
	FDisabledFontShadow = Enabled;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TCustomTouchButtonControl::SetLatchingType(TLatchingType LatchingType)
{
	if (LatchingType == ltNone)
	{
		for (unsigned i=0; i<FButtons.size(); i++)
		{
			TGridButton *Button = FButtons[i];
			Button->Latched = false;
		}
	}
	FLatchingType = LatchingType;
}
//---------------------------------------------------------------------------
void __fastcall TCustomTouchButtonControl::MouseDown(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	TGridButton *GridButton	= GetButtonXY(X, Y);
	if (GridButton && ((GridButton->Enabled && GridButton->Visible) || DesignMode))
	{
		FCurrentClickedButton = GridButton;
		SetButtonDown(GridButton);
		Invalidate();

		if (FOnMouseDown)
		{
			TRect ButtonRect = GetButtonRect(GridButton);
			FOnMouseDown(this, Button, Shift, GridButton, X - ButtonRect.Left, Y - ButtonRect.Top);
		}
	}
	else
	{
		FCurrentClickedButton = NULL;
	}
/*
	TGridButton *GridButton = GetButtonXY(X, Y);
	if (GridButton && ((GridButton->Enabled && GridButton->Visible) || DesignMode))
	{
		FCurrentClickedButton = GridButton;
		if (FCurrentClickedButton)
		{
			if (FOnMouseDown)
			{
				TRect ButtonRect = GetButtonRect(GridButton);
				FOnMouseDown(this, Button, Shift, GridButton, X - ButtonRect.Left, Y - ButtonRect.Top);
			}
			if (GetCaptureControl() == this)
			{
				SetButtonDown(GridButton);
				Invalidate();

				FMouseInCurrentButton = true;
			}
			else
			{
				FMouseInCurrentButton = false;
				FCurrentClickedButton = NULL;
			}
		}
		else
		{
			FMouseInCurrentButton = false;
		}
	}
	else
	{
		FCurrentClickedButton = NULL;
	}
*/
}
//---------------------------------------------------------------------------
void TCustomTouchButtonControl::SetButtonDown(TGridButton *GridButton)
{
	GridButton->FIndicatorTimer->Interval	= FIndicatorDelay;
	GridButton->FDown								= true;
	GridButton->Highlighted						= true;
}
//---------------------------------------------------------------------------
void __fastcall TCustomTouchButtonControl::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	TGridButton *CurrentClickedButton = FCurrentClickedButton;
	FCurrentClickedButton = NULL;

	TGridButton *GridButton = GetButtonXY(X, Y);
	try
	{
		if (GridButton)
		{
			if (GridButton == CurrentClickedButton)
			{
				if (FLatchingType == ltCheckbox)
				{
					GridButton->Latched = !GridButton->Latched;
				}
				else if (FLatchingType == ltRadioButton)
				{
					GridButton->Latched = true;
				}
				else
				{
					GridButton->Latched = false;
				}
				SetButtonUp(GridButton);
				Invalidate();

				if (FOnMouseClick)
				{
					FOnMouseClick(this, Button, Shift, GridButton);
				}
			}
		}
	}
	__finally
	{
		if (FOnMouseUp && CurrentClickedButton)
		{
			TRect ButtonRect = GetButtonRect(CurrentClickedButton);
			FOnMouseUp(this, Button, Shift, CurrentClickedButton, X - ButtonRect.Left, Y - ButtonRect.Top);
		}
//		FCurrentClickedButton = NULL;
	}
//	FCurrentClickedButton = NULL;
/*	TGridButton *GridButton = FCurrentClickedButton;

	if (FCurrentClickedButton)
	{
//		if (FMouseInCurrentButton)
		{
			if (FLatchingType == ltCheckbox)
			{
				GridButton->Latched = !GridButton->Latched;
			}
			else if (FLatchingType == ltRadioButton)
			{
				GridButton->Latched = true;
			}
			else
			{
				GridButton->Latched = false;
			}
		}
		SetButtonUp(GridButton);
		Invalidate();
		FCurrentClickedButton = NULL;
//		if (FMouseInCurrentButton)
		{
			if (FOnMouseClick)
			{
				FOnMouseClick(this, Button, Shift, GridButton);
			}
		}
	}
	if (FOnMouseUp)
	{
		TRect ButtonRect = GetButtonRect(GridButton);
		FOnMouseUp(this, Button, Shift, GridButton, X - ButtonRect.Left, Y - ButtonRect.Top);
	} */
}
//---------------------------------------------------------------------------
void TCustomTouchButtonControl::SetButtonUp(TGridButton *GridButton)
{
	GridButton->FDown = false;
	if (GridButton->FIndicatorTimer->Enabled == false)
	{
		GridButton->FHighlighted = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCustomTouchButtonControl::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
	TGridButton *GridButton = GetButtonXY(X, Y);
	if (FCurrentClickedButton)
	{
		FCurrentClickedButton->FHighlighted = (GridButton == FCurrentClickedButton);
		Invalidate();
	}
	if (FOnMouseMove)
	{
		TRect ButtonRect = GetClientRect();
		if (GridButton)
		{
			ButtonRect = GetButtonRect(GridButton);
		}
		FOnMouseMove(this, Shift, GridButton, X - ButtonRect.Left, Y - ButtonRect.Top);
	}
/*
	bool HadMouse = (GetCaptureControl() == this);

	TGridButton *GridButton = GetButtonXY(X, Y);

	if (HadMouse && GetCaptureControl() != this)
	{
		FCurrentClickedButton = NULL;
	}
	if (FCurrentClickedButton)
	{
		bool IsCurrentButton = (GridButton == FCurrentClickedButton);

		if (FMouseInCurrentButton != IsCurrentButton)
		{
			FMouseInCurrentButton = IsCurrentButton;
			if (!FCurrentClickedButton->Latched)
			{
				FCurrentClickedButton->FHighlighted = FMouseInCurrentButton;
			}
			Invalidate();
		}
	}
	try
	{
		if (FOnMouseMove)
		{
			TRect ButtonRect = GetButtonRect(GridButton);
			FOnMouseMove(this, Shift, FCurrentClickedButton, X - ButtonRect.Left, Y - ButtonRect.Top);
		}
	}
	__finally
	{
		if (HadMouse && GetCaptureControl() != this)
		{
			// If mouse capture is lost, it's all over. No mouse up. Certainly don't want a click.
			// Would be nice if the up event could be retained if the mouse button wasn't released
			// (and potentially reapplied) during the course of lost mouse capture.
			FMouseInCurrentButton = false;
			if (FCurrentClickedButton)
			{
				SetButtonUp(FCurrentClickedButton);
				FCurrentClickedButton = NULL;
			}
			Invalidate();
		}
	}
*/
}
//---------------------------------------------------------------------------
void __fastcall TCustomTouchButtonControl::WMCancelMode(TWMCancelMode& Message)
{
	TCustomPanel::Dispatch(&Message);
//	FMouseInCurrentButton = false;
	if (FCurrentClickedButton)
	{
		SetButtonUp(FCurrentClickedButton);
		FCurrentClickedButton = NULL;
	}
	Invalidate();
}
//---------------------------------------------------------------------------
void TCustomTouchButtonControl::SetButtonHighlight(TGridButton *GridButton, bool Highlight)
{
	GridButton->FHighlighted = Highlight;
}
//---------------------------------------------------------------------------
void TCustomTouchButtonControl::PaintButton(TGridButton *Button, TRect Rect, TRect ClipRect,
		Graphics::TBitmap *Bitmap, TColor BackgroundColor)
{
	int ButtonWidth 	= Rect.Right - Rect.Left;
	int ButtonHeight	= Rect.Bottom - Rect.Top;
	if (ButtonHeight < 2 || ButtonWidth < 2)
	{
		return;
	}
	HCL Hcl = RGBToHCL(ColorToRGB(Button->Color));
	if (Button->Visible || DesignMode)
	{
		TColor ButtonColor;
		if (Button == FCurrentClickedButton && !Button->Latched)
//		if (Button == FCurrentClickedButton && !FMouseInCurrentButton && !Button->Latched)
		{
			ButtonColor = Button->Color;
		}
		else if (Button->Latched && FLatchingProperties.Contains(lkLatchColor))
		{
			ButtonColor = Button->LatchedColor;
			Hcl = RGBToHCL(ColorToRGB(Button->LatchedColor));
		}
		else if (Button->Enabled)
		{
			ButtonColor = Button->Color;
		}
		else
		{
			ButtonColor = Button->DisabledColor;
			Hcl = RGBToHCL(ColorToRGB(Button->DisabledColor));
		}
		if (Button->Latched && FLatchingProperties.Contains(lkStayDown))
		{
			PaintShadedButton(FButtonBuffer, ButtonWidth, ButtonHeight, ButtonColor, BackgroundColor, true);
		}
		else
		{
			PaintShadedButton(FButtonBuffer, ButtonWidth, ButtonHeight, ButtonColor, BackgroundColor, Button->Highlighted);
		}
	}
	int ClipLeft=0, ClipTop=0;
	TRect ButtonClipRect = Rect;

	if (Rect.Left < 0)
	{
		ClipLeft						= -Rect.Left;
		ButtonClipRect.Left		= 0;
	}
	if (Rect.Top < 0)
	{
		ClipTop						= -Rect.Top;
		ButtonClipRect.Top		= 0;
	}
	if (Rect.Right >= Bitmap->Width)
	{
		ButtonClipRect.Right		= Bitmap->Width;
	}
	if (Rect.Bottom >= Bitmap->Height)
	{
		ButtonClipRect.Bottom	= Bitmap->Height;
	}
	ButtonClipRect.Bottom	= min(ButtonClipRect.Bottom, ClipRect.Bottom);
	ButtonClipRect.Right		= min(ButtonClipRect.Right, ClipRect.Right);

	if (ClipRect.Top > ButtonClipRect.Top)
	{
		ClipTop += ClipRect.Top - ButtonClipRect.Top;
		ButtonClipRect.Top = ClipRect.Top;
	}
	if (ClipRect.Left > ButtonClipRect.Left)
	{
		ClipLeft += ClipRect.Left - ButtonClipRect.Left;
		ButtonClipRect.Left = ClipRect.Left;
	}

	if (Button->Visible || DesignMode)
	{
		DWORD *Buffer			= FButtonBuffer + (ClipTop * ButtonWidth) + ClipLeft;
		DWORD *ScanLine		= (DWORD *)Bitmap->ScanLine[ButtonClipRect.Top] + ButtonClipRect.Left;

		int ScanLineLength	= (char *)Bitmap->ScanLine[0] - (char *)Bitmap->ScanLine[1];
		int RowCount			= ButtonClipRect.Bottom - ButtonClipRect.Top;

		int Length				= ButtonClipRect.Right - ButtonClipRect.Left;
		int ButtonWidthPad	= (ButtonWidth - Length) << 2;

		if (Length > 0)	// Should always be otherwise earlier clipping failed
		{
			// Copy from Buffer to ScanLine, Length*Words, RowCount Lines.

			asm
			{
				push	ebx						// delphi seems to be quirky, let's save all registers that we are using
				push	edx						// i believe it is because when it "optimizes" it used the registers
				push	edi						// and doesn't count on the fact that we might use them...
				push	esi
				push	ecx

				mov	edi,ScanLine			// edi = Dest
				mov	ebx,edi					// save it
				mov	esi,Buffer				// esi = Source
				cld

				mov	edx,RowCount			// Outer loop - copy lines
@loop1:

				mov	ecx,Length				// Line length
				rep	movsd						// Copy the line

				sub   ebx,ScanLineLength	// Next ScanLine in the TBitmap
				mov	edi,ebx

				add	esi,ButtonWidthPad	// Skip clipped pixels

				dec	edx
				jne	@loop1

				pop  ecx
				pop  esi
				pop  edi
				pop  edx
				pop  ebx
			}
		}
		TRect CentredRect = Rect;
		InflateRect(&CentredRect, -4, -4);
		if (Button->Highlighted || (Button->Latched && FLatchingProperties.Contains(lkStayDown)))
		{
			OffsetRect(&CentredRect, 1, 1);
		}
		Graphics::TBitmap *Glyph = Button->FGlyph.get();
		if (Glyph && !Glyph->Empty && Glyph->Height > 0 && Glyph->Width > 0)
		{
			Graphics::TBitmap *StretchedGlyph = Button->FStretchedGlyph.get();

			int StretchedGlyphWidth		= CentredRect.Width();
			int StretchedGlyphHeight	= CentredRect.Height();
			if (Button->KeepGlyphAspect)
			{
				DWORD Aspect = Glyph->Width * 0xFFFF / Glyph->Height;

				StretchedGlyphWidth	= CentredRect.Width();
				StretchedGlyphHeight	= StretchedGlyphWidth * 0xFFFF / Aspect;
				if (StretchedGlyphHeight > CentredRect.Height())
				{
					StretchedGlyphHeight	= CentredRect.Height();
					StretchedGlyphWidth	= StretchedGlyphHeight * Aspect / 0xFFFF;
				}
			}
			StretchedGlyphWidth	= StretchedGlyphWidth * Button->FGlyphScale / 100;
			StretchedGlyphHeight	= StretchedGlyphHeight * Button->FGlyphScale / 100;
			// Centre it if aspect/scale adjustment requires it.
			int GlyphLeftOffset	= (CentredRect.Width() - StretchedGlyphWidth) / 2;
			int GlyphTopOffset	= (CentredRect.Height() - StretchedGlyphHeight) / 2;

			if (StretchedGlyph->Width != StretchedGlyphWidth || StretchedGlyph->Height != StretchedGlyphHeight || StretchedGlyph->Empty)
			{
				StretchedGlyph->HandleType		= bmDIB;
				StretchedGlyph->PixelFormat	= pf32bit;
				StretchedGlyph->Width			= StretchedGlyphWidth;
				StretchedGlyph->Height			= StretchedGlyphHeight;

				StretchedGlyph->Canvas->CopyMode = cmSrcCopy;
				StretchedGlyph->Canvas->StretchDraw(TRect(0, 0, StretchedGlyph->Width, StretchedGlyph->Height), Glyph);
			}
			TRect GlyphButtonRect =
				TRect(
					CentredRect.Left + GlyphLeftOffset,
					CentredRect.Top + GlyphTopOffset,
					CentredRect.Left + GlyphLeftOffset + StretchedGlyphWidth,
					CentredRect.Top + GlyphTopOffset + StretchedGlyphHeight);	// Rect to draw gylph relative to the button

			int GlyphTop				= min(max(0, ButtonClipRect.Top - GlyphButtonRect.Top), StretchedGlyph->Height-1);
			int DestTop					= max(GlyphButtonRect.Top, ButtonClipRect.Top);

			if (DestTop < Bitmap->Height && GlyphButtonRect.Left < Bitmap->Width && DestTop >= 0)
			{
				int GlyphBottom			= GlyphButtonRect.Height() - max(0, GlyphButtonRect.Bottom - ButtonClipRect.Bottom) - 1;
				int GlyphHeight			= GlyphBottom - GlyphTop;

				int BtnClipLeft			= GlyphButtonRect.Left;
				if (BtnClipLeft < ButtonClipRect.Left)
				{
					BtnClipLeft				= ButtonClipRect.Left;
				}
				int GlyphLeft				= BtnClipLeft - GlyphButtonRect.Left;
				if (GlyphLeft >= StretchedGlyph->Width)
				{
					GlyphLeft				= StretchedGlyph->Width-1;
				}
				if (GlyphLeft < 0)
				{
					GlyphLeft				= 0;
				}

				int BtnClipRight			= GlyphButtonRect.Right;
				if (BtnClipRight > ButtonClipRect.Right)
				{
					BtnClipRight			= ButtonClipRect.Right;
				}

				int BtnClipWidth			= BtnClipRight - BtnClipLeft;
				int GlyphRight				= BtnClipWidth + GlyphLeft;
				if (GlyphRight >= StretchedGlyph->Width)
				{
					GlyphRight				= StretchedGlyph->Width-1;
				}
				if (GlyphRight < 0)
				{
					GlyphRight				= 0;
				}

				BYTE *GlyphPtr				= (BYTE *)StretchedGlyph->ScanLine[GlyphTop];
				BYTE *DestPtr				= (BYTE *)Bitmap->ScanLine[DestTop] + (GlyphButtonRect.Left * 4);

				int DestScanLineLength	= (BYTE *)Bitmap->ScanLine[0] - (BYTE *)Bitmap->ScanLine[1];
				int GlyphScanLineLength	= (BYTE *)StretchedGlyph->ScanLine[0] - (BYTE *)StretchedGlyph->ScanLine[1];

				if (Button->GlyphAlpha != 255)
				{
					int dwWeight1				= Button->GlyphAlpha;
					int dwWeight2				= 255 - dwWeight1;

					if (Button->FTransparentGlyph)
					{
						DWORD TransColor = Button->FTransparentColor & 0xFFFFFF;
						for (int y=0; y<GlyphHeight; y++)
						{
							for (int x=GlyphLeft*4; x<GlyphRight*4; x+=4)
							{
								DWORD C = *((DWORD *)(GlyphPtr + x));
								if (C != TransColor)
								{
									DestPtr[x]   = (BYTE)((((DWORD)GlyphPtr[x] * dwWeight1) +
																			  ((DWORD)DestPtr[x] * dwWeight2)) >> 8);
									DestPtr[x+1] = (BYTE)((((DWORD)GlyphPtr[x+1] * dwWeight1) +
																			  ((DWORD)DestPtr[x+1] * dwWeight2)) >> 8);
									DestPtr[x+2]  = (BYTE)((((DWORD)GlyphPtr[x+2] * dwWeight1) +
																			  ((DWORD)DestPtr[x+2] * dwWeight2)) >> 8);
								}
							}
							// Move to next scan line.
							GlyphPtr	-= GlyphScanLineLength;
							DestPtr	-= DestScanLineLength;
						}
					}
					else
					{
						for (int y=0; y<GlyphHeight; y++)
						{
							for (int x=GlyphLeft*4; x<GlyphRight*4; x+=4)
							{
								DestPtr[x]   = (BYTE)((((DWORD)GlyphPtr[x] * dwWeight1) +
																		  ((DWORD)DestPtr[x] * dwWeight2)) >> 8);
								DestPtr[x+1] = (BYTE)((((DWORD)GlyphPtr[x+1] * dwWeight1) +
																		  ((DWORD)DestPtr[x+1] * dwWeight2)) >> 8);
								DestPtr[x+2]  = (BYTE)((((DWORD)GlyphPtr[x+2] * dwWeight1) +
																		  ((DWORD)DestPtr[x+2] * dwWeight2)) >> 8);
							}
							// Move to next scan line.
							GlyphPtr	-= GlyphScanLineLength;
							DestPtr	-= DestScanLineLength;
						}
					}
				}
				else
				{
					if (Button->FTransparentGlyph)
					{
						DWORD TransColor = Button->FTransparentColor & 0xFFFFFF;
						for (int y=0; y<GlyphHeight; y++)
						{
							for (int x=GlyphLeft*4; x<GlyphRight*4; x+=4)
							{
								DWORD C = *((DWORD *)(GlyphPtr + x));
								if (C != TransColor)
								{
									*((DWORD *)(DestPtr + x)) = *((DWORD *)(GlyphPtr + x));
								}
							}
							// Move to next scan line.
							GlyphPtr	-= GlyphScanLineLength;
							DestPtr	-= DestScanLineLength;
						}
					}
					else
					{
						for (int y=0; y<GlyphHeight; y++)
						{
							for (int x=GlyphLeft*4; x<GlyphRight*4; x+=4)
							{
								*((DWORD *)(DestPtr + x)) = *((DWORD *)(GlyphPtr + x));
							}
							// Move to next scan line.
							GlyphPtr	-= GlyphScanLineLength;
							DestPtr	-= DestScanLineLength;
						}
					}
				}
			}
		}
//		InflateRect(&CentredRect, -4, -4);
		InflateRect(&CentredRect, -2, -1);

		Bitmap->Canvas->Brush->Style = bsClear;
		if (Button->Latched)
		{
			Bitmap->Canvas->Font = Button->LatchedFont;
		}
		else
		{
			Bitmap->Canvas->Font = Button->Font;
		}
		int TextHeight = DrawTextExW(Bitmap->Canvas->Handle,
											 Button->Caption.c_str(),
											 Button->Caption.Length(),
											 &TRect(0, 0, CentredRect.right - CentredRect.left, CentredRect.bottom - CentredRect.top),
											 DT_CALCRECT|DT_WORDBREAK|DT_CENTER|DT_NOPREFIX,
											 NULL);

		int RowHeight = CentredRect.Bottom - CentredRect.Top;
		if (RowHeight > TextHeight)
		{
			CentredRect.Top += (RowHeight - TextHeight) / 2;
			CentredRect.Bottom = CentredRect.Top + TextHeight;
		}
		TColor CLHighlight, CLShadow;
		TRect TopRect		= CentredRect;
		TRect BottomRect	= CentredRect;

		OffsetRect(&BottomRect, 1, 1);
		if (Button->Enabled)
		{
			HCL HclO = Hcl;
			if (Hcl.l > 128)
			{
				HclO.l = max(Hcl.l * 2 / 3, 0);
			}
			else
			{
				HclO.l = min(Hcl.l * 3 / 2, 255);
			}
			CLHighlight	= TColor(HLCToRGB(HclO));
			if (Button->Latched)
			{
				CLShadow	= TColor(ColorToRGB(Button->LatchedFontColor));
			}
			else
			{
				CLShadow	= TColor(ColorToRGB(Button->FontColor));
			}
		}
		else
		{
			HCL HclO		= Hcl;
			HclO.l		= Hcl.l + ((255 - Hcl.l) / 2);
			HclO.l		= min(HclO.l, 255);
			CLHighlight	= TColor(HLCToRGB(HclO));
			HclO.l		= Hcl.l * 2 / 3;
			HclO.l		= max(HclO.l, 0);
			CLShadow		= TColor(HLCToRGB(HclO));
		}
		if ((Button->Enabled && FFontShadow) || (!Button->Enabled && FDisabledFontShadow))
		{
			Bitmap->Canvas->Font->Color = CLHighlight;
			DrawTextExW(	Bitmap->Canvas->Handle,
							Button->Caption.c_str(),
							Button->Caption.Length(),
							&BottomRect,
							DT_WORDBREAK|DT_CENTER|DT_NOPREFIX, NULL);
		}
		Bitmap->Canvas->Font->Color = CLShadow;

		DrawTextExW(	Bitmap->Canvas->Handle,
						Button->Caption.c_str(),
						Button->Caption.Length(),
						&TopRect,
						DT_WORDBREAK|DT_CENTER|DT_NOPREFIX, NULL);
						
		if (Button->Latched && FLatchingProperties.Contains(lkIndicator))
		{
			HCL HclO		= Hcl;
			HclO.l		= Hcl.l + ((255 - Hcl.l) * 3 / 4);
			HclO.l		= min(HclO.l, 255);

			Bitmap->Canvas->Pen->Color = clBlack;
			Bitmap->Canvas->Brush->Color = TColor(HLCToRGB(HclO));
			Bitmap->Canvas->Brush->Style = bsSolid;
			if (Button->Highlighted || (Button->Latched && FLatchingProperties.Contains(lkStayDown)))
			{
				Bitmap->Canvas->Ellipse(TRect(Rect.Right-14, Rect.Bottom-14, Rect.Right-5, Rect.Bottom-5));
			}
			else
			{
				Bitmap->Canvas->Ellipse(TRect(Rect.Right-15, Rect.Bottom-15, Rect.Right-6, Rect.Bottom-6));
			}
		}
	}
//	else
//	{
//		Bitmap->Canvas->Brush->Color = Color;
//		Bitmap->Canvas->FillRect(ButtonClipRect);
//	}
}
//---------------------------------------------------------------------------
void TCustomTouchButtonControl::PaintStandardButton(TGridButton *Button, TRect Rect, Graphics::TBitmap *Buffer)
{
	if (Button->Visible || DesignMode)
	{
//		DrawEdge(Buffer->Canvas->Handle, &Rect, EDGE_RAISED, BF_RECT + BF_ADJUST);
		TColor BRO, BRI, TLO, TLI, BC;
		if (Button->Highlighted)
		{
//			DrawEdge(Buffer->Canvas->Handle, &Rect, EDGE_SUNKEN, BF_RECT + BF_ADJUST + BF_MONO);
			BYTE R	= GetRValue(ColorToRGB(Button->Color));
			BYTE G	= GetGValue(ColorToRGB(Button->Color));
			BYTE B	= GetBValue(ColorToRGB(Button->Color));
			BC			= TColor(RGB(R, G, B));
			HCL hcl	= RGBToHCL(BC);
			hcl.l		= hcl.l * 110 / 100;
			if (hcl.l > RGBMAX) hcl.l = RGBMAX;
			BC			= TColor(HLCToRGB(hcl));


			TLO		= clBlack;
			hcl.l		= hcl.l * 110 / 100;
			if (hcl.l > RGBMAX) hcl.l = RGBMAX;
			TLI		= TColor(HLCToRGB(hcl));
//			TLI = TColor(RGB(	R >> 1,
//									G >> 1,
//									B >> 1));

			BRO = clWhite;
			hcl.l		= hcl.l * 100 / 120;
			if (hcl.l < 0) hcl.l = 0;
			BRI		= TColor(HLCToRGB(hcl));
//			BRI = TColor(RGB(	255 - ((255 - R) >> 1),
//									255 - ((255 - G) >> 1),
//									255 - ((255 - B) >> 1)));
		}
		else
		{
			BC			= Button->Color;
			HCL hcl	= RGBToHCL(BC);

//			BYTE R = GetRValue(ColorToRGB(BC));
//			BYTE G = GetGValue(ColorToRGB(BC));
//			BYTE B = GetBValue(ColorToRGB(BC));


			BRO = clBlack;
			hcl.l		= hcl.l * 100 / 110;
			if (hcl.l > RGBMAX) hcl.l = RGBMAX;
			BRI		= TColor(HLCToRGB(hcl));
//			BRI = TColor(RGB(	R >> 1,
//									G >> 1,
//									B >> 1));

			TLO = clWhite;
			hcl.l		= hcl.l * 120 / 100;
			if (hcl.l < 0) hcl.l = 0;
			TLI		= TColor(HLCToRGB(hcl));
//			TLI = TColor(RGB(	255 - ((255 - R) >> 1),
//									255 - ((255 - G) >> 1),
//									255 - ((255 - B) >> 1)));
		}

		Buffer->Canvas->Pen->Color = TLO;
		Buffer->Canvas->MoveTo(Rect.Right-2, Rect.Top);
		Buffer->Canvas->LineTo(Rect.Left, Rect.Top);
		Buffer->Canvas->LineTo(Rect.Left, Rect.Bottom-1);

		Buffer->Canvas->MoveTo(Rect.Right-3, Rect.Top+1);
		Buffer->Canvas->LineTo(Rect.Left+1, Rect.Top+1);
		Buffer->Canvas->LineTo(Rect.Left+1, Rect.Bottom-2);

		Buffer->Canvas->Pen->Color = TLI;
		Buffer->Canvas->MoveTo(Rect.Right-4, Rect.Top+2);
		Buffer->Canvas->LineTo(Rect.Left+2, Rect.Top+2);
		Buffer->Canvas->LineTo(Rect.Left+2, Rect.Bottom-3);

		Buffer->Canvas->MoveTo(Rect.Right-5, Rect.Top+3);
		Buffer->Canvas->LineTo(Rect.Left+3, Rect.Top+3);
		Buffer->Canvas->LineTo(Rect.Left+3, Rect.Bottom-4);

		Buffer->Canvas->Pen->Color = BRO;
		Buffer->Canvas->MoveTo(Rect.Right-1, Rect.Top);
		Buffer->Canvas->LineTo(Rect.Right-1, Rect.Bottom-1);
		Buffer->Canvas->LineTo(Rect.Left-1, Rect.Bottom-1);

		Buffer->Canvas->MoveTo(Rect.Right-2, Rect.Top+1);
		Buffer->Canvas->LineTo(Rect.Right-2, Rect.Bottom-2);
		Buffer->Canvas->LineTo(Rect.Left, Rect.Bottom-2);

		Buffer->Canvas->Pen->Color = BRI;
		Buffer->Canvas->MoveTo(Rect.Right-3, Rect.Top+2);
		Buffer->Canvas->LineTo(Rect.Right-3, Rect.Bottom-3);
		Buffer->Canvas->LineTo(Rect.Left+1, Rect.Bottom-3);

		Buffer->Canvas->MoveTo(Rect.Right-4, Rect.Top+3);
		Buffer->Canvas->LineTo(Rect.Right-4, Rect.Bottom-4);
		Buffer->Canvas->LineTo(Rect.Left+2, Rect.Bottom-4);

		InflateRect(&Rect, -4, -4);
/*		if (Button->Down)
		{
			InflateRect(&Rect, -4, -4);
			DrawEdge(Buffer->Canvas->Handle, &Rect, EDGE_RAISED, BF_RECT + BF_ADJUST);
			InflateRect(&Rect, -4, -4);
		}*/
		Buffer->Canvas->Brush->Color = BC;
		Buffer->Canvas->FillRect(Rect);

		Buffer->Canvas->Font				= Button->Font;
		Buffer->Canvas->Brush->Style	= bsClear;
		Buffer->Canvas->Pen->Color		= clBlack;

		TRect CentredRect					= Rect;
		if (Button->Highlighted)
		{
			CentredRect.Top+=2;
			CentredRect.Left+=2;
		}
		int TextHeight = DrawTextExW(Buffer->Canvas->Handle,
											 Button->Caption.c_str(),
											 Button->Caption.Length(),
											 &TRect(0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top),
											 DT_CALCRECT|DT_WORDBREAK|DT_CENTER|DT_NOPREFIX,
											 NULL);

		int RowHeight = Rect.Bottom - Rect.Top;
		if (RowHeight > TextHeight)
		{
			CentredRect.Top += (RowHeight - TextHeight) / 2;
			CentredRect.Bottom = CentredRect.Top + TextHeight;
		}
		DrawTextExW(Buffer->Canvas->Handle, Button->Caption.c_str(), Button->Caption.Length(), &CentredRect, DT_WORDBREAK|DT_CENTER|DT_NOPREFIX, NULL);
	}
}
//---------------------------------------------------------------------------
void TCustomTouchButtonControl::PaintShadedButton(DWORD *Buffer, int ButtonWidth, int ButtonHeight,
		TColor ButtonColor, TColor BackgroundColor, bool ButtonHighlighted)
{
	if (ButtonHeight <= 8 || ButtonWidth <= 8)
	{
		return;
	}
	if (ButtonHighlighted)
	{
		DWORD Colours[5];

		COLORREF Background	= ColorToRGB(BackgroundColor);//Color);
		BYTE r					= GetRValue(Background);
		BYTE g					= GetGValue(Background);
		BYTE b					= GetBValue(Background);
		Background				= BGR(b, g, r);

		COLORREF c				= ColorToRGB(ButtonColor);
		r							= GetRValue(c);
		g							= GetGValue(c);
		b							= GetBValue(c);
		Colours[4]				= BGR(b, g, r);

		HCL Hcl		= RGBToHCL(ColorToRGB(ButtonColor));
		HCL NewHcl	= Hcl;

		int Adjust = (Hcl.l << 8) / 6;
		int Lum = Hcl.l << 8;
		for (int i=0; i<4; i++)
		{
			Lum				-= Adjust;
			NewHcl.l			= Lum >> 8;
			COLORREF c		= HLCToRGB(NewHcl);
			BYTE r			= GetRValue(c);
			BYTE g			= GetGValue(c);
			BYTE b			= GetBValue(c);
			Colours[3-i]	= BGR(b, g, r);
		}
		asm
		{
			push	ebx                // delphi seems to be quirky, let's save all registers that we are using
			push	edx                // i believe it is because when it "optimizes" it used the registers
			push	edi                // and doesn't count on the fact that we might use them...
			push	esi
			push	ecx

			mov	edi, Buffer
			cld

			mov	eax,Background
			stosd

			mov	eax,Colours[16]	// Colours[4]
			mov	ecx,ButtonWidth	// for (int i=4; i<ButtonWidth-4; i++)
			sub	ecx,2
			rep	stosd

			mov	eax,Background
			stosd
	////////////////////////
			mov	eax,Colours[16]		// Colours[2]
			stosd
			stosd

			mov	eax,Colours[12]	// Colours[3]
			mov	ecx,ButtonWidth	// for (int i=2; i<ButtonWidth-2; i++)
			sub	ecx,4
			rep	stosd

			mov	eax,Colours[16]	// Colours[4]
			stosd
			stosd
	////////////////////////
			mov	eax,Colours[16]		// Colours[3]
			stosd
			mov	eax,Colours[12]		// Colours[3]
			stosd
			stosd

			mov	eax,Colours[8]		// Colours[2]
			mov	ecx,ButtonWidth	// for (int i=3; i<ButtonWidth-3; i++)
			sub	ecx,6
			rep	stosd

			mov	eax,Colours[12]		// Colours[2]
			stosd
			stosd
			mov	eax,Colours[16]		// Colours[2]
			stosd
	////////////////////////
			mov	eax,Colours[16]	// Colours[3]
			stosd
			mov	eax,Colours[12]	// Colours[3]
			stosd
			mov	eax,Colours[8]		// Colours[2]
			stosd

			mov	eax,Colours[4]		// Colours[1]
			mov	ecx,ButtonWidth	// for (int i=3; i<ButtonWidth-3; i++)
			sub	ecx,6
			rep	stosd

			mov	eax,Colours[8]		// Colours[2]
			stosd
			mov	eax,Colours[12]	// Colours[2]
			stosd
			mov	eax,Colours[16]	// Colours[3]
			stosd
////////////////////////
			mov	ebx,ButtonHeight	// for (int i=4; i<ButtonHeight-4; i++)
			sub	ebx,8
@loop4:
			mov	eax,Colours[16]	// Colours[4]
			stosd
			mov	eax,Colours[12]	// Colours[3]
			stosd
			mov	eax,Colours[8]		// Colours[2]
			stosd
			mov	eax,Colours[4]		// Colours[1]
			stosd

			mov	eax,Colours[16]	// Colours[4]
			mov	ecx,ButtonWidth	// for (int i=4; i<ButtonWidth-4; i++)
			sub	ecx,8
			rep	stosd

			mov	eax,Colours[4]		// Colours[1]
			stosd
			mov	eax,Colours[8]		// Colours[2]
			stosd
			mov	eax,Colours[12]	// Colours[3]
			stosd
			mov	eax,Colours[16]	// Colours[4]
			stosd

			dec	ebx
jne @loop4
////////////////////////
			mov	eax,Colours[16]	// Colours[3]
			stosd
			mov	eax,Colours[12]	// Colours[2]
			stosd
			mov	eax,Colours[8]		// Colours[2]
			stosd

			mov	eax,Colours[4]		// Colours[1]
			mov	ecx,ButtonWidth	// for (int i=3; i<ButtonWidth-3; i++)
			sub	ecx,6
			rep	stosd

			mov	eax,Colours[8]		// Colours[2]
			stosd
			mov	eax,Colours[12]	// Colours[2]
			stosd
			mov	eax,Colours[16]	// Colours[2]
			stosd
////////////////////////
			mov	eax,Colours[16]		// Colours[3]
			stosd
			mov	eax,Colours[12]		// Colours[3]
			stosd
			stosd

			mov	eax,Colours[8]		// Colours[2]
			mov	ecx,ButtonWidth	// for (int i=3; i<ButtonWidth-3; i++)
			sub	ecx,6
			rep	stosd

			mov	eax,Colours[12]		// Colours[2]
			stosd
			stosd
			mov	eax,Colours[16]		// Colours[2]
			stosd
	////////////////////////
			mov	eax,Colours[16]		// Colours[2]
			stosd
			stosd

			mov	eax,Colours[12]	// Colours[3]
			mov	ecx,ButtonWidth	// for (int i=2; i<ButtonWidth-2; i++)
			sub	ecx,4
			rep	stosd

			mov	eax,Colours[16]	// Colours[4]
			stosd
			stosd
	////////////////////////
			mov	eax,Background
			stosd

			mov	eax,Colours[16]	// Colours[4]
			mov	ecx,ButtonWidth	// for (int i=4; i<ButtonWidth-4; i++)
			sub	ecx,2
			rep	stosd

			mov	eax,Background
			stosd
	////////////////////////
			pop  ecx
			pop  esi
			pop  edi
			pop  edx
			pop  ebx
		}
	}
	else
	{
		DWORD Colours[9];

		COLORREF Background	= ColorToRGB(BackgroundColor);//Color);
		BYTE r					= GetRValue(Background);
		BYTE g					= GetGValue(Background);
		BYTE b					= GetBValue(Background);
		Background				= BGR(b, g, r);

		COLORREF c				= ColorToRGB(ButtonColor);
		r							= GetRValue(c);
		g							= GetGValue(c);
		b							= GetBValue(c);
		Colours[4]				= BGR(b, g, r);

		HCL Hcl		= RGBToHCL(ColorToRGB(ButtonColor));
		HCL NewHcl	= Hcl;

		int Adjust = (Hcl.l << 8) / 6;
		int Lum = Hcl.l << 8;
		for (int i=0; i<4; i++)
		{
			Lum				-= Adjust;
			NewHcl.l			= Lum >> 8;
			COLORREF c		= HLCToRGB(NewHcl);
			BYTE r			= GetRValue(c);
			BYTE g			= GetGValue(c);
			BYTE b			= GetBValue(c);
			Colours[3-i]	= BGR(b, g, r);
		}
		Adjust = ((255 - Hcl.l) << 8) / 4;
		Lum = Hcl.l << 8;
		for (int i=0; i<4; i++)
		{
			Lum				+= Adjust;
			NewHcl.l			= Lum >> 8;
			COLORREF c		= HLCToRGB(NewHcl);
			BYTE r			= GetRValue(c);
			BYTE g			= GetGValue(c);
			BYTE b			= GetBValue(c);
			Colours[i+5]	= BGR(b, g, r);
		}
		asm
		{
			push	ebx                // delphi seems to be quirky, let's save all registers that we are using
			push	edx                // i believe it is because when it "optimizes" it used the registers
			push	edi                // and doesn't count on the fact that we might use them...
			push	esi
			push	ecx

			mov	edi, Buffer
			cld

			mov	eax,Background
			stosd
			mov	eax,Colours[20]	// Colours[5]
			stosd                   // Scanline[1] = Colours[5];

			mov	eax,Colours[16]	// Colours[4]

			mov	ecx,ButtonWidth	// for (int i=2; i<ButtonWidth-3; i++)
			sub	ecx,5
			rep	stosd

			mov	eax,Background
			stosd
			stosd
			stosd
	////////////////////////
			mov	eax,Colours[20]
			stosd                   // Scanline[0] = Colours[5];
			mov	eax,Colours[16]
			stosd                   // Scanline[1] = Colours[4];
			mov	eax,Colours[28]
			stosd                   // Scanline[2] = Colours[7];
			mov	eax,Colours[24]
			stosd                   // Scanline[3] = Colours[6];

			mov	eax,Colours[20]	// Colours[5];

			mov	ecx,ButtonWidth	// for (int i=4; i<ButtonWidth-4; i++)
			sub	ecx,8
			rep	stosd

			mov	eax,Colours[16]	// Scanline[ButtonWidth-4] = Colours[4];
			stosd
			mov	eax,Colours[12]	// Scanline[ButtonWidth-3] = Colours[3];
			stosd
			mov	eax,Background
			stosd
			stosd
	////////////////////////////////
			mov	eax,Colours[16]
			stosd                   // Scanline[0] = Colours[4];
			mov	eax,Colours[28]
			stosd                   // Scanline[1] = Colours[7];
			mov	eax,Colours[32]
			stosd                   // Scanline[2] = Colours[8];
			mov	eax,Colours[28]
			stosd                   // Scanline[3] = Colours[7];

			mov	ecx,ButtonWidth	// for (int i=4; i<ButtonWidth-5; i++)
			sub	ecx,9
			rep	stosd

			mov	eax,Colours[24]	// Scanline[ButtonWidth-5] = Colours[6];
			stosd
			mov	eax,Colours[16]	// Scanline[ButtonWidth-4] = Colours[4];
			stosd
			mov	eax,Colours[12]	// Scanline[ButtonWidth-3] = Colours[3];
			stosd
			mov	eax,Colours[8]		// Scanline[ButtonWidth-2] = Colours[2];
			stosd
			mov	eax,Background
			stosd
	////////////////////////////
			mov	eax,Colours[16]	// Scanline[0] = Colours[4];
			stosd
			mov	eax,Colours[24]	// Scanline[1] = Colours[6];
			stosd
			mov	eax,Colours[28]	// Scanline[2] = Colours[7];
			stosd
			mov	eax,Colours[24]	// Scanline[3] = Colours[6];
			stosd

			mov	ecx,ButtonWidth	// for (int i=4; i<ButtonWidth-4; i++)
			sub	ecx,8
			rep	stosd

			mov	eax,Colours[12]	// Scanline[ButtonWidth-4] = Colours[3];
			stosd
			mov	eax,Colours[8]		// Scanline[ButtonWidth-3] = Colours[2];
			stosd
			mov	eax,Colours[4]		// Scanline[ButtonWidth-2] = Colours[1];
			stosd
			mov	eax,Background
			stosd
	///////////////////////////////////////
			mov	ebx,ButtonHeight	// for (int i=4; i<ButtonHeight-4; i++)
			sub	ebx,8
@loop4a:
			mov	eax,Colours[16]	// Scanline[0]	= Colours[4];
			stosd
			mov	eax,Colours[20]	// Scanline[1]	= Colours[5];
			stosd
			mov	eax,Colours[24]	// Scanline[2]	= Colours[6];
			stosd
			mov	eax,Colours[20]	// Scanline[3]	= Colours[5];
			stosd

			mov	eax,Colours[16]	// Colours[4];
			mov	ecx,ButtonWidth	// for (int i=4; i<ButtonWidth-4; i++)
			sub	ecx,8
			rep	stosd

			mov	eax,Colours[12]	// Scanline[ButtonWidth-4]	= Colours[3];
			stosd
			mov	eax,Colours[8]		// Scanline[ButtonWidth-3]	= Colours[2];
			stosd
			mov	eax,Colours[4]		// Scanline[ButtonWidth-2]	= Colours[1];
			stosd
			mov	eax,Colours[0]		// Scanline[ButtonWidth-1]	= Colours[0];
			stosd

			dec	ebx
jne @loop4a

			mov	eax,Colours[16]
			stosd                   // Scanline[0]	= Colours[4];
			mov	eax,Colours[20]
			stosd                   // Scanline[1]	= Colours[5];

			mov	eax,Colours[12]	// Colours[3];
			mov	ecx,ButtonWidth	// for (int i=2; i<ButtonWidth-3; i++)
			sub	ecx,5
			rep	stosd

			mov	eax,Colours[8]
			stosd                   // Scanline[ButtonWidth-3]	= Colours[2];
			mov	eax,Colours[4]
			stosd                   // Scanline[ButtonWidth-2]	= Colours[1];
			stosd                   // Scanline[ButtonWidth-1]	= Colours[1];

	///////////////////////////////////
			mov	eax,Background
			stosd
			mov	eax,Colours[20]	// Scanline[1] = Colours[5];
			stosd

			mov	eax,Colours[8]	// Colours[2];
			mov	ecx,ButtonWidth	// for (int i=2; i<ButtonWidth-3; i++)
			sub	ecx,5
			rep	stosd

			mov	eax,Colours[4]
			stosd                   // Scanline[ButtonWidth-3]	= Colours[1];
			stosd                   // Scanline[ButtonWidth-2]	= Colours[1];
			stosd                   // Scanline[ButtonWidth-1]	= Colours[1];
	///////////////////////////////////
			mov	eax,Background
			stosd
			stosd

			mov	eax,Colours[4]		// Colours[1];
			mov	ecx,ButtonWidth	// for (int i=2; i<ButtonWidth-3; i++)
			sub	ecx,5
			rep	stosd

			mov	eax,Colours[4]		// Scanline[ButtonWidth-3]	= Colours[1];
			stosd
			mov	eax,Colours[0]		// Scanline[ButtonWidth-2]	= Colours[0];
			stosd
			mov	eax,Colours[8]//12]	// Scanline[ButtonWidth-1]	= Colours[3];
			stosd
	///////////////////////////////////
			mov	eax,Background
			stosd
			stosd
			stosd

			mov	eax,Colours[0]		// Colours[0];
			mov	ecx,ButtonWidth	// for (int i=3; i<ButtonWidth-4; i++)
			sub	ecx,7
			rep	stosd

			mov	eax,Colours[4]		// Scanline[ButtonWidth-4]	= Colours[1];
			stosd
			stosd
			mov	eax,Colours[8]//12]	// Scanline[ButtonWidth-2]	= Colours[3];
			stosd
			mov	eax,Background
			stosd

			pop  ecx
			pop  esi
			pop  edi
			pop  edx
			pop  ebx
		}
	}
}
//---------------------------------------------------------------------------
HCL TCustomTouchButtonControl::RGBToHCL(int col)
{
	int r, g, b, cmin, cmax;
	int ch, dr, dg, db;
	HCL hcl;

	r = GetRValue(col);
	g = GetGValue(col);
	b = GetBValue(col);
	cmin = min(min(r, g), b);
	cmax = max(max(r, g), b);
	hcl.c = cmax-cmin; // chromma // global_ret_2
	hcl.l = (cmax+cmin)/2; // luminance // global_ret_3

	// acromatic case
	if (hcl.c == 0)
	{
		hcl.h = 0; // or some undefined value
		return hcl;
	}

	ch = hcl.c/2;
	if (cmax == r)
	{
		dg = ((cmax-g)*HPE1 + ch)/hcl.c;
		db = ((cmax-b)*HPE1 + ch)/hcl.c;
		hcl.h = HPE1+db-dg;
	}
	else if (cmax == g)
	{
		dr = ((cmax-r)*HPE1 + ch)/hcl.c;
		db = ((cmax-b)*HPE1 + ch)/hcl.c;
		hcl.h = HPE3 + dr-db;
	}
	else
	{
		dr = ((cmax-r)*HPE1 + ch)/hcl.c;
		dg = ((cmax-g)*HPE1 + ch)/hcl.c;
		hcl.h = HPE5 + dg-dr;
	}

	if (hcl.h < 0) hcl.h += HUENUM;
	else if (hcl.h > HUEMAX) hcl.h -= HUENUM;
	return hcl;
}
//---------------------------------------------------------------------------
//
// Given h/c/l components, this function puts togeather the color in rgb
//
//---------------------------------------------------------------------------
int TCustomTouchButtonControl::HLCToRGB(HCL hcl)
{
	int r, g, b, cmax, cmin;
	int dl;
	//
	// Do a little checking, make sure we clamp cromma if outside (for given luminance).
	// Hue and Luminance wont be affected (which is where most
	// of the visual info is)
	//
	if (hcl.l < 128)
	{
		dl = 2*hcl.l;
		if (hcl.c > dl)
		{
			cmax = dl;
			cmin = 0;
			hcl.c = dl;
		}
		else
		{
			cmax = hcl.l+hcl.c/2;
			cmin = cmax-hcl.c;
		}
	}
	else
	{
		dl = 2*(255-hcl.l);
		if (hcl.c > dl)
		{
			cmax = 255;
			cmin = 255-dl;
			hcl.c = dl;
		}
		else
		{
			cmax = hcl.l+hcl.c/2;
			cmin = cmax-hcl.c;
		}
	}
	//
	// Now calculate the rgb components
	//
	if (hcl.h < HPE2)
	{
		r = cmax;
		if (hcl.h < HPE1)
		{
			g = cmin;
			b = cmin + (hcl.c*(HPE1-hcl.h)/HPE1);
		}
		else
		{
			b = cmin;
			g = cmin + (hcl.c*(hcl.h-HPE1)/HPE1);
		}
	}
	else if (hcl.h < HPE4)
	{
		g = cmax;
		if (hcl.h < HPE3)
		{
			b = cmin;
			r = cmin + (hcl.c*(HPE3-hcl.h)/HPE1);
		}
		else
		{
			r = cmin;
			b = cmin + (hcl.c*(hcl.h-HPE3)/HPE1);
		}
	}
	else
	{
		b = cmax;
		if (hcl.h < HPE5)
		{
			r = cmin;
			g = cmin + (hcl.c*(HPE5-hcl.h)/HPE1);
		}
		else
		{
			g = cmin;
			r = cmin + (hcl.c*(hcl.h-HPE5)/HPE1);
		}
	}
	return RGB(r, g, b);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TGridButton::TGridButton(TCustomTouchButtonControl* Owner) : //TCollectionItem(),//TComponent(NULL),
		FFont(new TFont),
		FLatchedFont(new TFont),
		FIndicatorTimer(new TTimer(NULL)),
		FGlyph(new Graphics::TBitmap),
		FStretchedGlyph(new Graphics::TBitmap)
{
	FOwner								= Owner;

	FFont->OnChange					= FOnFontChange;
	FLatchedFont->OnChange			= FOnFontChange;
	FVisible								= true;
	FEnabled								= true;
	FParentFont							= true;
	FParentFontColor					= true;
	FParentLatchedFont				= true;
	FParentLatchedFontColor			= true;

	FIndicatorTimer->Enabled		= false;
	if (FOwner)
	{
		FIndicatorTimer->Interval	= FOwner->IndicatorDelay;
		FColor							= FOwner->DefaultButtonColor;
		FDisabledColor					= FOwner->DefaultButtonColor;
		FLatchedColor					= FOwner->DefaultButtonLatchedColor;
		FFontColor						= FOwner->Font->Color;
		FLatchedFontColor				= FOwner->Font->Color;

		FFont->Assign(FOwner->Font);
		FLatchedFont->Assign(FOwner->Font);
	}
	else
	{
		FIndicatorTimer->Interval	= 300;
		FColor							= clBlack;
		FDisabledColor					= clBlack;
		FLatchedColor					= clBlack;
		FFontColor						= clBlack;
		FLatchedFontColor				= clBlack;
	}
	FIndicatorTimer->OnTimer		= IndicatorTimeout;
	Caption								= "";
	FGlyphAlpha							= 255;
	FGlyphScale							= 100;
	FKeepGlyphAspect					= true;
	FTransparentGlyph					= true;
	FTransparentColor					= clBlack;
	Tag									= 0;
}
//---------------------------------------------------------------------------
Graphics::TFont* __fastcall TGridButton::GetFont()
{
	return FFont.get();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetFont(Graphics::TFont* Value)
{
	FFont->Assign(Value);
	ParentFont = false;
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetFontColor(TColor Value)
{
	if (FFontColor != Value)
	{
		FFontColor			= Value;
		FParentFontColor	= false;
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetParentFont(bool Value)
{
	if (FParentFont != Value)
	{
		FParentFont = Value;
		if (FParentFont)
		{
			FFont->Assign(FOwner->Font);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetParentFontColor(bool Value)
{
	if (FParentFontColor != Value)
	{
		FParentFontColor = Value;
		if (FParentFontColor)
		{
			FFontColor = FOwner->Font->Color;
		}
		Invalidate();
	}
}
//---------------------------------------------------------------------------
Graphics::TFont* __fastcall TGridButton::GetLatchedFont()
{
	return FLatchedFont.get();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetLatchedFont(Graphics::TFont* Value)
{
	FLatchedFont->Assign(Value);
	ParentLatchedFont = false;
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetLatchedFontColor(TColor Value)
{
	if (FLatchedFontColor != Value)
	{
		FLatchedFontColor			= Value;
		FParentLatchedFontColor	= false;
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetParentLatchedFont(bool Value)
{
	if (FParentLatchedFont != Value)
	{
		FParentLatchedFont = Value;
		if (FParentLatchedFont)
		{
			FLatchedFont->Assign(FOwner->Font);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetParentLatchedFontColor(bool Value)
{
	if (FParentLatchedFontColor != Value)
	{
		FParentLatchedFontColor = Value;
		if (FParentLatchedFontColor)
		{
			FLatchedFontColor = FOwner->Font->Color;
		}
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::FOnFontChange(TObject* Sender)
{
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetColor(TColor Color)
{
	if (FColor != Color)
	{
		FColor = Color;
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetLatchedColor(TColor Color)
{
	if (FLatchedColor != Color)
	{
		FLatchedColor = Color;
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetLatched(bool Latched)
{
	if (FOwner->LatchingType == ltRadioButton)
	{
		if (Latched)
		{
			for (unsigned i=0; i<FOwner->FButtons.size(); i++)
			{
				TGridButton *Button = FOwner->FButtons[i];
				if (Button->Latched && this != Button)
				{
					Button->FLatched = false;
				}
			}
		}
	}
	FLatched = Latched;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetDisabledColor(TColor Color)
{
	if (FDisabledColor != Color)
	{
		FDisabledColor = Color;
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetEnabled(bool Enabled)
{
	FEnabled = Enabled;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetVisible(bool Visible)
{
	FVisible = Visible;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetCaption(UnicodeString Caption)
{
	FCaption = Caption;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::IndicatorTimeout(TObject *Sender)
{
	FIndicatorTimer->Enabled = false;
	if (!FDown)
	{
		FHighlighted = false;
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetHighlighted(bool Highlighted)
{
	FHighlighted					= Highlighted;
	FIndicatorTimer->Enabled	= false;
	FIndicatorTimer->Enabled	= true;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGridButton::GetGlyph()
{
	return FGlyph.get();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetGlyph(Graphics::TBitmap *Value)
{
	FGlyph->Assign(Value);

	FGlyph->HandleType	= bmDIB;
	FGlyph->PixelFormat	= pf32bit;

	FStretchedGlyph->Assign(NULL);
	FStretchedGlyph->HandleType	= bmDIB;
	FStretchedGlyph->PixelFormat	= pf32bit;

	Invalidate();
}
//---------------------------------------------------------------------------
int __fastcall TGridButton::GetGlyphAlpha()
{
	return FGlyphAlpha;
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetGlyphAlpha(int Value)
{
	if (Value < 0 || Value > 255)
	{
		throw Exception("Alpha must be between 0 and 255");
	}
	FGlyphAlpha = Value & 0xFF;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetGlyphAspect(bool KeepAspect)
{
	FKeepGlyphAspect = KeepAspect;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetTransparentColor(TColor Color)
{
	FTransparentColor = Color;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetTransparentGlyph(bool Transparent)
{
	FTransparentGlyph = Transparent;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::SetGlyphScale(int Scale)
{
	if (Scale < 0 || Scale > 100)
	{
		throw Exception("Scale must be between 0 and 100");
	}
	FGlyphScale = Scale;
	Invalidate();
}
//---------------------------------------------------------------------------
void TGridButton::Invalidate()
{
	if (FOwner)
	{
		FOwner->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::DefineProperties(TFiler *Filer)
{
	TPersistent::DefineProperties(Filer);
	Filer->DefineProperty("Color", ReadProperty1, WriteProperty1, true);
	Filer->DefineProperty("FontColor", ReadProperty2, WriteProperty2, true);
	Filer->DefineProperty("LatchedColor", ReadProperty3, WriteProperty3, true);
	Filer->DefineProperty("DisabledColor", ReadProperty4, WriteProperty4, true);
	Filer->DefineProperty("LatchedFontColor", ReadProperty5, WriteProperty5, true);
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::ReadProperty1(TReader *Reader)
{
	Color = static_cast<TColor>(Reader->ReadInteger());
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::ReadProperty2(TReader *Reader)
{
	FontColor = static_cast<TColor>(Reader->ReadInteger());
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::ReadProperty3(TReader *Reader)
{
	LatchedColor = static_cast<TColor>(Reader->ReadInteger());
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::ReadProperty4(TReader *Reader)
{
	DisabledColor = static_cast<TColor>(Reader->ReadInteger());
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::ReadProperty5(TReader *Reader)
{
	LatchedFontColor = static_cast<TColor>(Reader->ReadInteger());
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::WriteProperty1(TWriter *Writer)
{
	Writer->WriteInteger(Color);
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::WriteProperty2(TWriter *Writer)
{
	Writer->WriteInteger(FontColor);
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::WriteProperty3(TWriter *Writer)
{
	Writer->WriteInteger(LatchedColor);
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::WriteProperty4(TWriter *Writer)
{
	Writer->WriteInteger(DisabledColor);
}
//---------------------------------------------------------------------------
void __fastcall TGridButton::WriteProperty5(TWriter *Writer)
{
	Writer->WriteInteger(LatchedFontColor);
}
//---------------------------------------------------------------------------

