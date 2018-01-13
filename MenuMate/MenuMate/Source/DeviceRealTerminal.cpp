//---------------------------------------------------------------------------


#pragma hdrstop

#include "DeviceRealTerminal.h"

#include "Version.h"
#include "Comms.h"
#include "Printing.h"
#include "MMMessageBox.h"

#include "ListCourse.h"
#include "ItemSize.h"
#include "ItemRecipe.h"
#include "FiscalDataUtility.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
// ---------------------------------------------------------------------------

TDeviceRealTerminal::TDeviceRealTerminal()
  : TDeviceRealControl(new TManagerMenusPOS),
    Timer(new TTimer(NULL)),
    ManagerMembership(new TManagerMembershipGUI(DBControl,
	                                             System,
	                                             Modules)),
    Scales(new TScaleModel),
    PocketVouchers(new TManagerPocketVoucher),
	IMManager(new TIMManager),
	ManagerGeneralLedger(new TManagerGeneralLedger),
    FiscalPort(new TFiscalPort)
{
   PaymentSystem = new TListPaymentSystem;

   /* OSVERSIONINFO osvi;
   char szVersion [80];
   memset(&osvi, 0, sizeof(OSVERSIONINFO));
   osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
   GetVersionEx (&osvi);
   OS.dwPlatformId = osvi.dwPlatformId; */

   PoleDisplay = new TPoleDisplay();
   ProxReader = new TProxReader();
   BarcodeReader = new TMMBarcodeReader();
   Reboot = false;
   ShutDown = false;

   Timer->Enabled = false;
   Timer->Interval = 1000;
   Timer->OnTimer = OnTimerTick;
   EventLockOutLastFired = ::GetTickCount();
   IMStatusLastFired = ::GetTickCount();
}

__fastcall TDeviceRealTerminal::~TDeviceRealTerminal()
{
   delete PaymentSystem;
   delete PoleDisplay;
   delete ProxReader;
   delete BarcodeReader;
   delete Menus;
}

void TDeviceRealTerminal::LoadPrinterGraphic()
{
   if (TComms::Instance().ReceiptPrinter.PhysicalPrinterKey != 0)
   {
	  if (FileExists(ExtractFilePath(Application->ExeName) + "Header.bmp"))
	  {
         TPrintFormat *pPrinter = new TPrintFormat;
         try
         {
            pPrinter->DownloadGraphic(ExtractFilePath(Application->ExeName) + "Header.bmp", TComms::Instance().ReceiptPrinter.UNCName());
         }
         __finally
         {
            delete pPrinter;
         }
	  }
   }
}

void TDeviceRealTerminal::SaveHdrFtr(TStrings *inHeader, TStrings *inPHeader, TStrings *inFooter, TStrings *inVoidFooter,TStrings *inSubHeader)
{
   bool Registered = false;
   UnicodeString pRegisteredName = "";
   TDeviceRealTerminal::Instance().Registered(&Registered, &pRegisteredName);
   if (Registered)
   {
	  if (Receipt)
	  {
		 bool ShowMessage = false;
		 if (!ContainsCompanyName(inHeader, pRegisteredName))
		 {
			inHeader->Insert(0, pRegisteredName);
			ShowMessage = true;
		 }

		 if (!ContainsCompanyName(inPHeader, pRegisteredName))
		 {
			inPHeader->Insert(0, pRegisteredName);
			ShowMessage = true;
		 }

		 Receipt->SetHeaderFooter(inHeader, inPHeader, inFooter, inVoidFooter,inSubHeader);
		 if (ShowMessage)
		 {
			UnicodeString Message = "You must include your registered company name " + pRegisteredName +
				" somewhere in your Receipt Header and Preliminary Receipt Header." + " It will be inserted at the top now.";
			Application->MessageBox(Message.c_str(), UnicodeString("Information").w_str(), MB_OK + MB_DEFBUTTON1 + MB_ICONINFORMATION);
		 }
	  }
   }

   TGlobalSettings::Instance().Header->Clear();
   TGlobalSettings::Instance().PHeader->Clear();
   TGlobalSettings::Instance().Footer->Clear();
   TGlobalSettings::Instance().VoidFooter->Clear();
   TGlobalSettings::Instance().SubHeader->Clear();

   TGlobalSettings::Instance().Header->AddStrings(inHeader);
   TGlobalSettings::Instance().PHeader->AddStrings(inPHeader);
   TGlobalSettings::Instance().Footer->AddStrings(inFooter);
   TGlobalSettings::Instance().VoidFooter->AddStrings(inVoidFooter);
   TGlobalSettings::Instance().SubHeader->AddStrings(inSubHeader);

   TGlobalSettings::Instance().Header->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_HEADER);
   TGlobalSettings::Instance().PHeader->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_PRELIM_HEADER);
   TGlobalSettings::Instance().Footer->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_FOOTER);
   TGlobalSettings::Instance().VoidFooter->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_VOID_FOOTER);
   TGlobalSettings::Instance().SubHeader->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_SUBHEADER);
}

