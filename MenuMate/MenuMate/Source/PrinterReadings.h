//---------------------------------------------------------------------------

#ifndef PrinterReadingsH
#define PrinterReadingsH
//---------------------------------------------------------------------------
#include "ListManager.h"


class TPrinterReading
{
	public:
    TPrinterReading() :
    					PrinterName(""),
                        ShareName(""),
                        PrinterKey(NULL),
                        StartNumber(NULL),
                        FinishNumber(NULL),
                        Copies(NULL){};

	TPrinterReading( AnsiString inPrinterName, AnsiString inShareName,
						int inPrinterKey, int inStartNumber, int inFinishNumber,
                  int inCopies) :
                  PrinterName(inPrinterName),
                  ShareName(inShareName),
                  PrinterKey(inPrinterKey),
                  StartNumber(inStartNumber),
                  FinishNumber(inFinishNumber),
                  Copies(inCopies){}

    void SetPrinterName(AnsiString inPrinterName){PrinterName = inPrinterName;}
	 void SetStartNumber(int inStartNumber){StartNumber = inStartNumber;}
	 void SetFinishNumber(int inFinishNumber){FinishNumber = inFinishNumber;}
	 void SetCopies(void);
	 void SetCopies(int inCopies){Copies = inCopies;}

	 AnsiString GetPrinterName(void){return PrinterName;}
	 int GetStartNumber(void){return StartNumber;}
	 int GetFinishNumber(void){return FinishNumber;}
	 int GetCopies(void){return Copies;}

	private:
    AnsiString PrinterName;
    AnsiString ShareName;
    int PrinterKey;
    int StartNumber;
    int FinishNumber;
	 int Copies;
};


typedef std::map<int, TPrinterReading> TPrinterReadingsContainer;
class TPrinterReadingsInterface
{
	public:
	 void InsertToContainer(int row, AnsiString inPrinterReading);
	 void InsertCount(int row, int col,int count);
	 TPrinterReadingsContainer::iterator begin(void);
	 TPrinterReadingsContainer::iterator end(void);
	 bool Empty(void);
    int size(void);
	 int TotalCopies(void);

	private:
	 TPrinterReadingsContainer PrinterReadingsContainer;
};


class TPrinterReadingController
{
   private :
      std::auto_ptr<TfrmListManager> frmListManager;
      Database::TDBTransaction &DBTransaction;
      TForm *DisplayOwner;
		void PopulateListManager(int ZedKey);
      void Initialise(void);
		void UpdateDisplay(void);
		void GetFromDB(int Printer);
   public :
      TPrinterReadingController(TForm *inDisplayOwner,Database::TDBTransaction &inDBTransaction);
		void Run(int ZedKey);
		TPrinterReadingsInterface Get(void);
		TPrinterReadingsInterface PrinterReadings;
		void OnSelect(int Row, int Col);
		void OnClose(int Row, int Col);
};

#endif

