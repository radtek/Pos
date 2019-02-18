// ---------------------------------------------------------------------------
#pragma hdrstop

#include "ManagerPhysicalPrinter.h"
#include "MMLogging.h"
#include "ManagerVariable.h"

#include "Enum.h"
#include "StrUtils.hpp"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif
// ---------------------------------------------------------------------------

#pragma package(smart_init)

TManagerPhysicalPrinter::TManagerPhysicalPrinter()
{
}

TManagerPhysicalPrinter::~TManagerPhysicalPrinter()
{
}

void TManagerPhysicalPrinter::Initialise(Database::TDBTransaction &DBTransaction, int inDeviceKey, UnicodeString ComputerName)
{
   if (Win32Platform == VER_PLATFORM_WIN32_NT)
   {
	  InitialisePrinters(DBTransaction, inDeviceKey, ComputerName, /* PRINTER_ENUM_CONNECTIONS | */ PRINTER_ENUM_LOCAL);
   }
   else
   {
	  InitialisePrinters(DBTransaction, inDeviceKey, ComputerName, PRINTER_ENUM_LOCAL);
   }
}

int TManagerPhysicalPrinter::DBAddPrinter(Database::TDBTransaction &DBTransaction, TPrinterPhysical &Printer)
{

   try
   {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  Printer.PhysicalPrinterKey = GetPrinterKeyByShareName(DBTransaction, Printer.ServerName, Printer.ShareName);

	  if (Printer.PhysicalPrinterKey == 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_PHYSICALPRINTER, 1) FROM RDB$DATABASE";
		 IBInternalQuery->ExecQuery();
		 Printer.PhysicalPrinterKey = IBInternalQuery->Fields[0]->AsInteger;

		 IBInternalQuery->Close();

		 IBInternalQuery->SQL->Text =
			 "INSERT INTO PHYSICALPRINTER (" "PHYSICALPRINTER_KEY," "PRINTER_TYPE," "SHARE_NAME," "COMPUTER_NAME,"
			 "PROFILE_KEY) " "VALUES (" ":PHYSICALPRINTER_KEY," ":PRINTER_TYPE," ":SHARE_NAME," ":COMPUTER_NAME,"
			 ":PROFILE_KEY);";
		 IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = Printer.PhysicalPrinterKey;
		 IBInternalQuery->ParamByName("PRINTER_TYPE")->AsInteger = Printer.Type;
		 IBInternalQuery->ParamByName("SHARE_NAME")->AsString = Printer.ShareName.SubString(1, MAX_SHARENAME_LENGTH);
		 IBInternalQuery->ParamByName("COMPUTER_NAME")->AsString = Printer.ServerName;
		 IBInternalQuery->ParamByName("PROFILE_KEY")->AsInteger = Printer.PhysicalProfileKey;
		 IBInternalQuery->ExecQuery();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return Printer.PhysicalPrinterKey;
}

void TManagerPhysicalPrinter::DBDeletePrinter(Database::TDBTransaction &DBTransaction, TPrinterPhysical &Printer)
{

   try
   {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  if (Printer.PhysicalPrinterKey != 0)
	  {
		 IBInternalQuery->Close();
		 IBInternalQuery->SQL->Text = "DELETE FROM PHYSICALPRINTER WHERE PHYSICALPRINTER_KEY = :PHYSICALPRINTER_KEY";
		 IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = Printer.PhysicalPrinterKey;
		 IBInternalQuery->ExecQuery();
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

int TManagerPhysicalPrinter::GetPrinterKeyByShareName(Database::TDBTransaction &DBTransaction, UnicodeString ServerName,
   UnicodeString ShareName)
{
   int Retval = 0;

   try
   {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();
	  IBInternalQuery->SQL->Text = " SELECT " " PHYSICALPRINTER_KEY " " FROM " "  PHYSICALPRINTER " " WHERE "
		  "  SHARE_NAME = :SHARE_NAME AND" "  COMPUTER_NAME = :COMPUTER_NAME";
	  IBInternalQuery->ParamByName("SHARE_NAME")->AsString = ShareName.SubString(1, MAX_SHARENAME_LENGTH);
	  IBInternalQuery->ParamByName("COMPUTER_NAME")->AsString = ServerName;
	  IBInternalQuery->ExecQuery();
	  Retval = IBInternalQuery->FieldByName("PHYSICALPRINTER_KEY")->AsInteger;
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return Retval;
}

int TManagerPhysicalPrinter::LoadProfile(Database::TDBTransaction &DBTransaction, TPrinterPhysical &Printer)
{
   /* This is an effort to make the printing code more thread save and to now use the global singleton of TManagerVariable
     As it's Caching is not thread safe */

   std::auto_ptr<TManagerVariable> PrinterVariables(new TManagerVariable());
   PrinterVariables->LocationProfileKey = TManagerVariable::Instance().LocationProfileKey;
   PrinterVariables->DeviceProfileKey = TManagerVariable::Instance().DeviceProfileKey;

   int ProfileKey = PrinterVariables->GetProfile(DBTransaction, eEquipmentProfiles, Printer.ServerName + Printer.ShareName);
   try
   {
       Printer.PhysicalProfileKey = ProfileKey;
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmPrinterModel, Printer.PrinterModel);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmNormalCPL, Printer.NormalCharPerLine);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmNormalBoldCPL, Printer.BoldCharPerLine);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmCutterGap, Printer.CutterGap);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmTopMargin, Printer.TopMargin);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmKitchenTopMargin, Printer.KitchenTopMargin);
       int EmulationMode = static_cast <int> (Printer.EmulationMode);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmPrinterEmulationMode, EmulationMode);
       Printer.EmulationMode = static_cast <TEmulationMode> (EmulationMode);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmSingleLineChar, Printer.SingleLineChar);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmDoubleLineChar, Printer.DoubleLineChar);
       PrinterVariables->GetProfileBool(DBTransaction, ProfileKey, vmAlwaysUseFullCut, Printer.AlwaysUseFullCut);
       PrinterVariables->GetProfileBool(DBTransaction, ProfileKey, vmKanjiPrinter, Printer.KanjiPrinter);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmPrinterCodePage, Printer.PrinterCodePage);


       PrinterVariables->GetProfileStr(DBTransaction, ProfileKey, vmPrinterInitCmd, Printer.PrinterInitCmd);
       PrinterVariables->GetProfileStr(DBTransaction, ProfileKey, vmPrinterCodePageOn, Printer.PrinterCodePageOn);
       PrinterVariables->GetProfileStr(DBTransaction, ProfileKey, vmPrinterCodePageOff, Printer.PrinterCodePageOff);

       PrinterVariables->GetProfileBool(DBTransaction, ProfileKey, vmGraphicPrinter, Printer.Graphics.GraphicPrinter);
       PrinterVariables->GetProfileStr(DBTransaction, ProfileKey, vmGraphicFontName, Printer.Graphics.GraphicFontName);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicFontSize, Printer.Graphics.GraphicFontSize);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicHeaderX, Printer.Graphics.GraphicHeaderX);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicHeaderY, Printer.Graphics.GraphicHeaderY);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicHeaderWidth, Printer.Graphics.GraphicHeaderWidth);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicHeaderHeight, Printer.Graphics.GraphicHeaderHeight);

       PrinterVariables->GetProfileStr(DBTransaction, ProfileKey, vmGraphicHeaderFile, Printer.Graphics.GraphicHeaderFile);

       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicReceiptX, Printer.Graphics.GraphicReceiptX);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicReceiptY, Printer.Graphics.GraphicReceiptY);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicReceiptMaxLines, Printer.Graphics.GraphicReceiptMaxLines);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicBackGroundX, Printer.Graphics.GraphicBackGroundX);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicBackGroundY, Printer.Graphics.GraphicBackGroundY);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicBackGroundWidth, Printer.Graphics.GraphicBackGroundWidth);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicBackGroundHeight, Printer.Graphics.GraphicBackGroundHeight);
       PrinterVariables->GetProfileStr(DBTransaction, ProfileKey, vmGraphicBackGroundFile, Printer.Graphics.GraphicBackGroundFile);

       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicPageCountX, Printer.Graphics.GraphicPageCountX);
       PrinterVariables->GetProfileInt(DBTransaction, ProfileKey, vmGraphicPageCountY, Printer.Graphics.GraphicPageCountY);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return ProfileKey;
}

