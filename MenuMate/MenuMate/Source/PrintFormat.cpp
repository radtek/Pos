// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <consts.hpp>
#include <vector>
#include "PrintFormat.h"
#include "PrinterDefs.h"
#include "WindowsPrinters.h"
#include "Enum.h"
#include "GlobalSettings.h"
#include "ReceiptManager.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// These are the defaults for the Axiohm A715
// ---------------------------------------------------------------------------
__fastcall TPrintFormat::TPrintFormat()
{
   Lines = new TList;
   fLines = new TStringList;
   // wLines								= new TWideStringList;
   Line = new TFormatLine;

   TopMargin = 0;
   CutterGap = 0;
   BookMarkIndex = -1;

   NormalWidth = DEFAULT_WIDTH_A;
   BoldWidth = DEFAULT_BOLD_WIDTH_A;
   FontBWidth = DEFAULT_WIDTH_B;
   BoldFontBWidth = DEFAULT_BOLD_WIDTH_B;

   BarcodeHRIPosition = BARCODE_HRI_POSITION_NONE;
   BarcodeHRIFont = BARCODE_HRI_FONT_A;
   BarcodeCheckChar = true;
   BarcodeRatio = BARCODE_RATION_5_2;
   BarcodeHeight = 183;
   BarcodeHorzExp = 2;
   BarcodeType = BARCODE_TYPE_CODE_128;

   PrinterCodePage = 936;
   KanjiCodeSystem = KANJI_SJIS;
   KanjiPrinter = false;

   WordWrap = false;

   SolidLineChar = FONT_SOLID_LINE_TEXT;
   DoubleLineChar = FONT_DOUBLE_LINE_TEXT;

   Line->FontInfo.Reset();

   Line->ColCount = 1;
   Line->Columns[0]->Alignment = taLeftJustify;
   Line->Columns[0]->Width = DEFAULT_WIDTH_A;

   EmulationMode = emTextOnly;
}

// ---------------------------------------------------------------------------
__fastcall TPrintFormat::~TPrintFormat()
{
   delete fLines;
   // delete wLines;
   delete Line;

   while (Lines->Count > 0)
   {
	  delete(TFormatLine*)Lines->Items[0];
	  Lines->Delete(0);
   }
   delete Lines;
}
// ---------------------------------------------------------------------------

void TPrintFormat::BookMark()
{
   BookMarkIndex = Lines->Count;
}

void TPrintFormat::Undo()
{
   if (BookMarkIndex >= 0)
   {
	  while (Lines->Count > BookMarkIndex)
	  {
		 int Index = Lines->Count - 1;
		 delete(TFormatLine*)Lines->Items[Index];
		 Lines->Delete(Index);
	  }
	  BookMarkIndex = -1;
   }
}

// ---------------------------------------------------------------------------
void TPrintFormat::DownloadGraphic(UnicodeString FileName, UnicodeString PrinterName)
{
   Graphics::TBitmap *Logo = new Graphics::TBitmap();
   try
   {
	  Logo->LoadFromFile(FileName);

	  int m = Logo->Width / 8;
	  int n = Logo->Height / 8;

	  const char GraphicCommand[4] =
	  {
		 GS, DOWNLOAD_GRAPHIC, m, n
	  };

	  TMemoryStream *MemoryStream = new TMemoryStream;
	  try
	  {
		 MemoryStream->Write(GraphicCommand, 4);

		 char RawData[576];

		 for (int x = 0; x < m * 8; x++)
		 {
			for (int y = 0; y < n * 8; y += 8)
			{
			   unsigned char DataByte = 0;
			   for (int i = 0; i < 8; i++)
			   {
				  unsigned char On = (Logo->Canvas->Pixels[x][y + i] < 128) ? 1 : 0;
				  DataByte = DataByte | On;
				  if (i != 7)
					 DataByte *= 2;
			   }
			   RawData[y / 8] = DataByte;
			}
			MemoryStream->Write(RawData, n);
		 }
		 MemoryStream->Position = 0;
		 if (PrinterName != "")
		 {
			MemoryStream->Position = 0;
			const char* ptr = (const char*)MemoryStream->Memory;
			UnicodeString StreamData(ptr, (unsigned int)(MemoryStream->Size));
			Print(PrinterName, "", StreamData);
		 }
	  }
	  __finally
	  {
		 delete MemoryStream;
	  }
   }
   __finally
   {
	  delete Logo;
   }
}

