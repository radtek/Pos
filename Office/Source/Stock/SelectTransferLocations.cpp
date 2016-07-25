//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectTransferLocations.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
bool TSelectTransferLocations::Execute()
{
	// Connected to MM db?
	if (dmMMData->dbMenuMate->Connected)	// You should do this check before calling this function, and show appropriate message!
	{
		fMenu.GetAvailableLocations(&fLocationsInfo);
		// Are there any locations?
		if (!fLocationsInfo.Locations.empty())
		{
			frmSelectTransferLocations->lbSource->Items->Clear();
			frmSelectTransferLocations->lbDestination->Items->Clear();
			for (unsigned i=0; i<fLocationsInfo.Locations.size(); i++)
			{
				frmSelectTransferLocations->lbSource->Items->Add(fLocationsInfo.Locations[i]);
				frmSelectTransferLocations->lbDestination->Items->Add(fLocationsInfo.Locations[i]);
			}
	   /*		Stock::TLocationSet StockLocationSet;
			fStockControl.GetLocations(StockLocationSet);
			for (Stock::TiLocationSet i=StockLocationSet.begin(); i!=StockLocationSet.end(); i++)
			{
				if (frmSelectTransferLocations->lbSource->Items->IndexOf(*i) == -1)
				{
					frmSelectTransferLocations->lbSource->Items->Add(*i);
				}
			}   */
			// Valid UI selection?
			if (frmSelectTransferLocations->ShowModal() == mrOk)
			{
				fTransferLocations.Source			= frmSelectTransferLocations->lbSource->Items->Strings[
																	frmSelectTransferLocations->lbSource->ItemIndex];
				fTransferLocations.Destination	= frmSelectTransferLocations->lbDestination->Items->Strings[
																	frmSelectTransferLocations->lbDestination->ItemIndex];
				return true;
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------------
TTransferLocations TSelectTransferLocations::Locations()
{
	return TTransferLocations(fTransferLocations);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TfrmSelectTransferLocations::TfrmSelectTransferLocations(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectTransferLocations::btnOkClick(TObject *Sender)
{
	if (lbSource->ItemIndex > -1 && lbDestination->ItemIndex > -1 &&
		 lbSource->Items->Strings[lbSource->ItemIndex] != lbDestination->Items->Strings[lbDestination->ItemIndex])  //cww
	{
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
