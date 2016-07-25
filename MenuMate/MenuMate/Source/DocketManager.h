//---------------------------------------------------------------------------

#ifndef DocketManagerH
#define DocketManagerH
//---------------------------------------------------------------------------
#include "Manager.h"
#include "Printing.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------
enum TDocketFilter {eAllTill,eThisTill,eActiveDockets};

class TManagerDockets : public TManager
{
	private:
	TDateTime FSelectedDate;
	void SetCurrentDate(TDateTime SelectedDate);
	Database::TDBControl &DBControl;
	bool Get(Database::TDBTransaction &DBTransaction);
	public:
	TManagerDockets(Database::TDBControl &inDBControl);
	virtual ~TManagerDockets();
	void Initialise(UnicodeString inTerminalName,int inTerminalDeviceKey);
	TMemoryStream *Docket;
	TMemoryStream *DocketToArchive;
	bool ReceiptsExist();
	void Open();
	void Close();
	void Next();
	void Prev();
	void Last();
	void First();
	bool Eof();
	bool Bof();
	bool IsEmpty();
	bool Find(int DocketNumber);
	bool FindBarcode(UnicodeString BarCode);
	bool FindChit(UnicodeString ChitNumber);
	TDocketFilter Filter;
	UnicodeString TerminalName;
	int TerminalDeviceKey;
	void Get(TStringList *Lines);
   void GetWithSymbols(TStringList *Lines);   
	void GetLastReceipt(Database::TDBTransaction &DBTransaction);
	void PrintLastDocket();
	void Print();
	int GetNextDocketNumber();
	void Archive(TReqPrintJob * Request);
	__property TDateTime Date = { read = FSelectedDate ,write = SetCurrentDate};
	std::vector<int> Array;
	int ArrayIndex;
    int Sec_Ref;
    AnsiString InvoiceNumber;
    Currency ReceiptValue;
    void getOrderInfo(Database::TDBTransaction &DBTransaction);
    typedef std::pair<int,int> TableKeyPair;
	std::vector<TableKeyPair>;
    void OrderedArrayKey(AnsiString InvoiceNo);
  	std::vector<TableKeyPair> Array1;

    int DocketNumber;
    int CurrentDocketNumber;
};

extern TManagerDockets *ManagerDockets;

#endif
