//---------------------------------------------------------------------------

#ifndef ComboFormH
#define ComboFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ZForm.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ItemComplete.h"
#include "ItemCompleteSub.h"
#include "ItemMinorComplete.h"
#include "PnMOrder.h"


#include "MMLogging.h"

template <typename T>
class TItemOrders
{
	public:
	std::vector<T>::iterator begin(){ return data.begin();}
	std::vector<T>::iterator end(){ return data.end();}
	void erase(std::vector<T>::iterator it){ data.erase(it);}
	void insert(T value){ data.push_back(value);}
	std::vector<T> GetData(){ return data;}
	int size(){return data.size();}
	std::vector<T>::iterator find(T value)
	{
		for(std::vector<T>::iterator it = begin(); it != end(); ++it)
			if(*it == value)
				return it;
		return NULL;
	}
	T getelement(int pos)
	{
		if(data.size() > pos)
			return data.at(pos);
		else
			return NULL;
	}

void organiseints(void)
{
	 bool bDone = false;
    while (!bDone)
    {
		  bDone = true;
		  for (int i = 0; i != data.size() - 1; ++i)
		  {
				if ( data.at(i) > data.at(i+1) )
				{
					 int tmp = data.at(i);
					 data.at(i) = data.at(i+1);
					 data.at(i+1) = tmp;
					 bDone = false;
            }
        }
	 }
}

	private:
	std::vector<T> data;
};
//---------------------------------------------------------------------------

struct RowsInfo
{
   int MaxQuantityAffected;
   TItemOrders<int> Rows;
};

class TfrmComboForm : public TZForm
{
__published:	// IDE-managed Components
    TStringGrid *sgShowItemList;
    TPanel *pnlShowItemList;
    TPanel *pnlShowList;
    TEdit *edCurrentPoints;
    TEdit *edPointsSelected;
    TEdit *edPointsRemaining;
    TPanel *pnlShowPointsSections;
    TLabel *lblCurrentPoints;
    TLabel *lblPointsSelected;
    TLabel *lblPointsRemaining;
    TLabel *lblMember;
    TTouchBtn *btnOk;
    TTouchBtn *btnClose;
    TLabel *lblMemberName;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnCloseAutoRepeat(TObject *Sender);
    void __fastcall btnOkAutoRepeat(TObject *Sender);
    void __fastcall sgShowItemListSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
    void __fastcall sgShowItemListDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);

private:	// User declarations
   void AddItemsToGrid();
   void AddItemToGrid(TItemMinorComplete* order, int index);
   void SetRowCount();
   Currency GetPrice(TItemMinorComplete *order);
   void InsertItemRowInDiscountMap(TItemMinorComplete *item,int rowIndex);
   void DeSelectRow(TItemMinorComplete *item,int rowIndex);
public:		// User declarations
        __fastcall TfrmComboForm(TComponent* Owner);
        TList *OrdersList;
        TItemOrders<int> RowOrders;
        std::vector<TItemMinorComplete *> Items;
        std::map<UnicodeString,RowsInfo> SelectedRowMap;
        std::map<int,RowsInfo> DiscountedRowMap;
        Currency MemberCurrentPoints;
        UnicodeString MemberName;
        Currency SelectedPoints;
        Currency PriceSelected;
        Currency MembersPoints;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmComboForm *frmComboForm;
//---------------------------------------------------------------------------
#endif
