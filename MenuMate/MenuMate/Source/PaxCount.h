//---------------------------------------------------------------------------

#ifndef PaxCountH
#define PaxCountH
//---------------------------------------------------------------------------

#include "ListManager.h"

enum ePaxType{ eTotal, ePhotoPacks , ePrints,
					eProducts, eTransactionCount,
					eDateFrom, eDateTo, ePaxNumber};



class TPaxCount
{
	public:
	TPaxCount() :
		PhotoPacksSold(0),
		PrintsSold(0),
		ProductsSold(0),
		TransactionCount(0),
		DateFrom(Now()),
		DateTo(Now().operator++()),
		PaxNumber(0){}

	TPaxCount(double inPhotoPacksSold, double inPrintsSold, double inProductsSold,
				 int inTransactionCount, TDateTime inDateFrom,
				 TDateTime inDateTo, int inPaxNumber) :
				 PhotoPacksSold(inPhotoPacksSold),
				 PrintsSold(inPrintsSold),
				 ProductsSold(inProductsSold),
				 TransactionCount(inTransactionCount),
				 DateFrom(inDateFrom),
				 DateTo(inDateTo),
				 PaxNumber(inPaxNumber){}

	void SetPhotoPacksSold(double inPhotoPacksSold){PhotoPacksSold = inPhotoPacksSold;}
	void SetPrintsSold(double inPrintsSold){PrintsSold = inPrintsSold;}
	void SetProductsSold(double inProductsSold){ProductsSold = inProductsSold;}
	void SetTransactionCount(int inTransactionCount){TransactionCount = inTransactionCount;}
	void SetDateFrom(TDateTime inDateFrom){DateFrom = inDateFrom;}
	void SetDateTo(TDateTime inDateTo){DateTo = inDateTo;}
	void SetPaxNumber(int inPaxNumber){PaxNumber = inPaxNumber;}

	double GetPhotoPacksSold(void){return PhotoPacksSold;}
	double GetPrintsSold(void){return PrintsSold;}
	double GetProductsSold(void){return ProductsSold;}
	int GetTransactionCount(void){return TransactionCount;}
	TDateTime GetDateFrom(void){return DateFrom;}
	TDateTime GetDateTo(void){return DateTo;}
	int GetPaxNumber(void){return PaxNumber;}

	private:
	double PhotoPacksSold;
	double PrintsSold;
	double ProductsSold;
	int TransactionCount;
	TDateTime DateFrom;
	TDateTime DateTo;
	int PaxNumber;
};


class TPaxCountController
{
	public:
	TPaxCountController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction);
	void Run(void);
	void Show(void);
	void Save(AnsiString inDeviceName);
	TPaxCount Get(void);
	std::auto_ptr<TfrmListManager> frmListManager;

	private:
	Database::TDBTransaction &DBTransaction;
	TForm *DisplayOwner;
	TPaxCount PaxCount;

	void OnSelect(int Row, int Col);
	void OnClose(int Row, int Col);
	void OnDelete(int Row, int Col);
	void PopulateListManager(void);
	void UpdateDisplay(void);
	TDateTime DisplayDatePicker(TDateTime inDate);
	double LoadData(ePaxType type);
	TDateTime GetDateTime(void);
};
#endif
