//---------------------------------------------------------------------------


#pragma hdrstop


#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "Printouts.h"
#include "MMMessageBox.h"
#include "ManagerPhysicalPrinter.h"
#include "PrinterRedirect.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


void TPrintouts::Redirect(TPrinterPhysical From,TPrinterPhysical To)
{
	for (int i= 0; i< Count; i++)
	{
		TPrintout *Printout = (TPrintout *)Items[i];
		if(Printout->Printer.PhysicalPrinterKey == From.PhysicalPrinterKey)
		{
			Printout->Printer = To;
		}
	}
}

bool TPrintouts::Print(DeviceType inDevice,UnicodeString JobName)
{
	bool Success = true;

    for (int i = 0; i< Count; i++)
    {
        TPrintout *Printout = (TPrintout *)Items[i];
        Printout->Status = poPending;
	}

	if(inDevice == devPC)
	{
		for (int i = 0; i< Count; i++)
		{
			TPrintout *Printout = (TPrintout *)Items[i];

			while(Printout->Status == poPending)
			{
				if (Printout->Printer.PhysicalPrinterKey == 0)
				{
					Printout->Status = poNoPhysicalPrinter;
				}

				if(Printout->Status == poPending)
				{
					if (!Printout->Print(JobName))
					{
						Printout->Status = poFailed;
					}
				}

				switch(Printout->Status)
				{
					case poSuccess :
					break;
					case poFailed :
					{
						if(Printout->Printer.PhysicalPrinterKey != 0)
						{
							MessageBox("Printing To " + Printout->Printer.ServerName  + " Printer : " + Printout->Printer.ShareName + " Failed","Please Select Another Printer",MB_OK + MB_ICONWARNING);
						}
						else
						{
							MessageBox("Some Printing Failed to Print" ,"Please Select Another Printer",MB_OK + MB_ICONWARNING);
						}
						TPrinterPhysical TempConnect = GetAlternativePrinter("Please Select a Printer.");
						if(TempConnect.PhysicalPrinterKey == 0)
						{
							if(MessageBox("Are you sure you wish to CANCEL this print job?", "Printing CANCEL Confirmation",MB_YESNO + MB_ICONWARNING) == IDYES)
							{
								Printout->Status = poSuccess;
							}
							else
							{
								Printout->Status = poPending;
							}
						}
						else
						{
							Printout->Printer = TempConnect;
							Printout->Status = poPending;
							 // PrintFormat->ReFormat(); // Reformat the print job for this new printer.
						}
					}
					break;
					case poNoWindowsDriver :
					case poNoPhysicalPrinter :
					{
						MessageBox("Please select a printer for these orders","No Printer Selected",MB_OK + MB_ICONWARNING);
						TPrinterPhysical TempConnect = GetAlternativePrinter("Please Select a Printer.");
						if(TempConnect.PhysicalPrinterKey == 0)
						{
							if(MessageBox("Are you sure you wish to CANCEL this print job?", "Printing CANCEL Confirmation",MB_YESNO + MB_ICONWARNING) == IDYES)
							{
								Printout->Status = poSuccess;
							}
							else
							{
								Printout->Status = poPending;
							}
						}
						else
						{
							Printout->Printer = TempConnect;
							Printout->Status = poPending;
							 // PrintFormat->ReFormat(); // Reformat the print job for this new printer.
						}
					}
					break;
				}
			}
		}
	}
	else if (inDevice == devPalm)
	{
		for (int i=0; i<Count; i++)
		{
			TPrintout *Printout = (TPrintout *)Items[i];

			if (Printout->Printer.PhysicalPrinterKey == 0)
			{
				Printout->Status	= poNoPhysicalPrinter;
				Success				= false;
			}
			if(Printout->Status == poPending)
			{
				if (!Printout->Print(JobName))
				{
					Success = false;
				}
			}
			else
			{
				Success = false;
			}
		}
	}
	return Success;
}

