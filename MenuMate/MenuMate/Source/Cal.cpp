//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//#include "enum.h"

//#include "Reservation.h"
#include "Cal.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TCal *)
{
	new TCal(NULL);
}

   // Calendar lookup Bitmap index
int TCal::CalBitmapLookup[] =	{1,2,3,4,5,
                               6,5,1,3,4,
                               3,2,4,6,3,
                               5,6,1,2,4,
                               3,5,6,4,2,
                               1,2,4,5,3,
                               4};

int TCal::CalXBitmapLookup[] ={1,2,3,4,5,
                               6,5,1,3,4,
                               3,2,4,6,3,
                               5,6,1,2,4,
                               3,5,6,4,2,
                               1,2,4,5,3,
                               4};


//---------------------------------------------------------------------------

__fastcall TCal::TCal(TComponent* Owner)
	: TCustomGrid(Owner)
{
   FixedCols = 0;
   FixedRows = 1;
   ColCount = 7;
   RowCount = 7;
   ScrollBars = ssNone;
   Options.Clear();
	Options << goFixedVertLine << goFixedHorzLine << goVertLine << goHorzLine << goDrawFocusSelected;
   DefaultDrawing = true;

   Align = alNone;
   BorderStyle = bsSingle;
   Color = clWindow;
   Ctl3D = true;
   Cursor = crDefault;
   Enabled = true;
   GridLineWidth = 1;
   Visible = true;
	UseCurrentDate = true;
   FDate = Date();
	FReadOnly = false;
   this->ParentFont = false;
   ParentFont = true;
//	Font->Size = 10;
//   Font->Name = "Comic Sans MS";

   for(int i=0; i<7;i++)
   {
   	Bitmaps[i] = new Graphics::TBitmap;
      Bitmaps[i]->LoadFromResourceID((int)HInstance, i);
   }

	int j=7;
   for(int s=0; s<7;s++,j++)
   {
   	XBitmaps[s] = new Graphics::TBitmap;
      XBitmaps[s]->LoadFromResourceID((int)HInstance, j);
		XBitmaps[s]->TransparentColor = clWhite;
		XBitmaps[s]->Transparent = true;
   }

  	TextBitmap = new Graphics::TBitmap;
	TextBitmap->Height = 64;
   TextBitmap->Width = 128;
   TextBitmap->Canvas->Pen->Color = clBlack;
   TextBitmap->TransparentColor = clWhite;
   TextBitmap->Transparent = true;
   TextBitmap->Canvas->Font->Name = "Comic Sans MS";
   TextBitmap->Canvas->Font->Size = 16;

   TFontStyles Style;
   Style << fsBold;
   TextBitmap->Canvas->Font->Style = Style;
   UpdateCalendar();
}
//---------------------------------------------------------------------------

__fastcall TCal::~TCal()
{
   for(int i=0; i<7;i++)
   {
   	delete Bitmaps[i];
   }
   for(int s=0; s<7;s++)
   {
   	delete XBitmaps[s];
   }
 	delete TextBitmap;
}
//---------------------------------------------------------------------------

void TCal::Change()
{
   if(FOnChange)
   {
   	FOnChange(this);
   }
}
//---------------------------------------------------------------------------

void __fastcall TCal::Click()
{
  	AnsiString  TheCellText;
  	TheCellText = CellText[Col][Row];
  	if(!TheCellText.IsEmpty())
  	{
    	Day = (WORD)TheCellText.ToInt();
  	}
/*	if (OnClick)
   {
   	OnClick(this);
   } */
	UpdateCalendar();
}
//---------------------------------------------------------------------------

bool TCal::IsLeapYear(int AYear)
{
  return  ((AYear %   4) == 0) &&
         (((AYear % 100) != 0) ||
          ((AYear % 400) == 0));
}
//---------------------------------------------------------------------------

WORD TCal::DaysPerMonth(int AYear,int AMonth)
{
  WORD result;
  const WORD DaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  result = DaysInMonth[AMonth-1];
  if ((AMonth == 2) && IsLeapYear(AYear))
    ++result; // leap-year Feb is special

  return result;
}
//---------------------------------------------------------------------------

int TCal::DaysThisMonth()
{
  return DaysPerMonth(Year, Month);
}
//---------------------------------------------------------------------------
TCanvas * __fastcall TCal::GetCanvas()
{
	return TCustomControl::Canvas;
}