// ---------------------------------------------------------------------------
void TPrintFormat::Add(UnicodeString LineToAdd)
{
   if (LineToAdd != "")
   {
	  int ColumnNumber = 0;
	  int StrPtr = 0; // Scrolls thru the line looking for |
	  for (int j = 0; j < Line->ColCount; j++)
	  {
		 UnicodeString ColStr = "";
		 // Read the str up to but not including the pipe. If it is a || then the | is a standard char.
		 while (StrPtr < LineToAdd.Length() && LineToAdd.c_str()[StrPtr] != '|')
		 {
			ColStr += LineToAdd.c_str()[StrPtr];
			StrPtr++;
		 }
		 Line->Columns[ColumnNumber]->Text = ColStr;
		 StrPtr++; // Skip over the '|'
		 ColumnNumber++;
	  }
   }
   else
   {
	  for (int ColumnNumber = 0; ColumnNumber < Line->ColCount; ColumnNumber++)
	  {
		 Line->Columns[ColumnNumber]->Text = "";
	  }
   }
   AddLine();
}

// ---------------------------------------------------------------------------
void TPrintFormat::AddLine()
{
   // Create a duplicate of the Line to add to the list.
   TFormatLine *NewLine = new TFormatLine;
   NewLine->ColCount = Line->ColCount;
   for (int i = 0; i < Line->ColCount; i++)
   {
	  NewLine->Columns[i]->Width = Line->Columns[i]->Width;
	  NewLine->Columns[i]->Alignment = Line->Columns[i]->Alignment;
	  NewLine->Columns[i]->Text = Line->Columns[i]->Text;

	  NewLine->Columns[i]->Kanji = Line->Columns[i]->Kanji;

	  NewLine->Columns[i]->LineStyle = Line->Columns[i]->LineStyle;
	  NewLine->Columns[i]->LineLength = Line->Columns[i]->LineLength;

	  Line->Columns[i]->LineStyle = lsNone; // Reset back to normal;
   }
   NewLine->FontInfo = Line->FontInfo;

   NewLine->Cut = Line->Cut;
   NewLine->PartialCut = Line->PartialCut;

   Lines->Add(NewLine);
}

// ---------------------------------------------------------------------------
void TPrintFormat::AddBarcode(UnicodeString Code)
{
   TFormatLine *NewLine = new TFormatLine;
   NewLine->ColCount = 1;
   NewLine->Columns[0]->Width = Code.Length();
   NewLine->Columns[0]->Text = Code;
   NewLine->Barcode = true;

   NewLine->BarcodeHRIPosition = BarcodeHRIPosition;
   NewLine->BarcodeHRIFont = BarcodeHRIFont;
   NewLine->BarcodeCheckChar = BarcodeCheckChar;
   NewLine->BarcodeRatio = BarcodeRatio;
   NewLine->BarcodeHeight = BarcodeHeight;
   NewLine->BarcodeHorzExp = BarcodeHorzExp;
   NewLine->BarcodeType = BarcodeType;

   Lines->Add(NewLine);
}

// ---------------------------------------------------------------------------
void TPrintFormat::NewLine()
{
   TFormatLine *NewLine = new TFormatLine;
   NewLine->ColCount = 1;
   NewLine->Columns[0]->Width = NormalWidth;
   NewLine->Columns[0]->Alignment = taLeftJustify;
   NewLine->Columns[0]->Text = "";
   NewLine->Columns[0]->Kanji = false;

   NewLine->FontInfo.Reset();
   Lines->Add(NewLine);
}

// ---------------------------------------------------------------------------
void TPrintFormat::DrawLine()
{
   TFormatLine *NewLine = new TFormatLine;
   NewLine->ColCount = 1;
   NewLine->Columns[0]->Width = NormalWidth;
   NewLine->Columns[0]->Alignment = taLeftJustify;
   NewLine->Columns[0]->Text = "";

   NewLine->Columns[0]->LineLength = NormalWidth;
   NewLine->Columns[0]->LineStyle = lsSingle;
   Lines->Add(NewLine);
}

// ---------------------------------------------------------------------------
void TPrintFormat::PrintGraphic()
{
   TFormatLine *NewLine = new TFormatLine;
   NewLine->ColCount = 1;
   NewLine->Columns[0]->Width = NormalWidth;
   NewLine->Columns[0]->Alignment = taLeftJustify;
   NewLine->Columns[0]->Text = "";
   NewLine->PrintGraphic = true;
   Lines->Add(NewLine);
}

