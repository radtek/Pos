//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//#include <assert.h>
#include "Administration.h"
#include "MenuEdit.h"
#include "MMData.h"
#include "StockData.h"
#include "SelectLocation.h"
#include "MM_StockTransactions.h"
#include "BarcodeSyncFormController.h"
#include "BarcodeService.h"
#include "MMData.h"
#include <iostream.h>
#include <stdlib.h>
#include <ctype.h>
#include <algorithm>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

 const AnsiString Header[2][7] =
    {
    {
    "Stock_Category",
    "Stock_Group",
    "Code",
//    "Description",
//    "Stocktake_Unit",
//    "GST_Percent",
    "Location",
//    "Min_Level",
//    "Max_Level",
//    "Latest_Cost",
//    "Average_Cost",
    "Company_Name",
    "Supplier_Unit",
//    "Qty",
//    "Latest_Cost",
//    "Supplier_Code",
//    "Min_Order_Qty",
    "Barcode",
    },
    {
    "StockCategory",
    "StockGroup",
    "Stock",
//    "Stock",
//    "Stock",
//    "Stock",
    "StockLocation",
//    "StockLocation",
//    "StockLocation",
//    "StockLocation",
//    "StockLocation",
    "Contact",
    "SupplierStock",
//    "SupplierStock",
//    "SupplierStock",
//    "SupplierStock",
//    "SupplierStock",
    "Barcode"
    }
    };

   const AnsiString BikeShopHeader[2][7] =
    {
        {
        "Stock_Category",
        "Stock_Group",
        "Code",
        "Location",
        "Company_Name",
        "Supplier_Unit",
        "Barcode",
        },
        {
        "StockCategory",
        "StockGroup",
        "Stock",
        "StockLocation",
        "Contact",
        "SupplierStock",
        "Barcode"
        }
    };




