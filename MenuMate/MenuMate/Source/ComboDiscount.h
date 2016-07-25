//---------------------------------------------------------------------------

#ifndef ComboDiscountH
#define ComboDiscountH

#include "Discount.h"
#include "ListManager.h"
#include "ItemMinorComplete.h"
#include "ItemCompleteSub.h"
//---------------------------------------------------------------------------

template <typename T>
class TComboOrders
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

class TfrmComboGUI
{
	public:
	TfrmComboGUI();
	TfrmComboGUI(TForm *inDisplayOwner, TDiscount Discount);
	TfrmComboGUI(TForm *inDisplayOwner, TDiscount Discount, TList *inOrders);
	void LoadGUI(AnsiString Name, Currency Price, int pos);
	void DisplayGUI(Currency Max);
    void DisplayGUI(Currency Max, const int ItemCount);
	TComboOrders<int> GetSelectedOrders(void) {return Orders;}
	TModalResult MResult;
    bool MemberSelected;

	private:
	void InitialiseGUI(AnsiString Caption, const int rows);
	void CreateEdit();
	void ChangeObject(TControl *Object,
							int height,
							int width,
							int top,
							int left);
	void OnSelect(int Row, int Col);
	void OnClose(int Row, int Col);
	void OnCancel(int Row, int Col);
	void OnDrawCell(int Row, int Col);

	TComboOrders<int> Orders;
	TForm *Owner;
	std::auto_ptr <TfrmListManager> frmListManager;
	std::vector<Currency> Prices;
	Currency Total, Max;
	TEdit *Edit[3];
	TLabel *Label[3];
    int MaxItemAffected;
    int MinItemRequired;
};

class TComboDiscount
{
	public:
    TComboDiscount();
	TComboDiscount(TForm *inDisplayOwner, TDiscount inDiscount, TList *inOrders);
	TList *GetOrders(void);
    bool IsMemberSelected;

	private:
	void Initialise(TList *Orders);
    void addOrderToCollection(TItemMinorComplete* order, int index);
	TList *List;
//	TComboOrders<TItemMinorComplete *> Orders1;
	TDiscount Discount;
	TComboOrders<TItemMinorComplete *> Orders;
	TComboOrders<TItemMinorComplete *> SelectedOrders;
//	TList *SetOrders;
	TfrmComboGUI ComboGUI;
    bool MemberPoint;
};

#endif