void TDeviceRealTerminal::SaveZedHeader(TStrings *inZedHeader)
{
   bool Registered = false;
   UnicodeString pRegisteredName = "";
   TDeviceRealTerminal::Instance().Registered(&Registered, &pRegisteredName);
   if (Registered)
   {
	  if (Receipt)
	  {
		 bool ShowMessage = false;
		 if (!ContainsCompanyName(inZedHeader, pRegisteredName))
		 {
			inZedHeader->Insert(0, pRegisteredName);
			ShowMessage = true;
		 }

		 if (ShowMessage)
		 {
			UnicodeString Message = "You must include your registered company name " + pRegisteredName +
				" somewhere in the Zed Report Header." + " It will be inserted at the top now.";
			Application->MessageBox(Message.c_str(), UnicodeString("Information").w_str(), MB_OK + MB_DEFBUTTON1 + MB_ICONINFORMATION);
		 }
	  }
   }

   TGlobalSettings::Instance().ZedHeader->Clear();
   TGlobalSettings::Instance().ZedHeader->AddStrings(inZedHeader);
   TGlobalSettings::Instance().ZedHeader->SaveToFile(ExtractFilePath(Application->ExeName) + RECEIPT_ZEDHEADER);
}


// ---------------------------------------------------------------------------
#ifdef AutoMate

TDeviceRealTerminal::Network::Network()
{
   DeviceImages = new TList;
}

// ---------------------------------------------------------------------------
TDeviceRealTerminal::Network::~Network()
{
   while (DeviceImages->Count != 0)
   {
	  TDeviceImage *Result = (TDeviceImage*)DeviceImages->Items[0];
	  delete Result;
	  DeviceImages->Delete(0);
   }
   delete DeviceImages;
}

// ---------------------------------------------------------------------------
TDeviceImage* TDeviceRealTerminal::Network::LocateByIP(AnsiString inIP)
{
   for (int i = 0; i < DeviceImages->Count; i++)
   {
	  TDeviceImage *Result = (TDeviceImage*)DeviceImages->Items[i];
	  if (Result->ID.IP == inIP)
	  {
		 return Result;
	  }
   }
   return NULL;
}

// ---------------------------------------------------------------------------
TDeviceImage* TDeviceRealTerminal::Network::LocateByName(AnsiString inName)
{
   for (int i = 0; i < DeviceImages->Count; i++)
   {
	  TDeviceImage *Result = (TDeviceImage*)DeviceImages->Items[i];
	  if (Result->ID.Name == inName)
	  {
		 return Result;
	  }
   }
   return NULL;
}

// ---------------------------------------------------------------------------
void TDeviceRealTerminal::Network::AddDeviceImageFromStatus(TReqStatus * Request)
{
   if (LocateByName(Request->Header.RequestingDevice->ID.Name) == NULL)
   {
	  TDeviceImage *AddImage = new TDeviceImage;
	  AddImage->Copy(Request->Header.RequestingDevice);
	  DeviceImages->Add(AddImage);
   }
}

#endif

void TDeviceRealTerminal::RegisterTransaction(Database::TDBTransaction &DBTransaction)
{
   TDeviceRealControl::RegisterTransaction(DBTransaction);
   ManagerMembership->RegisterTransaction(DBTransaction);
}

bool TDeviceRealTerminal::OpenDatabases()
{
   bool Proceed;
   Proceed = TDeviceRealControl::OpenDatabases();
   if (!Proceed)
	  return Proceed;
   Proceed = OpenMembershipDB();
   return Proceed;
}

