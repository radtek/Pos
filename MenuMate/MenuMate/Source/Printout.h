//---------------------------------------------------------------------------

#ifndef PrintoutH
#define PrintoutH
//---------------------------------------------------------------------------
#include "DeviceImage.h"
#include "PrintFormat.h"
#include "MM_DBCore.h"

#include "PrinterPhysical.h"

enum TpoStatus{poPending,poSuccess,poFailed,poNoPhysicalPrinter,poNoWindowsDriver};

class TPrintout : public TObject
{
public:
	TpoStatus Status;

	TPrintout();
	__fastcall ~TPrintout();

	TPrintFormat 		*PrintFormat;		// Contains the formated printout ready for printing.
	bool PrintedSuccessfully;
    bool BlindBalanceUsed;

	__property TPrinterPhysical Printer  = { read=FPrinter, write=SetPrinter };

	bool OpenDrawer();
	bool virtual Print(UnicodeString JobName = "");
	bool PrintToStrings(TStrings *Strings);
	bool PrintToFile(UnicodeString FileName);
	bool PrintToStream(TStream *Stream);
	bool PrintToPrinterRaw(TStringList *Text, UnicodeString PrinterName);	// Bit of a hack for reprinting receipts.
	bool PrintToPrinterStream(TMemoryStream *Data, UnicodeString PrinterName);
	std::map<UnicodeString,UnicodeString> PrintInfo; // Stores Docket Number, Chit Numbers, Barcodes for this Print Out.
private:
	TPrinterPhysical FPrinter;

	void __fastcall SetPrinter(TPrinterPhysical Printer);
};

#endif
