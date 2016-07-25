//---------------------------------------------------------------------------


#pragma hdrstop

#include "ParkedSale.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TParkedSale::TParkedSale()
{
   StoreMembership = false;
   SelectedTable = 0;
   SelectedSeat = 0;
   SelectedParty = "";
   SelectedTabContainerName = "";
   ParkedBy = "";
}

__fastcall TParkedSale::~TParkedSale()
{
	for (UINT i=0; i<Orders.size(); i++)
   {
   	delete (TSeatOrders *)Orders[i];
   }
}

void TParkedSale::SetSale(UnicodeString inParkedBy, std::vector<TSeatOrders *> SeatOrders)
{
	ParkedBy = inParkedBy;
	for (int i = 0; i < SeatOrders.size() ; i++)
   {
		TSeatOrders *SeatToStore = new TSeatOrders(i);
		for (int j = 0; j < SeatOrders[i]->Orders->Count ; j++)
		{
				SeatToStore->Orders->Add(SeatOrders[i]->Orders->Items[j],SeatOrders[i]->Orders->Items[j]->ItemOrderedFrom);
		}

      if(StoreMembership)
      {
   		SeatToStore->Orders->AppliedMembership = SeatOrders[i]->Orders->AppliedMembership;
      }
		SeatToStore->Orders->TotalDiscount = SeatOrders[i]->Orders->TotalDiscount;
		SeatToStore->Orders->HideServingCourseLabels = SeatOrders[i]->Orders->HideServingCourseLabels;
		Orders.push_back(SeatToStore);
   }
}

void TParkedSale::AssignParkedSale(std::vector<TSeatOrders *> &SeatOrders)
{
   // Move Orders across
	for (int i = 0; i < Orders.size() ; i++)
   {
       while (Orders[i]->Orders->Count != 0)
      {
         if(Orders[i]->Orders->Count == 1)
         {
             Orders[i]->Orders->Items[0]->LastAddedItem = true;
         }
         SeatOrders[i]->Orders->Add(Orders[i]->Orders->Items[0],Orders[i]->Orders->Items[0]->ItemOrderedFrom);
         Orders[i]->Orders->Delete(0);
      }

      while (Orders[i]->Orders->PrevCount != 0)
      {
         SeatOrders[i]->Orders->AddPrev(Orders[i]->Orders->PrevItems[0]);
         Orders[i]->Orders->DeletePrev(0);
		}

      if(StoreMembership)
      {
		   SeatOrders[i]->Orders->AppliedMembership = Orders[i]->Orders->AppliedMembership;
      }

		SeatOrders[i]->Orders->TotalDiscount = Orders[i]->Orders->TotalDiscount;
		SeatOrders[i]->Orders->HideServingCourseLabels = Orders[i]->Orders->HideServingCourseLabels;
	}



	for (UINT i=0; i<Orders.size(); i++)
   {
   	delete Orders[i];
   }
   Orders.clear();
}

void TParkedSale::GetSaleCopy(std::vector<TSeatOrders *> &SeatOrders)
{
   // Move Orders across
	for (int i = 0; (i < Orders.size()) && (1 < SeatOrders.size()); i++)
	{
		for (int j = 0; j < Orders[i]->Orders->Count; j++)
		{
			SeatOrders[i]->Orders->Add(Orders[i]->Orders->Items[j],Orders[i]->Orders->Items[j]->ItemOrderedFrom);
		}

		for (int j = 0; j < Orders[i]->Orders->PrevCount ; j++)
		{
			SeatOrders[i]->Orders->AddPrev(Orders[i]->Orders->PrevItems[j]);
		}
	}
}