bool TDeviceRealTerminal::OpenMembershipDB(bool Reconfig)
{
   bool RetVal = false;
   if (Modules.Status[eRegMembers]["Registered"])
   {
	  Database::TDBTransaction DBBootTransaction(DBControl);
	  DBBootTransaction.StartTransaction();
	  RetVal = ManagerMembership->OpenMembershipDB(DBBootTransaction, Reconfig);
	  if (RetVal)
	  {
		 TManagerVariable::Instance().SetDeviceStr(DBBootTransaction, vmMembershipDatabaseIP, TGlobalSettings::Instance().MembershipDatabaseIP);
		 TManagerVariable::Instance().SetDeviceStr(DBBootTransaction, vmMembershipDatabasePath, TGlobalSettings::Instance().MembershipDatabasePath);
		 TManagerVariable::Instance().SetDeviceInt(DBBootTransaction, vmMembershipDatabasePort, TGlobalSettings::Instance().MembershipDatabasePort);
	  }
	  DBBootTransaction.Commit();
   }
   else
   {
	  RetVal = true;
   }
   return RetVal;
}

// ---------------------------------------------------------------------------

void TDeviceRealTerminal::UpdateCurrentRecipes(UnicodeString Code, UnicodeString Location, Currency Cost, double CostGSTPercent)
{
   for (int i = 0; i < Menus->Current->Count; i++)
   {
	  TListMenu *Menu = Menus->Current->MenuGet(i);

	  for (int k = 0; k < Menu->Count; k++)
	  {
		 TListCourse *Course = Menu->CourseGet(k);

		 for (int i = 0; i < Course->Count; i++)
		 {
			TItem *Item = Course->ItemGet(i);

			for (int i = 0; i < Item->Sizes->Count; i++)
			{
			   TItemSize *ItemSize = Item->Sizes->SizeGet(i);

			   for (int i = 0; i < ItemSize->Recipes->Count; i++)
			   {
				  TItemRecipe *RecipeItem = ItemSize->Recipes->RecipeGet(i);
				  if (RecipeItem->StockCode == Code && RecipeItem->StockLocation == Location)
				  {
					 RecipeItem->Cost = Cost * RecipeItem->Qty;
					 RecipeItem->CostGSTPercent = CostGSTPercent;
				  }
			   }

			   if (ItemSize->Recipes->Count > 0)
			   {
				  double CostIncGst = 0;
				  double OrderCost = 0;
				  for (int i = 0; i < ItemSize->Recipes->Count; i++)
				  {
					 TItemRecipe *RecipeItem = ItemSize->Recipes->RecipeGet(i);
					 OrderCost += double(RecipeItem->Cost);
					 CostIncGst += double(RecipeItem->Cost)*double(RecipeItem->CostGSTPercent + 100.0) / double(100.0);
				  }

				  if (OrderCost != 0)
				  {
					 ItemSize->CostGSTPercent = double(((CostIncGst - OrderCost) * 100) / double(OrderCost));
					 ItemSize->Cost = OrderCost;
				  }
			   }
			}
		 }
	  }
   }
}

// ---------------------------------------------------------------------------
void TDeviceRealTerminal::UpdateMessage(TNetMessageMsg * Request)
{
   MessageBox(Request->Message, "Message From : " + Request->User + " Terminal : " + Request->Device, MB_ICONINFORMATION + MB_OK);
}

// ---------------------------------------------------------------------------
void __fastcall TDeviceRealTerminal::OnTimerTick(TObject *Sender)
{
   if (TGlobalSettings::Instance().AutoLogoutPOS && TGlobalSettings::Instance().AutoLogoutSeconds > 0)
   {
       if (EventLockOutLastFired + (TGlobalSettings::Instance().AutoLogoutSeconds * 1000) < ::GetTickCount())
	  {
		 EventLockOutTimer.Occured();
		 ResetEventLockOutTimer();
	  }
   }

   if (IMManager->Registered && IMManager->Enabled)
   {
	  if (IMStatusLastFired + (60 * 1000) < ::GetTickCount())
	  {
		 IMManager->Status();
		 IMStatusLastFired = ::GetTickCount();
	  }
   }
}