TPrinterPhysical TManagerPhysicalPrinter::GetPhysicalPrinter(Database::TDBTransaction &DBTransaction, int inPhysicalPrinterKey)
{
   TPrinterPhysical RetVal;

   try
   {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  if (inPhysicalPrinterKey != 0)
	  {
		 IBInternalQuery->Close();

		 IBInternalQuery->SQL->Text = "SELECT * FROM PHYSICALPRINTER " "WHERE PHYSICALPRINTER_KEY = :PHYSICALPRINTER_KEY";
		 IBInternalQuery->ParamByName("PHYSICALPRINTER_KEY")->AsInteger = inPhysicalPrinterKey;
		 IBInternalQuery->ExecQuery();
		 if (IBInternalQuery->RecordCount)
		 {
			RetVal.PhysicalPrinterKey = IBInternalQuery->FieldByName("PHYSICALPRINTER_KEY")->AsInteger;
			RetVal.ShareName = IBInternalQuery->FieldByName("SHARE_NAME")->AsString;
			RetVal.ServerName = IBInternalQuery->FieldByName("COMPUTER_NAME")->AsString;
			RetVal.Type = (TPrinterType)IBInternalQuery->FieldByName("PRINTER_TYPE")->AsInteger;
			RetVal.PhysicalProfileKey = IBInternalQuery->FieldByName("PROFILE_KEY")->AsInteger;
			LoadProfile(DBTransaction, RetVal);
		 }
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return RetVal;
}

void TManagerPhysicalPrinter::InitialisePrinters(Database::TDBTransaction &DBTransaction, int inDeviceKey, UnicodeString inComputerName,
   DWORD Flags)
{
   try
   {
	  DWORD dwNeeded, dwNumPrinters;
	  EnumPrinters(Flags, NULL, 2, NULL, 0, &dwNeeded, &dwNumPrinters);

	  unsigned char *buffer = NULL;
	  try
	  {
		 buffer = new unsigned char[dwNeeded];
		 setmem(buffer, dwNeeded, 0);
		 EnumPrinters(Flags, NULL, 2, buffer, dwNeeded, &dwNeeded, &dwNumPrinters);

		 LPPRINTER_INFO_2 lppi2;

		 for (int index = 0; index < (int)dwNumPrinters; index++)
		 {
			lppi2 = (LPPRINTER_INFO_2)(buffer + index*sizeof(PRINTER_INFO_2));

			TPrinterPhysical PrinterPhysical;
			PrinterPhysical.ServerName = lppi2->pServerName;
			PrinterPhysical.ServerName = AnsiReplaceText(PrinterPhysical.ServerName, "\\\\", "");
			PrinterPhysical.ShareName = lppi2->pShareName;

			UnicodeString FullPrinterPath = lppi2->pPortName;
			if (FullPrinterPath.SubString(1, 2) == "\\\\")
			{
			   int ServerNameStart = 3, ServerNameLength;
			   ServerNameLength = FullPrinterPath.SubString(ServerNameStart, FullPrinterPath.Length()).Pos("\\") - 1;
			   if (ServerNameLength > 0)
			   {
				  PrinterPhysical.ServerName = FullPrinterPath.SubString(ServerNameStart, ServerNameLength);
			   }
			   int PrinterNameStart = ServerNameStart + ServerNameLength + 1;
			   UnicodeString ShareName = FullPrinterPath.SubString(PrinterNameStart, FullPrinterPath.Length());
			   PrinterPhysical.ShareName = ShareName;
			}

			if (PrinterPhysical.ServerName == "")
			{
			   PrinterPhysical.ServerName = inComputerName;
			}

			if (PrinterPhysical.ShareName == "")
			{
			   PrinterPhysical.ShareName = lppi2->pPrinterName;
			}

			DBAddPrinter(DBTransaction, PrinterPhysical);
			LoadProfile(DBTransaction, PrinterPhysical);
		 }
	  }
	  __finally
	  {
		 delete[]buffer;
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TManagerPhysicalPrinter::GetPrinterServerList(Database::TDBTransaction &DBTransaction, TStrings *StringsList, TPrinterType Type)
{

   try
   {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();

	  IBInternalQuery->SQL->Text = "SELECT COMPUTER_NAME FROM PHYSICALPRINTER WHERE PRINTER_TYPE = :PRINTER_TYPE " "ORDER BY COMPUTER_NAME";
	  IBInternalQuery->ParamByName("PRINTER_TYPE")->AsInteger = Type;
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 StringsList->Add(IBInternalQuery->FieldByName("COMPUTER_NAME")->AsString);
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TManagerPhysicalPrinter::DBGetValidPrinterList(Database::TDBTransaction &DBTransaction, int DeviceKey, TStrings *StringsList)
{

   try
   {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();

	  IBInternalQuery->SQL->Text = "SELECT * FROM PHYSICALPRINTER "
		  " ORDER BY COMPUTER_NAME,SHARE_NAME";
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 StringsList->AddObject("[" + IBInternalQuery->FieldByName("COMPUTER_NAME")->AsString + "] " + IBInternalQuery->FieldByName
			("SHARE_NAME")->AsString, (TObject*)(IBInternalQuery->FieldByName("PHYSICALPRINTER_KEY")->AsInteger));
	  }
	  IBInternalQuery->Close();
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

void TManagerPhysicalPrinter::DBGetPrinterList(Database::TDBTransaction &DBTransaction, TStrings *StringsList)
{

   try
   {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();

	  IBInternalQuery->SQL->Text = "SELECT * FROM PHYSICALPRINTER " "ORDER BY COMPUTER_NAME,SHARE_NAME";
	  IBInternalQuery->ExecQuery();
	  for (; !IBInternalQuery->Eof; IBInternalQuery->Next())
	  {
		 StringsList->AddObject("[" + IBInternalQuery->FieldByName("COMPUTER_NAME")->AsString + "] " + IBInternalQuery->FieldByName
			("SHARE_NAME")->AsString, (TObject*)(IBInternalQuery->FieldByName("PHYSICALPRINTER_KEY")->AsInteger));
	  }
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}

bool TManagerPhysicalPrinter::CheckIfChefMateEnabledForTerminal(Database::TDBTransaction &DBTransaction, TPrinterType Type)
{
   bool status = false;
   try
   {
      Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
	  DBTransaction.RegisterQuery(IBInternalQuery);

	  IBInternalQuery->Close();

      IBInternalQuery->SQL->Text =  "SELECT a.PHYSICALPRINTER_KEY FROM PHYSICALPRINTER a "
                                    "INNER JOIN VIRTUALPRINTER b ON a.PHYSICALPRINTER_KEY = b.PHYSICALPRINTER_KEY "
                                    "INNER JOIN DEVICEVIRTUALPRINTER c ON b.VIRTUALPRINTER_KEY = c.VIRTUALPRINTER_KEY "
                                    "WHERE a.PRINTER_TYPE = :PRINTER_TYPE AND c.DEVICE_KEY = :CURRENT_DEVICE_KEY ";
	  IBInternalQuery->ParamByName("PRINTER_TYPE")->AsInteger = Type;
      IBInternalQuery->ParamByName("CURRENT_DEVICE_KEY")->AsInteger =  TDeviceRealTerminal::Instance().ID.DeviceKey;
	  IBInternalQuery->ExecQuery();
      if(IBInternalQuery->RecordCount > 0)
        status = true;

   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
   return status;
}
