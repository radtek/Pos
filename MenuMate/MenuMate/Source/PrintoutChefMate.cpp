// ---------------------------------------------------------------------------

#pragma hdrstop

#include "PrintoutChefMate.h"
#include "MMLogging.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
// ---------------------------------------------------------------------------

#pragma package(smart_init)

bool TPrintoutChefMate::Print()
{
   try
   {
	  if (Printer.PhysicalPrinterKey == 0)
	  {
		 Status = poNoPhysicalPrinter;
	  }

	  if (Status == poPending)
	  {
		 if (Printer.Type == ptWindows_Printer)
		 {
            if (PrintFormat->Print(Printer.UNCName(), ""))
            {
               Status = poSuccess;
            }
            else
            {
               Status = poFailed;
            }
		 }
		 else if (Printer.Type == ptChefMate_Printer && TDeviceRealTerminal::Instance().KitchenMod->Enabled)
		 {
			std::auto_ptr <TNetMessageChefMate> ChefRequest(new TNetMessageChefMate);
			ChefRequest->OrderNumber = TGlobalSettings::Instance().KitchenOrderNumber;
			ChefRequest->RemoveOrder = false;
			ChefRequest->FinalOrder = true;
			ChefRequest->TillColour = TGlobalSettings::Instance().KitchenColour;
			ChefRequest->Device = TDeviceRealTerminal::Instance().ID.Name;
			ChefRequest->User = TDeviceRealTerminal::Instance().User.Name;
			ChefRequest->ChitNumber = PrintInfo["ChitNumber"];
			ChefRequest->TimeKey = StrToIntDef(PrintInfo["CurrentTimeKey"], 0);

			PrintToStrings(ChefRequest->Order);
			TDeviceRealTerminal::Instance().KitchenMod->SendRequest(ChefRequest.get(), Printer.ServerName);
			Status = poSuccess;
		 }
		 else
		 {
			Status = poSuccess;
		 }
	  }
   }
   catch(Exception & Err)
   {
	  TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Printing to Failed " + Err.Message);
	  Status = poFailed;
   }
   return(Status == poSuccess);
}