void __fastcall TCal::DrawCell(int ACol,int ARow,const TRect &ARect, TGridDrawState AState)
{
	if (FOnDrawCell)
   {
   	OnDrawCell(this, ACol, ARow, ARect, AState);
   }
   else
   {
   	const AnsiString  TheText = CellText[ACol][ARow];

      if (AState.Contains(gdFixed))
      {
         Canvas->TextRect(
            ARect,
            ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
            ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
            TheText);
      }
      else
      {
         WORD DayNum = (WORD)(FMonthOffset + (WORD)(ACol) + ((WORD)(ARow) - 1) * 7);
         if((DayNum >= 1) && (DayNum <= DaysThisMonth()) )
         {
            TDateTime CellDate = EncodeDate(Year,Month,DayNum);

            if (AState.Contains(gdSelected))
            {
               TColor StoreFontColor = Canvas->Font->Color;
               Canvas->Font->Color = clHighlightText;
               Canvas->TextRect(
                  ARect,
                  ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
                  ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
                  TheText);
               Canvas->Font->Color = StoreFontColor;
            }
/*            else if (CellDate == Date())
            {
               TColor ColorStore = Canvas->Brush->Color;
               TBrushStyle StyleStore = Canvas->Brush->Style;
               Canvas->Brush->Color = clGray;
               Canvas->Brush->Style = bsDiagCross;
               Canvas->Rectangle(ARect.Left, ARect.Top, ARect.Right, ARect.Bottom);
               Canvas->TextOut(
                  ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
                  ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
                  TheText);
/*               Canvas->TextRect(
                  ARect,
                  ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
                  ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
                  TheText); */
/*               Canvas->Brush->Color = ColorStore;
               Canvas->Brush->Style = StyleStore;
            }   */
            else if (CellDate < Date())
            {
               TColor ColorStore = Canvas->Font->Color;
               Canvas->Font->Color = clGrayText;
               Canvas->TextRect(
                  ARect,
                  ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
                  ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
                  TheText);
               Canvas->Font->Color = ColorStore;
            }
            else
            {
               TColor StoreBrushColor 	= Canvas->Brush->Color;
               TColor StoreFontColor 	= Canvas->Font->Color;
               if (AState.Contains(gdSelected))
               {
               	Canvas->Brush->Color = clHighlight;
               	Canvas->Font->Color = clHighlightText;
               }
               Canvas->TextRect(
                  ARect,
                  ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
                  ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
                  TheText);
               Canvas->Brush->Color = StoreBrushColor;
               Canvas->Font->Color = StoreFontColor;
            }
            if (CellDate == Date())
            {
               Canvas->CopyMode = cmSrcCopy;
               TRect SizeRect = ARect;
               SizeRect.Left += 10;
               SizeRect.Right -= 10;
               SizeRect.Top += 10;
               SizeRect.Bottom -= 10;
               Bitmaps[CalBitmapLookup[DayNum-1]]->Transparent = true;
               Canvas->StretchDraw(SizeRect, Bitmaps[CalBitmapLookup[DayNum-1]]); // Red Ring.
            }
         }
      }
   }
}
//---------------------------------------------------------------------------

AnsiString TCal::GetCellText(int ACol,int ARow)
{
	int DayNum;
	AnsiString  result;

	if (!ARow)
   {
   // day names at tops of columns
   //
		result = ShortDayNames[(StartOfWeek + ACol) % 7];
   }
  	else
  	{
   	DayNum = FMonthOffset + ACol + (ARow - 1) * 7;
    	if ((DayNum < 1) || (DayNum > DaysThisMonth()))
      {
      	result = "";
      }
    	else
      {
      	result = IntToStr(DayNum);
      }
  	}
	return result;
}
//---------------------------------------------------------------------------

bool __fastcall TCal::SelectCell(int ACol, int ARow)
{
	bool  result;
	if ((!FUpdating && FReadOnly) || !CellText[ACol][ARow].c_str())
	{
  		result = false;
	}
   else
  	// Not allowed to change the cell selection.
	result = TCustomGrid::SelectCell(ACol, ARow);

  	return result;
}
//---------------------------------------------------------------------------

void __fastcall TCal::SetCalendarDate(TDateTime Value)
{
	FDate = Value;
	UpdateCalendar();
	Change();
}
//---------------------------------------------------------------------------

bool __fastcall TCal::StoreCalendarDate()
{
	return !FUseCurrentDate;
}
//---------------------------------------------------------------------------

WORD __fastcall TCal::GetDateElement(int Index)
{
	Word  AYear, AMonth, ADay;
	WORD result;

	DecodeDate(FDate, AYear, AMonth, ADay);
	switch(Index)
	{
   	case 1:
      	result = AYear;break;

    	case 2:
      	result = AMonth;break;

    	case 3:
      	result = ADay;break;

    	default:
      	result = -1;
  	}

  	return result;
}
//---------------------------------------------------------------------------

