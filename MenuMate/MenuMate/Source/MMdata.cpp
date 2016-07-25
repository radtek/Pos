//--------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MMdata.h"
#include "Printing.h"
#include "Processing.h"
#include "MMMessageBox.h"
#include "Requests.h"
#include "Secure.h"
#include "Rooms.h"
#include "Invoice.h"
#include "ManagerVariable.h"
#include "TableManager.h"

#ifdef  PalmMate
	#include "Palm.h"
#endif

#ifdef MenuMate
	#include "Selectdish.h"
	#include "BillGroup.h"
	#include "Transfer.h"
#endif


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmData *frmData;

__fastcall TfrmData::TfrmData(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmData::FormCreate(TObject *Sender)
{
   // Used to streamup the latest Menu Stream.
   MenuSync1Stream = new TMemoryStream;
   MenuSync2Stream = new TMemoryStream;
	// Reconstruct DB as Nessary.
   /*HappyHourStart = new TDateTime;
	HappyHourFinish = new TDateTime;*/
}

//---------------------------------------------------------------------------

void __fastcall TfrmData::FormDestroy(TObject *Sender)
{
   delete MenuSync1Stream;
   MenuSync1Stream = NULL;
   delete MenuSync2Stream;
	MenuSync2Stream = NULL;
}

//----------------------------Order Interface--------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*
bool __fastcall ProcessCredits(TList *OrderObjects)
{
return true;
} */

//---------------------------------------------------------------------------
/*bool __fastcall TfrmData::ProcessZone(SZone *Zone)
{
   if (Zone->TabStatus == TabNone)
   {
      return true;
   }
   if ((Zone->TableNo <= 0 || Zone->SeatNo <= 0) && Zone->Tab_Key <= 0)
   {
      return false;
   }
   else if (Zone->Tab_Key >= 1)
   {  // Tab suposedly already exists.
      Zone->Tab_Key = dbTabFindOrCreate(Zone);
   }
   else
   {
      dbTableSeatFindOrCreate(Zone);
      Zone->Tab_Key = dbTabFindOrCreate(Zone);
   }
   return true;
}*/
//---------------------------------------------------------------------------


//------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TTableSeat::TTableSeat()
{
	TableNo = 0;
	SeatNo = 0;
	TableName = "";
	PartyName = "";
   SeatName = "";
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
SZone::SZone()
{
	TableNo = NULL;
	SeatNo = NULL;
	TabKey = NULL;                    // Tab LK
	TabName = "";
	TableName = "";
	PartyName = "";
}
//---------------------------------------------------------------------------



void __fastcall TfrmData::CreateNewRecord(TDataSet *DataSet)
{
	if (DataSet->State == dsInsert)
   {
      AnsiString GeneratorName = "";
      TIBTable *CurrentTable = (TIBTable *) DataSet;

      if (CurrentTable->TableName == "ARCBILL")                  { GeneratorName = "GEN_ARCBILL" ;}
      else if (CurrentTable->TableName == "ARCBILLPAY")          { GeneratorName = "GEN_ARCBILLPAY" ;}
      else if (CurrentTable->TableName == "ARCHIVE")             { GeneratorName = "GEN_ARCHIVE" ;}
      else if (CurrentTable->TableName == "CAPACITY")            { GeneratorName = "GEN_CAPACITY" ;}
      else if (CurrentTable->TableName == "CONNECTIONS")         { GeneratorName = "GEN_CONNECTIONS" ;}
      else if (CurrentTable->TableName == "COURSE")              { GeneratorName = "GEN_COURSE" ;}
	  else if (CurrentTable->TableName == "DAYARCBILL")          { GeneratorName = "GEN_DAYARCBILL" ;}
      else if (CurrentTable->TableName == "DAYARCBILLPAY")       { GeneratorName = "GEN_DAYARCBILLPAY" ;}
	  else if (CurrentTable->TableName == "DAYARCHIVE")          { GeneratorName = "GEN_DAYARCHIVE" ;}
      else if (CurrentTable->TableName == "DEVICES")             { GeneratorName = "GEN_DEVICES" ;}
      else if (CurrentTable->TableName == "ITEM")                { GeneratorName = "GEN_ITEM" ;}
      else if (CurrentTable->TableName == "OPTIONS")             { GeneratorName = "GEN_OPTION" ;}
      else if (CurrentTable->TableName == "RECIPE")              { GeneratorName = "GEN_RECIPE" ;}
      else if (CurrentTable->TableName == "ITEMSIZE")            { GeneratorName = "GEN_DRINKGLASSJN" ;}
      else if (CurrentTable->TableName == "SIZE")                { GeneratorName = "GEN_SIZE" ;}
      else if (CurrentTable->TableName == "ORDEROPTION")         { GeneratorName = "GEN_ORDEROPTION" ;}
      else if (CurrentTable->TableName == "MENU")                { GeneratorName = "GEN_MENU" ;}
      else if (CurrentTable->TableName == "ORDERMIX")            { GeneratorName = "GEN_ORDERMIX" ;}
      else if (CurrentTable->TableName == "ORDERS")              { GeneratorName = "GEN_ORDERS" ;}
      else if (CurrentTable->TableName == "PRNORDER")            { GeneratorName = "GEN_PRNORDER" ;}
      else if (CurrentTable->TableName == "RESERVATIONS")        { GeneratorName = "GEN_RESERVATIONS" ;}
      else if (CurrentTable->TableName == "RESERVETABLE")        { GeneratorName = "GEN_RESERVETABLE" ;}
      else if (CurrentTable->TableName == "SEAT")                { GeneratorName = "GEN_SEAT" ;}
      else if (CurrentTable->TableName == "ARCHIVESTOCKTRACK")   { GeneratorName = "GEN_ARCHIVESTOCKTRACK" ;}
      else if (CurrentTable->TableName == "STOCKTRACK")          { GeneratorName = "GEN_STOCKTRACK" ;}
	  else if (CurrentTable->TableName == "STOCK")               { GeneratorName = "GEN_STOCK" ;}
      else if (CurrentTable->TableName == "TABLES")              { GeneratorName = "GEN_TABLES" ;}
      else if (CurrentTable->TableName == "TAB")                 { GeneratorName = "GEN_TAB" ;}
      else if (CurrentTable->TableName == "USERSTIME")           { GeneratorName = "GEN_USERSTIME" ;}
      else if (CurrentTable->TableName == "USERS")               { GeneratorName = "GEN_USERS" ;}
      else {ShowMessage("Unknown Table to Find LK for " + CurrentTable->TableName);}

      if(GeneratorName != "")
      {
         int key;
			IBSQL->Close();
			IBSQL->SQL->Text = "SELECT GEN_ID(" + GeneratorName+ ", 1) FROM RDB$DATABASE";
			IBSQL->ExecQuery();
			key = IBSQL->Fields[0]->AsInteger;
			DataSet->Fields->Fields[0]->AsInteger = key;
		}
   }
}


