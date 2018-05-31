//---------------------------------------------------------------------------

#ifndef ReceiptManagerH
#define ReceiptManagerH
//---------------------------------------------------------------------------
#include "Manager.h"
#include "MM_DBCore.h"
//---------------------------------------------------------------------------

class TManagerReceipt : public TManager
{
	private:
	TDateTime FSelectedDate;
	Database::TDBControl &DBControl;
	typedef std::pair<int,int> TableKeyPair;
	std::vector<TableKeyPair> Array;
	int ArrayIndex;
    bool IsStartOfReceiptInfo(TStringList *Lines);
    void InsertReprintLabel(TStringList *Lines,bool &IsFirstOccurance);

	void SetCurrentDate(TDateTime SelectedDate);
	bool Get(Database::TDBTransaction &DBTransaction);
    void AddDuplicateLabel(TMemoryStream* ReceiptToEdit,TStringList *Lines);
    void PrintDuplicateReceipt(TMemoryStream* DuplicateReceipt,bool IsCompanyDetailsReprintReceipt = false);
    bool CanReprintReceipt(Database::TDBTransaction &DBTransaction, AnsiString InvoiceNumber);
	public:
	TManagerReceipt(Database::TDBControl &inDBControl);
	virtual ~TManagerReceipt();
	void Initialise(AnsiString inTerminalName,int inTerminalDeviceKey);
	TMemoryStream *Receipt;
	TMemoryStream *ReceiptToArchive;
    Currency ReceiptValue;
	bool ReceiptsExist();
	void Open();
	void Close();
	void Next();
	void Prev();
	void First();
	bool Eof();
	bool Bof();
	bool IsEmpty();
	bool Find(AnsiString InvoiceNo);
	bool FindRef(AnsiString Reference);
   bool FindMember(int ContactKey);
	bool AllTerminals;
	AnsiString TerminalName;
	int TerminalDeviceKey;
	void Get(TStringList *Lines);
	void GetLastReceipt(Database::TDBTransaction &DBTransaction);
	void PrintLastReceipt();
	void Print();
    AnsiString Companydetails;
   
	__property TDateTime Date = { read = FSelectedDate ,write = SetCurrentDate};
	bool CanApplyTipOnThisReceiptsTransaction(
		WideString &outPaymentRefNumber,
		Currency &outOriginalVisaPaymentAmount,
		int &outArcbillKey);  // specifies whether a tip can be applied to this receipt's owner transaction and gets some information back
        int Sec_Ref;
        AnsiString InvoiceNumber;
        };

extern TManagerReceipt *ManagerReceipt;

#endif