void __fastcall TCal::SetDateElement(int Index, Word Value)
{
	Word  AYear, AMonth, ADay;
  	bool  Update  = false;

  	if (Value > 0)
  	{
    	DecodeDate(FDate, AYear, AMonth, ADay);
    	switch(Index)
    	{
      	case 1:
        		if (AYear != Value)
        		{
          		AYear   = Value;
          		Update  = true;
        		}
        		break;

      	case 2:
        		if ((Value <= 12) && (Value != AMonth))
        		{
          		AMonth  = Value;
          		Update  = true;
        		}
        		break;

      	case 3:
        		if ((Value <= DaysThisMonth()) && (Value != ADay))
        		{
          		ADay    = Value;
          		Update  = true;
        		}
        		break;
    	}
    	if (Update)
    	{
			if(ADay > DaysPerMonth(AYear, AMonth))
      	{
				ADay = DaysPerMonth(AYear, AMonth);
      	}
      	FDate = EncodeDate(AYear, AMonth, ADay);
      	FUseCurrentDate = false;
      	UpdateCalendar();
      	Change();
    	}
  	}
}
//---------------------------------------------------------------------------

void __fastcall TCal::SetStartOfWeek(TDayOfWeek Value)
{
   if( (Value > 7) || (Value < 1) )
   {
   	FStartOfWeek=1; // bad value sets to 1 (Sunday)
   }
   else
   {
      if (Value != FStartOfWeek)
      {
         FStartOfWeek = Value;
         UpdateCalendar();
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TCal::SetUseCurrentDate(bool Value)
{
	if(Value != FUseCurrentDate)
  	{
   	FUseCurrentDate = Value;
    	if(Value)
    	{
      	FDate = Date();// { use the current date, then }
      	UpdateCalendar();
    	}
  	}
}
//---------------------------------------------------------------------------

// Given a value of 1 or -1, moves to Next or Prev month accordingly.
//
void __fastcall TCal::ChangeMonth(int Delta)
{
  Word  AYear, AMonth, ADay;
  TDateTime NewDate;
  WORD CurDay;

  DecodeDate(FDate, AYear, AMonth, ADay);
  CurDay = ADay;
  if (Delta > 0)
    ADay = DaysPerMonth(AYear, AMonth);
  else
    ADay = 1;

  NewDate = EncodeDate(AYear, AMonth, ADay);
  NewDate = NewDate + Delta;
  DecodeDate(NewDate, AYear, AMonth, ADay);
  if (DaysPerMonth(AYear, AMonth) > CurDay)
    ADay = CurDay;
  else
    ADay = DaysPerMonth(AYear, AMonth);

  CalendarDate = EncodeDate(AYear, AMonth, ADay);
}
//---------------------------------------------------------------------------

void __fastcall TCal::PrevMonth()
{
	ChangeMonth(-1);
}
//---------------------------------------------------------------------------

void __fastcall TCal::NextMonth()
{
	ChangeMonth(1);
}
//---------------------------------------------------------------------------

void __fastcall TCal::NextYear()
{
  if (IsLeapYear(Year) && (Month == 2) && (Day == 29))
    Day = 28;
  ++Year;
}
//---------------------------------------------------------------------------

void __fastcall TCal::PrevYear()
{
	if (IsLeapYear(Year) && (Month == 2) && (Day == 29))
   	Day = 28;
  	--Year;
}
//---------------------------------------------------------------------------

void __fastcall TCal::UpdateCalendar()
{
   Word  AYear, AMonth, ADay;
   TDateTime FirstDate;

   FUpdating = true;
   DecodeDate(FDate, AYear, AMonth, ADay);
   FirstDate = EncodeDate(AYear, AMonth, 1);

   // Day of week for 1st of month.
   //
   FMonthOffset = 2 - ((DayOfWeek(FirstDate) - StartOfWeek + 7) % 7);
   if (FMonthOffset == 2)
   {
   	FMonthOffset = -5;
   }

   MoveColRow((ADay - FMonthOffset) % 7,
             (ADay - FMonthOffset) / 7 + 1,
             False, False);
   Invalidate();

	FUpdating = false;
}
//---------------------------------------------------------------------------

void TCal::WMSize(Messages::TWMSize &Message)
{
   int WidthRemaining = Message.Width - (GridLineWidth * (ColCount - 1));
   for (int i=0; i<ColCount; i++)
   {
		ColWidths[i] = WidthRemaining / (ColCount - i);
      WidthRemaining -= ColWidths[i];
   }

   int HeightRemaining = Message.Height - (GridLineWidth * (RowCount - 1));
   for (int s=0; s<RowCount; s++)
   {
		RowHeights[s] = HeightRemaining / (RowCount - s);
      HeightRemaining -= RowHeights[s];
   }
}
//---------------------------------------------------------------------------