void TDeviceRealTerminal::ResetEventLockOutTimer()
{
   EventLockOutLastFired = ::GetTickCount();
}

void TDeviceRealTerminal::Initialise(Database::TDBTransaction &DBTransaction)
{
   TDeviceRealControl::Initialise(DBTransaction);
   Menus->Initialise(DBTransaction);
   ManagerMembership->Initialise(DBTransaction);
   ManagerGeneralLedger->Initialise(DBTransaction);
   PaymentSystem->Initialise(DBTransaction);
   PocketVouchers->Initialise(DBTransaction);
   ProxReader->Initialise(DBTransaction);
   ProxReader->Open(ProxReader->PortNumber);

   BarcodeReader->PortNumber = TManagerVariable::Instance().GetInt(DBTransaction, vmBarCodeReaderPort, -1);
   BarcodeReader->Open(BarcodeReader->PortNumber);

   PoleDisplay->Initialise(DBTransaction);
   PoleDisplay->PortNumber = TManagerVariable::Instance().GetInt(DBTransaction, vmPoleDisplayPort, -1);
   PoleDisplay->EmulationMode = TManagerVariable::Instance().GetInt(DBTransaction, vmPoleDisplayEmulationMode);
   PoleDisplay->Open(PoleDisplay->PortNumber);

   Scales->Initialise(DBTransaction);
   FiscalPort->Initialise(DBTransaction);
   if (!Scales->Open(TDeviceRealTerminal::Instance().Scales->PortNumber))
   {
	  MessageBox("Scales Error : " + TDeviceRealTerminal::Instance().Scales->LastError, "Scales will not work", MB_OK + MB_ICONERROR);
   }

   TGlobalSettings::Instance().IsFiscalStorageEnabled = false;
   if(FiscalPort->PortNumber != 0)
   {
       if(FiscalPort->Open(FiscalPort->PortNumber))
       {
            AnsiString response = FiscalPort->SetFiscalData("ver 0000", eFiscalVerNumber);
            std::auto_ptr<TFiscalDataUtility> dataUtility(new TFiscalDataUtility());
            bool isSuccessful = dataUtility->AnalyzeResponse(response, eFiscalVerNumber);
            if(isSuccessful)
                TGlobalSettings::Instance().IsFiscalStorageEnabled = true;
            else
                MessageBox("Details are not correct for successful PosPlus communication. Please Check.","Error",MB_OK + MB_ICONERROR);
       }
       else
       {
          MessageBox("Unable to open Port for PosPlus. Please check details.", "Error",MB_OK + MB_ICONERROR);
       }
   }
   TManagerVariable::Instance().SetDeviceBool(DBTransaction, vmIsFiscalStorageEnabled, TGlobalSettings::Instance().IsFiscalStorageEnabled);
   Timer->Enabled = true;

   if (Modules.Status[eIntaMate]["Registered"])
   {
	  IMManager->Registered = true;
	  IMManager->Initialise(TGlobalSettings::Instance().IntaMateIPAddress, TGlobalSettings::Instance().IntaMatePort, TGlobalSettings::Instance().IntaMateTerminalID, TGlobalSettings::Instance().IntaMateTCPTimeOut_ms);
   }
}

void TDeviceRealTerminal::TriggerTabStateChanged()
{
   AfterTabStateChanged.Occured();
}

void TDeviceRealTerminal::BuildXMLVersion( TPOS_XMLBase &Data, int SiteID )
{
   try
   {
	  Data.Doc.Clear();

	  TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	  Data.Doc.LinkEndChild(decl);

	  AnsiString version = GetFileVersion();

	  // Insert DOCTYPE definiation here.
	  TiXmlElement * List = new TiXmlElement( xmlEleVersion );
	  List->SetAttribute( xmlAttrID,               AnsiString(Data.IntaMateID).c_str() );
	  List->SetAttribute( xmlAttrSiteID,           SiteID );
	  List->SetAttribute( xmlAttrPOSVersion,       version.c_str() );
	  List->SetAttribute( xmlAttrInterfaceVersion, version.c_str() );

	  Data.Doc.LinkEndChild(List);
   }
   catch(Exception & E)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	  throw;
   }
}
//..............................................................................
