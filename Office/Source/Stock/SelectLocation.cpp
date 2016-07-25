//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectLocation.h"
#include "MMData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmSelectLocation *frmSelectLocation;
//---------------------------------------------------------------------------
__fastcall TfrmSelectLocation::TfrmSelectLocation(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocation::FormShow(TObject *Sender)
{
	if (!Transaction->InTransaction)
	{
		Transaction->StartTransaction();
	}

        qrLocationList->Close();
        qrLocationList->Open();
        DBGrid1->Columns->Items[0]->Width = DBGrid1->ClientWidth;
        
}
//---------------------------------------------------------------------------
bool TfrmSelectLocation::Execute(AnsiString &Location)
{
	bool Result = false;
	ShowModal();
	if (ModalResult == mrOk)
	{
		Location = qrLocationList->FieldByName("Name")->AsString;
		Result = true;
	}
	else if (ModalResult == mrYes)
	{
		Location = "";
		if (InputQuery("New Location", "Please enter the name of the new Location.", Location))
		{
			if (Location != "")
			{
				qrFindLocation->Close();
				qrFindLocation->ParamByName("Name")->AsString = Location.UpperCase();
				qrFindLocation->ExecQuery();
				if (qrFindLocation->Eof)
				{
					//Add the location!
					sqlAddLK->Close();
					sqlAddLK->ExecQuery();

					qrAddLocation->Close();
					qrAddLocation->ParamByName("Location_Key")->AsInteger		= sqlAddLK->Fields[0]->AsInteger;
					qrAddLocation->ParamByName("Location_Type")->AsInteger	= 1;
					qrAddLocation->ParamByName("Name")->AsString					= Location;
					qrAddLocation->ExecQuery();
				}
				else
				{
					// Found one. Does its type need updating?
					if (qrFindLocation->FieldByName("Location_Type")->AsInteger == 0)
					{
						qrUpdateLocation->Close();
						qrUpdateLocation->ParamByName("Location_Key")->AsInteger		= qrFindLocation->FieldByName("Location_Key")->AsInteger;
						qrUpdateLocation->ParamByName("Location_Type")->AsInteger	= 2;
						qrUpdateLocation->ExecQuery();
						Location																		= qrFindLocation->FieldByName("Name")->AsString;
					}
					else
					{
						// Naa, just didn't see it in the list!
						Location = qrFindLocation->FieldByName("Name")->AsString;
					}
				}
				Result = true;
			}
		}
		else
		{
			Result = false;
		}
	}
	else
	{
		Location = "";
		Result = false;
	}
	if (Transaction->InTransaction)
	{
		Transaction->Commit();
	}
	return Result;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocation::BitBtn2Click(TObject *Sender)
{
	ModalResult = mrYes; // Will be detected in Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectLocation::DBGrid1DblClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

