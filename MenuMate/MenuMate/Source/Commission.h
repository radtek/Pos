//---------------------------------------------------------------------------

#ifndef CommissionH
#define CommissionH
//---------------------------------------------------------------------------

#include "ListManager.h"

class TCommission
{
	public:
	TCommission() :
    	Name(""),
        Number(NULL),
        DateFrom(Now()),
        DateTo(Now()),
        Commission(0),
        isDateFromNull(true),
        isDateToNull(true),
        Position(NULL){}

    TCommission(AnsiString inName, int inNumber,
    			TDateTime inDateFrom, TDateTime inDateTo,
                Currency inCommission, bool inisDateFromNull,
                bool inisDateToNull, int inPosition) :
                Name(inName),
                Number(inNumber),
                DateFrom(inDateFrom),
                DateTo(inDateTo),
                Commission(inCommission),
                isDateFromNull(inisDateFromNull),
                isDateToNull(inisDateToNull),
                Position(inPosition){}

    	AnsiString GetName(void) const{return Name;}
    	int GetNumber(void) const{return Number;}
    	TDateTime GetDateFrom(void) const{return DateFrom;}
    	TDateTime GetDateTo(void) const{return DateTo;}
    	Currency GetCommission(void) const{return Commission;}
        bool GetisDateFromNull(void) const{return isDateFromNull;}
        bool GetisDateToNull(void) const{return isDateToNull;}
        int GetPosition(void) const{return Position;}

        void SetDateFrom(TDateTime inDateFrom) {DateFrom = inDateFrom;}
        void SetDateTo(TDateTime inDateTo) {DateTo = inDateTo;}
        void SetisDateFromNull(bool inisDateFromNull) {isDateFromNull = inisDateFromNull;}
        void SetisDateToNull(bool inisDateToNull) {isDateToNull = inisDateToNull;}
        void SetCommission(Currency inCommission) {Commission = inCommission;}

	private:
    AnsiString Name;
    int Number;
    TDateTime DateFrom;
    TDateTime DateTo;
    Currency Commission;
    bool isDateFromNull;
    bool isDateToNull;
    int Position;
};

typedef std::map<int, TCommission> TCommissionContainer;
class TCommissionCache
{
	public:
    TCommissionContainer::iterator begin();
    TCommissionContainer::iterator end();
    TCommissionContainer::iterator find(int Key);
    int size();

    void UpdateCache(int Key, TCommission inCommission);
	 bool Empty(void);

	private:
    TCommissionContainer CommissionContainer;


};


class TCommissionController
{
	public:
	TCommissionController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction);
	void Run(int ZedKey);
	TCommissionCache Get(void);

	private:
	std::auto_ptr<TfrmListManager> frmListManager;
	Database::TDBTransaction &DBTransaction;
	TForm *DisplayOwner;
	 TCommissionCache CommissionCache;
	double TotalCommission;
    void OnSelect(int Row, int Col);
    void OnClose(int Row, int Col);
	void PopulateListManager(int ZedKey);
    void UpdateDisplay(void);
    TDateTime DisplayDatePicker(TDateTime Date);

};
#endif