// ---------------------------------------------------------------------------
void TPrintFormat::Cut()
{
   /* for (int i=0; i<CutterGap; i++)
   {
   NewLine();
   } */
   TFormatLine *NewLine = new TFormatLine;
   NewLine->Cut = true;
   Lines->Add(NewLine);
}

// ---------------------------------------------------------------------------
void TPrintFormat::PartialCut()
{
   TFormatLine *NewLine = new TFormatLine;
   NewLine->PartialCut = true;
   Lines->Add(NewLine);
}

// ---------------------------------------------------------------------------
void TPrintFormat::OpenDrawer()
{
   TFormatLine *NewLine = new TFormatLine;
   NewLine->OpenDrawer = true;
   Lines->Add(NewLine);
}

// ---------------------------------------------------------------------------
bool TPrintFormat::OpenDrawer(UnicodeString PrinterName)
{
   bool Success = false;
   if (!GraphicPrinterSettings.GraphicPrinter)
   {
	  if (EmulationMode == emStar)
	  {
		 char ConfigDrawer[4] =
		 {
			ESC, STAR_OPEN_DRAWER, 100, 160
		 }; // Check Relay speed.
		 char KickDraw1[1] =
		 {
			STAR_OPEN_DRAWER
		 }; // Check Relay speed.

		 AnsiString Text = AnsiString(ConfigDrawer, 4) + AnsiString(KickDraw1, 1);
		 int JobID;
		 Success = WinPrinters::PrintRaw(PrinterName, JobID, Text, JobID);
	  }
	  else
	  {
		 char KickDraw1[5] =
		 {
			ESC, OPEN_DRAWER, DRAWER1, 100, 160
		 }; // Check Relay speed.
		 char KickDraw2[5] =
		 {
			ESC, OPEN_DRAWER, DRAWER2, 100, 160
		 }; // Check Relay speed.

		 AnsiString Text = AnsiString(KickDraw1, 5) + AnsiString(KickDraw2, 5);
		 int JobID;
		 Success = WinPrinters::PrintRaw(PrinterName, JobID, Text, JobID);
	  }
   }
   return Success;
}

// ---------------------------------------------------------------------------
bool TPrintFormat::Print(UnicodeString PrinterName, UnicodeString DocName)
{
   int JobID;
   bool Success;
   if (GraphicPrinterSettings.GraphicPrinter)
   {
	  Success = WinPrinters::PrintGraphics(PrinterName, DocName, GetText()->Text, JobID, GraphicPrinterSettings);
   }
   else
   {
	  Success = WinPrinters::PrintRaw(PrinterName, DocName, GetText()->Text, JobID);
   }
   return Success;
}

