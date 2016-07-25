//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "enum.h"
#include "Comms.h"
#include "PrinterConfig.h"
#include "MMRegistry.h"
#include "ManagerVariable.h"
#include "TouchKeyboard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchButton"
#pragma resource "*.dfm"
TfrmPrinterConfiguration *frmPrinterConfiguration;
//---------------------------------------------------------------------------
__fastcall TfrmPrinterConfiguration::TfrmPrinterConfiguration(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterConfiguration::FormCreate(TObject *Sender)
{
   PrinterList = new TList;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::FormDestroy(TObject *Sender)
{
	delete PrinterList;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::FormShow(TObject *Sender)
{
	FormResize(NULL);
	UpdateLists();
}
//---------------------------------------------------------------------------
void TfrmPrinterConfiguration::UpdateLists()
{
   lbPrinters->Clear();
   TComms::Instance().ConnectLst->Pack();
   for(int i=0; i<TComms::Instance().ConnectLst->Count; i++)
   {
      if( ((TConnect *)(TComms::Instance().ConnectLst->Items[i]))->ClassNameIs("TConnectPrinter"))
      {
         TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().ConnectLst->Items[i];
         lbPrinters->Items->Add(Printer->Name);
      }
   }

	bool CarbonCopy = VM->GetBool(vmDuplicateReceipts);
   cbCarbonCopy->Checked = CarbonCopy;
	TComms::Instance().CarbonCopy = CarbonCopy;

	bool PrintNote = VM->GetBool(vmPrintNoteWithDiscount);
   cbPrintNoteWithDiscount->Checked = PrintNote;
   Receipt->PrintNoteWithDiscount = PrintNote;

	bool Kitchenfontsize = VM->GetBool(vmLargeKitchenFont);
	cbKitchenFont->Checked = !Kitchenfontsize;
	Kitchen->LargeFont = Kitchenfontsize;

	bool HeaderForEachCourse = VM->GetBool(vmHeaderForEachCourse);
	cbCourseHeader->Checked = HeaderForEachCourse;
	Kitchen->HeaderForEachCourse = HeaderForEachCourse;

	cbUnravelKitchenPrinting->Checked = VM->GetBool(vmUnRavelKitchenPrinting);
	Kitchen->UnRavelKitchenPrinting = cbUnravelKitchenPrinting->Checked;

	AnsiString BulletSide = VM->GetStr(vmKitchenBulletSide);
	edSideBullet->Text = BulletSide;
	Kitchen->BulletSide = BulletSide;

	AnsiString BulletOpt = VM->GetStr(vmKitchenBulletOpt);
	edOptionsBullet->Text = BulletOpt;
	Kitchen->BulletOpt = BulletOpt;

	AnsiString NoteHeader = VM->GetStr(vmKitchenHeaderNote);
	edNoteHeader->Text = NoteHeader;
	Kitchen->NoteHeader = NoteHeader;

	AnsiString SideHeader = VM->GetStr(vmKitchenHeaderSide);
	edSidesHeader->Text = SideHeader;
	Kitchen->SideHeader = SideHeader;

	AnsiString MixHeader =  VM->GetStr(vmKitchenHeaderMix);
	edMixHeader->Text = MixHeader;
	Kitchen->MixHeader = MixHeader;

	AnsiString DishesPerSeatMultiplerString = VM->GetStr(vmKitchenPerSeatMultiplerString);
	edPerSeatMultiplier->Text = DishesPerSeatMultiplerString;
	Kitchen->DishesPerSeatMultiplerString = DishesPerSeatMultiplerString;

	int CutType = VM->GetInt(vmCutType);
	// Normal CutType = 0; (Single Cut)
	// CutByDish CutType = 1;
	// CutByCourse CutType = 2;
	switch(CutType)
   {
      case 0: rbCutByNone->Checked = true; break;
		case 2: rbCutByCourse->Checked = true; break;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::lbPrintersClick(TObject *Sender)
{
   TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().FetchByName(lbPrinters->Items->Strings[lbPrinters->ItemIndex],Windows_Printer);
   if(Printer)
   {
      lbLargeFnt->ItemIndex = Printer->BoldCharPerLine - StrToInt(lbLargeFnt->Items->Strings[0]);
      lbNormalFnt->ItemIndex = Printer->NormalCharPerLine - StrToInt(lbNormalFnt->Items->Strings[0]);
		lbCutterGap->ItemIndex = Printer->CutterGap;
		lbTopMargin->ItemIndex = Printer->TopMargin;
		chbAlwaysFullCut->Checked		= Printer->AlwaysUseFullCut;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::lbNormalFntClick(TObject *Sender)
{
	if(lbPrinters->ItemIndex != -1)
   {
      TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().FetchByName(lbPrinters->Items->Strings[lbPrinters->ItemIndex],Windows_Printer);
      if(Printer)
      {
         Printer->NormalCharPerLine = StrToInt(lbNormalFnt->Items->Strings[lbNormalFnt->ItemIndex]);
      }

      AnsiString PrinterName = lbPrinters->Items->Strings[lbPrinters->ItemIndex];
      AnsiString RegKey;
      if(PrinterName.SubString(1,2) == "\\\\")
      {
         RegKey = PrinterName.SubString(3,PrinterName.Length()-2);
      }
      else
      {
         RegKey = PrinterName;
		}
		int ProfileKey = VM->SetProfile(eEquipmentProfiles,RegKey);
		VM->SetProfileInt(ProfileKey,vmNormalCPL, StrToInt(lbNormalFnt->Items->Strings[lbNormalFnt->ItemIndex]));
}
	else
   {
		Application->MessageBox("Please Select a Printer.","Information",
      MB_OK + MB_ICONINFORMATION);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::lbLargeFntClick(TObject *Sender)
{
	if(lbPrinters->ItemIndex != -1)
   {
      TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().FetchByName(lbPrinters->Items->Strings[lbPrinters->ItemIndex],Windows_Printer);
      if(Printer)
      {
         Printer->BoldCharPerLine = StrToInt(lbLargeFnt->Items->Strings[lbLargeFnt->ItemIndex]);
      }

      AnsiString PrinterName = lbPrinters->Items->Strings[lbPrinters->ItemIndex];
      AnsiString RegKey;
      if(PrinterName.SubString(1,2) == "\\\\")
      {
         RegKey = PrinterName.SubString(3,PrinterName.Length()-2);
      }
      else
      {
         RegKey = PrinterName;
      }
		int ProfileKey = VM->SetProfile(eEquipmentProfiles,RegKey);
		VM->SetProfileInt(ProfileKey,vmNormalBoldCPL, StrToInt(lbLargeFnt->Items->Strings[lbLargeFnt->ItemIndex]));
	}
	else
   {
		Application->MessageBox("Please Select a Printer.","Information",
      MB_OK + MB_ICONINFORMATION);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::lbCutterGapClick(TObject *Sender)
{
	if(lbPrinters->ItemIndex != -1)
   {
      TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().FetchByName(lbPrinters->Items->Strings[lbPrinters->ItemIndex],Windows_Printer);
      if(Printer)
      {
         Printer->CutterGap = lbCutterGap->ItemIndex;
      }

      AnsiString PrinterName = lbPrinters->Items->Strings[lbPrinters->ItemIndex];
      AnsiString RegKey;
      if(PrinterName.SubString(1,2) == "\\\\")
      {
         RegKey = PrinterName.SubString(3,PrinterName.Length()-2);
      }
      else
      {
         RegKey = PrinterName;
      }
		int ProfileKey = VM->SetProfile(eEquipmentProfiles,RegKey);
		VM->SetProfileInt(ProfileKey,vmCutterGap, lbCutterGap->ItemIndex);
}
   else
	{
		Application->MessageBox("Please Select a Printer.","Information",
      MB_OK + MB_ICONINFORMATION);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::lbTopMarginClick(TObject *Sender)
{
	if(lbPrinters->ItemIndex != -1)
	{
      TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().FetchByName(lbPrinters->Items->Strings[lbPrinters->ItemIndex],Windows_Printer);
      if(Printer)
      {
			Printer->KitchenTopMargin = lbTopMargin->ItemIndex;
      }

      AnsiString PrinterName = lbPrinters->Items->Strings[lbPrinters->ItemIndex];
      AnsiString RegKey;
      if(PrinterName.SubString(1,2) == "\\\\")
      {
         RegKey = PrinterName.SubString(3,PrinterName.Length()-2);
      }
      else
      {
         RegKey = PrinterName;
		}
		int ProfileKey = VM->SetProfile(eEquipmentProfiles,RegKey);
		VM->SetProfileInt(ProfileKey,vmKitchenTopMargin, Printer->KitchenTopMargin);
	}
   else
   {
		Application->MessageBox("Please Select a Printer.","Information",
      MB_OK + MB_ICONINFORMATION);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::cbCarbonCopyClick(
      TObject *Sender)
{
	TComms::Instance().CarbonCopy = cbCarbonCopy->Checked;
	VM->SetDeviceBool(vmDuplicateReceipts,cbCarbonCopy->Checked);
   if (cbCarbonCopy->Checked)
   {
	}
	else
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::cbPrintNoteWithDiscountClick(
      TObject *Sender)
{
   VM->SetDeviceBool(vmPrintNoteWithDiscount,cbPrintNoteWithDiscount->Checked);
   if(cbPrintNoteWithDiscount->Checked)
   {
		if(Receipt)
      {
         Receipt->PrintNoteWithDiscount = true;
      }
   }
   else
   {
		if(Receipt)
      {
         Receipt->PrintNoteWithDiscount = false;
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::cbKitchenFontClick(
      TObject *Sender)
{
	VM->SetDeviceBool(vmLargeKitchenFont,!cbKitchenFont->Checked);
	Kitchen->LargeFont = !cbKitchenFont->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::btnCloseClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;

   	ScaleBy(Screen->Width, Temp);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::rbCutByNoneClick(TObject *Sender)
{
	VM->SetDeviceInt(vmCutType,0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::cbCourseHeaderClick(
      TObject *Sender)
{
	if (rbCutByCourse->Checked) rbCutByNone->Checked = true;
	VM->SetDeviceBool(vmHeaderForEachCourse, cbCourseHeader->Checked);
	Kitchen->HeaderForEachCourse = cbCourseHeader->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::rbCutByCourseClick(
      TObject *Sender)
{
	cbCourseHeader->Checked = true;
	rbCutByCourse->Checked = true;
	VM->SetDeviceInt(vmCutType,2);
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::btnTestPrintClick(
      TObject *Sender)
{
	if (lbPrinters->ItemIndex != -1)
	{
		TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().FetchByName(lbPrinters->Items->Strings[lbPrinters->ItemIndex],Windows_Printer);
		if (Printer)
		{
			TPrintFormat *pPrinter			= new TPrintFormat;
			try
			{
				pPrinter->EmulationMode								= emEpson;
				pPrinter->NormalWidth								= 55;
				pPrinter->BoldWidth									= 55;
				pPrinter->CutterGap									= Printer->CutterGap;
				pPrinter->TopMargin									= Printer->TopMargin;
				pPrinter->AlwaysUseFullCut							= chbAlwaysFullCut->Checked;

				pPrinter->WordWrap									= false;

				pPrinter->Line->ColCount							= 1;
				pPrinter->Line->Bold									= false;
				pPrinter->Line->FontHeight							= fsNormalSize;
				pPrinter->Line->FontWidth							= fsNormalSize;
				pPrinter->Line->Columns[0]->Width				= 55;
				pPrinter->Line->Columns[0]->Alignment			= taLeftJustify;

				pPrinter->Line->Columns[0]->Text					= "First Line";
				pPrinter->AddLine();
				pPrinter->NewLine();
				pPrinter->NewLine();
				pPrinter->Line->Columns[0]->Text					= "Normal Font       20-234567-30-234567-40-234567-50-2345";
				pPrinter->AddLine();
				pPrinter->NewLine();
				pPrinter->Line->Bold									= true;
				pPrinter->Line->Columns[0]->Text					= "Bold Font         20-234567-30-234567-40-234567-50-2345";
				pPrinter->AddLine();
				pPrinter->NewLine();
				pPrinter->NewLine();
				pPrinter->Line->Bold									= false;
				pPrinter->Line->Columns[0]->Text					= "Last Line";
				pPrinter->AddLine();
				pPrinter->PartialCut();
				pPrinter->Print(Printer);
			}
			__finally
			{
				delete pPrinter;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrinterConfiguration::chbAlwaysFullCutClick(TObject *Sender)
{
	if(lbPrinters->ItemIndex != -1)
	{
		AnsiString PrinterName = lbPrinters->Items->Strings[lbPrinters->ItemIndex];

		TConnectPrinter *Printer = (TConnectPrinter *)TComms::Instance().FetchByName(PrinterName, Windows_Printer);
		if(Printer)
		{
			Printer->AlwaysUseFullCut = chbAlwaysFullCut->Checked;
			Printer->PrinterModel = 0;
		}

		AnsiString RegKey;
		if(PrinterName.SubString(1,2) == "\\\\")
		{
			RegKey = PrinterName.SubString(3, PrinterName.Length()-2);
		}
		else
		{
			RegKey = PrinterName;
		}
		int ProfileKey = VM->SetProfile(eEquipmentProfiles,RegKey);
		VM->SetProfileBool(ProfileKey,vmAlwaysUseFullCut, chbAlwaysFullCut->Checked);
	}
	else
	{
		Application->MessageBox("Please Select a Printer.","Information",
		MB_OK + MB_ICONINFORMATION);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterConfiguration::cbUnravelKitchenPrintingClick(
      TObject *Sender)
{
	VM->SetDeviceBool(vmUnRavelKitchenPrinting,cbUnravelKitchenPrinting->Checked);
	Kitchen->UnRavelKitchenPrinting = cbUnravelKitchenPrinting->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterConfiguration::edSideBulletMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	frmTouchKeyboard->MaxLength = 255;        
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = Kitchen->BulletSide;
	frmTouchKeyboard->Caption = "Enter the Bullet to go in front of all sides.";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		VM->SetDeviceStr(vmKitchenBulletSide,frmTouchKeyboard->KeyboardText);
		Kitchen->BulletSide = frmTouchKeyboard->KeyboardText;
		edSideBullet->Text = Kitchen->BulletSide;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterConfiguration::edOptionsBulletMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	frmTouchKeyboard->MaxLength = 255;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = Kitchen->BulletOpt;
	frmTouchKeyboard->Caption = "Enter the Bullet to go in front of all Options.";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		VM->SetDeviceStr(vmKitchenBulletOpt,frmTouchKeyboard->KeyboardText);
		Kitchen->BulletOpt = frmTouchKeyboard->KeyboardText;
		edOptionsBullet->Text = Kitchen->BulletOpt;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterConfiguration::edPerSeatMultiplierMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	frmTouchKeyboard->MaxLength = 255;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = Kitchen->DishesPerSeatMultiplerString;
	frmTouchKeyboard->Caption = "Enter the per seat multiplier ( 3 on 4 ).";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		VM->SetDeviceStr(vmKitchenPerSeatMultiplerString,frmTouchKeyboard->KeyboardText);
		Kitchen->DishesPerSeatMultiplerString = frmTouchKeyboard->KeyboardText;
		edPerSeatMultiplier->Text = Kitchen->DishesPerSeatMultiplerString;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterConfiguration::edNoteHeaderMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	frmTouchKeyboard->MaxLength = 255;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = Kitchen->NoteHeader;
	frmTouchKeyboard->Caption = "Enter the header that precedes a note.";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		VM->SetDeviceStr(vmKitchenHeaderNote,frmTouchKeyboard->KeyboardText);
		Kitchen->NoteHeader = frmTouchKeyboard->KeyboardText;
		edNoteHeader->Text = Kitchen->NoteHeader;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterConfiguration::edSidesHeaderMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	frmTouchKeyboard->MaxLength = 255;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = Kitchen->SideHeader;
	frmTouchKeyboard->Caption = "Enter the header that precedes the sides.";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		VM->SetDeviceStr(vmKitchenHeaderSide,frmTouchKeyboard->KeyboardText);
		Kitchen->SideHeader = frmTouchKeyboard->KeyboardText;
		edSidesHeader->Text = Kitchen->SideHeader;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrinterConfiguration::edMixHeaderMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	frmTouchKeyboard->MaxLength = 255;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = Kitchen->MixHeader;
	frmTouchKeyboard->Caption = "Enter the header that precedes the beverage mix.";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		VM->SetDeviceStr(vmKitchenHeaderMix,frmTouchKeyboard->KeyboardText);
		Kitchen->MixHeader = frmTouchKeyboard->KeyboardText;
		edMixHeader->Text = Kitchen->MixHeader;
	}
}
//---------------------------------------------------------------------------