bool TPrintouts::Print(int PrintoutIndex,DeviceType inDevice)
{
	bool Success = true;

   if(PrintoutIndex < Count)
   {
      TPrintout *Printout = (TPrintout *)Items[PrintoutIndex];
      Printout->Status = poPending;

      if(inDevice == devPC)
      {
         while(Printout->Status == poPending)
         {
            if (Printout->Printer.PhysicalPrinterKey == 0)
            {
               Printout->Status = poNoPhysicalPrinter;
            }

            if(Printout->Status == poPending)
            {
               if (!Printout->Print())
               {
                  Printout->Status = poFailed;
               }
            }

            switch(Printout->Status)
            {
               case poSuccess :
               break;
               case poFailed :
               {
                  if(Printout->Printer.PhysicalPrinterKey != 0)
                  {
                     MessageBox("Printing To " + Printout->Printer.ServerName  + " Printer : " + Printout->Printer.ShareName + " Failed","Please Select Another Printer",MB_OK + MB_ICONWARNING);
                  }
                  else
                  {
                     MessageBox("Some Printing Failed to Print" ,"Please Select Another Printer",MB_OK + MB_ICONWARNING);
                  }
                  TPrinterPhysical TempConnect = GetAlternativePrinter("Please Select a Printer.");
                  if(TempConnect.PhysicalPrinterKey == 0)
                  {
                     if(MessageBox("Are you sure you wish to CANCEL this print job?", "Printing CANCEL Confirmation",MB_YESNO + MB_ICONWARNING) == IDYES)
                     {
                        Printout->Status = poSuccess;
                     }
                     else
                     {
                        Printout->Status = poPending;
                     }
                  }
                  else
                  {
                     Printout->Printer = TempConnect;
                     Printout->Status = poPending;
                      // PrintFormat->ReFormat(); // Reformat the print job for this new printer.
                  }
               }
               break;
               case poNoWindowsDriver :
               case poNoPhysicalPrinter :
               {
                  MessageBox("Please select a printer for these orders","No Printer Selected",MB_OK + MB_ICONWARNING);
                  TPrinterPhysical TempConnect = GetAlternativePrinter("Please Select a Printer.");
                  if(TempConnect.PhysicalPrinterKey == 0)
                  {
                     if(MessageBox("Are you sure you wish to CANCEL this print job?", "Printing CANCEL Confirmation",MB_YESNO + MB_ICONWARNING) == IDYES)
                     {
                        Printout->Status = poSuccess;
                     }
                     else
                     {
                        Printout->Status = poPending;
                     }
                  }
                  else
                  {
                     Printout->Printer = TempConnect;
                     Printout->Status = poPending;
                      // PrintFormat->ReFormat(); // Reformat the print job for this new printer.
                  }
               }
               break;
            }
         }
      }
      else if (inDevice == devPalm)
      {
         if (Printout->Printer.PhysicalPrinterKey == 0)
         {
            Printout->Status	= poNoPhysicalPrinter;
            Success				= false;
         }
         if(Printout->Status == poPending)
         {
            if (!Printout->Print())
            {
               Success = false;
            }
         }
         else
         {
            Success = false;
         }
      }
      return Success;
   }
}

void TPrintouts::PrintToPrinterRaw(TStringList *Text, UnicodeString PrinterName)
{
	for (int i = 0; i< Count; i++)
	{
		TPrintout *Printout = (TPrintout *)Items[i];
		Printout->PrintToPrinterRaw(Text,PrinterName);
	}
}
//---------------------------------------------------------------------------
void TPrintouts::PrintToPrinterStream(TMemoryStream *Data, UnicodeString PrinterName)
{
	for (int i = 0; i< Count; i++)
	{
		TPrintout *Printout = (TPrintout *)Items[i];
		Printout->PrintToPrinterStream(Data,PrinterName);
	}
}

void TPrintouts::PrintToStrings(TStrings *Strings)
{
	for (int i = 0; i< Count; i++)
	{
		TPrintout *Printout = (TPrintout *)Items[i];
		Printout->PrintToStrings(Strings);
	}
}
//---------------------------------------------------------------------------
void TPrintouts::PrintToFile(UnicodeString FileName)
{
	for (int i = 0; i< Count; i++)
	{
		TPrintout *Printout = (TPrintout *)Items[i];
		Printout->PrintToFile(FileName);
	}
}

void TPrintouts::PrintToStream(TStream *Stream)
{
	for (int i = 0; i< Count; i++)
	{
		TPrintout *Printout = (TPrintout *)Items[i];
		Printout->PrintToStream(Stream);
	}
}


void TPrintouts::ClearPrinters()
{
	for (int i = 0; i < Count; i++)
	{
		TPrintout *Printout = (TPrintout *)Items[i];
		Printout->Printer.PhysicalPrinterKey = 0;
	}
}

//---------------------------------------------------------------------------


TPrinterPhysical TPrintouts::GetAlternativePrinter(UnicodeString Message)
{
#ifdef MenuMate
	TPrinterPhysical TempPrinter;

	Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
	DBTransaction.StartTransaction();
	std::auto_ptr<TfrmPrinterRedirect> frmPrinterRedirect(TfrmPrinterRedirect::Create<TfrmPrinterRedirect>(Screen->ActiveForm,DBTransaction));
	frmPrinterRedirect->lbCaption->Caption = Message;
	if(frmPrinterRedirect->ShowModal() == mrOk)
	{
		TManagerPhysicalPrinter ManagerPhysicalPrinter;
		TempPrinter = ManagerPhysicalPrinter.GetPhysicalPrinter(DBTransaction,frmPrinterRedirect->SelectedPrinterKey);
	}
	DBTransaction.Commit();
	return TempPrinter;
#endif
}

void TPrintouts::FilterForChefMate()
{
	for (int i = 0; i< Count; i++)
	{
		TPrintout *Printout = (TPrintout *)Items[i];
		if(Printout->Printer.Type != ptChefMate_Printer)
		{
			Remove(Printout);
			delete Printout;
			i = 0;
		}
	}
}