// ---------------------------------------------------------------------------
bool TPrintFormat::Print(UnicodeString PrinterName, UnicodeString DocName, AnsiString PrintData,bool CompanyDetailsPrint)
{
   int JobID;

   bool Success;
   if (GraphicPrinterSettings.GraphicPrinter)
   {
      if(TGlobalSettings::Instance().EnableCompanyDetailOnReprintReceipt && CompanyDetailsPrint )
      {
        PrintData = PrintData + "Sold to:" + "\r\n" + ManagerReceipt->Companydetails ;
      }
	  Success = WinPrinters::PrintGraphics(PrinterName, JobID, PrintData, JobID, GraphicPrinterSettings);
   }
   else
   {
      if(TGlobalSettings::Instance().EnableCompanyDetailOnReprintReceipt && CompanyDetailsPrint )
      {
        PrintData = PrintData + "Sold to:" + "\r\n" + ManagerReceipt->Companydetails ;
      }

	  Success = WinPrinters::PrintRaw(PrinterName, JobID, PrintData, JobID);
   }
   return Success;
}
// ---------------------------------------------------------------------------
TStringList *TPrintFormat::GetText()
{
   if (EmulationMode == emScript)
   {
	  GetScriptText();
   }
   else
   {
	  GetFormattedText();
   }
   return fLines;

}
// ---------------------------------------------------------------------------
void TPrintFormat::GetFormattedText()
{
   fLines->Clear();
   char FontCommand[3] =
   {
	  ESC, PRINT_MODE, MODE_FONT_A
   };
   AnsiString LineStr = AnsiString(FontCommand, 3);
   for (int i = 0; i < TopMargin; i++)
   {
	  if (EmulationMode == emEpson)
	  {
		 fLines->Add(LineStr);
	  }
	  else
	  {
		 fLines->Add("");
	  }
   }

   for (int i = 0; i < Lines->Count; i++)
   {
	  AnsiString LineStr = ""; // Concatination of all column strs

	  // If this line is a cut, don't bother checking columns.
	  TFormatLine *FormatLine = (TFormatLine*)Lines->Items[i];
	  if (EmulationMode == emEpson && FormatLine->OpenDrawer)
	  {
		 char KickDraw[5] =
		 {
			ESC, OPEN_DRAWER, DRAWER1, 100, 160
		 }; // Check Relay speed.
		 char KickDraw1[5] =
		 {
			ESC, OPEN_DRAWER, DRAWER2, 100, 160
		 }; // Check Relay speed.
		 fLines->Add(AnsiString(KickDraw, 5));
		 // fLines->Add(AnsiString(KickDraw1, 5));
	  }
	  else if (EmulationMode == emEpson && FormatLine->PrintGraphic)
	  {
		 char PrintGraphicCommand[3] =
		 {
			GS, PRINT_GRAPHIC, DOUBLE_HEIGHT_DOUBLE_WIDTH
		 };
		 fLines->Add(AnsiString(PrintGraphicCommand, 3));
	  }
	  else if (EmulationMode == emEpson && FormatLine->Barcode)
	  {
		 char ResetCommand[] =
		 {
			ESC, RESET
		 };
		 fLines->Add(AnsiString(ResetCommand, 2));

		 char PrintBarcodeCommand[] =
		 {
			GS, BARCODE_HRI_FONT, FormatLine->BarcodeHRIFont, GS, BARCODE_HRI_POSITION, FormatLine->BarcodeHRIPosition, GS,
			BARCODE_HORZ_EXPANSION, FormatLine->BarcodeHorzExp, GS, BARCODE_AUTO_CHECK_CHAR, FormatLine->BarcodeCheckChar ? 1 : 0, GS,
			BARCODE_RATIO, FormatLine->BarcodeRatio, GS, BARCODE_HEIGHT, FormatLine->BarcodeHeight, GS, BARCODE_PRINT,
			FormatLine->BarcodeType, FormatLine->Columns[0]->Text.Length()+sizeof(CODE_FONT_A)
		 };

		 fLines->Add(AnsiString(PrintBarcodeCommand, 22) + AnsiString(CODE_FONT_A, 2) + FormatLine->Columns[0]->Text);
	  }
	  else if (EmulationMode == emEpson && (FormatLine->Cut || FormatLine->PartialCut))
	  {
		 for (int i = 0; i < CutterGap; i++)
		 {
			fLines->Add(LineStr);
		 }
		 if (AlwaysUseFullCut || FormatLine->Cut)
		 {
			char CutCommand[4] =
			{
			   GS, CUT_MODE, MODE_FULL_CUT, 0
			};
			fLines->Add(AnsiString(CutCommand, 4));
		 }
		 else if (FormatLine->PartialCut)
		 {
			char CutCommand[4] =
			{
			   GS, CUT_MODE, MODE_PARTIAL_CUT, 0
			};
			fLines->Add(AnsiString(CutCommand, 4));
		 }
	  }
	  else if (EmulationMode == emStar && (FormatLine->Cut || FormatLine->PartialCut))
	  {
		 for (int i = 0; i < CutterGap; i++)
		 {
			fLines->Add(LineStr);
		 }
		 if (AlwaysUseFullCut || FormatLine->Cut)
		 {
			char CutCommand[3] =
			{
			   ESC, STAR_CUT_MODE, STAR_MODE_FULL_CUT
			};
			fLines->Add(AnsiString(CutCommand, 3));
		 }
		 else if (FormatLine->PartialCut)
		 {
			char CutCommand[3] =
			{
			   ESC, STAR_CUT_MODE, STAR_MODE_PARTIAL_CUT
			};
			fLines->Add(AnsiString(CutCommand, 3));
		 }
	  }
	  else if (EmulationMode == emTextOnly && (FormatLine->Cut || FormatLine->PartialCut))
	  {
		 for (int i = 0; i < CutterGap; i++)
		 {
			fLines->Add(LineStr);
		 }

		 AnsiString ColumnStr = AnsiString(".........  .........");
		 if (AlwaysUseFullCut || FormatLine->Cut)
		 {
			ColumnStr = AnsiString("....................");
		 }

		 int ColWidth = Width;
		 if ((ColWidth - ColumnStr.Length()) > 0)
		 {
			int FirstGap, LastGap;
			FirstGap = (ColWidth - ColumnStr.Length()) / 2;
			LastGap = ColWidth - FirstGap - ColumnStr.Length();
			ColumnStr = AnsiString::StringOfChar(' ', FirstGap) + ColumnStr + AnsiString::StringOfChar(' ', LastGap);
		 }
		 fLines->Add(ColumnStr);
	  }
	  else
	  {
		 // For WordWrap, we start at the first character in each column. We print as much as we can
		 // and set this position in the WordWrapPosition array. A column is finished when this position
		 // is == to the columns text length. The next print loop then starts from these positions (if
		 // there is anything to print) until all columns are printed.
		 std::vector <int> WordWrapPosition(FormatLine->ColCount, 0);
		 bool FininshedLine = false;
		 while (!FininshedLine)
		 {
			for (int j = 0; j < FormatLine->ColCount; j++)
			{
			   WideString ColumnStr;
			   if (FormatLine->Columns[j]->LineStyle == lsNone)
			   {
				  if (FormatLine->Columns[j]->Width <= 0)
				  {
					 FormatLine->Columns[j]->Text = ""; // Stop lock-up if trying to write to a 0 length column.
				  }
				  // Get the string starting from where it was left for the last pass.
				  if (WordWrapPosition[j] < FormatLine->Columns[j]->Text.Length())
				  {
					 int LineLength = FormatLine->Columns[j]->Text.Length() - WordWrapPosition[j];
					 ColumnStr = FormatLine->Columns[j]->Text.SubString(WordWrapPosition[j] + 1, LineLength);
				  }
			   }
			   else
			   {
				  if (EmulationMode == emEpson)
				  {
					 if (FormatLine->Columns[j]->LineStyle == lsSingle)
					 {
						ColumnStr = AnsiString::StringOfChar(SolidLineChar, FormatLine->Columns[j]->LineLength);
					 }
					 else if (FormatLine->Columns[j]->LineStyle == lsDouble)
					 {
						ColumnStr = AnsiString::StringOfChar(DoubleLineChar, FormatLine->Columns[j]->LineLength);
					 }
					 // Fill this in so that the wrapping code below knows what it's trying to wrap, otherwise, the
					 // Text field is random (probably the previous line!)
					 FormatLine->Columns[j]->Text = ColumnStr;
				  }
				  else
				  {
					 if (FormatLine->Columns[j]->LineStyle == lsSingle)
					 {
						ColumnStr = AnsiString::StringOfChar(SolidLineChar, FormatLine->Columns[j]->LineLength);
					 }
					 else if (FormatLine->Columns[j]->LineStyle == lsDouble)
					 {
						ColumnStr = AnsiString::StringOfChar(DoubleLineChar, FormatLine->Columns[j]->LineLength);
					 }
					 FormatLine->Columns[j]->Text = ColumnStr;
				  }
			   }
			   int ColWidth = FormatLine->Columns[j]->Width;

			   if (WordWrap)
			   {
				  // Check if it fits the column
				  if (ColumnStr.Length() <= ColWidth)
				  {
					 WordWrapPosition[j] += ColumnStr.Length();
				  }
				  // Check first if it already breaks on a ' ' or \r
				  else if (ColumnStr[ColWidth + 1] == ' ' || ColumnStr[ColWidth + 1] == '\n' || ColumnStr[ColWidth + 1] == '\r')
				  {
					 ColumnStr.SetLength(ColWidth);
					 WordWrapPosition[j] += ColWidth + 1;
				  }
				  else
				  {
					 ColumnStr.SetLength(ColWidth);
					 // Look forwards for a \r or \n to force a break.
					 bool FoundBreak = false;
					 for (int k = 1; k <= ColumnStr.Length(); k++)
					 {
						if (ColumnStr[k] == '\n' || ColumnStr[k] == '\r')
						{
						   if (k < ColumnStr.Length())
						   {
							  if ((ColumnStr[k] == '\n' && ColumnStr[k + 1] == '\r') || (ColumnStr[k] == '\r' && ColumnStr[k + 1] == '\n'))
							  {
								 WordWrapPosition[j] += k + 1;
							  }
							  else
							  {
								 WordWrapPosition[j] += k;
							  }
						   }
						   else
						   {
							  WordWrapPosition[j] += k;
						   }
						   ColumnStr.SetLength(k - 1);
						   FoundBreak = true;
						   break;
						}
					 }
					 // Scroll back, looking for a ' ' or a \r to wrap on. Otherwise, just break the word.
					 if (!FoundBreak)
					 {
						for (int k = ColumnStr.Length(); k > 0; k--)
						{
						   if (ColumnStr[k] == ' ' || ColumnStr[k] == '\n' || ColumnStr[k] == '\r')
						   {
							  ColumnStr.SetLength(k - 1);
							  WordWrapPosition[j] += k;
							  FoundBreak = true;
							  break;
						   }
						}
						// Solid letters. Just break it.
						if (!FoundBreak)
						{
						   WordWrapPosition[j] += ColWidth;
						}
					 }
				  }
			   }
			   else
			   {
				  if (ColumnStr.Length() > ColWidth)
				  {
					 ColumnStr.SetLength(ColWidth);
				  }
				  if (ColumnStr.Pos(WideString('\n')) > 0)
				  {
					 ColumnStr.SetLength(ColumnStr.Pos(WideString('\n')) - 1);
				  }
				  if (ColumnStr.Pos(WideString('\r')) > 0)
				  {
					 ColumnStr.SetLength(ColumnStr.Pos(WideString('\r')) - 1);
				  }
			   }

			   if (FormatLine->Columns[j]->Alignment == taLeftJustify)
			   {
				  if ((ColWidth - ColumnStr.Length()) > 0)
				  {
					 ColumnStr = ColumnStr + AnsiString::StringOfChar(' ', ColWidth - ColumnStr.Length());
				  }
			   }
			   else if (FormatLine->Columns[j]->Alignment == taRightJustify)
			   {
				  if ((ColWidth - ColumnStr.Length()) > 0)
				  {
					 ColumnStr = AnsiString::StringOfChar(' ', ColWidth - ColumnStr.Length()) + ColumnStr;
				  }
			   }
			   else if (FormatLine->Columns[j]->Alignment == taCenter)
			   {
				  if ((ColWidth - ColumnStr.Length()) > 0)
				  {
					 int FirstGap, LastGap;
					 FirstGap = (ColWidth - ColumnStr.Length()) / 2;
					 LastGap = ColWidth - FirstGap - ColumnStr.Length();
					 ColumnStr = AnsiString::StringOfChar(' ', FirstGap) + ColumnStr + AnsiString::StringOfChar(' ', LastGap);
				  }
			   }
			   else
			   {
				  ColumnStr = "";
			   }

               if (PrinterCodePageOn.Length() > 1)
               {
                  std::vector <char> BufferOn(PrinterCodePageOn.Length() / 2, 0x00);
                  HexToBin(PrinterCodePageOn.LowerCase().c_str(), &BufferOn[0], BufferOn.size());
                  LineStr = LineStr + AnsiString(&BufferOn[0], BufferOn.size());
               }

               if(PrinterCodePage != 0)
               {
                  int BufferSize = WideCharToMultiByte(PrinterCodePage, 0, ColumnStr.c_bstr(), ColumnStr.Length(), NULL, 0, NULL, NULL);
                  char *dest = new char[BufferSize];

                  WideCharToMultiByte(PrinterCodePage, 0, ColumnStr.c_bstr(), ColumnStr.Length(), dest, BufferSize, NULL, NULL);
                  LineStr = LineStr + AnsiString(dest, BufferSize);
                  delete[]dest;
               }
               else
               {
                    LineStr += ColumnStr;
               }
               // Chars			= Chars + AnsiString(ColumnStr.c_bstr(), ColumnStr.Length());

               if (PrinterCodePageOff.Length() > 1)
               {
                  std::vector <char> BufferOff(PrinterCodePageOff.Length() / 2, 0x00);
                  HexToBin(PrinterCodePageOff.LowerCase().c_str(), &BufferOff[0], BufferOff.size());
                  LineStr = LineStr + AnsiString(&BufferOff[0], BufferOff.size());
               }

			}
			if (EmulationMode == emEpson)
			{
			   char ModeChar = MODE_FONT_A;

			   if (FormatLine->FontInfo.Font == ftFontB)
			   {
				  ModeChar |= MODE_FONT_B;
			   }

			   if (FormatLine->FontInfo.Height == fsDoubleSize)
			   {
				  ModeChar |= MODE_DOUBLE_HEIGHT;
			   }
			   if (FormatLine->FontInfo.Width == fsDoubleSize)
			   {
				  ModeChar |= MODE_DOUBLE_WIDTH;
			   }
			   if (FormatLine->FontInfo.Bold)
			   {
				  ModeChar |= MODE_BOLD;
			   }
			   if (FormatLine->FontInfo.Underlined)
			   {
				  ModeChar |= MODE_UNDERLINE;
			   }

			   char FontCommand[3] =
			   {
				  ESC, PRINT_MODE, ModeChar
			   };
			   LineStr = AnsiString(FontCommand, 3) + LineStr;

			   char ColourMode = MODE_COLOUR_OFF;
			   if (FormatLine->FontInfo.Colour == fcRed)
			   {
				  ColourMode |= MODE_COLOUR_ON;
			   }
			   char ColourCommand[3] =
			   {
				  ESC, COLOUR_MODE, ColourMode
			   };
			   LineStr = AnsiString(ColourCommand, 3) + LineStr;

			}
			else if (EmulationMode == emStar)
			{
			   if (FormatLine->FontInfo.Font == ftFontB)
			   {
				  char FontCommand[2] =
				  {
					 ESC, STAR_FONT_B
				  };
				  LineStr = AnsiString(FontCommand, 2) + LineStr;
			   }
			   else
			   {
				  char FontCommand[2] =
				  {
					 ESC, STAR_FONT_A
				  };
				  LineStr = AnsiString(FontCommand, 2) + LineStr;
			   }

			   char BoldCommand[2] =
			   {
				  ESC, STAR_FONT_INTENSITY_NORMAL
			   };
			   if (FormatLine->FontInfo.Bold)
			   {
				  BoldCommand[1] = STAR_FONT_INTENSITY_BOLD;
			   }
			   LineStr = AnsiString(BoldCommand, 2) + LineStr;

			   char FontWidthCommand[1] =
			   {
				  STAR_FONT_NORMAL_WIDTH
			   };
			   if (FormatLine->FontInfo.Width == fsDoubleSize)
			   {
				  FontWidthCommand[0] = STAR_FONT_DOUBLE_WIDTH;
			   }
			   LineStr = AnsiString(FontWidthCommand, 1) + LineStr;

			   char FontHeightCommand[2] =
			   {
				  ESC, STAR_FONT_NORMAL_HEIGHT
			   };
			   if (FormatLine->FontInfo.Height == fsDoubleSize)
			   {
				  FontHeightCommand[1] = STAR_FONT_DOUBLE_HEIGHT;
			   }
			   LineStr = AnsiString(FontHeightCommand, 2) + LineStr;

			   if (FormatLine->FontInfo.Underlined)
			   {
				  char UnderlineCommand[3] =
				  {
					 ESC, STAR_UNDERLINE, 0x01
				  };
				  LineStr = AnsiString(UnderlineCommand, 3) + LineStr;
			   }
			   else
			   {
				  char UnderlineCommand[3] =
				  {
					 ESC, STAR_UNDERLINE, 0x00
				  };
				  LineStr = AnsiString(UnderlineCommand, 3) + LineStr;
			   }
			}
			fLines->Add(LineStr);
			LineStr = "";
			if (WordWrap)
			{
			   // This checks to see that we have printed all text for all columns.
			   FininshedLine = true;
			   for (int j = 0; j < FormatLine->ColCount; j++)
			   {
				  if (WordWrapPosition[j] < FormatLine->Columns[j]->Text.Length())
				  {
					 FininshedLine = false;
				  }
			   }
			}
			else
			{
			   FininshedLine = true;
			}
		 }
	  }
   }
}

