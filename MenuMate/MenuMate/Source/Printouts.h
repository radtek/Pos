//---------------------------------------------------------------------------

#ifndef PrintoutsH
#define PrintoutsH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "Printout.h"

class TPrintouts : public TList
{
	public :
	void Redirect(TPrinterPhysical From,TPrinterPhysical To);
	bool Print(DeviceType inDevice,UnicodeString JobName = "");
   bool Print(int PrintoutIndex,DeviceType inDevice);
	void PrintToStrings(TStrings *Strings);
	void PrintToFile(UnicodeString FileName);
	void PrintToStream(TStream *Stream);
	void PrintToPrinterRaw(TStringList *Text, UnicodeString PrinterName);	// Bit of a hack for reprinting receipts.
	void PrintToPrinterStream(TMemoryStream *Data, UnicodeString PrinterName);
	void ClearPrinters();
	TPrinterPhysical GetAlternativePrinter(UnicodeString Message);
	void FilterForChefMate();
};
#endif