//---------------------------------------------------------------------------
__fastcall TfrmAdministration::TfrmAdministration(TComponent* Owner)
	: TForm(Owner)
{
	UpdateArchiveCategories = NULL;
    dmMMData->Registered(&Registered, &Modules);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::FormDestroy(TObject *Sender)
{
	delete UpdateArchiveCategories;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::FormCreate(TObject *Sender)
{
	IBSQLMonitor->Enabled = false;
	ClientHeight = (Panel2->Top * 2) + Panel2->Height;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnImportClick(TObject *Sender)
{
    if (qrGetPriceLevelLimit->Database->Connected && !qrGetPriceLevelLimit->Transaction->InTransaction)
    {
        qrGetPriceLevelLimit->Transaction->StartTransaction();
        qrGetPriceLevelLimit->Close();
        qrGetPriceLevelLimit->ExecQuery();
        if(qrGetPriceLevelLimit->RecordCount>0)
        {
          frmMenuEdit->allPriceLevels =    qrGetPriceLevelLimit->Fields[0]->AsInteger;
        }
        else
        {
           frmMenuEdit->allPriceLevels =  2;
        }
        qrGetPriceLevelLimit->Transaction->Commit();
    }

   std::auto_ptr<TfrmSelectImportType>frmSelectImportType(new TfrmSelectImportType(NULL));
   if(frmSelectImportType->ShowModal() == mrOk)
     {
        if(frmSelectImportType->IsImportStock)
          {
             ImportStock();
          }
        else
          {
             ImportSupplier();
          }
     }
}
//---------------------------------------------------------------------------


void TfrmAdministration::ImportStock()
{
  	
     BikeShopImport();
}//---------------------------------------------------------------------------
void TfrmAdministration::ImportSupplier()
{
     	if (dlgOpen->Execute())
	{
		AnsiString LastItem;
		try
		{
			TCsv ImportCSV;
			ImportCSV.Qualifier = '\"';
			ImportCSV.LoadFromFile(dlgOpen->FileName);
         	if (ImportCSV.RowCount > 0)
            {
            if (!Transaction->DefaultDatabase->Connected)
				{
					Application->MessageBox("You must be connected to both the MenuMate and Stock databases.", "Error", MB_OK + MB_ICONERROR);
					return;
				}
				if (!Transaction->InTransaction)
				{
					Transaction->StartTransaction();
				}

				qrContactGroup->Close();
				qrContactGroup->Open();

				if (qrContactGroup->Eof)
				{
					qrLK->Close();
					qrLK->SQL->Text = "Select gen_id(gen_contact_group_key, 1) from rdb$database";
					qrLK->ExecQuery();

					qrAddContactGroup->Close();
					qrAddContactGroup->ParamByName("Contact_Group_Key")->AsInteger	= qrLK->Fields[0]->AsInteger;
					qrAddContactGroup->ParamByName("Contact_Group")->AsString		= "All";
					qrAddContactGroup->ParamByName("Sort_Order")->AsInteger			= 0;
					qrAddContactGroup->ExecSQL();
					qrLK->Close();

					qrContactGroup->Close();
					qrContactGroup->Open();
					if (qrContactGroup->Eof)
					{
						return;
					}
				}
                AnsiString FailedList;
                AnsiString MissingLocations;

            if (!Transaction->InTransaction)
				{
					Transaction->StartTransaction();
				}
            for (int i=1; i<ImportCSV.RowCount; i++)
				{
             int StockGroupKey;
             int SupplierKey;
             if (!Transaction->InTransaction)
                        {
                            Transaction->StartTransaction();
                        }
                        IBQuery->Close();
                        IBQuery->SQL->Text = "Select * From " + Header[1][4] + " Where " + Header[0][4] + " = :Data ";
                         IBQuery->ParamByName("Data")->AsString = ImportCSV.Cells[0][i];
                        IBQuery->ExecQuery();
                       if(ImportCSV.Cells[7][i] == "" ||   validate_weborder_target_uri(ImportCSV.Cells[7][i]) ){


             if (IBQuery->Eof && ImportCSV.Cells[0][i] != "")
                        {

                        qrLK->Close();
								qrLK->SQL->Text = "Select gen_id(Gen_Contact, 1) from rdb$database";
								qrLK->ExecQuery();
                                SupplierKey = qrLK->Fields[0]->AsInteger;

                                qrContactGroup->Close();
                                qrContactGroup->Open();
                                int Key                                                = qrContactGroup->FieldByName("Contact_Group_Key")->AsInteger;

								qrImportSupplier->Close();
								qrImportSupplier->ParamByName("Contact_LK")->AsInteger			        = SupplierKey;
								qrImportSupplier->ParamByName("Contact_Group_Key")->AsInteger	        = Key;
								qrImportSupplier->ParamByName("Company_Name")->AsString		    	= ImportCSV.Cells[0][i];
								qrImportSupplier->ParamByName("Bad_Debtor")->AsString				    = "F";
								qrImportSupplier->ParamByName("Deleted")->AsString				    	= "F";
                        qrImportSupplier->ParamByName("CONTACT_NAME")->AsString		    	= ImportCSV.Cells[1][i];
                        qrImportSupplier->ParamByName("SUPPLIER_ID")->AsString				    	= ImportCSV.Cells[2][i];
                        qrImportSupplier->ParamByName("PHONE")->AsString				    	= ImportCSV.Cells[3][i];
                        qrImportSupplier->ParamByName("FAX")->AsString				    	= ImportCSV.Cells[4][i];
                        qrImportSupplier->ParamByName("MOBILE")->AsString				    	= ImportCSV.Cells[5][i];
                        qrImportSupplier->ParamByName("EMAIL")->AsString				    	= ImportCSV.Cells[6][i];
                        qrImportSupplier->ParamByName("LOCATION_ADDRESS")->AsString				    	= AnsiString(ImportCSV.Cells[8][i]);
                        qrImportSupplier->ParamByName("WEBORDER_TARGET_URI")->AsString				    	= ImportCSV.Cells[7][i];
                        qrImportSupplier->ParamByName("BILL_ADDRESS")->AsString				    	= ImportCSV.Cells[9][i];
                        qrImportSupplier->ParamByName("NOTE")->AsString				    	= ImportCSV.Cells[10][i];
								qrImportSupplier->ExecQuery();

                        }
                        else{
                          qrFindSupplier->Close();
                                qrFindSupplier->ParamByName("Company_Name")->AsString	= ImportCSV.Cells[0][i];
                                qrFindSupplier->Open();
                                SupplierKey  = qrFindSupplier->FieldByName("Contact_LK")->AsInteger;
                                qrUpdateSupplier->Close();
                                qrUpdateSupplier->ParamByName("Contact_LK")->AsInteger			        = SupplierKey;
								qrUpdateSupplier->ParamByName("Company_Name")->AsString		    	= ImportCSV.Cells[0][i];
								qrUpdateSupplier->ParamByName("Bad_Debtor")->AsString				    = "F";
								qrUpdateSupplier->ParamByName("Deleted")->AsString				    	= "F";
                        qrUpdateSupplier->ParamByName("CONTACT_NAME")->AsString		    	= ImportCSV.Cells[1][i];
                        qrUpdateSupplier->ParamByName("SUPPLIER_ID")->AsString				    	= ImportCSV.Cells[2][i];
                        qrUpdateSupplier->ParamByName("PHONE")->AsString				    	= ImportCSV.Cells[3][i];
                        qrUpdateSupplier->ParamByName("FAX")->AsString				    	= ImportCSV.Cells[4][i];
                        qrUpdateSupplier->ParamByName("MOBILE")->AsString				    	= ImportCSV.Cells[5][i];
                        qrUpdateSupplier->ParamByName("EMAIL")->AsString				    	= ImportCSV.Cells[6][i];
                        qrUpdateSupplier->ParamByName("LOCATION_ADDRESS")->AsString				    	= ImportCSV.Cells[8][i];
                        qrUpdateSupplier->ParamByName("WEBORDER_TARGET_URI")->AsString				    	= ImportCSV.Cells[7][i];
                        qrUpdateSupplier->ParamByName("BILL_ADDRESS")->AsString				    	= ImportCSV.Cells[9][i];
                        qrUpdateSupplier->ParamByName("NOTE")->AsString				    	= ImportCSV.Cells[10][i];
								qrUpdateSupplier->Open();

                        }
                        }
                         if (Transaction->InTransaction)
					{
						Transaction->Commit();
					}
               }
                  	ProgressBar1->Position = 0;
				if (MissingLocations == "")
				{
					if (Transaction->InTransaction)
					{
						Transaction->Commit();
					}
					if (FailedList != "")
					{
						Application->MessageBox(("Some products failed to import.\r\r" + FailedList).c_str(), "Warning", MB_OK + MB_ICONWARNING);
					}
					else
					{
						Application->MessageBox("Import complete", "Complete", MB_OK + MB_ICONINFORMATION);
					}
				}
				else
				{
					if (Transaction->InTransaction)
					{
						Transaction->Rollback();
					}
					Application->MessageBox(("Some locations are missing from the MenuMate System.\r\r" + MissingLocations).c_str(), "Error", MB_OK + MB_ICONERROR);
				}
				if (MMTransaction->InTransaction)
				{
//					MMTransaction->Commit();
				}
            }
      }
      catch(Exception &E)
      {
           if (Transaction->InTransaction)
			{
				Transaction->Rollback();
			}
			if (MMTransaction->InTransaction)
			{
				MMTransaction->Rollback();
			}
			Application->MessageBox(("Failed on item " + LastItem).c_str(), "Error", MB_ICONERROR + MB_OK);
			ProgressBar1->Position = 0;
			throw;
		}
      }

}

 bool TfrmAdministration::validate_weborder_target_uri(AnsiString uri)
{
  //  AnsiString uri = dbeWeborderTarget->Text;

    const char *hstr = uri.c_str();
    char *pend;
    const char *pstr;
    const char *strend = hstr + uri.Length();
    unsigned short p;

    if (!(pstr = strchr(hstr, ':')) || pstr == hstr || pstr == strend) {
        Application->MessageBox(
          "Invalid URI; The URI you enter must follow the format host:port   " ,
          "Error", MB_ICONERROR | MB_OK);
        return false;
    }

    if (!(p = strtoul(++pstr, &pend, 10)) || pend != strend) {
        Application->MessageBox(
          "The URI you entered is invalid. Missing or incorrect port " ,
          "Error", MB_ICONERROR | MB_OK);
        return false;
    }

    return true;
}

int TfrmAdministration::DuplicateCodesBikeShop(TCsv *ImportCSV,AnsiString &StockCode,int &Row)
{

   if ( !normalImport)
   {


                    if (!Transaction->DefaultDatabase->Connected)
                    {
                            Application->MessageBox("You must be connected to both the MenuMate and Stock databases.", "Error", MB_OK + MB_ICONERROR);
                            return true;
                    }
                    if (!Transaction->InTransaction)
                    {
                       Transaction->StartTransaction();
                    }

                     if (!MMTransaction->InTransaction)
                    {
                             MMTransaction->StartTransaction();
                    }


               for(int i = 1; i < ImportCSV->RowCount; i++)
                {
                    int groupKey;
                    IBQuery->Close();
                    IBQuery->SQL->Text = "Select STOCK_GROUP_KEY From STOCKGROUP  Where STOCK_GROUP = :STOCK_GROUP ";
                    IBQuery->ParamByName("STOCK_GROUP")->AsString = ImportCSV->Cells[1][i].Trim();
                    IBQuery->ExecQuery();

                   if(!IBQuery->Eof)
                   {
                        groupKey=  IBQuery->Fields[0]->AsInteger;

                        IBQuery->Close();
                        IBQuery->SQL->Text = "Select STOCK_KEY From Stock Where STOCK_GROUP_KEY = :STOCK_GROUP_KEY AND CODE= :CODE and ENABLED = :ENABLED and DELETED =:DELETED ";

                        IBQuery->ParamByName("STOCK_GROUP_KEY")->AsInteger = groupKey;
                        IBQuery->ParamByName("CODE")->AsString = ImportCSV->Cells[3][i].Trim();
                        IBQuery->ParamByName("ENABLED")->AsString = "T";
                        IBQuery->ParamByName("DELETED")->AsString = "F";
                        IBQuery->ExecQuery();

                        if(!IBQuery->Eof)
                        {
                            StockCode= ImportCSV->Cells[3][i].Trim();
                            Row =  i;
                            return 1;
                        }

                   }

                   if( ImportCSV->Cells[20][i].Trim() != "")
                      {
                        IBQuery->Close();
                        IBQuery->SQL->Text = "SELECT  a.BARCODE FROM BARCODE a Where BARCODE= :BARCODE  ";
                        IBQuery->ParamByName("BARCODE")->AsString = ImportCSV->Cells[20][i].Trim();
                        IBQuery->ExecQuery();

                        if(!IBQuery->Eof)
                        {
                            StockCode= ImportCSV->Cells[20][i].Trim();
                            Row =  i;
                            return 2;
                        }

                     }



                }


        }else
        {

                if (!Transaction->DefaultDatabase->Connected)
                    {
                            Application->MessageBox("You must be connected to both the MenuMate and Stock databases.", "Error", MB_OK + MB_ICONERROR);
                            return true;
                    }
                    if (!Transaction->InTransaction)
                    {
                       Transaction->StartTransaction();
                    }

                     if (!MMTransaction->InTransaction)
                    {
                             MMTransaction->StartTransaction();
                    }


               for(int i = 1; i < ImportCSV->RowCount; i++)
                {
                        IBQuery->Close();
                        IBQuery->SQL->Text = "Select STOCK_KEY From Stock Where CODE= :CODE ";


                        IBQuery->ParamByName("CODE")->AsString = ImportCSV->Cells[3][i].Trim();
                        IBQuery->ExecQuery();

                        if(!IBQuery->Eof)
                        {
                            StockCode= ImportCSV->Cells[3][i].Trim();
                            Row =  i;
                            return 1;
                        }

                        IBQuery->Close();
                        IBQuery->SQL->Text = "Select STOCK_KEY From Stock Where DESCRIPTION= :DESCRIPTION and ENABLED = :ENABLED and DELETED =:DELETED ";


                        IBQuery->ParamByName("DESCRIPTION")->AsString = ImportCSV->Cells[4][i].Trim();
                        IBQuery->ExecQuery();

                        if(!IBQuery->Eof)
                        {
                            StockCode= ImportCSV->Cells[4][i].Trim();
                            Row =  i;
                            return 3;
                        }

                       
                      if( ImportCSV->Cells[20][i].Trim() != "")
                      {
                        IBQuery->Close();
                        IBQuery->SQL->Text = "SELECT  a.BARCODE FROM BARCODE a Where BARCODE= :BARCODE  ";
                        IBQuery->ParamByName("BARCODE")->AsString = ImportCSV->Cells[20][i].Trim();
                        IBQuery->ExecQuery();

                        if(!IBQuery->Eof)
                        {
                            StockCode= ImportCSV->Cells[20][i].Trim();
                            Row =  i;
                            return 2;
                        }

                     }



                    for(int row = 1;row < ImportCSV->RowCount; row++)
                    {
                        if (row==i)continue;

                             if (ImportCSV->Cells[4][i].Trim()== ImportCSV->Cells[4][row].Trim()  )
                             {   //check for description   in csv
                                  StockCode= ImportCSV->Cells[4][i].Trim();
                                  Row =  i;
                                  return 3;

                             }

                           if ( ImportCSV->Cells[3][i].Trim() != "")
                           {
                                 if (ImportCSV->Cells[3][i].Trim()== ImportCSV->Cells[3][row].Trim()  )
                                 {    //check for code in csv

                                     StockCode= ImportCSV->Cells[4][i].Trim();
                                      Row =  i;
                                      return 1;
                                 }

                           }


                           if ( ImportCSV->Cells[20][i].Trim() != "")
                           {
                                 if (ImportCSV->Cells[20][i].Trim()== ImportCSV->Cells[20][row].Trim()  )
                                 {    //check for barcode in csv

                                      StockCode= ImportCSV->Cells[20][i].Trim();
                                      Row =  i;
                                      return 2;
                                 }
                          }
                 }
               }
        }
   return 0;
}
//---------------------------------------------------------------------------
AnsiString TfrmAdministration::Query(AnsiString Field, AnsiString retval, AnsiString Data)
{
    IBQuery->Close();
    IBQuery->SQL->Text = "Select * From Stock Where " + Field + " = :Data";
    IBQuery->ParamByName("Data")->AsString = Data;
    IBQuery->ExecQuery();
    if(!IBQuery->Eof)
    {
        return IBQuery->FieldByName(retval)->AsString;
    }
    return "";
}

//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnCloseClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnBarcodesClick(TObject *Sender)
{
	std::auto_ptr<BarcodeSync::TBarcodeSyncForm> form(new BarcodeSync::TBarcodeSyncForm());
	BarcodeSync::IBarcodeServiceFactory& barcodeServiceFactory = BarcodeSync::BarcodeServiceFactory();
	BarcodeSync::BarcodeSyncFormController controller(form.get(), barcodeServiceFactory);
	form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnClearSuppliersClick(TObject *Sender)
{
	if (Application->MessageBox("This will delete all suppliers from the system!\rDo you wish to continue?",
										 "Warning",
										 MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == ID_OK)
	{
		if (!Transaction->InTransaction)
		{
			Transaction->StartTransaction();
		}
		qrClearSuppliers->Close();
		qrClearSuppliers->ExecQuery();
		if (Transaction->InTransaction)
		{
			Transaction->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnClearStockClick(TObject *Sender)
{
	if (Application->MessageBox("This will delete all stock from the system!\rDo you wish to continue?",
										 "Warning",
										 MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == ID_OK)
	{
		if (!Transaction->InTransaction)
		{
			Transaction->StartTransaction();
		}
		qrClearStock->Close();
		qrClearStock->ExecQuery();
		qrClearBarcodes->Close();
		qrClearBarcodes->ExecQuery();
		if (Transaction->InTransaction)
		{
			Transaction->Commit();
	        Application->MessageBox("Stock has been cleared", "Warning", MB_OKCANCEL + MB_ICONWARNING);

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnCreateDBClick(TObject *Sender)
{
	AnsiString DBName = "localhost:C:\\Program Files\\MenuMate\\Stock.fdb";
	if (InputQuery("Database Name", "Please enter the Interbase database name.", DBName))
	{
		if (DBName != "")
		{
			dmStockData->CreateDB(DBName);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnClearHistoryClick(TObject *Sender)
{
	if (Application->MessageBox("This will delete all stock history from the system!\rDo you wish to continue?",
										 "Warning",
										 MB_OKCANCEL + MB_ICONWARNING + MB_DEFBUTTON2) == ID_OK)
	{
		if (!Transaction->InTransaction)
		{
			Transaction->StartTransaction();
		}

		// also clears stocktrans ( cascade delete )
		qrClearTransBatch->Close();
		qrClearTransBatch->ExecQuery();
//		qrClearImportTrack->Close();
//		qrClearImportTrack->ExecQuery();
		qrClearStocktake->Close();
		qrClearStocktake->ExecQuery();
		qrClearPurchaseStock->Close();
		qrClearPurchaseStock->ExecQuery();
		qrClearPurchaseOrders->Close();
		qrClearPurchaseOrders->ExecQuery();
		qrStocktakeProfiles->Close();
		qrStocktakeProfiles->ExecQuery();

		if (Transaction->InTransaction)
		{
			Transaction->Commit();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnAddStockLocationsClick(TObject *Sender)
{
	AnsiString LocationName = "";
	if (dmMMData->dbMenuMate->Connected)
	{
		std::auto_ptr<TfrmSelectLocation>frmSelectLocation(new TfrmSelectLocation(NULL));
		if (!frmSelectLocation->Execute(LocationName))
		{
			LocationName = "";
		}
	}
	else
	{
		if (!InputQuery("New Location", "Please enter the name of the new Location.", LocationName))
		{
			LocationName = "";
		}
	}
	if (LocationName != "")
	{
		Stock::TStockControl CreateLocation(dmStockData->dbStock);
		std::set<int> StockKeys;
		qrStockList->Close();
		for (qrStockList->ExecQuery(); !qrStockList->Eof; qrStockList->Next())
		{
			StockKeys.insert(qrStockList->FieldByName("Stock_Key")->AsInteger);
		}
		CreateLocation.CreateLocations(StockKeys, LocationName);
/*		if (!Transaction->InTransaction)
		{
			Transaction->StartTransaction();
		}
		qrStockList->Close();
		for (qrStockList->ExecQuery(); !qrStockList->Eof; qrStockList->Next())
		{
			dmStockData->CreateStockLocation(Transaction, qrStockList->FieldByName("Stock_Key")->AsInteger, LocationName);
		}
		if (Transaction->InTransaction)
		{
			Transaction->Commit();
		}*/
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnShowReceiptClick(TObject *Sender)
{
   	const char ESC       = 0x1B;
	const char GS        = 0x1D;
	//---------------------------------------------------------------------------
//	const char CR       = 0x0D; // Carriage Return
//	const char LF       = 0x0A; // Line Feed
//	const char FF       = 0x0C; // Form Feed

	AnsiString InvoiceNumberText = "";
	if (InputQuery("Invoice Number", "Enter the invoice number of the receipt you wish to view.", InvoiceNumberText))
	{
		if (InvoiceNumberText != "")
		{
			try
			{
				int InvoiceNumber = StrToInt(InvoiceNumberText);
				if (!MMTransaction->InTransaction)
				{
					MMTransaction->StartTransaction();
				}
				try
				{
					qrReceipt->Close();
					qrReceipt->ParamByName("Invoice_Number")->AsInteger = InvoiceNumber;
					qrReceipt->Open();
					if (qrReceipt->Eof)
					{
						Application->MessageBox("Invoice not found.", "Error", MB_OK + MB_ICONERROR);
					}
					else
					{
						TMemoryStream *ReceiptStream = new TMemoryStream;
						try
						{
							TBlobField *ReceiptBlob = (TBlobField *)qrReceipt->FieldByName("Receipt");
							ReceiptBlob->SaveToStream(ReceiptStream);

							TStringList *Lines = new TStringList;
							try
							{
								ReceiptStream->Position = 0;
								AnsiString TrimmedLine;
								char *Line = (char *)ReceiptStream->Memory;
								for(int j=0; j<ReceiptStream->Size; j++)
								{
									if (Line[j] == ESC)
									{
//										if (Line[j+1] == PRINT_MODE)
										{
											j += 2;
										}
									}
									else if (Line[j] == GS)
									{
//										if (Line[j+1] == CUT_MODE)     ]
										{
											j += 2;
										}
//										else if (Line[j+1] == PRINT_GRAPHIC)
//										{
//											j += 2;
//										}
									}
									else if (Line[j] ==Tntsysutils::CR )
									{
										if (Line[j+1] == Tntsysutils::LF)
										{
											j += 1;
										}
										Lines->Add(TrimmedLine);
										TrimmedLine = "";
									}
									else if (Line[j] == Tntsysutils::LF)
									{
										if (Line[j+1] == Tntsysutils::CR)
										{
											j += 1;
										}
										Lines->Add(TrimmedLine);
										TrimmedLine = "";
									}
									else
									{
										if(Line[j] == char(196))
										{
											TrimmedLine += "-";
										}
										else
										{
											TrimmedLine += Line[j];
										}
									}
								}
								if(TrimmedLine != "")
								Lines->Add(TrimmedLine);

								memReceipt->Lines->Assign(Lines);
							}
							__finally
							{
								delete Lines;
							}
						}
						__finally
						{
							delete ReceiptStream;
						}
					}
				}
				__finally
				{
					if (MMTransaction->InTransaction)
					{
						MMTransaction->Commit();
					}
				}
			}
			catch (Exception &E)
			{
				Application->MessageBox("Please enter a valid number.", "Error", MB_OK + MB_ICONERROR);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::BitBtn6Click(TObject *Sender)
{
	memReceipt->Lines->Clear();
	IBSQLMonitor->Enabled = !IBSQLMonitor->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::IBSQLMonitorSQL(AnsiString EventText,
		TDateTime EventTime)
{
	memReceipt->Lines->Add(EventTime.DateTimeString() + ": " + EventText);
	while(memReceipt->Lines->Count > 1024)
	{
		memReceipt->Lines->Delete(0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnIBLogClick(TObject *Sender)
{
	IBLogService->Active = false;
	IBLogService->ServerName = dmStockData->dbinfStock->DBSiteName; // Needs to resolve to IP
	IBLogService->Protocol = TCP;
	IBLogService->Params->Add("user_name=SYSDBA");
	IBLogService->Params->Add("password=masterkey");
	IBLogService->Attach();
	try
	{
		Screen->Cursor = crHourGlass;
		IBLogService->ServiceStart();
		memReceipt->Lines->BeginUpdate();
		while (IBLogService->IsServiceRunning)
		{
			String Line;
			while (!IBLogService->Eof)
			{
				Line += IBLogService->GetNextChunk();
			}
			memReceipt->Lines->SetText(Line.c_str());
		}
	}
	__finally
	{
		Screen->Cursor = crDefault;
		memReceipt->Lines->EndUpdate();
		if (IBLogService->Active)
		{
			IBLogService->Detach();
		}
	}
}
//---------------------------------------------------------------------------
void TfrmAdministration::RepairDB(TIBDatabase *IBDatabase)
{
	if (IBDatabase->Connected)
	{
		IBDatabaseInfo->Database = IBDatabase;
		if (IBDatabaseInfo->UserNames->Count > 1)
		{
			Application->MessageBox("There must be nothing connected to the database.", "Error", MB_ICONERROR + MB_OK);
		}
		else
		{
			memReceipt->Lines->Clear();
			AnsiString Cmd;
			IBDatabase->Connected = false;
			Application->MessageBox("It is now safe to make a copy of the database.\rClick Ok when ready to check the database integrity.", "Backup", MB_ICONINFORMATION + MB_OK);
			try
			{
				Cmd = "gfix.exe -v -full -z -user \"SYSDBA\" -password \"masterkey\" \"" + IBDatabase->DatabaseName + "\"";
				memReceipt->Lines->Add(Cmd);
				if (!GetCmdLnOutput(Cmd, memReceipt->Lines))
				{
					Application->MessageBox("Failed to run command. Make sure gfix.exe is in your MenuMate directory.", "Error", MB_ICONERROR + MB_OK);
				}
				else
				{
					if (Application->MessageBox("Do you want to try fixing it?.\r"
														 "Make sure you have a backup copy first! (You will need to do this from Windows Explorer on the server)", "Fix?", MB_ICONQUESTION + MB_YESNO) == ID_YES)
					{
						Cmd = "gfix.exe -mend -full -ignore -z -user \"SYSDBA\" -password \"masterkey\" \"" + IBDatabase->DatabaseName + "\"";
						memReceipt->Lines->Add(Cmd);
						GetCmdLnOutput(Cmd, memReceipt->Lines);
						Cmd = "gfix.exe -v -full -z -user \"SYSDBA\" -password \"masterkey\" \"" + IBDatabase->DatabaseName + "\"";
						memReceipt->Lines->Add(Cmd);
						GetCmdLnOutput(Cmd, memReceipt->Lines);
					}
					if (Application->MessageBox("Do you want to do a backup/restore?.", "Backup/Restore?", MB_ICONQUESTION + MB_YESNO) == ID_YES)
					{
						Cmd = "gbak.exe -backup -v -ignore -z -user \"SYSDBA\" -password \"masterkey\" \"" + IBDatabase->DatabaseName + "\" \"" + ChangeFileExt(IBDatabase->DatabaseName, ".gbk") + "\"";
						memReceipt->Lines->Add(Cmd);
						if (!GetCmdLnOutput(Cmd, memReceipt->Lines))
						{
							Application->MessageBox("Failed to run command. Make sure gfix.exe is in your MenuMate directory.", "Error", MB_ICONERROR + MB_OK);
						}
						else
						{
							Cmd = "gbak.exe -create -v -ignore -z -user \"SYSDBA\" -password \"masterkey\" \"" + ChangeFileExt(IBDatabase->DatabaseName, ".gbk") + "\" \"" + ChangeFileExt(IBDatabase->DatabaseName, ".fixed.fdb") + "\"";
							memReceipt->Lines->Add(Cmd);
							GetCmdLnOutput(Cmd, memReceipt->Lines);
							Application->MessageBox("If this was successful, there wil be a database on the server with the extension '.fixed.fdb'.\rTry using this new file.", "Complete", MB_ICONINFORMATION + MB_OK);
						}
					}
				}
			}
			__finally
			{
				IBDatabase->Connected = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
bool TfrmAdministration::GetCmdLnOutput(AnsiString ExecCmd, TStrings *StringList)
{
	int FBreak;
	Screen->Cursor = crHourGlass;
	try
	{
		Application->ProcessMessages();

		// If NT do security stuff
		SECURITY_DESCRIPTOR sd;
		SECURITY_ATTRIBUTES sa;
		LPSECURITY_ATTRIBUTES lpsa = NULL;
		if(Win32Platform == VER_PLATFORM_WIN32_NT)//IsWindowsNT)
		{
			InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
			SetSecurityDescriptorDacl(&sd, true, NULL, false);
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = true;
			sa.lpSecurityDescriptor = &sd;
			lpsa = &sa;
		}
		// Create the Pipe and get r/w handles
		HANDLE hReadPipe;
		HANDLE hWritePipe;
		/*assert(*/CreatePipe(&hReadPipe,
								&hWritePipe,
								lpsa,
								2500000);//);

		// initialize STARTUPINFO struct
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;
		si.hStdOutput = hWritePipe;
		si.hStdError = hWritePipe;
		PROCESS_INFORMATION pi;
//		assert(hWritePipe);

		// Run the cmdLine tool
		Application->ProcessMessages();

		bool ProcOpen = CreateProcess(NULL,
							  ExecCmd.c_str(),
							  NULL,//security
							  NULL,// security
							  TRUE,//inherits handles
							  0, 0, 0, &si, &pi);
		if(ProcOpen)
		{
			CloseHandle(pi.hThread);
			WaitForSingleObject(pi.hProcess, 90000);
			// read from the pipe and put output to strings
//			assert(hReadPipe);
			DWORD BytesRead; //unsigned long
			char dest[4000];
			bool RdLoopDone = false;
			FBreak = 1;

			while(!RdLoopDone)
			{
				memset(dest, 0, 4000);
				ReadFile(hReadPipe, &dest, sizeof(dest), &BytesRead, NULL);
				StringList->Add((AnsiString)dest);
				if(BytesRead < 4000) RdLoopDone = true;
				if(FBreak > 150) RdLoopDone = true;
				FBreak++;
			}
			// Process cleanup
			CloseHandle(hReadPipe);
			CloseHandle(hWritePipe);
			CloseHandle(pi.hProcess);
			Screen->Cursor = crDefault;
			return true;
		}
		else
		{
			Screen->Cursor = crDefault;
			return false;
		}
	}
	__finally
	{
		Screen->Cursor = crDefault;
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::btnRepairMMClick(TObject *Sender)
{
	RepairDB(dmMMData->dbMenuMate);
}
//---------------------------------------------------------------------------

void __fastcall TfrmAdministration::btnRepairStockClick(TObject *Sender)
{
	RepairDB(dmStockData->dbStock);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAdministration::BitBtn1Click(TObject *Sender)
{
	if (UpdateArchiveCategories && UpdateArchiveCategories->Completed())
	{
		delete UpdateArchiveCategories;
		UpdateArchiveCategories = NULL;
	}
	if (!UpdateArchiveCategories && dmMMData->dbMenuMate->Connected)
	{
		UpdateArchiveCategories = new CategoryTranslation::TUpdateArchiveCategories(dmMMData->dbMenuMate->DatabaseName, ProgressBar1);
		if (!UpdateArchiveCategories->StartUpdate())
		{
			delete UpdateArchiveCategories;
			UpdateArchiveCategories = NULL;
		}
	}
}
//---------------------------------------------------------------------------
#include "Users.h"
void __fastcall TfrmAdministration::btnUsersClick(TObject *Sender)
{
	frmUsers->ShowModal();
}
//---------------------------------------------------------------------------
#include "ConfigureRolesForm.h"
void __fastcall TfrmAdministration::btnSecurityClick(TObject *Sender)
{
	std::auto_ptr<Security::TfrmConfigureRoles> form(new Security::TfrmConfigureRoles());
	Security::ConfigureRolesFormController controller(form.get());
	form->Show();
}

  bool  TfrmAdministration::NormalImportCaseValidations(TCsv *ImportCSV,bool menuUpdate,bool recipeupdate,bool itemSizeUnique)
  {
     TimportMenu *menu;
     const int Selector[7] = {0, 1, 3, 9, 14, 15, 20 };
   
      int columnCheck;
      const int Numeric[12]={7,8,10,11,12,13,16,17,19,25, 30,41};

         for (int row=1; row<ImportCSV->RowCount; row++)
            {
                 for (int j=0; j<7; j++)
                     {
                          if (j==6 || j==2)continue;

                            if ( ImportCSV->Cells[Selector[j]] [row].Trim()=="" )
                            {
                                Application->MessageBox(("You must enter" +  BikeShopHeader[0][j] + " in Row No " + row + " ").c_str() , "Error", MB_OK + MB_ICONERROR);
                                return false;
                            }
                      }


                        if (menuUpdate)
                        {
                           columnCheck=38;
                            if(recipeupdate)
                            {
                                columnCheck=41;
                            }

                        }else
                        {
                            columnCheck=20;
                        }
                  //check if any mandatory field is left blank    in stock columns
                for(int column =0; column<=columnCheck;column++)
                {

                    if ( column!=18  && (column!=20) && (column!=24) )
                    {
                        if (  ( (column==2) || (column==6) || (column==7)  ) ) continue;
                          if (   (column==32) || (column==33) || (column==34) || (column==35)  )  continue;   //donot check tax and stock code



                           if (ImportCSV->Cells[column][row].Trim()=="")
                           {
                               Application->MessageBox(("Please Enter Info in  Row No " + IntToStr(row) + " and column no " + IntToStr(column+1)  ).c_str() , "Error", MB_OK + MB_ICONERROR);
                              return false;
                           }

                    }

                }

               // check if only numberic values are entered in numeric columns
                for (int j=0; j<12; j++)
                     {
                            std::string word= ImportCSV->Cells[Numeric[j]] [row].Trim().c_str();
                            bool isNumber = true;
                            for(string::const_iterator k = word.begin(); k != word.end(); ++k)
                              {
                                  if ((*k)=='.')continue;
                                  isNumber &= (   isdigit(*k));
                              }
                             if (! isNumber)
                             {
                              Application->MessageBox(("You must enter Numeric Value in Row No " +  IntToStr(row)+ " and column no  " + IntToStr(Numeric[j]) + " ").c_str() , "Error", MB_OK + MB_ICONERROR);
                                    return false;
                             }

                             if ( atof(  (ImportCSV->Cells[Numeric[j]] [row].c_str()  ) ) <0    )
                             {
                                   Application->MessageBox(("You have  enter Negative  Value in Row No " +  IntToStr(row)+ " and column no  " + IntToStr(Numeric[j]+1) + " ").c_str() , "Error", MB_OK + MB_ICONERROR);
                                    return false;
                             }

                      }

               //check for special characters
                    for(int column =0; column<=columnCheck;column++)
                    {
                        std::string  word= ImportCSV->Cells[column][row].Trim().c_str();

                         if (column ==4)continue;

                        if (word.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_%=( )-.") != std::string::npos)
                        {
                          Application->MessageBox(("You have entered special characters in Row No " +  IntToStr(row)+ " and column no  " + IntToStr(column+1) + " ").c_str() , "Error", MB_OK + MB_ICONERROR);
                          return false;

                        }

                    }


          if(menuUpdate)
             {
                  // check for menu name length ,it should not be greator than 15 characters
                   if(ImportCSV->Cells[26][row].Trim().Length() >15)
                   {
                      Application->MessageBox(("Menu Name cannot be greator than 15 characters.") , "Error", MB_OK + MB_ICONERROR);
                      return false;


                   }

                 // check for common  tax name
                //check whether tax has been enterred in correct format , TaxName-TaxPercent%
                  if(!CheckTaxFormat(ImportCSV->Cells[32][row].Trim(),32,row))return false;
                  if(!CheckTaxFormat(ImportCSV->Cells[33][row].Trim(),33,row))return false;
                  if(!CheckTaxFormat(ImportCSV->Cells[34][row].Trim(),34,row))return false;
                  if(!CheckTaxFormat(ImportCSV->Cells[35][row].Trim(),35,row))return false;



                  std::string salesTaxString  =  ImportCSV->Cells[32][row].Trim().c_str();
                  std::string  serviceChargeString =ImportCSV->Cells[33][row].Trim().c_str();
                  std::string  localTaxString =   ImportCSV->Cells[34][row].Trim().c_str();
                  std::string  profitTaxString =  ImportCSV->Cells[35][row].Trim().c_str();


                  AnsiString salesTax = menu->GetTaxName(salesTaxString).c_str();
                  AnsiString serviceCharge= menu->GetTaxName(serviceChargeString).c_str();
                  AnsiString localTax= menu->GetTaxName(localTaxString).c_str();
                  AnsiString profitTax= menu->GetTaxName(profitTaxString).c_str();

                  double  salesTaxPercent = menu->GetTaxPercent(salesTaxString);
                  double  serviceChargePercent = menu->GetTaxPercent(serviceChargeString);
                  double  localTaxPercent = menu->GetTaxPercent(localTaxString);
                  double  profitTaxPercent = menu->GetTaxPercent(profitTaxString);
                  AnsiString taxHeader[4]={salesTax,serviceCharge,localTax,profitTax};
                  AnsiString taxname[4]={"SalesTax","ServiceCharge","LocalTax","ProfitTax"};
                  double  taxPercent[4]={salesTaxPercent,serviceChargePercent,localTaxPercent,profitTaxPercent};

                 if ( profitTaxPercent >0)
                  {
                         if ((salesTaxPercent>0)  || (localTaxPercent >0)  )
                         {
                            Application->MessageBox(("Profit  Tax Percent cannot be applied simultaneously with other Tax Except Service Charge.") , "Error", MB_OK + MB_ICONERROR);
                           return false;

                         }

                  } else
                  {
                       int result =1;

                       for (int i=0 ;i<=3;i++)
                       {
                           if(taxHeader[i].Trim()!="" )
                           {
                             result  *= 0;
                           }
                           else
                           {
                              result  *= 1;
                            }
                        }
                       if(result)
                       {
                          Application->MessageBox(("Please Enter Any one Of the Taxes.") , "Error", MB_OK + MB_ICONERROR);
                            return false;
                         }
                 }

                  for (int i=0 ;i<=3;i++)
                   {
                           // check for tax value    whether it is positive or not

                       if (taxPercent[i] <0)
                         {
                            Application->MessageBox(("The " + taxname[i] + " Percent  in   " +  IntToStr(row)+ " is Negative.Please Enter Correct value.").c_str() , "Error", MB_OK + MB_ICONERROR);
                            return false;
                         }


                         for(int j= i+1;j<=3;j++)
                          {
                             if(taxHeader[i].Trim()=="" )continue;
                              if ( taxHeader[i] ==taxHeader[j] ) // check for same tax name
                              {
                                 Application->MessageBox(("The " + taxname[i] + " name and " + taxname[j] + " name in  row No " +  IntToStr(row)+ " is same.Please enter different tax names.").c_str() , "Error", MB_OK + MB_ICONERROR);
                                 return false;

                              }

                          }
                   }




                 if (recipeupdate)
                 {
                           // check for location value
                         if(ImportCSV->Cells[9][row].Trim() != ImportCSV->Cells[40][row].Trim()  )
                         {
                           Application->MessageBox(("You have Entered Different Location Name  in Row No " +  IntToStr(row)+ " and column no 10 and 41 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                                 return false;
                         }
                         else if (ImportCSV->Cells[3][row].Trim() != ImportCSV->Cells[39][row].Trim()  )
                          {
                              Application->MessageBox(("You have Entered Different Stock Code  in Row No " +  IntToStr(row)+ " and column no 4 and 40 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                             return false;
                         }
                  }
              }
            }


       return true;
          }
          
void  TfrmAdministration::BikeShopImport()
{
     bool updateMenuInfo=false;
     bool updateRecipieInfo=false;
     const int Selector[7] = {0, 1, 3, 9, 14, 15, 20 };
     bool recipeupdate=false;
     bool itemSizeUniqueCase=false;
     TimportMenu *menu;
     TfrmMenuEdit *createXml;
     const int Numeric[12]={7,8,10,11,12,13,16,17,19,25, 30,41};
     int columnCheck;
    
   	if (dlgOpen->Execute())
	{
		ImportCSV.Qualifier = '\"';
        ImportCSV.LoadFromFile(dlgOpen->FileName);
        bool menuUpdate=false;
        //update the menu
        if(Application->MessageBox(AnsiString("Do you want to Update Menu ?").c_str(), "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
        {
            menuUpdate=true;
                // update the recipe
            if(Application->MessageBox(AnsiString("Do you want to Update Recipe ?").c_str(), "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
            {
                recipeupdate=true;

                 if(Application->MessageBox(AnsiString("Do You want ItemSize To be Unique?").c_str(), "Warning", MB_YESNO + MB_ICONWARNING) == ID_YES)
                 {
                     itemSizeUniqueCase=true;
                 }


            }
        }

         if( !itemSizeUniqueCase || (!menuUpdate))
         {
                if(!NormalImportCaseValidations(&ImportCSV,menuUpdate,recipeupdate,itemSizeUniqueCase) ) return;
                normalImport=true;
         }
         else
         {
         //validations of bikeshop import

           normalImport=false;



           for (int row=1; row<ImportCSV.RowCount; row++)
            {
                 for (int j=0; j<7; j++)
                     {
                          if (j==6)continue;

                            if ( ImportCSV.Cells[Selector[j]] [row].Trim()=="" )
                            {
                                Application->MessageBox(("You must enter" +  BikeShopHeader[0][j] + " in Row No " + row + " ").c_str() , "Error", MB_OK + MB_ICONERROR);
                                return;
                            }
                      }


                        if (menuUpdate)
                        {
                           columnCheck=38;
                            if(recipeupdate)
                            {
                                columnCheck=41;
                            }

                        }else
                        {
                            columnCheck=20;
                        }
                  //check if any mandatory field is left blank    in stock columns
                for(int column =0; column<=columnCheck;column++)
                {

                    if ( column!=18  && (column!=20) && (column!=24) )
                    {
                        if (   ( (column==2) || (column==6) || (column==7)  ) ) continue;

                        if (   (column==32) || (column==33) || (column==34) || (column==35) )  continue;   //donot check tax


                           if (ImportCSV.Cells[column][row].Trim()=="")
                           {
                               Application->MessageBox(("Please Enter Info in  Row No " + IntToStr(row) + " and column no " + IntToStr(column+1)  ).c_str() , "Error", MB_OK + MB_ICONERROR);
                               return;
                           }

                    }

                }

               // check if only numberic values are entered in numeric columns
                for (int j=0; j<12; j++)
                     {
                            std::string word= ImportCSV.Cells[Numeric[j]] [row].Trim().c_str();
                            bool isNumber = true;
                            for(string::const_iterator k = word.begin(); k != word.end(); ++k)
                              {
                                  if ((*k)=='.')continue;
                                  isNumber &= (   isdigit(*k));
                              }
                             if (! isNumber)
                             {
                              Application->MessageBox(("You must enter Numeric Value in Row No " +  IntToStr(row)+ " and column no  " + IntToStr(Numeric[j]+1) + " ").c_str() , "Error", MB_OK + MB_ICONERROR);
                                    return;
                             }

                             if ( atof(  (ImportCSV.Cells[Numeric[j]] [row].c_str()  ) ) <0    )
                             {
                                   Application->MessageBox(("You have  enter Negative  Value in Row No " +  IntToStr(row)+ " and column no  " + IntToStr(Numeric[j]+1) + " ").c_str() , "Error", MB_OK + MB_ICONERROR);
                                    return;
                             }

                      }

               //chcek for special characters
                    for(int column =0; column<=columnCheck;column++)
                    {
                        std::string  word= ImportCSV.Cells[column][row].Trim().c_str();

                        if (column ==4)continue;

                        if (word.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_%=( )-.") != std::string::npos)
                        {
                          Application->MessageBox(("You have entered special characters in Row No " +  IntToStr(row)+ " and column no  " + IntToStr(column+1) + " ").c_str() , "Error", MB_OK + MB_ICONERROR);
                          return;

                        }

                    }


          if(menuUpdate)
             {
                   // check for menu name length ,it should not be greator than 15 characters
                   if(ImportCSV.Cells[26][row].Trim().Length() >15)
                   {
                      Application->MessageBox(("Menu Name cannot be greator than 15 characters.") , "Error", MB_OK + MB_ICONERROR);
                      return;

                   }


                 //check for category value
                 if ( ImportCSV.Cells[0][row].Trim() != ImportCSV.Cells[37][row].Trim() )
                 {
                     Application->MessageBox(("You have Entered Different Category Name  in Row No " +  IntToStr(row)+ " and column no 1 and 38 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                          return;
                 } //check for category group value
                 else if (ImportCSV.Cells[1][row].Trim() != ImportCSV.Cells[28][row].Trim()  )
                 {
                    Application->MessageBox(("You have Entered Different Category Group  Name  in Row No " +  IntToStr(row)+ " and column no 2 and 29 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                          return;
                 } //stock sub group
                 else if (ImportCSV.Cells[2][row].Trim() != ImportCSV.Cells[29][row].Trim() )
                 {
                    Application->MessageBox(("You have Entered Different Category Group  Name  in Row No " +  IntToStr(row)+ " and column no 3 and 30 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                          return;

                 }  //check for price
                 else if (ImportCSV.Cells[17][row].Trim() != ImportCSV.Cells[30][row].Trim() )
                 {
                    Application->MessageBox(("You have Entered Different Price  in Row No " +  IntToStr(row)+ " and column no 18 and 31 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                          return;

                 }//stock code
                 else if (ImportCSV.Cells[3][row].Trim() != ImportCSV.Cells[31][row].Trim()  )
                 {
                    Application->MessageBox(("You have Entered Different Stock Code  in Row No " +  IntToStr(row)+ " and column no 4 and 32 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                          return;
                 } //check for item code
                 else if(ImportCSV.Cells[23][row].Trim() != ImportCSV.Cells[31][row].Trim())
                 {
                    Application->MessageBox(("You have Entered Different Size Name Part 2   in Row No " +  IntToStr(row)+ " and column no 24 and 33 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                          return;
                 }//check for menu item name
                   else if(ImportCSV.Cells[21][row].Trim() != ImportCSV.Cells[29][row].Trim())
                 {
                    Application->MessageBox(("You have Entered Different menu Item Name  in Row No " +  IntToStr(row)+ " and column no 22 and 30 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                          return;
                 }


                 // check for common  tax name

                     //check whether tax has been enterred in correct format , TaxName-TaxPercent%
                     if(!CheckTaxFormat(ImportCSV.Cells[32][row].Trim(),32,row))return ;
                  if(!CheckTaxFormat(ImportCSV.Cells[33][row].Trim(),33,row))return ;
                  if(!CheckTaxFormat(ImportCSV.Cells[34][row].Trim(),34,row))return ;
                  if(!CheckTaxFormat(ImportCSV.Cells[35][row].Trim(),35,row))return ;



                  std::string salesTaxString  =  ImportCSV.Cells[32][row].Trim().c_str();


                  std::string  serviceChargeString =ImportCSV.Cells[33][row].Trim().c_str();
                  std::string  localTaxString =   ImportCSV.Cells[34][row].Trim().c_str();
                  std::string  profitTaxString =  ImportCSV.Cells[35][row].Trim().c_str();


                  AnsiString salesTax = menu->GetTaxName(salesTaxString).c_str();
                  AnsiString serviceCharge= menu->GetTaxName(serviceChargeString).c_str();
                  AnsiString localTax= menu->GetTaxName(localTaxString).c_str();
                  AnsiString profitTax= menu->GetTaxName(profitTaxString).c_str();



                  double  salesTaxPercent = menu->GetTaxPercent(salesTaxString);
                  double  serviceChargePercent = menu->GetTaxPercent(serviceChargeString);
                  double  localTaxPercent = menu->GetTaxPercent(localTaxString);
                  double  profitTaxPercent = menu->GetTaxPercent(profitTaxString);
                  AnsiString taxHeader[4]={salesTax,serviceCharge,localTax,profitTax};
                  AnsiString taxname[4]={"SalesTax","ServiceCharge","LocalTax","ProfitTax"};
                  double  taxPercent[4]={salesTaxPercent,serviceChargePercent,localTaxPercent,profitTaxPercent};

                 if ( profitTaxPercent >0)
                  {
                         if ((salesTaxPercent>0)  || (localTaxPercent >0)  )
                         {
                            Application->MessageBox(("Profit Tax Percent cannot be applied simultaneously with other Tax Except Service Charge.") , "Error", MB_OK + MB_ICONERROR);
                            return;

                         }

                  } else
                  {
                       int result =1;

                       for (int i=0 ;i<=3;i++)
                       {
                           if(taxHeader[i].Trim()!="" )
                           {
                             result  *= 0;
                           }
                           else
                           {
                              result  *= 1;
                            }
                        }
                       if(result)
                       {
                          Application->MessageBox(("Please Enter Any one Of the Taxes.") , "Error", MB_OK + MB_ICONERROR);
                            return;
                         }
                 }


                  for (int i=0 ;i<=3;i++)
                   {
                         // check for tax value    whether it is positive or not
                       if(taxPercent[i]==0)
                         {
                          // Application->MessageBox(("The " + taxname[i] + " Percent  in  row No " +  IntToStr(row)+ " is invalid.Please Enter Correct value.").c_str() , "Error", MB_OK + MB_ICONERROR);
                         //   return;
                         }else if (taxPercent[i] <0)
                         {
                            Application->MessageBox(("The " + taxname[i] + " Percent  in   " +  IntToStr(row)+ " is Negative.Please Enter Correct value.").c_str() , "Error", MB_OK + MB_ICONERROR);
                            return;
                         }


                         for(int j= i+1;j<=3;j++)
                          {
                              if(taxHeader[i].Trim()=="" )continue;
                             if ( taxHeader[i] ==taxHeader[j] ) // check for same tax name
                              {
                                 Application->MessageBox(("The " + taxname[i] + " name and " + taxname[j] + " name in  row No " +  IntToStr(row)+ " is same.Please enter different tax names.").c_str() , "Error", MB_OK + MB_ICONERROR);
                                 return;
                            }
                         }
                   }
                 if (recipeupdate)
                 {
                           // check for location value
                         if(ImportCSV.Cells[9][row].Trim() != ImportCSV.Cells[40][row].Trim()  )
                         {
                           Application->MessageBox(("You have Entered Different Location Name  in Row No " +  IntToStr(row)+ " and column no 10 and 41 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                                  return;
                         }  //stock  code
                          else if (ImportCSV.Cells[31][row].Trim() != ImportCSV.Cells[39][row].Trim()  )
                          {
                              Application->MessageBox(("You have Entered Different Stock Code  in Row No " +  IntToStr(row)+ " and column no 32 and 40 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                             return;
                         }
                         else if (ImportCSV.Cells[3][row].Trim() != ImportCSV.Cells[39][row].Trim()  )
                          {
                              Application->MessageBox(("You have Entered Different Stock Code  in Row No " +  IntToStr(row)+ " and column no 4 and 40 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                             return;
                         }   //check for item code
                         else if(ImportCSV.Cells[23][row].Trim() != ImportCSV.Cells[39][row].Trim())
                         {
                            Application->MessageBox(("You have Entered Different Size Name Part 2   in Row No " +  IntToStr(row)+ " and column no 24 and 40 ").c_str() , "Error", MB_OK + MB_ICONERROR);
                             return;
                         }
                  }

                      //remove all the spaces
                        AnsiString  desp=menu->delUnnecessary( ImportCSV.Cells[4][row].Trim().c_str() ).c_str();
                        AnsiString  str1=menu->delUnnecessary( ImportCSV.Cells[22][row].Trim().c_str() ).c_str();
                        AnsiString  str2=menu->delUnnecessary( ImportCSV.Cells[21][row].Trim().c_str() ).c_str();
                        AnsiString  str3=menu->delUnnecessary( ImportCSV.Cells[23][row].Trim().c_str() ).c_str();

                       /// check for description and size combination
                        if (desp != str1+str2+str3)
                        {

                                Application->MessageBox( ("You have Entered Wrong Description(E = W + V + X)  in Row No " +  IntToStr(row)+ " .").c_str() , "Error", MB_OK + MB_ICONERROR);
                                return;
                        }

              }
            }

          }

          AnsiString stockCode;
          int row;
          int duplicateCheckResult = DuplicateCodesBikeShop(&ImportCSV,stockCode,row);
          removeDuplicateStockCode=false;
            //check for duplicate stock codes
             if( (itemSizeUniqueCase && menuUpdate && recipeupdate))// || (!menuUpdate)   )
             {
                switch(duplicateCheckResult )
                {
                   case 1:
                         {   AnsiString error = " Stock Import failed, please remove or rename Duplicate Stock Code " + stockCode + " in Row no " + IntToStr(row) ;
                            Application->MessageBox(error.c_str(), "Error", MB_ICONERROR + MB_OK);
                            break;
                         }
                   case 2 :
                         {
                            AnsiString error = " Stock Import failed, please remove or rename Duplicate BarCode " + stockCode + " in Row no " + IntToStr(row) ;
                            Application->MessageBox(error.c_str(), "Error", MB_ICONERROR + MB_OK);
                            break;
                         }

                   case 3:
                       {
                          AnsiString error = " Stock Import failed, please remove or rename Duplicate Description " + stockCode + " in Row no " + IntToStr(row) ;
                          Application->MessageBox(error.c_str(), "Error", MB_ICONERROR + MB_OK);
                          break;
                       }
                 }
                if(!duplicateCheckResult==0)
                {  return;
                }
            }
             else
             {
                 if(!duplicateCheckResult==0)
                {
                     if(Application->MessageBox(AnsiString("One or more Stock code in csv is already assigned to existing stock code items .Do you wish to update ...???").c_str(), "Warning", MB_YESNO + MB_ICONWARNING) == ID_NO)
                     {
                        removeDuplicateStockCode=true;
                     }

                }

             }


          	if (!Transaction->DefaultDatabase->Connected)
				{
					Application->MessageBox("You must be connected to both the MenuMate and Stock databases.", "Error", MB_OK + MB_ICONERROR);
					return;
				}

				if (!Transaction->InTransaction)
				{
					Transaction->StartTransaction();
				}

                if (!MMTransaction->InTransaction)
				{
					MMTransaction->StartTransaction();
				}

               try
               {



                    //update the stock
                  bool stockCompleted =  SaveStockImport(&ImportCSV);
                  if(!stockCompleted )
                  {
                    if (Transaction->InTransaction)
                    {
                        Transaction->Rollback();
                    }
                    if (MMTransaction->InTransaction)
                    {
                        MMTransaction->Rollback();
                    }

                  }
                  else if (stockCompleted )
                    {

                     if ( menuUpdate)
                     {

                           //savig all menus in vector
                          std::vector<TimportMenu> allMenus;
                          allMenus= menu->FetchAllMenusInVector(&ImportCSV,qrCSV,recipeupdate,qrcsv1,removeDuplicateStockCode,qrClearStocktake);

                            //sending the vector to menuedit to create xml file
                          createXml->CreateXmlFromCsvFile(allMenus,itemSizeUniqueCase);
                      }

                      if (Transaction->InTransaction)
                        {
                            Transaction->Commit();
                        }

                      if (MMTransaction->InTransaction)
                        {
                            MMTransaction->Commit();
                        }
                    }
                }
                catch (Exception &E)
		        {
                    if (Transaction->InTransaction)
                    {
                        Transaction->Rollback();
                    }
                    if (MMTransaction->InTransaction)
                    {
                        MMTransaction->Rollback();
                    }
		        }

        }


}

std::string TimportMenu::delUnnecessary(std::string &str)
{
    int size = str.length();
    for(int j = 0; j<=size; j++)
    {
        for(int i = 0; i <=j; i++)
        {
            if(str[i] == ' ' )
            {
                str.erase(str.begin() + i);
            }
            else if(str[0]== ' ')
            {
                str.erase(str.begin());
            }
            else if(str[i] == '\0' && str[i-1]== ' ')
            {
                str.erase(str.end() - 1);
            }
        }
    }
    return str;
}

 bool TfrmAdministration::SaveStockImport(TCsv *ImportCSV)
 {
    bool result = false;
    AnsiString LastItem;

    int maxStockCode=0;
    int copyStockCode ;

    const int Selector[7] = {0, 1, 3, 9, 14, 15, 20 };
    try
		{

    		if (ImportCSV->RowCount > 0)
			{

				qrContactGroup->Close();
				qrContactGroup->Open();

				if (qrContactGroup->Eof)
				{
					qrLK->Close();
					qrLK->SQL->Text = "Select gen_id(gen_contact_group_key, 1) from rdb$database";
					qrLK->ExecQuery();

					qrAddContactGroup->Close();
					qrAddContactGroup->ParamByName("Contact_Group_Key")->AsInteger	= qrLK->Fields[0]->AsInteger;
					qrAddContactGroup->ParamByName("Contact_Group")->AsString		= "All";
					qrAddContactGroup->ParamByName("Sort_Order")->AsInteger			= 0;
					qrAddContactGroup->ExecSQL();
					qrLK->Close();

					qrContactGroup->Close();
					qrContactGroup->Open();
					if (qrContactGroup->Eof)
					{
						return false;
					}
				}

				AnsiString FailedList;
				AnsiString MissingLocations;
				qrLK->Close();
				qrLK->SQL->Text = "Select Max(Sort_Order) from StockGroup";
				qrLK->ExecQuery();

				int GroupSortOrder = qrLK->Fields[0]->AsInteger + 1;

				qrLK->Close();
				qrLK->SQL->Text = "Select Max(Sort_Order) from StockCategory";
				qrLK->ExecQuery();

				int CategorySortOrder = qrLK->Fields[0]->AsInteger + 1;

				qrLK->Close();

				qrLocations->Close();
				qrLocations->Open();

				enum TReading { RSuppliers, RLocations, RCategories, RGroups, RStock, RStockLocations, RStockSuppliers } Reading;
				for (int i=1; i<ImportCSV->RowCount; i++)
				{
                   if(removeDuplicateStockCode)
                   {
                     if( CheckDuplicateStockCode(ImportCSV->Cells[3][i].Trim() ) )
                     {
                       continue;
                     }
                   }

                    int StockCategoryKey;
                    int StockGroupKey;
                    int StockKey;
                    int SupplierKey;
                    bool newCategory=false;
                    for (int j=0; j<7; j++)
                    {
                        const int CSVColumn = j;
                        IBQuery->Close();
                        IBQuery->SQL->Text = "Select * From " + BikeShopHeader[1][j] + " Where " + BikeShopHeader[0][j] + " = :Data ";

                        if(j == 3 || j == 5)
                        {
                            IBQuery->SQL->Text = IBQuery->SQL->Text + " and Stock_Key = :Stock_Key ";
                            if(j == 5)
                            {
                              IBQuery->SQL->Text = IBQuery->SQL->Text + " and Supplier_Key = :Supplier_Key ";
                              IBQuery->ParamByName("Supplier_Key")->AsInteger = SupplierKey;
                            }
                            IBQuery->ParamByName("Stock_Key")->AsInteger = StockKey;
                        }

                     /*   if(j == 2)
                        {
                            IBQuery->SQL->Text = IBQuery->SQL->Text + " and Description = :Description ";
                            IBQuery->ParamByName("Description")->AsString = ImportCSV->Cells[4][i].Trim();
                        }  */

                        IBQuery->ParamByName("Data")->AsString = ImportCSV->Cells[Selector[j]][i].Trim();
                        IBQuery->ExecQuery();

                        if (IBQuery->Eof && ImportCSV->Cells[Selector[j]][i].Trim() != "")
                        {
                          switch (CSVColumn)
                          {
                            case 0:
							{
                                newCategory=true;
                                qrLK->Close();
                                qrLK->SQL->Text = "Select gen_id(Gen_Stock_Category_Key, 1) from rdb$database";
                                qrLK->ExecQuery();
                                StockCategoryKey = qrLK->Fields[0]->AsInteger;
                                qrAddCategory->Close();
                                qrAddCategory->ParamByName("Stock_Category_Key")->AsInteger	= StockCategoryKey;
                                qrAddCategory->ParamByName("Stock_Category")->AsString		= ImportCSV->Cells[0][i].Trim();
                                qrAddCategory->ParamByName("Sort_Order")->AsInteger			= 0;
                                qrAddCategory->ParamByName("GL_Code")->AsString		        = "";
                                qrAddCategory->ParamByName("Deleted")->AsString			    = "F";
                                qrAddCategory->ExecQuery();
								break;
                            }
                            case 1:
							{
                                qrLK->Close();
                                qrLK->SQL->Text = "Select gen_id(Gen_Stock_Group_Key, 1) from rdb$database";
                                qrLK->ExecQuery();
                                StockGroupKey = qrLK->Fields[0]->AsInteger;
                                qrAddGroup->Close();
                                qrAddGroup->ParamByName("Stock_Group_Key")->AsInteger	              = StockGroupKey;
                                qrAddGroup->ParamByName("Stock_Category_Key")->AsInteger              = StockCategoryKey;
                                qrAddGroup->ParamByName("Stock_Group")->AsString		              = ImportCSV->Cells[1][i].Trim();
                                qrAddGroup->ParamByName("Sort_Order")->AsInteger		              = 0;
                                qrAddGroup->ParamByName("GL_Code")->AsString		                  = "";
                                qrAddGroup->ParamByName("Deleted")->AsString			              = "F";
                                qrAddGroup->ExecQuery();
                                break;

                            }
                            case 2:
							{
                                qrLK->Close();
                                qrLK->SQL->Text = "Select gen_id(Gen_Stock_Key, 1) from rdb$database";
                                qrLK->ExecQuery();
                                StockKey = qrLK->Fields[0]->AsInteger;
                                int stockcode=0;


                               AnsiString stockCode;
                               if (ImportCSV->Cells[3][i].Trim() =="")
                               {
                                        qrLK->Close();
                                        qrLK->SQL->Text = "Select gen_id(GEN_STOCKCODE, 1) from rdb$database";
                                        qrLK->ExecQuery();
                                        stockCode = qrLK->Fields[0]->AsInteger;
                               } else
                               {
                                        stockCode = ImportCSV->Cells[3][i].Trim();


                                        std::string word= ImportCSV->Cells[3] [i].Trim().c_str();
                                        bool isNumber = true;
                                        for(string::const_iterator k = word.begin(); k != word.end(); ++k)
                                        {
                                            if ((*k)=='.')continue;
                                               isNumber &= (   isdigit(*k));
                                        }
                                        if ( isNumber)
                                        {
                                          if (  StrToInt( ImportCSV->Cells[3] [i].Trim() ) > maxStockCode)
                                            {
                                               maxStockCode =StrToInt( ImportCSV->Cells[3] [i].Trim() );
                                            }
                                        }

                                }

                                qrAddStock->Close();
                                qrAddStock->ParamByName("Stock_Key")->AsInteger                      = StockKey;
                                qrAddStock->ParamByName("Stock_Group_Key")->AsInteger                = StockGroupKey;
                                qrAddStock->ParamByName("Code")->AsString                            = stockCode;
                                qrAddStock->ParamByName("Description")->AsString                     = ImportCSV->Cells[4][i].Trim();
                                qrAddStock->ParamByName("Stocktake_Unit")->AsString                  = ImportCSV->Cells[5][i].Trim();
                                qrAddStock->ParamByName("REDUCTION_UNIT")->AsString                  = ImportCSV->Cells[6][i].Trim();
                                qrAddStock->ParamByName("CONVERSION_FACTOR")->AsFloat                = StrToFloat( (ImportCSV->Cells[7][i].Trim()=="")?IntToStr(1):ImportCSV->Cells[7][i].Trim()  );
                                qrAddStock->ParamByName("STOCK_SUB_GROUP")->AsString                 = ImportCSV->Cells[2][i].Trim();
                                qrAddStock->ParamByName("GST_Percent")->AsFloat                      = StrToFloat(ImportCSV->Cells[8][i].Trim());
                                qrAddStock->ParamByName("Enabled")->AsString                         = "T";
                                qrAddStock->ParamByName("Deleted")->AsString                         = "F";
                                qrAddStock->ExecQuery();
                                break;
                            }
                            case 3:
							{
                                qrAddStockLocation->Close();
                                qrAddStockLocation->ParamByName("Stock_Key")->AsInteger             = StockKey;
                                qrAddStockLocation->ParamByName("Location")->AsString               = ImportCSV->Cells[9][i].Trim();
                                qrAddStockLocation->ParamByName("Max_Level")->AsFloat               = StrToFloat(ImportCSV->Cells[11][i].Trim());
                                qrAddStockLocation->ParamByName("Min_Level")->AsFloat               = StrToFloat(ImportCSV->Cells[10][i].Trim());
                                qrAddStockLocation->ParamByName("On_Hand")->AsFloat                 = 1;
                                qrAddStockLocation->ParamByName("Average_Cost")->AsFloat            = StrToFloat(ImportCSV->Cells[13][i].Trim());
                                qrAddStockLocation->ParamByName("Latest_Cost")->AsFloat             = StrToFloat(ImportCSV->Cells[13][i].Trim());
                                qrAddStockLocation->ParamByName("Initialised")->AsString            = "F";
                                qrAddStockLocation->ParamByName("Default_Location")->AsString       = "F";
                                qrAddStockLocation->ExecQuery();
                                break;
                            }
                            case 4:
							{
								qrLK->Close();
								qrLK->SQL->Text = "Select gen_id(Gen_Contact, 1) from rdb$database";
								qrLK->ExecQuery();
                                SupplierKey = qrLK->Fields[0]->AsInteger;

                                qrContactGroup->Close();
                                qrContactGroup->Open();
                                int Key                                                = qrContactGroup->FieldByName("Contact_Group_Key")->AsInteger;
								qrAddSupplier->Close();
								qrAddSupplier->ParamByName("Contact_LK")->AsInteger			        = SupplierKey;
								qrAddSupplier->ParamByName("Contact_Group_Key")->AsInteger	        = Key;
								qrAddSupplier->ParamByName("Company_Name")->AsString		    	= ImportCSV->Cells[14][i].Trim();
								qrAddSupplier->ParamByName("Bad_Debtor")->AsString				    = "F";
								qrAddSupplier->ParamByName("Deleted")->AsString				    	= "F";
								qrAddSupplier->ExecQuery();
								break;
                            }
                            case 5:
							{
                                qrLK->Close();
                                qrLK->SQL->Text = "Select gen_id(Gen_SupplierStock, 1) from rdb$database";
                                qrLK->ExecQuery();
                                qrAddStockSupplier->Close();
                                qrAddStockSupplier->ParamByName("Supplier_Stock_Key")->AsInteger	= qrLK->Fields[0]->AsInteger;
                                qrAddStockSupplier->ParamByName("Supplier_Key")->AsInteger			= SupplierKey;
                                qrAddStockSupplier->ParamByName("Stock_Key")->AsInteger				= StockKey;
                                qrAddStockSupplier->ParamByName("Supplier_Unit")->AsString			= ImportCSV->Cells[15][i].Trim();
                                qrAddStockSupplier->ParamByName("Supplier_Code")->AsString          = ImportCSV->Cells[18][i].Trim();
                                qrAddStockSupplier->ParamByName("Preferred_Supplier")->AsString		= "T";
                                qrAddStockSupplier->ParamByName("Default_Order_Unit")->AsString		= "T";
                                qrAddStockSupplier->ParamByName("Qty")->AsFloat     		        = StrToFloat(ImportCSV->Cells[16][i].Trim());
                                qrAddStockSupplier->ParamByName("Latest_Cost")->AsFloat             = StrToFloat(ImportCSV->Cells[17][i].Trim());
                                qrAddStockSupplier->ParamByName("Min_Order_Qty")->AsFloat			= StrToFloat(ImportCSV->Cells[10][i].Trim());
                                qrAddStockSupplier->ExecQuery();
                                break;
                            }
                            case 6:
							{
                                qrAddBarcode->Close();
                                qrAddBarcode->ParamByName("Stock_Key")->AsInteger                   = StockKey;
                                qrAddBarcode->ParamByName("Barcode")->AsString                      = ImportCSV->Cells[20][i].Trim();
                                qrAddBarcode->ParamByName("Supplier_Stock_Key")->AsInteger          = -1;
                                qrAddBarcode->ExecQuery();
                                break;
                            }

                          }
                      }
                      else
                      {
						switch (CSVColumn)
						{
                            case 0:
							{
                                qrFindCategory->Close();
                                qrFindCategory->ParamByName("Stock_Category")->AsString	        	= ImportCSV->Cells[0][i].Trim();
                                qrFindCategory->Open();
                                StockCategoryKey                                                    = qrFindCategory->FieldByName("Stock_Category_Key")->AsInteger;
                                break;
                            }
                            case 1:
							{
                                qrFindGroup->Close();
                                qrFindGroup->ParamByName("Stock_Group")->AsString	        	= ImportCSV->Cells[1][i].Trim();
                                qrFindGroup->Open();
                                StockGroupKey                                                = qrFindGroup->FieldByName("Stock_Group_Key")->AsInteger;
                                if(newCategory)
                                {
                                    qrLK->Close();
                                    qrLK->SQL->Text = "Select gen_id(Gen_Stock_Group_Key, 1) from rdb$database";
                                    qrLK->ExecQuery();
                                    StockGroupKey = qrLK->Fields[0]->AsInteger;
                                    qrAddGroup->Close();
                                    qrAddGroup->ParamByName("Stock_Group_Key")->AsInteger	              = StockGroupKey;
                                    qrAddGroup->ParamByName("Stock_Category_Key")->AsInteger              = StockCategoryKey;
                                    qrAddGroup->ParamByName("Stock_Group")->AsString		              = ImportCSV->Cells[1][i].Trim();
                                    qrAddGroup->ParamByName("Sort_Order")->AsInteger		              = 0;
                                    qrAddGroup->ParamByName("GL_Code")->AsString		                  = "";
                                    qrAddGroup->ParamByName("Deleted")->AsString			              = "F";
                                    qrAddGroup->ExecQuery();
                                 }
                                break;
                            }
                            case 2:
							{

                                qrFindStock->Close();
                                qrFindStock->ParamByName("Code")->AsString	        	= ImportCSV->Cells[3][i].Trim();
                            //    qrFindStock->ParamByName("Description")->AsString       = ImportCSV->Cells[4][i].Trim();
                                qrFindStock->Open();
                                StockKey                                                = qrFindStock->FieldByName("Stock_Key")->AsInteger;

                                if ( StockKey==0)
                                {
                                        qrLK->Close();
                                        qrLK->SQL->Text = "Select gen_id(GEN_STOCK_KEY, 1) from rdb$database";
                                        qrLK->ExecQuery();
                                        StockKey = qrLK->Fields[0]->AsInteger;

                                        qrAddStock->Close();
                                        qrAddStock->ParamByName("Stock_Key")->AsInteger                      = StockKey;
                                        qrAddStock->ParamByName("Stock_Group_Key")->AsInteger                = StockGroupKey;

                                        if (ImportCSV->Cells[3][i].Trim() =="")
                                        {
                                            qrLK->Close();
                                            qrLK->SQL->Text = "Select gen_id(GEN_STOCKCODE, 1) from rdb$database";
                                            qrLK->ExecQuery();
                                            qrAddStock->ParamByName("Code")->AsString = qrLK->Fields[0]->AsInteger;
                                        } else
                                        {
                                               qrAddStock->ParamByName("Code")->AsString = ImportCSV->Cells[3][i].Trim();

                                        }

                                        qrAddStock->ParamByName("Description")->AsString                     = ImportCSV->Cells[4][i].Trim();
                                        qrAddStock->ParamByName("Stocktake_Unit")->AsString                  = ImportCSV->Cells[5][i].Trim();
                                        qrAddStock->ParamByName("REDUCTION_UNIT")->AsString                  = ImportCSV->Cells[6][i].Trim();
                                        qrAddStock->ParamByName("STOCK_SUB_GROUP")->AsString                 = ImportCSV->Cells[2][i].Trim();
                                        qrAddStock->ParamByName("STOCK_SUB_GROUP")->AsString                 = ImportCSV->Cells[2][i].Trim();
                                        qrAddStock->ParamByName("CONVERSION_FACTOR")->AsFloat                = StrToFloat( (ImportCSV->Cells[7][i].Trim()=="")?IntToStr(1):ImportCSV->Cells[7][i].Trim()  );

                                        qrAddStock->ParamByName("GST_Percent")->AsFloat                      = StrToFloat(ImportCSV->Cells[8][i].Trim());
                                        qrAddStock->ParamByName("Enabled")->AsString                         = "T";
                                        qrAddStock->ParamByName("Deleted")->AsString                         = "F";
                                        qrAddStock->ExecQuery();



                                }

                                    std::string word= ImportCSV->Cells[3] [i].Trim().c_str();
                                    bool isNumber = true;
                                    for(string::const_iterator k = word.begin(); k != word.end(); ++k)
                                    {
                                             if ((*k)=='.')continue;
                                                     isNumber &= (   isdigit(*k));
                                    }
                                    if ( isNumber)
                                    {
                                            if (  StrToInt( ImportCSV->Cells[3] [i].Trim() ) > maxStockCode)
                                            {
                                                    maxStockCode =StrToInt( ImportCSV->Cells[3] [i].Trim() );
                                            }
                                    }



                                qrUpdateStock->Close();
                                qrUpdateStock->ParamByName("Description")->AsString	                  = ImportCSV->Cells[4][i].Trim();
                                qrUpdateStock->ParamByName("Stocktake_Unit")->AsString                = ImportCSV->Cells[5][i].Trim();
                                qrUpdateStock->ParamByName("REDUCTION_UNIT")->AsString                = ImportCSV->Cells[6][i].Trim();
                                qrUpdateStock->ParamByName("STOCK_SUB_GROUP")->AsString               = ImportCSV->Cells[2][i].Trim();
                                qrUpdateStock->ParamByName("GST_Percent")->AsFloat                    = StrToFloat(ImportCSV->Cells[8][i].Trim());
                                qrUpdateStock->ParamByName("Description")->AsString                   = ImportCSV->Cells[4][i].Trim();
                                qrUpdateStock->ParamByName("Code")->AsString                          = ImportCSV->Cells[3][i].Trim();
                                qrUpdateStock->ParamByName("CONVERSION_FACTOR")->AsFloat                = StrToFloat( (ImportCSV->Cells[7][i].Trim()=="")?IntToStr(1):ImportCSV->Cells[7][i].Trim()  );
                                qrUpdateStock->ParamByName("STOCK_GROUP_KEY")->AsInteger                = StockGroupKey;

                                qrUpdateStock->Open();
                                break;
                            }
                            case 3:
							{

                                qrUpdateLocation->Close();
                                qrUpdateLocation->ParamByName("Max_Level")->AsFloat                    = StrToFloat(ImportCSV->Cells[11][i].Trim());
                                qrUpdateLocation->ParamByName("Min_Level")->AsFloat           = StrToFloat(ImportCSV->Cells[10][i].Trim());
                                qrUpdateLocation->ParamByName("Average_Cost")->AsFloat               = StrToFloat(ImportCSV->Cells[13][i].Trim());
                                qrUpdateLocation->ParamByName("Latest_Cost")->AsFloat             = StrToFloat(ImportCSV->Cells[17][i].Trim());
                                qrUpdateLocation->ParamByName("Stock_Key")->AsInteger             = StockKey;
                                qrUpdateLocation->ParamByName("Location")->AsString                    = ImportCSV->Cells[9][i].Trim();
                                qrUpdateLocation->Open();
                                break;

                            }
                            case 4:
							{

                                qrFindSupplier->Close();
                                qrFindSupplier->ParamByName("Company_Name")->AsString	        	= ImportCSV->Cells[14][i].Trim();
                                qrFindSupplier->Open();
                                SupplierKey                                                    = qrFindSupplier->FieldByName("Contact_LK")->AsInteger;
                                break;
                            }
                            case 5:
							{
                                qrUpdateStockSupplier->Close();
                                qrUpdateStockSupplier->ParamByName("Supplier_Code")->AsString                    = ImportCSV->Cells[18][i].Trim();
                                qrUpdateStockSupplier->ParamByName("Qty")->AsFloat                  = StrToFloat(ImportCSV->Cells[16][i].Trim());
                                qrUpdateStockSupplier->ParamByName("Min_Order_Qty")->AsFloat               = StrToFloat(ImportCSV->Cells[10][i].Trim());
                                qrUpdateStockSupplier->ParamByName("Latest_Cost")->AsFloat             = StrToFloat(ImportCSV->Cells[17][i].Trim());
                                qrUpdateStockSupplier->ParamByName("Stock_Key")->AsInteger             = StockKey;
                                qrUpdateStockSupplier->ParamByName("Supplier_Key")->AsInteger                    = SupplierKey;
                                qrUpdateStockSupplier->ParamByName("Supplier_Unit")->AsString                    = ImportCSV->Cells[15][i].Trim();
                                qrUpdateStockSupplier->Open();
                                break;
                            }
                            case 6:
							{
                                 break;
                            }

                        }
                      }
                }


         	}
			ProgressBar1->Position = 0;
              if (maxStockCode!=0)
              {
                   IBQuery->Close();
                   IBQuery->SQL->Text = "Select gen_id(GEN_STOCKCODE," + IntToStr(maxStockCode+1) + ") from rdb$database";
                   IBQuery->ExecQuery();
              }


			if (MissingLocations == "")
			 {
                    result = true;
					if (FailedList != "")
					{
						Application->MessageBox(("Some products failed to import.\r\r" + FailedList).c_str(), "Warning", MB_OK + MB_ICONWARNING);
					}
					else
					{
						Application->MessageBox("Stock Import complete", "Complete", MB_OK + MB_ICONINFORMATION);
					}
			  }
			else
			 {
                   result = false;
				   Application->MessageBox(("Some locations are missing from the MenuMate System.\r\r" + MissingLocations).c_str(), "Error", MB_OK + MB_ICONERROR);
			 }
			}
        }
		catch (Exception &E)
		{
            result = false;
			Application->MessageBox(("Failed on item" + LastItem).c_str(), "Error", MB_ICONERROR + MB_OK);
			ProgressBar1->Position = 0;
		}
        return result;
 }

int TimportMenu::GetOrCreateMenuKey(AnsiString  menuName,TIBSQL *qrCSV)
{
    int menuKey = 0;
    qrCSV->Close();
    qrCSV->SQL->Text = "Select MENU_KEY From MENU Where  MENU.MENU_NAME= :MENU_NAME";
    qrCSV->ParamByName("MENU_NAME")->AsString = menuName;
    qrCSV->ExecQuery();
    if (!qrCSV->Eof)
    {
      menuKey = qrCSV->FieldByName("MENU_KEY")->AsInteger;
    }
    else
    {
        qrCSV->Close();
        qrCSV->SQL->Text = "SELECT GEN_ID(GEN_MENU, 1) FROM RDB$DATABASE";
        qrCSV->ExecQuery();
        menuKey = qrCSV->Fields[0]->AsInteger;
    }
    return menuKey;
}

 std::vector<TimportMenu> TimportMenu::FetchAllMenusInVector(TCsv *ImportCSV,TIBSQL *qrCSV,bool doRecipeUpdate,TIBSQL *qrcsv1,bool RemoveRecipe,TIBSQL *qrStockQuery)
  {

    Tmenucontainer allMenus;
    for(int row=1;row<ImportCSV->RowCount;row++)
      {
         TimportMenu menu;
         int menuIndex;
         int menuKey = 0;
         bool menuAlreadyAdded = false;
         menu.menuName = ImportCSV->Cells[26][row].Trim();

         if ( ImportCSV->Cells[27][row].Trim().LowerCase() =="food")
         {
            menu.menuType = 0;
         }else if(ImportCSV->Cells[27][row].Trim().LowerCase() =="beverage" )
         {
           menu.menuType = 1;
         }

        if(  RemoveRecipe  && CheckDuplicateStockCodeInDatabase(ImportCSV->Cells[39][row].Trim(),qrStockQuery) )
        {
            doRecipeUpdate =false;
        }
        //checking if menu  has is already present in allCsvMenus vector
         for(int menuNo=0; menuNo < allMenus.allCsvMenus.size();menuNo++)
          {
                 if (menu.menuName == allMenus.allCsvMenus[menuNo].menuName)
                 {
                     menuAlreadyAdded = true;
                     menu = allMenus.allCsvMenus[menuNo];
                     menuIndex = menuNo;
                     break;
                 }
          }

         if(!menuAlreadyAdded )
         {
            menu.menuKey = GetOrCreateMenuKey(menu.menuName,qrCSV);

            //load all available serving courses in vector
            LoadAllServingCourses(menu,qrCSV);

            // load all the tax Profiles in vector
            LoadAllTaxProfiles(menu,qrCSV);

            //load all category group
            LoadallCategoryGroups(menu,qrCSV);

            //load all categories
            LoadallCategories(menu,qrCSV);

            //Load all Items
            LoadallItems(menu,qrCSV);

            //load all the size
            LoadallSize(menu,qrCSV,ImportCSV);

            //Load all unsold itemsize
            LoadallUnsoldItemSize(menu,qrCSV,qrcsv1,ImportCSV);
         }
          // save the serving course
          SaveServingCourses(ImportCSV,row,menu,qrCSV);

          //save the course
          SaveCourses(ImportCSV,row,menu,qrCSV);

          //save the item
          SaveItem(ImportCSV,row,menu,qrCSV);

          //save the item Size
          SaveItemSize(ImportCSV, row,menu,qrCSV,doRecipeUpdate);

          if(menuAlreadyAdded)
           {
               allMenus.allCsvMenus[menuIndex] = menu;
           }
           else
           {
                allMenus.allCsvMenus.push_back(menu);
           }
      }

    return  allMenus.allCsvMenus;

  }


void  TimportMenu::LoadallUnsoldItemSize(TimportMenu &menu,TIBSQL *qrCSV,TIBSQL *qrIBQuery,TCsv *ImportCSV)
    {
        qrCSV->Close();
        qrCSV->SQL->Text = "Select IT.ITEMSIZE_KEY,IT.SIZE_NAME,IT.CATEGORY_KEY , arccat.CATEGORY , arccat.CATEGORYGROUPS_KEY,catG.NAME, IT.ITEM_KEY,item.ITEM_NAME,IT.PRICE,IT.SIZES_KEY,item.COURSE_KEY  "
                            "from ITEMSIZE IT "
                            "inner join SIZES sz on sz.SIZES_KEY = IT.SIZES_KEY  "
                            "left join ARCCATEGORIES arccat on arccat.CATEGORY_KEY=IT.CATEGORY_KEY  "
                            "left join CATEGORYGROUPS  catG on catG.CATEGORYGROUPS_KEY =arccat.CATEGORYGROUPS_KEY   "
                            "left join item  on item.ITEM_KEY = IT.ITEM_KEY  "
                            "left join COURSE cou on cou.COURSE_KEY =item.COURSE_KEY  "
                            "where IT.ISINVISIBLE  = :ISINVISIBLE and cou.MENU_KEY = :MENU_KEY" ;

        qrCSV->ParamByName("ISINVISIBLE")->AsString = "F";
        qrCSV->ParamByName("MENU_KEY")->AsInteger = menu.menuKey;
        qrCSV->ExecQuery();

        while(!qrCSV->Eof)
        {
            TitemSize itemSize;
            itemSize.itemSizeKey= qrCSV->FieldByName("ITEMSIZE_KEY")->AsInteger;
            itemSize.itemSizeName= qrCSV->FieldByName("SIZE_NAME")->AsString;
            itemSize.itemKey= qrCSV->FieldByName("ITEM_KEY")->AsInteger;
            itemSize.itemName= qrCSV->FieldByName("ITEM_NAME")->AsString;

            bool presentInCsv=false;
            for(int row=1;row<ImportCSV->RowCount;row++)
              {
                 if(ImportCSV->Cells[29][row].Trim() == itemSize.itemName && ImportCSV->Cells[31][row].Trim() == itemSize.itemSizeName)
                  {
                       presentInCsv =true;
                       break;
                  }
              }
             if(presentInCsv)
             {
                qrCSV->Next();
                continue;
             }
            itemSize.categoryGroupKey= qrCSV->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;
            itemSize.categoryGroupName= qrCSV->FieldByName("NAME")->AsString;
            itemSize.categoryKey= qrCSV->FieldByName("CATEGORY_KEY")->AsInteger;
            itemSize.categoryName= qrCSV->FieldByName("CATEGORY")->AsString;
            itemSize.itemSizePrice= qrCSV->FieldByName("PRICE")->AsFloat;
            itemSize. courseKey= qrCSV->FieldByName("COURSE_KEY")->AsInteger;
            itemSize.sizeFKey= qrCSV->FieldByName("SIZES_KEY")->AsInteger;
            // load the tax profile
            qrIBQuery->Close();
            qrIBQuery->SQL->Text ="SELECT a.PROFILE_KEY, a.NAME, a.RATE "
                            "FROM TAXPROFILES_ITEMSIZE tiz  "
                            "inner join TAXPROFILES a on a.PROFILE_KEY = tiz.PROFILE_KEY  "
                            "where tiz.ITEMSIZE_KEY = :ITEMSIZE_KEY ";

            qrIBQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = itemSize.itemSizeKey;
            qrIBQuery->ExecQuery();

            while (!qrIBQuery->Eof)
            {
               TitemSizeTaxProfile profile;
               profile.taxProfileKey=qrIBQuery->FieldByName("PROFILE_KEY")->AsInteger;
               profile.taxName=qrIBQuery->FieldByName("NAME")->AsString;
               profile.taxPercent=qrIBQuery->FieldByName("RATE")->AsFloat;
               itemSize.itemSizeTaxes.push_back(profile);
               qrIBQuery->Next();
            }
            //load the recipe
            qrIBQuery->Close();
            qrIBQuery->SQL->Text ="SELECT a.RECIPE_KEY, a.ITEMSIZE_KEY, a.STOCK_CODE, a.QTY, a.STOCK_LOCATION "
                                  "FROM RECIPE a "
                                  "where a.ITEMSIZE_KEY = :ITEMSIZE_KEY ";

            qrIBQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = itemSize.itemSizeKey;
            qrIBQuery->ExecQuery();

            while (!qrIBQuery->Eof)
            {
                Trecipe recipe;
                recipe.qty= qrIBQuery->FieldByName("QTY")->AsFloat;
                recipe.recipeKey=qrIBQuery->FieldByName("RECIPE_KEY")->AsInteger;
                recipe.location =qrIBQuery->FieldByName("STOCK_LOCATION")->AsString;
                recipe.stockCode =qrIBQuery->FieldByName("STOCK_CODE")->AsString;
                itemSize.itemSizeRecipe.push_back(recipe);
                qrIBQuery->Next();
            }
             //adding item size to vector containing all item sizes of menus
            menu.allItemSizeInMenu.push_back(itemSize);
            qrCSV->Next();
        }
}


void  TimportMenu::LoadAllServingCourses(TimportMenu &menu,TIBSQL *qrCSV)
{
    qrCSV->Close();
    qrCSV->SQL->Text = "Select SERVINGCOURSES_KEY,SERVINGCOURSE_NAME From SERVINGCOURSES Where   SERVINGCOURSES.DELETED= 'F' ";
    qrCSV->ExecQuery();
    while(!qrCSV->Eof)
    {
       TservingCourse servingCourse;
       servingCourse.servingCourseKey = qrCSV->FieldByName("SERVINGCOURSES_KEY")->AsInteger;
       servingCourse.servingCourseName=qrCSV->FieldByName("SERVINGCOURSE_NAME")->AsString;
       menu.allServingCourse.push_back(servingCourse);
       qrCSV->Next();
     }
}

void  TimportMenu::LoadallItems(TimportMenu &menu,TIBSQL *qrCSV)
{
    qrCSV->Close();
    qrCSV->SQL->Text = "Select ITEM_KEY, ITEM_NAME, COURSE_KEY From ITEM Where  ITEM.ENABLED = :ENABLED";
    qrCSV->ParamByName("ENABLED")->AsString = "T";
    qrCSV->ExecQuery();
    while(!qrCSV->Eof)
    {
        Titem  item;
        item.itemKey= qrCSV->FieldByName("ITEM_KEY")->AsInteger;
        item.courseKey = qrCSV->FieldByName("COURSE_KEY")->AsInteger;
        item.itemName = qrCSV->FieldByName("ITEM_NAME")->AsString;
        menu.allItem.push_back(item);
        qrCSV->Next();

     }
}

void  TimportMenu::LoadallCategoryGroups(TimportMenu &menu,TIBSQL *qrCSV)
{
    qrCSV->Close();
    qrCSV->SQL->Text = "Select CATEGORYGROUPS_KEY,NAME From CATEGORYGROUPS ";
    qrCSV->ExecQuery();
    while(!qrCSV->Eof)
    {
        TcategoryGroup categoryGroup;
        categoryGroup.categoryGroupKey = qrCSV->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;
        categoryGroup.categoryGroupName = qrCSV->FieldByName("NAME")->AsString;
        menu.allCategoryGroups.push_back(categoryGroup);
        qrCSV->Next();

    }
}

void TimportMenu::LoadallCategories(TimportMenu &menu,TIBSQL *qrCSV)
{
    qrCSV->Close();
    qrCSV->SQL->Text = "Select CATEGORY_KEY, CATEGORY, CATEGORYGROUPS_KEY, COO From ARCCATEGORIES ";
    qrCSV->ExecQuery();
    while(!qrCSV->Eof)
     {
        Tcategory  category;
        category.categoryKey = qrCSV->FieldByName("CATEGORY_KEY")->AsInteger;
        category.categoryName = qrCSV->FieldByName("CATEGORY")->AsString;
        category.categoryGroupKey = qrCSV->FieldByName("CATEGORYGROUPS_KEY")->AsInteger;
        menu.allCategories.push_back(category);
        qrCSV->Next();
      }

}


void TimportMenu::LoadallSize(TimportMenu &menu,TIBSQL *qrCSV,TCsv *ImportCSV)
{
    qrCSV->Close();
    qrCSV->SQL->Text = "Select SIZES_KEY, MENU_KEY, SIZE_NAME From SIZES Where  SIZES.MENU_KEY= :MENU_KEY and SIZES.ISINVISIBLE = :ISINVISIBLE ";
    qrCSV->ParamByName("MENU_KEY")->AsInteger = menu.menuKey;
    qrCSV->ParamByName("ISINVISIBLE")->AsString = "F";
    qrCSV->ExecQuery();
    while(!qrCSV->Eof)
    {
        Tsize  size;
        size.sizeKey= qrCSV->FieldByName("SIZES_KEY")->AsInteger;
        size.sizeName=qrCSV->FieldByName("SIZE_NAME")->AsString;
        size.menuKey=menu.menuKey;
        bool presentInCsv=false;

       for(int row=1;row<ImportCSV->RowCount;row++)
          {
             if(ImportCSV->Cells[31][row].Trim() == size.sizeName)
              {
                   presentInCsv =true;
                   break;
              }
          }

          if(!presentInCsv)
          {
             menu.allMenuSize.push_back(size);
          }
         qrCSV->Next();
    }
}

void  TimportMenu::LoadAllTaxProfiles(TimportMenu &menu,TIBSQL *qrCSV)
{
    qrCSV->Close();
    qrCSV->SQL->Text = "Select PROFILE_KEY, NAME, TYPE, RATE From TAXPROFILES Where  TAXPROFILES.SELECTABLE= 'T'";
    qrCSV->ExecQuery();
    while(!qrCSV->Eof)
    {
        TallTaxProfiles taxProfile;
        taxProfile.taxProfileKey= qrCSV->FieldByName("PROFILE_KEY")->AsInteger;
        taxProfile.taxName=qrCSV->FieldByName("NAME")->AsString;
        taxProfile.taxPercent=qrCSV->FieldByName("RATE")->AsFloat;
        taxProfile.taxType = qrCSV->FieldByName("TYPE")->AsInteger;
        menu.allTaxProfiles.push_back(taxProfile);
        qrCSV->Next();

    }
}


void TimportMenu::SaveServingCourses(TCsv *ImportCSV, int row,TimportMenu &menu,TIBSQL *qrCSV)
{
    bool newServingCourse = true;
    TservingCourse servingCourse;
    servingCourse.servingCourseName = ImportCSV->Cells[38][row].Trim();
    // if it new menu then only add it to serving course vector
    for (int courseNo=0; courseNo< menu.allServingCourse.size();courseNo++)
    {
        if ( menu.allServingCourse[courseNo].servingCourseName == servingCourse.servingCourseName)
        {
           return ;
        }
    }

    qrCSV->Close();
    qrCSV->SQL->Text = "SELECT GEN_ID(GEN_SERVINGCOURSES, 1) FROM RDB$DATABASE";
    qrCSV->ExecQuery();
    servingCourse.servingCourseKey= qrCSV->Fields[0]->AsInteger;
    menu.allServingCourse.push_back(servingCourse);
 }

 void TimportMenu::SaveCourses(TCsv *ImportCSV, int row,TimportMenu &menu,TIBSQL *qrCSV)
 {
        Tcourse course;
        course.courseName = ImportCSV->Cells[28][row].Trim();
        course.corrospondingServingCourseName = ImportCSV->Cells[38][row].Trim();
        for (int courseNo=0;courseNo<menu.course.size();courseNo++)
        {
            if (menu.course[courseNo].courseName == course.courseName)
            {
               return ;
            }
        }

         for (int index=0;index<menu.allServingCourse.size();index++)
        {
            if (menu.allServingCourse[index].servingCourseName == course.corrospondingServingCourseName)
            {
               course.corrospondingServingCourseKey =  menu.allServingCourse[index].servingCourseKey;
               break;
            }
        }

        qrCSV->Close();
        qrCSV->SQL->Text = "Select COURSE_KEY From COURSE Where  COURSE.COURSE_NAME= :COURSE_NAME and COURSE.MENU_KEY= :MENU_KEY ";
        qrCSV->ParamByName("COURSE_NAME")->AsString = course.courseName;
        qrCSV->ParamByName("MENU_KEY")->AsInteger = menu.menuKey;
        qrCSV->ExecQuery();

        if (!qrCSV->Eof)
        {
           course.courseKey = qrCSV->FieldByName("COURSE_KEY")->AsInteger;
        }
        else
        {
            qrCSV->Close();
            qrCSV->SQL->Text = "SELECT GEN_ID(GEN_COURSE, 1) FROM RDB$DATABASE";
            qrCSV->ExecQuery();
            course.courseKey= qrCSV->Fields[0]->AsInteger;
        }
        menu.course.push_back(course);
}


void TimportMenu::SaveItem(TCsv *ImportCSV, int row,TimportMenu &menu,TIBSQL *qrCSV)
{
   Titem item;
   item.itemName = ImportCSV->Cells[29][row].Trim();
   item.categoryGroup = ImportCSV->Cells[36][row].Trim();
   AnsiString courseName = ImportCSV->Cells[28][row].Trim();

   for (int itemNo=0;itemNo<menu.allItem.size();itemNo++)
    {
        if ( menu.allItem[itemNo].itemName  == item.itemName)
        {
           return;
        }
    }

   for (int courseNo=0;courseNo < menu.course.size(); courseNo++)
    {
        if ( menu.course[courseNo].courseName == courseName)
        {
           item.courseKey = menu.course[courseNo].courseKey;
           break;
        }
     }
    qrCSV->Close();
    qrCSV->SQL->Text = "Select ITEM_KEY From ITEM Where  ITEM.ITEM_NAME= :ITEM_NAME and ITEM.COURSE_KEY = :COURSE_KEY ";
    qrCSV->ParamByName("ITEM_NAME")->AsString = item.itemName;
    qrCSV->ParamByName("COURSE_KEY")->AsInteger = item.courseKey;
    qrCSV->ExecQuery();
    if (!qrCSV->Eof)
     {
       item.itemKey= qrCSV->FieldByName("ITEM_KEY")->AsInteger;
     }
     else
     {
        qrCSV->Close();
        qrCSV->SQL->Text = "SELECT GEN_ID(GEN_ITEM, 1) FROM RDB$DATABASE";
        qrCSV->ExecQuery();
        item.itemKey= qrCSV->Fields[0]->AsInteger;
     }
     menu.allItem.push_back(item);
}

void TimportMenu::SaveItemSize(TCsv *ImportCSV, int row,TimportMenu &menu,TIBSQL *qrCSV,bool doRecipeUpdate)
{
    TitemSize itemSize;
    itemSize.itemSizeName= ImportCSV->Cells[31][row].Trim();
    

    //create new category group if it is new
    SaveItemSizeCategoryGroup(ImportCSV,row,menu,itemSize,qrCSV);

    //create new category if it is new
    SaveItemSizeCategory(ImportCSV,row,menu,itemSize,qrCSV);

    //create new Tax Profile if it is new
    SaveItemSizeTaxes(ImportCSV,row,menu,itemSize,qrCSV);

    itemSize.itemSizePrice = StrToFloat(ImportCSV->Cells[30][row].Trim());
    AnsiString itemName = ImportCSV->Cells[29][row].Trim();
    AnsiString courseName = ImportCSV->Cells[28][row].Trim();

    for (int itemNo=0;itemNo<menu.allItem.size();itemNo++)
    {
        if ( menu.allItem[itemNo].itemName == itemName)
        {
           itemSize.itemKey = menu.allItem[itemNo].itemKey;
           break;
        }
     }

    for (int courseNo=0;courseNo < menu.course.size(); courseNo++)
    {
        if ( menu.course[courseNo].courseName == courseName)
        {
           itemSize.courseKey = menu.course[courseNo].courseKey;
           break;
        }
    }

 
        qrCSV->Close();
        qrCSV->SQL->Text = "SELECT GEN_ID(GEN_ITEMSIZE, 1) FROM RDB$DATABASE";
        qrCSV->ExecQuery();
        itemSize.itemSizeKey= qrCSV->Fields[0]->AsInteger;
  

   //update the recipe if user wants to update it
   if (doRecipeUpdate)
   {
        Trecipe recipe;
        qrCSV->Close();
        qrCSV->SQL->Text = "SELECT GEN_ID(GEN_RECIPE, 1) FROM RDB$DATABASE";
        qrCSV->ExecQuery();
        recipe.recipeKey= qrCSV->Fields[0]->AsInteger;
        recipe.stockCode= ImportCSV->Cells[39][row].Trim();
        recipe.location= ImportCSV->Cells[40][row].Trim();
        recipe.qty =StrToInt( ImportCSV->Cells[41][row].Trim() ) ;
        itemSize.itemSizeRecipe.push_back(recipe);
   }
    bool sizePresent=false;
    for (int sizeNo=0; sizeNo<menu.allMenuSize.size(); sizeNo++)
        {
           if ( menu.allMenuSize[sizeNo].sizeName == itemSize.itemSizeName )
            {
                   itemSize.sizeFKey = menu.allMenuSize[sizeNo].sizeKey;
                   sizePresent=true;
            }
        }
  if(!sizePresent)
  {
        Tsize size;
        qrCSV->Close();
        qrCSV->SQL->Text = "SELECT GEN_ID(GEN_SIZE, 1) FROM RDB$DATABASE";
        qrCSV->ExecQuery();
        size.sizeKey = qrCSV->Fields[0]->AsInteger;
        size.menuKey = menu.menuKey;
        size.sizeName = itemSize.itemSizeName;
        menu.allMenuSize.push_back(size);

        itemSize.sizeFKey= size.sizeKey;
  }

  //adding item size to vector containing all item sizes of menus
   menu.allItemSizeInMenu.push_back(itemSize);
}

void TimportMenu::SaveItemSizeCategoryGroup(TCsv *ImportCSV,int row,TimportMenu &menu,TitemSize &itemSize,TIBSQL *qrCSV)
{
    itemSize.categoryGroupName = ImportCSV->Cells[36][row].Trim();
    for (int groupNo=0;groupNo<menu.allCategoryGroups.size();groupNo++)
    {
        if ( menu.allCategoryGroups[groupNo].categoryGroupName == itemSize.categoryGroupName)
        {
            itemSize.categoryGroupKey = menu.allCategoryGroups[groupNo].categoryGroupKey;
            return;
        }
    }
    qrCSV->Close();
    qrCSV->SQL->Text = "SELECT GEN_ID(GEN_CATEGORYGROUPS, 1) FROM RDB$DATABASE";
    qrCSV->ExecQuery();
    itemSize.categoryGroupKey = qrCSV->Fields[0]->AsInteger;

    TcategoryGroup categoryGroup;
    categoryGroup.categoryGroupKey = itemSize.categoryGroupKey;
    categoryGroup.categoryGroupName = itemSize.categoryGroupName;
    menu.allCategoryGroups.push_back(categoryGroup);
}


void TimportMenu::SaveItemSizeCategory(TCsv *ImportCSV,int row, TimportMenu &menu,TitemSize &itemSize,TIBSQL *qrCSV)
{
   itemSize.categoryName = ImportCSV->Cells[37][row].Trim();

   for (int categoryNo=0; categoryNo<menu.allCategories.size(); categoryNo++)
    {
        if ( menu.allCategories[categoryNo].categoryName == itemSize.categoryName && menu.allCategories[categoryNo].categoryGroupKey == itemSize.categoryGroupKey)
        {
            itemSize.categoryKey = menu.allCategories[categoryNo].categoryKey;
            return;
        }

    }

    qrCSV->Close();
    qrCSV->SQL->Text = "SELECT GEN_ID(GEN_ARCCATEGORIES, 1) FROM RDB$DATABASE";
    qrCSV->ExecQuery();
    itemSize.categoryKey = qrCSV->Fields[0]->AsInteger;

    // if it is new category then add it to allcategories vector
    Tcategory category;
    category.categoryName = itemSize.categoryName;
    category.categoryKey = itemSize.categoryKey;
    category.categoryGroupKey = itemSize.categoryGroupKey;
    menu.allCategories.push_back(category);
}

int TimportMenu::GetTaxKey(AnsiString taxName, double taxPercent,int taxType,TimportMenu &menu,TIBSQL *qrCSV)
{
    for (int taxNo = 0; taxNo < menu.allTaxProfiles.size(); taxNo++)
    {
        if (menu.allTaxProfiles[taxNo].taxName == taxName && menu.allTaxProfiles[taxNo].taxPercent == taxPercent &&
            menu.allTaxProfiles[taxNo].taxType == taxType)
        {
            return menu.allTaxProfiles[taxNo].taxProfileKey;
        }
    }

    qrCSV->Close();
    qrCSV->SQL->Text = "SELECT GEN_ID(GEN_TAXPROFILES, 1) FROM RDB$DATABASE";
    qrCSV->ExecQuery();
    int taxKey = qrCSV->Fields[0]->AsInteger;

    //if it is a new tax profile then add it to all tax profile vector
    TallTaxProfiles taxProfile;
    taxProfile.taxProfileKey = taxKey;
    taxProfile.taxName= taxName;
    taxProfile.taxPercent= taxPercent;
    taxProfile.taxType = taxType;
    menu.allTaxProfiles.push_back(taxProfile);
    return taxKey;
}

void TimportMenu::SaveItemSizeTaxes(TCsv *ImportCSV,int row,TimportMenu &menu,TitemSize &itemSize,TIBSQL *qrCSV)
{
    std::string salesTaxString  =  ImportCSV->Cells[32][row].Trim().c_str();
    std::string  serviceChargeString =  ImportCSV->Cells[33][row].Trim().c_str();
    std::string  localTaxString =   ImportCSV->Cells[34][row].Trim().c_str();
    std::string  profitTaxString =  ImportCSV->Cells[35][row].Trim().c_str();

  if (salesTaxString !="")
  {
    //add sales tax
    TitemSizeTaxProfile taxProfile;
    taxProfile.taxName =  GetTaxName(salesTaxString).c_str();
    taxProfile.taxPercent=  GetTaxPercent(salesTaxString);
    taxProfile.taxProfileKey= GetTaxKey(taxProfile.taxName.Trim(),taxProfile.taxPercent,0, menu,qrCSV);
    itemSize.itemSizeTaxes.push_back(taxProfile);
   }


   if (serviceChargeString !="")
  {

    // add service charge
    TitemSizeTaxProfile taxProfile1;
    taxProfile1.taxName =  GetTaxName(serviceChargeString).c_str();
    taxProfile1.taxPercent=  GetTaxPercent(serviceChargeString);
    taxProfile1.taxProfileKey= GetTaxKey(taxProfile1.taxName.Trim(),taxProfile1.taxPercent,2, menu,qrCSV);
    itemSize.itemSizeTaxes.push_back(taxProfile1);
   }

  if (localTaxString !="")
  {

    // add local tax
    TitemSizeTaxProfile taxProfile2;
    taxProfile2.taxName =  GetTaxName(localTaxString).c_str();
    taxProfile2.taxPercent=  GetTaxPercent(localTaxString);
    taxProfile2.taxProfileKey= GetTaxKey(taxProfile2.taxName.Trim(),taxProfile2.taxPercent,4, menu,qrCSV);
    itemSize.itemSizeTaxes.push_back(taxProfile2);
   }
  if (profitTaxString !="")
  {

     // add Profit tax
    TitemSizeTaxProfile taxProfile3;
    taxProfile3.taxName =  GetTaxName(profitTaxString).c_str();
    taxProfile3.taxPercent=  GetTaxPercent(profitTaxString);
    taxProfile3.taxProfileKey= GetTaxKey(taxProfile3.taxName.Trim(),taxProfile3.taxPercent,5 ,menu,qrCSV);
    itemSize.itemSizeTaxes.push_back(taxProfile3);
   }

}

std::string TimportMenu::GetTaxName(std::string taxString)
{
    std::vector < std::string > taxInfo; //we'll put all of the tokens in here
    std::string  temp;
    while (taxString.find("-", 0) != std::string::npos)
    {
        int  pos = taxString.find("-", 0); //store the position of the delimiter
        temp = taxString.substr(0, pos);      //get the token
        taxString.erase(0, pos + 1);          //erase it from the source
        taxInfo.push_back(temp);                //and put it into the vector
    }
    taxInfo.push_back(taxString);           //the last token is all alone
    return taxInfo[0];
}



 double TimportMenu::GetTaxPercent(std::string taxString)
 {
    if(taxString.length())
    {
    std::vector < std::string > taxInfo; //we'll put all of the tokens in here
    std::string  temp;
    while (taxString.find("-", 0) != std::string::npos)
    {
        int  pos = taxString.find("-", 0); //store the position of the delimiter
        temp = taxString.substr(0, pos);      //get the token
        taxString.erase(0, pos + 1);          //erase it from the source
        taxInfo.push_back(temp);                //and put it into the vector
    }
    taxInfo.push_back(taxString);           //the last token is all alone
    return FetchTaxPercent( taxInfo[1] );
  }
  {
    return 0;

  }
}

double TimportMenu::FetchTaxPercent( std::string percentString)
{
    std::vector < std::string > taxInfo; //we'll put all of the tokens in here
    std::string  temp;
    while (percentString.find("%", 0) != std::string::npos)
    {
        int  pos = percentString.find("%", 0); //store the position of the delimiter
        temp = percentString.substr(0, pos);      //get the token
        percentString.erase(0, pos + 1);          //erase it from the source
        taxInfo.push_back(temp);                //and put it into the vector
    }
    taxInfo.push_back(percentString);           //the last token is all alone
    return atof(taxInfo[0].c_str());  //atof converts  cons char* to double
}
////---------------------------------------------------------------------------------
bool TfrmAdministration::CheckTaxFormat( AnsiString taxStringInCsv,int column ,int row)
{
     std::string taxString = taxStringInCsv.Trim().c_str();
      if(taxString.length()>0)
      {
         if(taxString.find("-", 0) == std::string::npos)
         {
            Application->MessageBox(("Please Enter Correct Tax Format which is TaxName-TaxPercent% in Row No " +  IntToStr(row)+ " and column no  " + IntToStr(column+1) + " ").c_str(), "Error", MB_ICONERROR + MB_OK);
            return false;
         }

      }

   return true;

}
///-------------------------------------------------------------------------------
 bool TfrmAdministration::CheckDuplicateStockCode(AnsiString StockCode)
 {
    IBQuery->Close();
    IBQuery->SQL->Text = "Select STOCK_KEY From Stock Where CODE= :CODE ";


    IBQuery->ParamByName("CODE")->AsString = StockCode.Trim();
    IBQuery->ExecQuery();

    if(!IBQuery->Eof)
    {
        return true;
    }
  return false;
 }
///----------------------------------------------------------------------------

 bool TimportMenu::CheckDuplicateStockCodeInDatabase(AnsiString StockCode,TIBSQL *qrStockQuery )
 {
    qrStockQuery->Close();
    qrStockQuery->SQL->Text = "Select STOCK_KEY From Stock Where CODE= :CODE ";
    qrStockQuery->ParamByName("CODE")->AsString = StockCode;
    qrStockQuery->ExecQuery();

    if(!qrStockQuery->Eof)
    {
        return true;
    }
    return false;
 }



//---------------------------------------------------------------------------