// ---------------------------------------------------------------------------
void TPrintFormat::GetScriptText()
{
   fLines->Clear();

   fLines->Add("Start");
   fLines->Add("TopMargin=" + IntToStr(TopMargin));
   fLines->Add("CutterGap=" + IntToStr(CutterGap));

   fLines->Add("NormalWidth=" + IntToStr(NormalWidth));
   fLines->Add("BoldWidth=" + IntToStr(BoldWidth));
   fLines->Add("FontBWidth=" + IntToStr(FontBWidth));
   fLines->Add("BoldFontBWidth=" + IntToStr(BoldFontBWidth));

   fLines->Add("SolidLineChar=" + IntToStr(SolidLineChar));
   fLines->Add("DoubleLineChar=" + IntToStr(DoubleLineChar));

   fLines->Add("WordWrap=" + IntToStr((int)WordWrap));
   fLines->Add("BoldFontBWidth=" + IntToStr(NormalWidth));

   for (int i = 0; i < Lines->Count; i++)
   {
	  TFormatLine *FormatLine = (TFormatLine*)Lines->Items[i];

	  fLines->Add("Line=");

	  if (FormatLine->PrintGraphic)
		 fLines->Add("   PrintGraphic=True");
	  else if (FormatLine->Cut)
		 fLines->Add("   Cut=True");
	  else if (FormatLine->OpenDrawer)
		 fLines->Add("   OpenDrawer=True");
	  else if (FormatLine->PartialCut)
		 fLines->Add("   PartialCut=True");
	  else
	  {
		 if (FormatLine->FontInfo.Bold)
			fLines->Add("   Bold=True");

		 if (FormatLine->FontInfo.Width != fsNormalSize)
			fLines->Add("   FontWidth=DoubleSize");
		 if (FormatLine->FontInfo.Height != fsNormalSize)
			fLines->Add("   FontWidth=FontHeight");

		 for (int j = 0; j < FormatLine->ColCount; j++)
		 {
			TFormatColumn *FormatColumn = FormatLine->Columns[j];

			fLines->Add("   Column=");

			if (FormatColumn->LineStyle != lsNone)
			{
			   fLines->Add("      LineStyle=" + IntToStr(FormatColumn->LineStyle));
			   fLines->Add("      LineLength=" + IntToStr(FormatLine->Columns[j]->LineLength));
			}
			else
			{
			   fLines->Add("      Width=" + IntToStr(FormatLine->Columns[j]->Width));
			   fLines->Add("      Alignment=" + IntToStr(FormatLine->Columns[j]->Alignment));
			   fLines->Add("      Text=" + FormatLine->Columns[j]->Text);
			   if (FormatLine->Columns[j]->Kanji)
				  fLines->Add("      Kanji=True");
			}
		 }
	  }
   }
   fLines->Add("End");
}

