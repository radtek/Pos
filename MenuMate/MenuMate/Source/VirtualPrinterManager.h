//---------------------------------------------------------------------------

#ifndef VirtualPrinterManagerH
#define VirtualPrinterManagerH
//---------------------------------------------------------------------------
#include "Manager.h"
#include <vector>
#include "MM_DBCore.h"
#include "ItemComplete.h"
class TPrinterVirtual
{
	public :
	int VirtualPrinterKey;
	int PhysicalPrinterKey;
	UnicodeString VirtualPrinterName;
};

class TManagerVirtualPrinter : public TManager
{
	private:
   	  static int DBCheckVirtualPrinter(Database::TDBTransaction &DBTransaction,TPrinterVirtual inPrinterVirtual);
      static void DBRenamePrinter(Database::TDBTransaction &DBTransaction,int PrinterKey,UnicodeString Name);
      static void DBDeletePrinter(Database::TDBTransaction &DBTransaction,int PrinterKey);
	public :
	TManagerVirtualPrinter();
	virtual ~TManagerVirtualPrinter();
	void Initialise();
	static void InitialisePrinters(Database::TDBTransaction &DBTransaction,int inDeviceKey);
    static void InitialisePrinters(Database::TDBTransaction &DBTransaction, int inDeviceKey,AnsiString DeviceName);
	static void CreateWindowsConnection(Database::TDBTransaction &DBTransaction,int inDeviceKey);
    static void CreateWindowsConnection(Database::TDBTransaction &DBTransaction,int inDeviceKey,AnsiString DeviceName);
	static int DBCheckVirtualPrinterByName(Database::TDBTransaction &DBTransaction,UnicodeString Name);
	static void DBReAssignPrinter(Database::TDBTransaction &DBTransaction,int PrinterKey,int PhysicalPrinterKey);
    static int DBAddPrinter(Database::TDBTransaction &DBTransaction,TPrinterVirtual inPrinter);
   
	static int GetVirtualPrintersDevice(Database::TDBTransaction &DBTransaction,int PrinterKey);
	static void GetVirtualPrinterList(Database::TDBTransaction &DBTransaction,TStrings * List);

    static TPrinterVirtual * GetVirtualPrinterByKey(Database::TDBTransaction &DBTransaction,int VirtualPrinterKey);
	static TPrinterVirtual * GetVirtualPrinterByName(Database::TDBTransaction &DBTransaction,UnicodeString VirtualPrinterName);
   	// void ReloadPrinters(Database::TDBTransaction &DBTransaction);
	// static void AddPrinter(TPrinterVirtual PrinterVirtual);
	static void RenamePrinter(Database::TDBTransaction &DBTransaction,int PrinterKey, UnicodeString Name);
	static void DeletePrinter(Database::TDBTransaction &DBTransaction,int inPrinterKey);

    static void DBLoadPrinters(Database::TDBTransaction &DBTransaction,std::vector<TPrinterVirtual> &VirtualPrinters);
    static void DBLoadPrinters(Database::TDBTransaction &DBTransaction,std::vector<TPrinterVirtual> &VirtualPrinters,AnsiString DeviceName);
    static void DBLoadPrinters(Database::TDBTransaction &DBTransaction,int DeviceKey,std::vector<TPrinterVirtual> &DevicePrinters,bool isChefmatePrinter=false);
    static TStringList* LoadChefmatePrinter(Database::TDBTransaction &DBTransaction,TItemComplete* inOrderItem);
};
#endif
