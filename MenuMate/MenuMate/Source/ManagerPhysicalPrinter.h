//---------------------------------------------------------------------------

#ifndef ManagerPhysicalPrinterH
#define ManagerPhysicalPrinterH

#define MAX_SHARENAME_LENGTH 80
//---------------------------------------------------------------------------
#include "Manager.h"
#include "PrintFormat.h"
#include "MM_DBCore.h"
#include "GraphicPrinterSettings.h"

#include <vector>

enum TPrinterTypeFilter	{pfAll_Printers,pfWindows_Printer,pfChefMate_Printer};


class TManagerPhysicalPrinter : public TManager
{
	private:
		void InitialisePrinters(Database::TDBTransaction &DBTransaction,int inDeviceKey,UnicodeString inComputerName,DWORD Flags);
	public :

	TManagerPhysicalPrinter();

	virtual ~TManagerPhysicalPrinter();

	int GetPrinterKeyByShareName(Database::TDBTransaction &DBTransaction,UnicodeString ServerName,UnicodeString ShareName);
	void Initialise(Database::TDBTransaction &DBTransaction,int inDeviceKey,UnicodeString inComputerName);

	int DBAddPrinter(Database::TDBTransaction &DBTransaction,TPrinterPhysical &Printer);
	void DBDeletePrinter(Database::TDBTransaction &DBTransaction,TPrinterPhysical &Printer);

	int LoadProfile(Database::TDBTransaction &DBTransaction,TPrinterPhysical &Printer);

	TPrinterPhysical GetPhysicalPrinter(Database::TDBTransaction &DBTransaction,int inPhysicalPrinterKey);
	void DBGetPrinterList(Database::TDBTransaction &DBTransaction,TStrings *StringsList);
	void DBGetValidPrinterList(Database::TDBTransaction &DBTransaction,int DeviceKey,TStrings *StringsList);	
	void GetPrinterServerList(Database::TDBTransaction &DBTransaction,TStrings *StringsList,TPrinterType Type);
};

#endif