// ---------------------------------------------------------------------------
int TPrintFormat::GetWidth()
{
   if (EmulationMode == emTextOnly)
   {
	  return NormalWidth;
   }
   else
   {
	  if (Line->FontInfo.Width == fsNormalSize)
	  {
		 if (Line->FontInfo.Bold)
		 {
			return BoldWidth;
		 }
		 else
		 {
			return NormalWidth;
		 }
	  }
	  else if (Line->FontInfo.Width == fsDoubleSize)
	  {
		 if (Line->FontInfo.Bold)
		 {
			return BoldWidth / 2;
		 }
		 else
		 {
			return NormalWidth / 2;
		 }
	  }
	  else
		 return 0;
   }
}

// ---------------------------------------------------------------------------
__fastcall TFormatLine::TFormatLine()
{
   fColumns = new TList;
   Cut = false;
   PartialCut = false;
   Barcode = false;
   OpenDrawer = false;
   FontInfo.Reset();
}

// ---------------------------------------------------------------------------
__fastcall TFormatLine::~TFormatLine()
{
   for (int i = 0; i < fColumns->Count; i++)
   {
	  delete(TFormatColumn*)fColumns->Items[i];
   }
   delete fColumns;
}

// ---------------------------------------------------------------------------
void TFormatLine::SetColCount(int Count)
{
   if (Count < 1)
	  Count = 1;
   fColCount = Count;
   if (fColCount > fColumns->Count)
   {
	  while (fColCount > fColumns->Count)
	  {
		 TFormatColumn *NewCol = new TFormatColumn;
		 fColumns->Add(NewCol);
	  }
   }
   else if (fColCount < fColumns->Count)
   {
	  while (fColCount < fColumns->Count)
	  {
		 delete(TFormatColumn*)fColumns->Items[fColumns->Count - 1];
		 fColumns->Delete(fColumns->Count - 1);
	  }
   }
}
// ---------------------------------------------------------------------------
