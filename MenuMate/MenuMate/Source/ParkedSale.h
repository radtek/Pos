//---------------------------------------------------------------------------

#ifndef ParkedSaleH
#define ParkedSaleH

#include "ContactMemberApplied.h"
#include "SeatOrders.h"
//---------------------------------------------------------------------------
class TParkedSale
{
private:
    UnicodeString GetThirdPartyCode(long key);
    int GetRevenueCode(int _itemKey, UnicodeString _sizeName);
public:
   bool StoreMembership;
   TContactMemberApplied Membership;

   UnicodeString SelectedTabContainerName;
	UnicodeString ParkedBy;
   UnicodeString SelectedParty;

   int SelectedTable;
   int SelectedSeat;

	TDateTime Time;

   std::vector<TSeatOrders *> Orders;

   void SetSale(UnicodeString inParkedBy, std::vector<TSeatOrders *> SeatOrders);
	void AssignParkedSale(std::vector<TSeatOrders *> &SeatOrders);
	void GetSaleCopy(std::vector<TSeatOrders *> &SeatOrders);
	TParkedSale();
	virtual __fastcall ~TParkedSale();
};

#endif
