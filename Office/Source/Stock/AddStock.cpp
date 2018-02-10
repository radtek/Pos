//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <cassert.h>
#include "AddStock.h"
#include "StockData.h"
#include "MMData.h"
#include "SelectSupplier.h"
#include "SelectLocation.h"
#include "GetBarcode.h"
#include "Stock.h"
#include "MM_Math.h"
#include "Connections.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumericEdit"
#pragma link "NumericEdit"
#pragma resource "*.dfm"
//TfrmAddStock *frmAddStock;
//---------------------------------------------------------------------------
__fastcall TfrmAddStock::TfrmAddStock(TComponent* Owner)
	: TForm(Owner),
	frmAddSupplierUnit(new TfrmAddSupplierUnit(NULL))
{
	AutoStockCode = true;
	DefaultLocation = "";
   
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::FormShow(TObject *Sender)
{
	FormResize(NULL);
  
	try
	{
		if (!Transaction->InTransaction) Transaction->StartTransaction();

		cbStockGroup->Items->Clear();
		IBQuery->Close();
		IBQuery->SQL->Text =
			"Select "
				"Stock_Group_Key, "
				"Stock_Group "
			"From "
				"StockGroup "
			"Where "
				"Deleted Is Null Or "
				"Deleted = 'F' "
			"Order By "
				"Stock_Group";
		IBQuery->Open();
		while (!IBQuery->Eof)
		{
			cbStockGroup->Items->AddObject(IBQuery->FieldByName("Stock_Group")->AsString,
													(TObject *)IBQuery->FieldByName("Stock_Group_Key")->AsInteger);
			IBQuery->Next();
		}
      //Load sale units, stocktake units and reductionunits in respective combobox
      LoadUnits();

		dbcbPurchaseUnit->Items->Clear();
		IBQuery->Close();
		IBQuery->SQL->Text =
			"Select Distinct "
				"Stocktake_Unit "
			"From "
				"Stock "
			"Order By "
				"Stocktake_Unit";
		IBQuery->Open();
		while (!IBQuery->Eof)
		{
			if (IBQuery->FieldByName("Stocktake_Unit")->AsString != "")
			{
				dbcbPurchaseUnit->Items->Add(IBQuery->FieldByName("Stocktake_Unit")->AsString);
			}
			IBQuery->Next();
		}
		IBQuery->Close();




        	cbStockSubGroup->Items->Clear();
		IBQuery->Close();
		IBQuery->SQL->Text =
			"Select Distinct "
				"STOCK_SUB_GROUP "
			"From "
				"Stock "
			"Order By "
				"STOCK_SUB_GROUP";
		IBQuery->Open();
		while (!IBQuery->Eof)
		{
			if (IBQuery->FieldByName("STOCK_SUB_GROUP")->AsString != "")
			{
				cbStockSubGroup->Items->Add(IBQuery->FieldByName("STOCK_SUB_GROUP")->AsString);
			}
			IBQuery->Next();
		}
		IBQuery->Close();


		dtStockEdit->Close();

		dbcSaleUnit->Items->Clear();
		IBQuery->Close();
		IBQuery->SQL->Text =
			"SELECT DISTINCT "
				"SALE_UNIT "
			"FROM "
				"STOCK "
			"ORDER BY "
				"SALE_UNIT";
		IBQuery->Open();
		while (!IBQuery->Eof)
		{
			if (IBQuery->FieldByName("SALE_UNIT")->AsString != "")
			{
				dbcSaleUnit->Items->Add(IBQuery->FieldByName("SALE_UNIT")->AsString);
			}
			IBQuery->Next();
		}
		IBQuery->Close();

		if (Mode == amAddStock)
		{
			Caption = "Add Stock";

			tsItemDetails->TabVisible	= true;
			PageControl1->ActivePage	= tsItemDetails;
			tsLocations->TabVisible		= true;
			tsSuppliers->TabVisible		= true;

			btnNext1->Visible				= true;
			btnPrev1->Visible				= true;
			btnNext2->Visible				= true;

			// New stock item needs a primary key
			sqlAddLK->Close();
			sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_STOCK_KEY, 1) FROM RDB$DATABASE";
			sqlAddLK->ExecQuery();
			int NewItemKey = sqlAddLK->Fields[0]->AsInteger;

			// Enter a new item, then post it. Allows addition of locations and suppliers.
			qrNewItem->Close();
			qrNewItem->ParamByName("Stock_Key")->AsInteger			= NewItemKey;
			qrNewItem->ParamByName("Stock_Group_Key")->AsInteger	= StockGroupKey;
			qrNewItem->ParamByName("Description")->AsString			= NewItemName;
			qrNewItem->ParamByName("Stocktake_Unit")->AsString		= DefaultUnit;
			qrNewItem->ParamByName("GST_Percent")->AsFloat			= CurrentConnection.DefaultStockGST;
         qrNewItem->ParamByName("STOCK_SUB_GROUP")->AsString		= DefaultSubGroup;


			sqlAddLK->Close();
			// If auto stock code, get from generator, try to post, and repeat until unique code found.
			if (AutoStockCode)
			{
				sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_STOCKCODE, 1) FROM RDB$DATABASE";
				sqlAddLK->ExecQuery();
				qrNewItem->ParamByName("Code")->AsString = sqlAddLK->Fields[0]->AsInteger;
				bool SuccessfulPost = false;
				while (!SuccessfulPost)
				{
					try
					{
						qrNewItem->ExecSQL();
						SuccessfulPost = true;
					   //	btnNextCode->Visible = false;
					}
					catch (EDatabaseError &E)
					{
						if (E.Message.AnsiPos("STOCK_PRIMARY") != 0)
						{
							sqlAddLK->ExecQuery();
							qrNewItem->ParamByName("Code")->AsString  = sqlAddLK->Fields[0]->AsInteger;
						}
						else throw;
					}
				}
			}
			else
			{
				qrNewItem->ParamByName("Code")->AsString = "";
				qrNewItem->ExecSQL();
				btnNextCode->Visible = true;
			}
			// Allow stock code to be changed.
			dbtStockCode->Visible = false;
			dbeStockCode->Visible = true;

			// Blank item should be the only one with no stock code.
//			dtStockEdit->ParamByName("CODE")->AsString = "";
			dtStockEdit->ParamByName("Stock_Key")->AsInteger = NewItemKey;
			dtStockEdit->Open();
			if (dtStockEdit->RecordCount == 0)
			{
				throw Exception("Error adding new stock item.");
			}
			dtStockEdit->Edit();

			dtStockLocationEdit->Open();
			// This code is duplicated below for editing items with no location.
			dtStockLocationEdit->BeforeInsert = NULL;
			try
			{
				if (CurrentConnection.SingleLocation)
				{
					if (CurrentConnection.DefaultLocation != "")
					{
						dtStockLocationEdit->Insert();

						dtStockLocationEdit->FieldByName("STOCK_KEY")->AsInteger			= dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
						dtStockLocationEdit->FieldByName("LOCATION")->AsString			= CurrentConnection.DefaultLocation;

						dtStockLocationEdit->FieldByName("ON_HAND")->AsFloat				= 0;
//						dtStockLocationEdit->FieldByName("ON_ORDER")->AsFloat				= 0;
						dtStockLocationEdit->FieldByName("MIN_LEVEL")->AsFloat			= 0;
						dtStockLocationEdit->FieldByName("MAX_LEVEL")->AsFloat			= 0;

						dtStockLocationEdit->FieldByName("DEFAULT_LOCATION")->AsString	= "T";

						dtStockLocationEdit->Post();
					}
				}
				else
				{
					if (dmMMData->dbMenuMate->Connected)
					{
						bool SetDefaultLocation = false;
						if (!trMenuMate->InTransaction) trMenuMate->StartTransaction();
						qrLocations->Close();
						qrLocations->Open();
						qrLocations->FetchAll();
						for (qrLocations->First(); !qrLocations->Eof; qrLocations->Next())
						{
							// Dont add a null location
							if ( qrLocations->FieldByName("Name")->AsString != "")
							{
								dtStockLocationEdit->Insert();

								dtStockLocationEdit->FieldByName("STOCK_KEY")->AsInteger			= dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
								dtStockLocationEdit->FieldByName("LOCATION")->AsString			= qrLocations->FieldByName("Name")->AsString;

								dtStockLocationEdit->FieldByName("ON_HAND")->AsFloat				= 0;
//							   dtStockLocationEdit->FieldByName("ON_ORDER")->AsFloat				= 0;
								dtStockLocationEdit->FieldByName("MIN_LEVEL")->AsFloat			= 0;
								dtStockLocationEdit->FieldByName("MAX_LEVEL")->AsFloat			= 0;

								if (qrLocations->FieldByName("Name")->AsString == CurrentConnection.DefaultLocation ||
									 qrLocations->RecordCount == 1)
								{
									dtStockLocationEdit->FieldByName("DEFAULT_LOCATION")->AsString	= "T";
									SetDefaultLocation = true;
								}
								else
								{
									dtStockLocationEdit->FieldByName("DEFAULT_LOCATION")->AsString	= "F";
								}
								dtStockLocationEdit->Post();
							}
						}
						trMenuMate->Commit();
						dtStockLocationEdit->First();
						if (!SetDefaultLocation)
						{
							Application->MessageBox("You have not specified a default location for stock receipt.\r"
															"Go into setup and select a valid default location.", "Warning", MB_ICONWARNING + MB_OK);
						}
					}
				}
			}
			__finally
			{
				dtStockLocationEdit->BeforeInsert = dtStockLocationEditBeforeInsert;
			}
			// Line up dropboxes
			cbStockGroup->ItemIndex			= cbStockGroup->Items->IndexOfObject((TObject *)StockGroupKey);
			dbcbPurchaseUnit->ItemIndex	= dbcbPurchaseUnit->Items->IndexOf(dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString);
         dbcbReductionUnit->ItemIndex	= dbcbReductionUnit->Items->IndexOf(dtStockEdit->FieldByName("REDUCTION_UNIT")->AsString);
         dbeConversionFactor->Text = dtStockEdit->FieldByName("CONVERSION_FACTOR")->AsString;
        	cbStockSubGroup->ItemIndex	= cbStockSubGroup->Items->IndexOf(dtStockEdit->FieldByName("STOCK_SUB_GROUP")->AsString);



			dbeStockCode->SetFocus();

			qrSuppliers->Close();
			qrSuppliers->Open();
			dtSuppliersStock->Open();
		}

		else if (Mode == amEditStock)// Edit mode
		{
             
			Caption = "Edit Stock";

			tsItemDetails->TabVisible	= true;
			PageControl1->ActivePage	= tsItemDetails;
		   	tsLocations->TabVisible		= true;
			tsSuppliers->TabVisible		= true;

			btnNext1->Visible				= true;
			btnPrev1->Visible				= true;
			btnNext2->Visible				= true;

			if (StockKey != NULL)
			{
				dbtStockCode->Visible	= true;
				dbeStockCode->Visible	= false;
				btnNextCode->Visible		= false;
			}
			else
			{
				dbtStockCode->Visible	= false;
				dbeStockCode->Visible	= true;
				btnNextCode->Visible		= true;
			}

			dtStockEdit->ParamByName("Stock_Key")->AsString = StockKey;
			dtStockEdit->Open();
			dtStockEdit->Edit();

			dtStockLocationEdit->Open();


			if (!dtStockLocationEdit->Eof)
			{
				dtStockLocationEdit->Edit();
			}
/*			else
			{
				// This code is a duplicate of that for new items
				dtStockLocationEdit->BeforeInsert = NULL;
				try
				{
					if (CurrentConnection.SingleLocation)
					{
						dtStockLocationEdit->Insert();

						dtStockLocationEdit->FieldByName("STOCK_KEY")->AsInteger		= dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
						dtStockLocationEdit->FieldByName("LOCATION")->AsString		= CurrentConnection.DefaultLocation;

						dtStockLocationEdit->FieldByName("ON_HAND")->AsFloat			= 0;
						dtStockLocationEdit->FieldByName("ON_ORDER")->AsFloat			= 0;
						dtStockLocationEdit->FieldByName("MIN_LEVEL")->AsFloat		= 0;
						dtStockLocationEdit->FieldByName("MAX_LEVEL")->AsFloat		= 0;

						dtStockLocationEdit->Post();
					}
					else
					{
						dmMMData->qrLocations->Close();
						for (dmMMData->qrLocations->Open(); !dmMMData->qrLocations->Eof; dmMMData->qrLocations->Next())
						{
							dtStockLocationEdit->Insert();

							dtStockLocationEdit->FieldByName("STOCK_KEY")->AsInteger		= dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
							dtStockLocationEdit->FieldByName("LOCATION")->AsString		= dmMMData->qrLocations->FieldByName("Name")->AsString;

							dtStockLocationEdit->FieldByName("ON_HAND")->AsFloat			= 0;
							dtStockLocationEdit->FieldByName("ON_ORDER")->AsFloat			= 0;
							dtStockLocationEdit->FieldByName("MIN_LEVEL")->AsFloat		= 0;
							dtStockLocationEdit->FieldByName("MAX_LEVEL")->AsFloat		= 0;

							dtStockLocationEdit->Post();
						}
					}
				}
				__finally
				{
					dtStockLocationEdit->BeforeInsert = dtStockLocationEditBeforeInsert;
				}
			}*/

			cbStockGroup->ItemIndex = cbStockGroup->Items->IndexOfObject((TObject *)StockGroupKey);
//			dbcbPurchaseUnit->ItemIndex	= dbcbPurchaseUnit->Items->IndexOf(dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString);
			// IndexOf doesn't seem to be case sensitive!
			for (int i=0; i<dbcbPurchaseUnit->Items->Count; i++)
			{
				if (dbcbPurchaseUnit->Items->Strings[i] == dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString)
				{
					dbcbPurchaseUnit->ItemIndex = i;
					break;
				}
			}

			dbcbReductionUnit->ItemIndex	= dbcbReductionUnit->Items->IndexOf(dtStockEdit->FieldByName("REDUCTION_UNIT")->AsString);
         dbeConversionFactor->Text = dtStockEdit->FieldByName("CONVERSION_FACTOR")->AsString;


            	for (int i=0; i<cbStockSubGroup->Items->Count; i++)
			{
				if (cbStockSubGroup->Items->Strings[i] == dtStockEdit->FieldByName("STOCK_SUB_GROUP")->AsString)
				{
					cbStockSubGroup->ItemIndex = i;
					break;
				}
			}




			if (StockKey != NULL)
			{
				dbeDescription->SetFocus();
			}
			else
			{
				dbeStockCode->SetFocus();
			}
			qrSuppliers->Close();
			qrSuppliers->Open();
			dtSuppliersStock->Open();
		}
		else if (Mode == amAddSupplierUnit)
		{
			dtStockEdit->ParamByName("Stock_Key")->AsString = StockKey;
			dtStockEdit->Open();
			Caption = "Add Supplier Unit (" + dtStockEdit->FieldByName("Description")->AsString + ")";
			dtStockEdit->Edit();

			dtStockLocationEdit->Open();

			cbStockGroup->ItemIndex			= cbStockGroup->Items->IndexOfObject((TObject *)StockGroupKey);
			dbcbPurchaseUnit->ItemIndex	= dbcbPurchaseUnit->Items->IndexOf(dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString);
         dbcbReductionUnit->ItemIndex	= dbcbReductionUnit->Items->IndexOf(dtStockEdit->FieldByName("REDUCTION_UNIT")->AsString);
         dbeConversionFactor->Text = dtStockEdit->FieldByName("CONVERSION_FACTOR")->AsString;
			tsSuppliers->TabVisible			= true;
			PageControl1->ActivePage		= tsSuppliers;
			tsItemDetails->TabVisible		= false;
			tsLocations->TabVisible			= false;

			btnNext1->Visible					= false;
			btnPrev1->Visible					= false;
			btnNext2->Visible					= false;

			if (NewSupplierKey)
			{
				sqlFindSupplier->Close();
				sqlFindSupplier->ParamByName("SUPPLIER_KEY")->AsString	= NewSupplierKey;
				sqlFindSupplier->ParamByName("STOCK_KEY")->AsInteger		= dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
				sqlFindSupplier->ExecQuery();
				bool AddedUnit = false;
				if (sqlFindSupplier->Fields[0]->AsInteger == 0)
				{
					qrSupplierName->Close();
					qrSupplierName->ParamByName("Supplier_Key")->AsInteger = NewSupplierKey;
					qrSupplierName->ExecQuery();
					frmAddSupplierUnit->SupplierName = qrSupplierName->FieldByName("Company_Name")->AsString;
					frmAddSupplierUnit->ItemName = dtStockEdit->FieldByName("Description")->AsString;
					qrSupplierName->Close();
					if (frmAddSupplierUnit->ShowModal() == mrOk)
					{
						AddedUnit = true;
						sqlAddLK->Close();
						sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_SUPPLIERSTOCK, 1) FROM RDB$DATABASE";
						sqlAddLK->ExecQuery();

						qrAddSupplier->Close();
						qrAddSupplier->ParamByName("Supplier_Stock_Key")->AsInteger	= sqlAddLK->Fields[0]->AsInteger;
						qrAddSupplier->ParamByName("Stock_Key")->AsInteger				= dtStockEdit->FieldByName("Stock_Key")->AsInteger;
						qrAddSupplier->ParamByName("Supplier_Key")->AsInteger			= NewSupplierKey;
						qrAddSupplier->ParamByName("Supplier_Unit")->AsString			= frmAddSupplierUnit->cbUnits->Text;
						qrAddSupplier->ParamByName("Default_Order_Unit")->AsString	= "T"; // First supplier unit

						qrSuppliers->Open();
						if (qrSuppliers->RecordCount == 0)	// Adding the first supplier. Make it the preferred.
						{
							qrAddSupplier->ParamByName("Preferred_Supplier")->AsString	= "T";
						}
						else
						{
							qrAddSupplier->ParamByName("Preferred_Supplier")->AsString	= "F";
						}
						qrAddSupplier->ExecSQL();

						qrAddSupplier->Close();

						dtSuppliersStock->Close();
						qrSuppliers->Close();
					}
				}
				qrSuppliers->Open();
				qrSuppliers->FetchAll();
				qrSuppliers->Locate("SUPPLIER_KEY", NewSupplierKey, TLocateOptions());
				dtSuppliersStock->Open();
				dtSuppliersStock->FetchAll();
				if (AddedUnit)
				{
					dtSuppliersStock->Locate("Supplier_Unit", frmAddSupplierUnit->cbUnits->Text, TLocateOptions());
				}
				if (dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString == "")
				{
					chbPreferredSupplier->Checked = true;
//					dtSuppliersStock->Edit();
//					dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString = "F";
//					dtSuppliersStock->Post();
				}
				if (qrSuppliers->RecordCount == 1)
				{
					chbPreferredSupplier->Checked = true;
				}
				if (dtSuppliersStock->RecordCount == 1)
				{
					chbDefaultOrderUnit->Checked = true;
				}
			}
			else
			{
				// No Suppliers yet!
				PostMessage(btnAddSupplier->Handle, WM_LBUTTONDOWN, 0, 0);
				PostMessage(btnAddSupplier->Handle, WM_LBUTTONUP, 0, 0);
			}
			ResizeGrids();
			dbcOrderQty->SetFocus();
		}
		else if (Mode == amAddLocation)
		{
			dtStockEdit->ParamByName("Stock_Key")->AsString = StockKey;
			dtStockEdit->Open();
			dtStockEdit->Edit();
			Caption = "Add Location (" + dtStockEdit->FieldByName("Description")->AsString + ")";

			dtStockLocationEdit->Open();

			cbStockGroup->ItemIndex			= cbStockGroup->Items->IndexOfObject((TObject *)StockGroupKey);
			dbcbPurchaseUnit->ItemIndex	= dbcbPurchaseUnit->Items->IndexOf(dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString);
         dbcbReductionUnit->ItemIndex	= dbcbReductionUnit->Items->IndexOf(dtStockEdit->FieldByName("REDUCTION_UNIT")->AsString);
         dbeConversionFactor->Text = dtStockEdit->FieldByName("CONVERSION_FACTOR")->AsString;
            cbStockSubGroup->ItemIndex	= cbStockSubGroup->Items->IndexOf(dtStockEdit->FieldByName("STOCK_SUB_GROUP")->AsString);     //tp
			tsItemDetails->TabVisible		= false;
			tsLocations->TabVisible			= true;
			tsSuppliers->TabVisible			= false;
			PageControl1->ActivePage		= tsLocations;

			btnNext1->Visible					= false;
			btnPrev1->Visible					= false;
			btnNext2->Visible					= false;
			dtSuppliersStock->Open();
		}
		ResizeGrids();


           PopulateSubstituteStockControl();
         PopulateSubstituteField();

	}
	catch (Exception &E)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		Application->ShowException(&E);
		PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}

 /***********************************************/
    void TfrmAddStock::  PopulateSubstituteField()
     {
        SubstituteStockkey1=0;
        SubstituteStockkey2=0;
        SubstituteStockkey3=0;
         IBQuery->Close();
        IBQuery->SQL->Text =
                    "SELECT REVERT_TO_ORIGINAL "
                    "FROM STOCK "
                    "where STOCK_KEY=:Stock_Key ";
        IBQuery->ParamByName("Stock_Key")->AsInteger		= StockKey;
        IBQuery->Open();
       AnsiString revertToOriginal=  IBQuery->FieldByName("REVERT_TO_ORIGINAL")->AsString;

       chbminLevel->Checked=  revertToOriginal=="T"? true:false;


        IBQuery->Close();
        IBQuery->SQL->Text =
					"SELECT SUBSTITUTE_KEY , STOCK_KEY ,SUBSTITUTE_STOCK_KEY "
					"FROM STOCK_SUBSTITUTES "
					"where STOCK_KEY=:Stock_Key ";
        IBQuery->ParamByName("Stock_Key")->AsInteger		= StockKey;
        IBQuery->Open();
        int counter=0;

       	while (!IBQuery->Eof)
		{       qrpopulateSubstitute->Close();
                int SubstituteKey=IBQuery->FieldByName("SUBSTITUTE_KEY")->AsInteger;
                    int stocksubKey=IBQuery->FieldByName("SUBSTITUTE_STOCK_KEY")->AsInteger;
                AnsiString Description= PopulateSubstituteDescription(stocksubKey)  ;
                IBQuery->Next();
                qrpopulateSubstitute ->ParamByName("SUBSTITUTE_KEY")->AsInteger = SubstituteKey;
                qrpopulateSubstitute->Open();


                if(counter==0 && SubstituteKey>0)
                {
                SubstituteStockkey1= stocksubKey;
                SubstituteUniqueId1= SubstituteKey  ;
                 edSearch->Text =  Description;

                    DBCmbsubststockitem1->ItemIndex= qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND1")->AsInteger;
                    DBCmbsubsttem1->ItemIndex=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND2")->AsInteger;
                    txtvariance1->Value=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND3")->AsInteger;
                  cbReadyForSubstitute1->Checked= qrpopulateSubstitute->FieldByName("SELECTABLE")->AsBoolean;
                }


                  if(counter==1 && SubstituteKey>0)
                { SubstituteStockkey2= stocksubKey;
                SubstituteUniqueId2= SubstituteKey  ;
                   edSearch1->Text =  Description;
                    DBCmbsubststockitem2->ItemIndex= qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND1")->AsInteger;
                    DBCmbsubsttem2->ItemIndex=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND2")->AsInteger;
                    txtvariance2->Value=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND3")->AsInteger;
                     cbReadyForSubstitute2->Checked= qrpopulateSubstitute->FieldByName("SELECTABLE")->AsBoolean;
                }


                   if(counter==2 && SubstituteKey>0)
                {
                SubstituteStockkey3= stocksubKey;
                SubstituteUniqueId3= SubstituteKey  ;
                edSearch2->Text =  Description;
                DBCmbsubststockitem3->ItemIndex= qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND1")->AsInteger;
                DBCmbsubsttem3->ItemIndex=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND2")->AsInteger;
                txtvariance3->Value=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND3")->AsInteger;
                 cbReadyForSubstitute3->Checked= qrpopulateSubstitute->FieldByName("SELECTABLE")->AsBoolean;
                }


                counter=counter+1;
		}

        if(SubstituteStockkey1<1)
        {    DBCmbsubsttem1->ItemIndex=-1;
                    DBCmbsubststockitem1->ItemIndex=-1;
                    txtvariance1->Value=0;
                    edSearch->Text ="";
                    SubstituteStockkey1=0;
                    SubstituteUniqueId1= 0  ;
        }
         if(SubstituteStockkey2<1)
        {DBCmbsubsttem2->ItemIndex=-1;
                    DBCmbsubststockitem2->ItemIndex=-1;
                    txtvariance2->Value=0;
                    edSearch1->Text ="";
                    SubstituteStockkey2=0;
                     SubstituteUniqueId2= 0  ;
        }
         if(SubstituteStockkey3<1)
        { DBCmbsubsttem3->ItemIndex=-1;
                    DBCmbsubststockitem3->ItemIndex=-1;
                    txtvariance3->Value=0;
                    edSearch2->Text ="";
                    SubstituteStockkey3=0;
                     SubstituteUniqueId3= 0  ;
        }


          qrpopulateSubstitute->Close();
          /***********************************************/


     }
AnsiString TfrmAddStock:: PopulateSubstituteDescription(int temStockKey)
 {
           	qrStockSubstitute->Close();
               qrStockSubstitute->SQL->Text =
			"SELECT "
				"Code,"
				"DESCRIPTION,"
				"STOCKTAKE_UNIT "
			"FROM "
				"STOCK "
			"WHERE "
				"Stock_Key = :Stock_Key";
		qrStockSubstitute->ParamByName("Stock_Key")->AsString = temStockKey;
		qrStockSubstitute->Open();
				  AnsiString	SubsituteDiscription = qrStockSubstitute->FieldByName("DESCRIPTION")->AsString;
                  return    SubsituteDiscription;
 }
//---------------------------------------------------------------------------

void TfrmAddStock::LoadUnits()
{
  IBQuery->Close();

		dbcbPurchaseUnit->Items->Clear();
		IBQuery->Close();
		IBQuery->SQL->Text =
			"Select Distinct "
				"Stocktake_Unit "
			"From "
				"Stock "
			"Order By "
				"Stocktake_Unit";
		IBQuery->Open();
		while (!IBQuery->Eof)
		{
			if (IBQuery->FieldByName("Stocktake_Unit")->AsString != "")
			{
				dbcbPurchaseUnit->Items->Add(IBQuery->FieldByName("Stocktake_Unit")->AsString);
			}
			IBQuery->Next();
		}
		IBQuery->Close();

		dtStockEdit->Close();

		dbcSaleUnit->Items->Clear();
		IBQuery->Close();
		IBQuery->SQL->Text =
			"SELECT DISTINCT "
				"SALE_UNIT "
			"FROM "
				"STOCK "
			"ORDER BY "
				"SALE_UNIT";
		IBQuery->Open();
		while (!IBQuery->Eof)
		{
			if (IBQuery->FieldByName("SALE_UNIT")->AsString != "")
			{
				dbcSaleUnit->Items->Add(IBQuery->FieldByName("SALE_UNIT")->AsString);
			}
			IBQuery->Next();
		}
   dbcbReductionUnit->Clear();
	IBQuery->Close();
   		IBQuery->SQL->Text =
			"SELECT DISTINCT "
				"REDUCTION_UNIT "
			"FROM "
				"STOCK "
			"ORDER BY "
				"REDUCTION_UNIT";
		IBQuery->Open();
		while (!IBQuery->Eof)
		{
			if (IBQuery->FieldByName("REDUCTION_UNIT")->AsString != "")
			{
				dbcbReductionUnit->Items->Add(IBQuery->FieldByName("REDUCTION_UNIT")->AsString);
			}
			IBQuery->Next();
		}
		IBQuery->Close();
}



void TfrmAddStock::PopulateSubstituteStockControl()
{
    DBCmbsubststockitem1->Items->Clear();
	DBCmbsubststockitem1->Items->Add("On Hand<min-level");
    DBCmbsubststockitem1->Items->Add("On Hand<0");
    DBCmbsubsttem1->Items->Clear();
	DBCmbsubsttem1->Items->Add("On Hand<min-level");
    DBCmbsubsttem1->Items->Add("On Hand<0");
    DBCmbsubststockitem2->Items->Clear();
	DBCmbsubststockitem2->Items->Add("On Hand<min-level");
    DBCmbsubststockitem2->Items->Add("On Hand<0");
    DBCmbsubsttem2->Items->Clear();
	DBCmbsubsttem2->Items->Add("On Hand<min-level");
    DBCmbsubsttem2->Items->Add("On Hand<0");
    DBCmbsubststockitem3->Items->Clear();
	DBCmbsubststockitem3->Items->Add("On Hand<min-level");
    DBCmbsubststockitem3->Items->Add("On Hand<0");
    DBCmbsubsttem3->Items->Clear();
	DBCmbsubsttem3->Items->Add("On Hand<min-level");
    DBCmbsubsttem3->Items->Add("On Hand<0");


}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::FormClose(TObject *Sender,
		TCloseAction &Action)
{
	try
	{
		if (Transaction->InTransaction) Transaction->Rollback();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
	}
//	if (dtStockEdit->State != dsBrowse)	dtStockEdit->Cancel();
//	if (Transaction->InTransaction)		Transaction->Commit();
	NewItemName = "";
}
//---------------------------------------------------------------------------
/*int TfrmAddStock::CopyStockItem(AnsiString CodeToCopy)
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	try
	{

		// New stock item needs a primary key
		sqlAddLK->Close();
		sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_STOCK_KEY, 1) FROM RDB$DATABASE";
		sqlAddLK->ExecQuery();
		int StockKey = sqlAddLK->Fields[0]->AsInteger;
		sqlAddLK->Close();

		qrStockCopy->Close();
		qrStockCopy->ParamByName("CODE")->AsString = CodeToCopy;
      qrStockCopy->Open();

      // Enter a new item, then post it. Allows addition of locations and suppliers.
      qrNewItem->Close();
      qrNewItem->ParamByName("STOCK_KEY")->AsInteger		= StockKey;
		qrNewItem->ParamByName("DESCRIPTION")->AsString		= "";//qrStockCopy->FieldByName("DESCRIPTION")->AsString;
//      qrNewItem->ParamByName("Stock_Group")->AsString			= qrStockCopy->FieldByName("Stock_Group")->AsString;
//		StockGroup															= qrStockCopy->FieldByName("Stock_Group")->AsString;
      qrNewItem->ParamByName("ITEM_SIZE")->AsFloat			= qrStockCopy->FieldByName("ITEM_SIZE")->AsFloat;
		qrNewItem->ParamByName("STOCKTAKE_UNIT")->AsString	= qrStockCopy->FieldByName("STOCKTAKE_UNIT")->AsString;
      qrNewItem->ParamByName("SALE_UNIT")->AsString		= qrStockCopy->FieldByName("SALE_UNIT")->AsString;

      // If auto stock code, get from generator, try to post, and repeat until unique code found.
      if (AutoStockCode)
      {
         sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_STOCKCODE, 1) FROM RDB$DATABASE";
         sqlAddLK->ExecQuery();
         qrNewItem->ParamByName("CODE")->AsString = sqlAddLK->Fields[0]->AsInteger;
         bool SuccessfulPost = false;
         while (!SuccessfulPost)
         {
            try
            {
               qrNewItem->ExecSQL();
               SuccessfulPost = true;
               Code = sqlAddLK->Fields[0]->AsInteger;
					sqlAddLK->Close();
            }
            catch (EDatabaseError &E)
            {
               if (E.Message.AnsiPos("STOCK_PRIMARY") != 0)
               {
                  sqlAddLK->ExecQuery();
                  qrNewItem->ParamByName("CODE")->AsString  = sqlAddLK->Fields[0]->AsInteger;
               }
               else throw;
            }
         }
      }
      else
		{
         qrNewItem->ParamByName("CODE")->AsString = "";
			qrNewItem->ExecSQL();
         btnNextCode->Visible = true;
         Code = "";
      }
      dtStockLocationEdit->Close();
      try
      {
         dtStockLocationEdit->DataSource											= NULL;
         dtStockLocationEdit->ParamByName("STOCK_KEY")->AsInteger			= StockKey;
         dtStockLocationEdit->Open();

			for (qrStockLocationCopy->Open(); !qrStockLocationCopy->Eof; qrStockLocationCopy->Next())
         {
				dtStockLocationEdit->BeforeInsert = NULL;
				try
				{
					dtStockLocationEdit->Insert();
				}
				__finally
				{
					dtStockLocationEdit->BeforeInsert = dtStockLocationEditBeforeInsert;
				}
				dtStockLocationEdit->FieldByName("STOCK_KEY")->AsInteger		= StockKey;
            dtStockLocationEdit->FieldByName("LOCATION")->AsString		= qrStockLocationCopy->FieldByName("LOCATION")->AsString;

            dtStockLocationEdit->FieldByName("ON_HAND")->AsFloat			= 0;
				dtStockLocationEdit->FieldByName("ON_ORDER")->AsFloat			= 0;
            dtStockLocationEdit->FieldByName("MIN_LEVEL")->AsFloat		= qrStockLocationCopy->FieldByName("MIN_LEVEL")->AsFloat;
            dtStockLocationEdit->FieldByName("MAX_LEVEL")->AsFloat		= qrStockLocationCopy->FieldByName("MAX_LEVEL")->AsFloat;

            dtStockLocationEdit->Post();
         }
      }
      __finally
      {
         dtStockLocationEdit->DataSource = dsStockEdit;
      }
      qrStockLocationCopy->Close();
		qrStockCopy->Close();

      frmAddStock->Mode = amEditStock;
      return ShowModal();
   }
	catch (Exception &E)
	{
		if (Transaction->InTransaction) Transaction->Rollback();
		throw;
	}
}*/
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dtStockLocationEditAfterOpen(
      TDataSet *DataSet)
{
	((TFloatField *)DataSet->FieldByName("LATEST_COST"))->currency = false;
	((TFloatField *)DataSet->FieldByName("AVERAGE_COST"))->currency = false;
	((TFloatField *)DataSet->FieldByName("ASSESSED_VALUE"))->currency = false;
	((TField *)DataSet->FieldByName("MIN_LEVEL"))->Alignment = taLeftJustify;
	((TField *)DataSet->FieldByName("MAX_LEVEL"))->Alignment = taLeftJustify;
}
//---------------------------------------------------------------------------
void TfrmAddStock::ResizeGrids()
{
	dbgLocations->Columns->Items[0]->Width = dbgLocations->ClientWidth;
	dbgSuppliers->Columns->Items[0]->Width = dbgSuppliers->ClientWidth;
	dbgOrderUnit->Columns->Items[0]->Width = dbgOrderUnit->ClientWidth;
}
//---------------------------------------------------------------------------
void TfrmAddStock::ResizeBarcodeGrids()
{
	dbgItemBarcode->Columns->Items[2]->Width = (dbgItemBarcode->ClientWidth - dbgItemBarcode->Columns->Items[0]->Width -
		dbgItemBarcode->Columns->Items[1]->Width );
	dbgSupplierBarcode->Columns->Items[2]->Width = (dbgSupplierBarcode->ClientWidth - dbgSupplierBarcode->Columns->Items[0]->Width -
		dbgSupplierBarcode->Columns->Items[1]->Width );
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnOkClick(TObject *Sender)
{
    if (dbeStockCode->Text.Trim().Length() == 0) {
        Application->MessageBox("You must enter a stock code!",
                                "Error", MB_ICONERROR + MB_OK);
        return;
    }

    if (!dbeGST->Text.Length()
        || StrToFloatDef(dbeGST->Text, -1.0) < 0.0) {
            Application->MessageBox("You cannot specify a negative GST.",
                                    "Bad GST value",
                                    MB_ICONERROR + MB_OK);
        dbeGST->Text = "15.0";
        return;
    }

   if(!CheckReductionUnit())
      {
        return;
      }

	btnOk->SetFocus(); // Post current editing field.
	if (PostData())
	{
		if (dtStockLocationEdit->RecordCount == 0)
		{
         PageControl1->ActivePage = tsLocations;
         dtStockEdit->Edit();
			Application->MessageBox("Please add a location.", "Error", MB_ICONERROR + MB_OK);
			btnAddLocationClick(NULL);
      }
      else
      {
			if (Transaction->InTransaction) Transaction->Commit();
			ModalResult = mrOk;
      }
   }
         else{return;}

       //  ResetSubstitueControl();
            qrSetMinLevel->Close();
       if (chbminLevel->Checked )
			{
                	qrSetMinLevel->ParamByName("STOCK_KEY")->AsInteger = StockKey;
				qrSetMinLevel->ParamByName("REVERT_TO_ORIGINAL")->AsString	= "T" ;
			}
			else
			{
                qrSetMinLevel->ParamByName("STOCK_KEY")->AsInteger = StockKey;
				qrSetMinLevel->ParamByName("REVERT_TO_ORIGINAL")->AsString	= "F" ;;
			}
				qrSetMinLevel->ExecSQL();
                	qrSetMinLevel->Close();

                        	if(Transaction->InTransaction)
            Transaction->Commit();
    	SubsituteData();
        PopulateSubstituteField();
}
//---------------------------------------------------------------------------
void TfrmAddStock::ResetSubstitueControl()
{
        DBCmbsubsttem1->ItemIndex=-1;
		DBCmbsubsttem2->ItemIndex=-1;
		DBCmbsubsttem3->ItemIndex=-1;
		DBCmbsubststockitem1->ItemIndex=-1;
		DBCmbsubststockitem2->ItemIndex=-1;
		DBCmbsubststockitem3->ItemIndex=-1;
		txtvariance1->Value=0;
		txtvariance2->Value=0;
		txtvariance3->Value=0;
		edSearch->Text ="";
		edSearch1->Text ="";
		edSearch2->Text ="";
		SubstituteStockkey1=0;
		SubstituteStockkey2=0;
		SubstituteStockkey3=0;
}


 int TfrmAddStock::InsertSubstituteField(int StockKey, int SubstituteStockkey,int SubstituteUniqueId)
{
		int substituteId=0;
		qrChkStockIdExist->Close();
		qrChkStockIdExist->ParamByName("SUBSTITUTE_KEY")->AsInteger = SubstituteUniqueId;

		qrChkStockIdExist->Open();
		int chksubstituteIExist=  qrChkStockIdExist->FieldByName("IdExist")->AsInteger;

  if(!chksubstituteIExist)
	{
		qrInsertSTOCK_SUBSTITUTES->ParamByName("STOCK_KEY")->AsInteger	= StockKey;
		qrInsertSTOCK_SUBSTITUTES->ParamByName("SUBSTITUTE_STOCK_KEY")->AsInteger = SubstituteStockkey;
		qrInsertSTOCK_SUBSTITUTES->ExecSQL();
		qrInsertSTOCK_SUBSTITUTES->Close();
		qrSubstituteId->ParamByName("STOCK_KEY")->AsInteger = StockKey;
		qrSubstituteId->ParamByName("SUBSTITUTE_STOCK_KEY")->AsInteger = SubstituteStockkey;
		qrSubstituteId->Open();
		substituteId=qrSubstituteId->FieldByName("SUBSTITUTE_KEY")->AsInteger;

	}
	else
	{


		qrUpdateSTOCK_SUBSTITUTES->ParamByName("SUBSTITUTE_KEY")->AsInteger	=SubstituteUniqueId ;
		qrUpdateSTOCK_SUBSTITUTES->ParamByName("STOCK_KEY")->AsInteger	= StockKey;
		qrUpdateSTOCK_SUBSTITUTES->ParamByName("SUBSTITUTE_STOCK_KEY")->AsInteger	  = SubstituteStockkey;
		qrUpdateSTOCK_SUBSTITUTES->ExecSQL();
		qrUpdateSTOCK_SUBSTITUTES->Close();
       substituteId= SubstituteUniqueId ;
	}
		if(Transaction->InTransaction)
            Transaction->Commit();

return substituteId;
}


void TfrmAddStock::InsertSubstituteSettings(int substituteId, AnsiString SUBSTITUTE_COND1, AnsiString SUBSTITUTE_COND2, AnsiString SUBSTITUTE_COND3,bool isSelectable)
{



		qrSubstitutePersmissionID->Close();
		qrSubstitutePersmissionID->ParamByName("SUBSTITUTE_KEY")->AsInteger = substituteId;

		qrSubstitutePersmissionID->Open();
		int chksubstituteIExist=  qrSubstitutePersmissionID->FieldByName("IdExist")->AsInteger;

  if(!chksubstituteIExist)
	{
                     //qrUpdateStockPermission->Close();
            //qrUpdateStockPermission->ParamByName("SUBSTITUTE_KEY")->AsInteger = substituteId;
             //qrUpdateStockPermission->ParamByName("SELECTABLE")->AsString	  = 'F';
            //qrUpdateStockPermission->ExecSQL();
           // qrUpdateStockPermission->Close();

		qrSubstitute->ParamByName("SUBSTITUTE_KEY")->AsInteger	= substituteId;
		qrSubstitute->ParamByName("SUBSTITUTE_COND1")->AsString	=SUBSTITUTE_COND1 ;
		qrSubstitute->ParamByName("SUBSTITUTE_COND2")->AsString	=SUBSTITUTE_COND2;
		qrSubstitute->ParamByName("SUBSTITUTE_COND3")->AsString	= SUBSTITUTE_COND3;
		qrSubstitute->ParamByName("TIME_STAMP")->AsString	=Now();
		qrSubstitute->ParamByName("SELECTABLE")->AsString	=    isSelectable==true? "T":"F";
		qrSubstitute->ExecSQL();
		qrSubstitute->Close();

        }
        else
        {


        	qrupdateSubstituteSetting->ParamByName("SUBSTITUTE_KEY")->AsInteger	= substituteId;
		qrupdateSubstituteSetting->ParamByName("SUBSTITUTE_COND1")->AsString	=SUBSTITUTE_COND1 ;
		qrupdateSubstituteSetting->ParamByName("SUBSTITUTE_COND2")->AsString	=SUBSTITUTE_COND2;
		qrupdateSubstituteSetting->ParamByName("SUBSTITUTE_COND3")->AsString	= SUBSTITUTE_COND3;
		qrupdateSubstituteSetting->ParamByName("TIME_STAMP")->AsString	=Now();
		qrupdateSubstituteSetting->ParamByName("SELECTABLE")->AsString	=    isSelectable==true? "T":"F";
		qrupdateSubstituteSetting->ExecSQL();
		qrupdateSubstituteSetting->Close();


        }
}

void TfrmAddStock::SubsituteData()
{
         int substituteId=0;
            qrSubstitute->Close();
            if((DBCmbsubststockitem1->ItemIndex != -1) && ( DBCmbsubsttem1->ItemIndex !=-1) && SubstituteStockkey1!=0)
            {     substituteId=  InsertSubstituteField( StockKey,  SubstituteStockkey1,SubstituteUniqueId1);
                  InsertSubstituteSettings(substituteId,DBCmbsubststockitem1->ItemIndex ,DBCmbsubsttem1->ItemIndex,txtvariance1->Value,cbReadyForSubstitute1->Checked);
            }
             if(DBCmbsubststockitem2->ItemIndex!=-1 && DBCmbsubsttem2->ItemIndex!=-1 && SubstituteStockkey2!=0)
            {
               substituteId=     InsertSubstituteField( StockKey,  SubstituteStockkey2,SubstituteUniqueId2);
                InsertSubstituteSettings(substituteId,DBCmbsubststockitem2->ItemIndex ,DBCmbsubsttem2->ItemIndex,txtvariance2->Value,cbReadyForSubstitute2->Checked);

            }
              if(DBCmbsubststockitem3->ItemIndex!=-1 && DBCmbsubsttem3->ItemIndex!=-1  && SubstituteStockkey3!=0)
            {
                substituteId=     InsertSubstituteField( StockKey,  SubstituteStockkey3,SubstituteUniqueId3);
                InsertSubstituteSettings(substituteId,DBCmbsubststockitem3->ItemIndex ,DBCmbsubsttem3->ItemIndex,txtvariance3->Value,cbReadyForSubstitute3->Checked);

            }
           // chbminLevel->Checked = true;
            if(Transaction->InTransaction)
            Transaction->Commit();

     /*-----------------------------------------------------*/
}


bool TfrmAddStock::PostData()
{

	if (dtStockLocationEdit->State == dsEdit || dtStockLocationEdit->State == dsInsert)
	{
		dtStockLocationEdit->Post();
	}
	if (dtSuppliersStock->State == dsEdit || dtSuppliersStock->State == dsInsert)
	{
		if(dbgSuppliers->DataSource->DataSet->RecordCount == 0)
        {
			Application->MessageBox("You must add a supplier.", "Error", MB_ICONERROR + MB_OK);
			return false;           
        }
        else
        {
		    dtSuppliersStock->Post();
        }
	}
	if (Mode == amAddSupplierUnit)
	{
		if (dtSuppliersStock->Active && dtSuppliersStock->RecordCount > 0)
		{
			SupplierUnit = dtSuppliersStock->FieldByName("SUPPLIER_UNIT")->AsString;
			NewSupplierKey = qrSuppliers->FieldByName("Supplier_Key")->AsInteger;
		}
		else
		{
			Application->MessageBox("You must add a supplier.", "Error", MB_ICONERROR + MB_OK);
			return false;
		}
	}
	else
	{
		SupplierUnit = "";
	}
	if (Mode == amAddLocation)
	{
		Location = dtStockLocationEdit->FieldByName("LOCATION")->AsString;
	}
	else
	{
		Location = "";
	}
	if (!CheckDetails())
   {
   	return false;
   }
   if (!CheckPurchaseUnit())
	{
   	return false;
   }
	if (!CheckOrderUnits())
   {
   	return false;
	}
//	if (!CheckSalesUnit())
//	{
//		return false;
//	}
   if (!CheckLocations())
   {
   	return false;
   }

	try
	{
		dtStockEdit->Post();

		StockKey				= dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
		StockGroupKey	    	= dtStockEdit->FieldByName("Stock_Group_Key")->AsInteger;
		DefaultUnit		    	= dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString;
        DefaultSubGroup         = dtStockEdit->FieldByName("STOCK_SUB_GROUP")->AsString;
		return true;
	}
	catch (EDatabaseError &E)
	{
		if (E.Message.AnsiPos("STOCK_CODE_INDEX") != 0)
		{
			tsItemDetails->TabVisible = true;
			PageControl1->ActivePage = tsItemDetails;
			Application->MessageBox("The stock code was not unique. Please try again.", "Error", MB_ICONERROR + MB_OK);
			dbeStockCode->Visible = true;
         dbtStockCode->Visible = false;
			dbeStockCode->SetFocus();
			return false;
		}
		else if (E.Message.AnsiPos("STOCK_DESCINDEX") != 0)
		{
			tsItemDetails->TabVisible = true;
			PageControl1->ActivePage = tsItemDetails;
			Application->MessageBox("The stock description must be unique.", "Error", MB_ICONERROR + MB_OK);
			dbeDescription->SetFocus();
			return false;
		}
		else
		{
			Application->ShowException(&E);
			return false;
		}
	}
}
//---------------------------------------------------------------------------
bool TfrmAddStock::CheckDetails()
{
   bool Success = true;

   if (dtStockEdit->FieldByName("CODE")->AsString == "")
   {
      Application->MessageBox("You must enter a stock code.", "Error", MB_ICONERROR + MB_OK);
		tsItemDetails->TabVisible = true;
		PageControl1->ActivePage = tsItemDetails;
      dbeStockCode->SetFocus();
      Success = false;
   }
/*	else if (dtStockEdit->FieldByName("Stock_Group")->AsString == "")
   {
      Application->MessageBox("You must enter a stock group.", "Error", MB_ICONERROR + MB_OK);
      PageControl1->ActivePage = tsItemDetails;
      dbeDescription->SetFocus();
      Success = false;
	}*/
   else if (dtStockEdit->FieldByName("DESCRIPTION")->AsString == "")
   {
      Application->MessageBox("You must enter a description.", "Error", MB_ICONERROR + MB_OK);
		tsItemDetails->TabVisible = true;
		PageControl1->ActivePage = tsItemDetails;
      dbeDescription->SetFocus();
      Success = false;
   }
   else
   {
      // Find duplicate descriptions. Make sure not to compare with itself.
      IBQuery->Close();
      IBQuery->SQL->Text =
			"Select "
				"Stock.Description, "
				"Stock.Deleted, "
				"StockGroup.Stock_Group "
			"From "
				"Stock Inner Join StockGroup On "
					"Stock.Stock_Group_Key = StockGroup.Stock_Group_Key "
			"Where "
				"Upper(Description) = :Description And Stock_Key <> :Stock_Key";
		IBQuery->ParamByName("Description")->AsString	= dtStockEdit->FieldByName("Description")->AsString.UpperCase();
		IBQuery->ParamByName("Stock_Key")->AsInteger		= dtStockEdit->FieldByName("Stock_Key")->AsInteger;
      IBQuery->Open();
		if (!IBQuery->IsEmpty())
		{
			if (IBQuery->FieldByName("Deleted")->AsString == 'T')
			{
				Application->MessageBox(("The deleted item '" + IBQuery->FieldByName("Description")->AsString +
												"' already exists. Either permenantly delete first or restore this item.").c_str(), "Error", MB_ICONERROR + MB_OK);
			}
			else
			{
				Application->MessageBox(("The item '" + IBQuery->FieldByName("Description")->AsString +
												"' already exists in the stock group '" + IBQuery->FieldByName("Stock_Group")->AsString +
												"'.").c_str(), "Error", MB_ICONERROR + MB_OK);
			}
			tsItemDetails->TabVisible = true;
			PageControl1->ActivePage = tsItemDetails;
         dbeDescription->SetFocus();
         Success = false;
      }
      // Find duplicate stock codes. Make sure not to compare with itself.
      IBQuery->Close();
      IBQuery->SQL->Text =
         "SELECT "
            "CODE, "
            "DESCRIPTION "
         "FROM "
            "STOCK "
         "WHERE "
            "UPPER(CODE) = :CODE AND STOCK_KEY <> :STOCK_KEY";
      IBQuery->ParamByName("CODE")->AsString = dtStockEdit->FieldByName("CODE")->AsString.UpperCase();
      IBQuery->ParamByName("STOCK_KEY")->AsString = dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
      IBQuery->Open();
      if (!IBQuery->IsEmpty())
      {
         Application->MessageBox(("The code '" + IBQuery->FieldByName("CODE")->AsString +
                                 "' already exists for the item '" + IBQuery->FieldByName("DESCRIPTION")->AsString +
                                 "'.").c_str(), "Error", MB_ICONERROR + MB_OK);
			tsItemDetails->TabVisible = true;
			PageControl1->ActivePage = tsItemDetails;
         dbeStockCode->SetFocus();
         Success = false;
      }
   }
   return Success;
}
//---------------------------------------------------------------------------
bool TfrmAddStock::CheckPurchaseUnit()
{
   if (dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString == "")
   {
		Application->MessageBox("You must enter a stock unit.\r\r"
										"Press 'Add' to add a new unit.\r\r"
										"This is the unit you will count during a stocktake.", "Error", MB_ICONERROR + MB_OK);
		tsItemDetails->TabVisible = true;
		PageControl1->ActivePage = tsItemDetails;
		dbcbPurchaseUnit->SetFocus();
		return false;
   }
   else return true;
}
//---------------------------------------------------------------------------
bool TfrmAddStock::CheckOrderUnits()
{
	bool FoundDefaultSupplier = false;
	qrSuppliers->Close();
	qrSuppliers->Open();
	for (qrSuppliers->First(); !qrSuppliers->Eof; qrSuppliers->Next())
	{
		for (dtSuppliersStock->First(); !dtSuppliersStock->Eof; dtSuppliersStock->Next())
		{
			if (dtSuppliersStock->FieldByName("Qty")->AsFloat == 0)
			{
				tsSuppliers->TabVisible = true;
				PageControl1->ActivePage = tsSuppliers;
				dbcOrderQty->SetFocus();
				Application->MessageBox("You must enter a stock quantity.", "Error", MB_ICONERROR + MB_OK);
				return false;
			}
			if (dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString == "T")
			{
				FoundDefaultSupplier = true;
			}
			if (dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString == "")
			{
				dtSuppliersStock->Edit();
				dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString = "F";
				dtSuppliersStock->Post();
			}
		}
	}
   /*	 if (!FoundDefaultSupplier && qrSuppliers->RecordCount > 0)
	{
		qrSuppliers->First();
		dtSuppliersStock->First();
		tsSuppliers->TabVisible = true;
		PageControl1->ActivePage = tsSuppliers;
		dbgSuppliers->SetFocus();
		Application->MessageBox("You must select a default supplier.", "Error", MB_ICONERROR + MB_OK);
		return false;
	}  */
	return true;
}
//---------------------------------------------------------------------------
/*bool TfrmAddStock::CheckSalesUnit()
{
   if (dtStockEdit->FieldByName("ITEM_SIZE")->AsFloat == 0)
   {
      Application->MessageBox("You must enter a stock item size.\r\r"
                              "If the item is a bottle, enter the size of the bottle.\r\r"
                              "If the item is bulk beer, or you sell the item as a unit (e.g. R.T.D.s), enter '1'", "Error", MB_ICONERROR + MB_OK);
		PageControl1->ActivePage = tsItemDetails;
		dbeItemSize->SetFocus();
		return false;
	}
	else if (dtStockEdit->FieldByName("SALE_UNIT")->AsString == "")
	{
		Application->MessageBox("You must enter a stock sale unit.\r\r"
										"If the item is a bottle, enter 'Bottle'.\r\r"
										"If the item is bulk beer, enter 'Litre'.\r\r"
										"If you sell the item as a unit (e.g. R.T.D.s), enter 'Unit'", "Error", MB_ICONERROR + MB_OK);
		PageControl1->ActivePage = tsItemDetails;
      dbcSaleUnit->SetFocus();
      return false;
   }
   else return true;
} */
//---------------------------------------------------------------------------
bool TfrmAddStock::CheckLocations()
{
	dtStockLocationEdit->Open();
	if (dtStockLocationEdit->State == dsEdit || dtStockLocationEdit->State == dsInsert) dtStockLocationEdit->Post();
	if (dtStockLocationEdit->RecordCount == 0)
   {
      Application->MessageBox("You must enter at least 1 location.", "Error", MB_ICONERROR + MB_OK);
		tsLocations->TabVisible = true;
		PageControl1->ActivePage = tsLocations;
		btnAddLocation->SetFocus();
		return false;
	}
	else return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnCancelClick(TObject *Sender)
{
	try
	{
		if (Transaction->InTransaction) Transaction->Rollback();
	}
	__finally
	{
		ModalResult = mrCancel;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dbeKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		SelectNext((TWinControl *)Sender, true, true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dbeKeyDown(TObject *Sender,
		WORD &Key, TShiftState Shift)
{
/*	if (Key == VK_RETURN || Key == VK_DOWN)
	{
		Key = NULL;
		SelectNext((TWinControl *)Sender, true, true);
	}
	else if (Key == VK_UP)
	{
		Key = NULL;
		SelectNext((TWinControl *)Sender, false, true);
	} */
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnAddStockGroupClick(TObject *Sender)
{
/*	AnsiString GrpName = "";
	if (InputQuery("New Stock Group", "Please enter the name of the new stock group.", CatName))
	{
		if (GrpName != "")
		{
			IBQuery->Close();
			IBQuery->SQL->Text =
				"SELECT "
					"Stock_Group "
				"FROM "
					"StockGroup "
				"ORDER BY "
					"SORT_ORDER";
			IBQuery->Open();
			if (!IBQuery->Locate("Stock_Group", CatName, TLocateOptions() << loCaseInsensitive))
			{
				qrAddStockGroup->ParamByName("Stock_Group")->AsString = GrpName;

				sqlAddLK->Close();
				sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_Stock_Group, 1) FROM RDB$DATABASE";
				sqlAddLK->ExecQuery();

				qrAddStockGroup->ParamByName("SORT_ORDER")->AsString = sqlAddLK->Fields[0]->AsInteger;
				sqlAddLK->Close();

				qrAddStockGroup->ExecSQL();

				IBQuery->Close();
				IBQuery->Open();
			}
			else
			{
				GrpName = IBQuery->FieldByName("Stock_Group")->AsString;
			}
			StockGroup = GrpName;
			dbcbStockGroup->Items->Clear();
			while (!IBQuery->Eof)
			{
				dbcbStockGroup->Items->Add(IBQuery->FieldByName("Stock_Group")->AsString);
				IBQuery->Next();
			}
			IBQuery->Close();
			dbcbStockGroup->ItemIndex = dbcbStockGroup->Items->IndexOf(StockGroup);
			dtStockEdit->FieldByName("Stock_Group")->AsString = StockGroup;
      }
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dbeMaxLevelKeyPress(TObject *Sender,
      char &Key)
{

	if (Key == VK_RETURN)
   {
		btnNext2->SetFocus();
	}


}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::FormResize(TObject *Sender)
{
   ClientHeight						= btnCancel->Top + btnCancel->Height + PageControl1->Top;
   ClientWidth							= PageControl1->Width + (PageControl1->Left * 2);
   Left									= (Screen->Width - Width) / 2;
   Top									= (Screen->Height - Height) / 2;
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnAddPurchaseUnitClick(TObject *Sender)
{
	AnsiString UnitName = "";
	if (InputQuery("New Unit", "Please enter the name of the new unit.", UnitName))
	{
		if (UnitName != "")
		{
			bool Found = false;
			for (int i=0; i<dbcbPurchaseUnit->Items->Count; i++)
			{
				if (UnitName.UpperCase() == dbcbPurchaseUnit->Items->Strings[i].UpperCase())
				{
					dbcbPurchaseUnit->ItemIndex = i;
					dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString = dbcbPurchaseUnit->Items->Strings[i];
					Found = true;
					break;
				}
			}
			if (!Found)
			{
				dbcbPurchaseUnit->ItemIndex = dbcbPurchaseUnit->Items->Add(UnitName);
				dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString = UnitName;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnAddSalesUnitClick(TObject *Sender)
{
	AnsiString UnitName = "";
	if (InputQuery("New Unit", "Please enter the name of the new unit.", UnitName))
	{
		if (UnitName != "")
		{
			bool Found = false;
			for (int i=0; i<dbcSaleUnit->Items->Count; i++)
			{
				if (UnitName.UpperCase() == dbcSaleUnit->Items->Strings[i].UpperCase())
				{
					dbcSaleUnit->ItemIndex = i;
					dtStockEdit->FieldByName("SALE_UNIT")->AsString = dbcSaleUnit->Items->Strings[i];
					Found = true;
					break;
				}
			}
			if (!Found)
			{
				dbcSaleUnit->ItemIndex = dbcSaleUnit->Items->Add(UnitName);
				dtStockEdit->FieldByName("SALE_UNIT")->AsString = UnitName;
			}
		}
	}
}
//---------------------------------------------------------------------------
/*void __fastcall TfrmAddStock::dbcbLocationKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if (!dbcbLocation->DroppedDown)
	{
		if (Key == VK_DOWN)
		{
			dbcbLocation->DroppedDown = true;
			Key = NULL;
		}
		else if (Key == VK_RIGHT)
		{
			btnAddLocation->SetFocus();
			Key = NULL;
		}
	}
}  */
//---------------------------------------------------------------------------
/*void __fastcall TfrmAddStock::dbcbLocationKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN && !dbcbLocation->DroppedDown)
	{
		dbcbStockGroup->SetFocus();
	}
} */
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dbcbPurchaseUnitKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if (!dbcbPurchaseUnit->DroppedDown)
	{
		if (Key == VK_DOWN)
		{
			dbcbPurchaseUnit->DroppedDown = true;
			Key = NULL;
		}
		else if (Key == VK_RIGHT)
		{
			btnAddPurchaseUnit->SetFocus();
			Key = NULL;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dbcbPurchaseUnitKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN && !dbcbPurchaseUnit->DroppedDown)
	{
		dbeGST->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::cbStockGroupKeyDown(TObject *Sender,
		WORD &Key, TShiftState Shift)
{
	if (!cbStockGroup->DroppedDown)
	{
		if (Key == VK_DOWN)
		{
			cbStockGroup->DroppedDown = true;
			Key = NULL;
		}
		else if (Key == VK_RIGHT)
		{
			btnAddStockGroup->SetFocus();
			Key = NULL;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::cbStockGroupKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN && !cbStockGroup->DroppedDown)
	{
		Key = NULL;
		dbcbPurchaseUnit->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::cbStockGroupChange(TObject *Sender)
{
	StockGroupKey = int(cbStockGroup->Items->Objects[cbStockGroup->ItemIndex]);
	dtStockEdit->FieldByName("Stock_Group_Key")->AsInteger = StockGroupKey;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnAddSupplierClick(TObject *Sender)
{
	if (dtSuppliersStock->State == dsEdit || dtSuppliersStock->State == dsInsert)
	{
		 if(dbgSuppliers->DataSource->DataSet->RecordCount > 0)
        {
		    dtSuppliersStock->Post();
        }
	}
	frmSelectSupplier->Reset = true;
	if (frmSelectSupplier->ShowModal() == mrOk)
	{
		sqlFindSupplier->Close();
		sqlFindSupplier->ParamByName("SUPPLIER_KEY")->AsString	= frmSelectSupplier->ContactKey;
		sqlFindSupplier->ParamByName("STOCK_KEY")->AsInteger		= dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
		sqlFindSupplier->ExecQuery();
		if (sqlFindSupplier->Fields[0]->AsInteger == 0)
		{
			qrSupplierName->Close();
			qrSupplierName->ParamByName("Supplier_Key")->AsInteger = frmSelectSupplier->ContactKey;
			qrSupplierName->ExecQuery();
			frmAddSupplierUnit->SupplierName = qrSupplierName->FieldByName("Company_Name")->AsString;
			frmAddSupplierUnit->ItemName = dtStockEdit->FieldByName("Description")->AsString;
			qrSupplierName->Close();
			if (frmAddSupplierUnit->ShowModal() == mrOk)
			{
				sqlAddLK->Close();
				sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_SUPPLIERSTOCK, 1) FROM RDB$DATABASE";
				sqlAddLK->ExecQuery();

				qrAddSupplier->Close();
				qrAddSupplier->ParamByName("Supplier_Stock_Key")->AsInteger		= sqlAddLK->Fields[0]->AsInteger;
				qrAddSupplier->ParamByName("Stock_Key")->AsInteger					= dtStockEdit->FieldByName("Stock_Key")->AsInteger;
				qrAddSupplier->ParamByName("Supplier_Key")->AsInteger				= frmSelectSupplier->ContactKey;
				qrAddSupplier->ParamByName("Supplier_Unit")->AsString				= frmAddSupplierUnit->cbUnits->Text;
//				if (dtSuppliersStock->RecordCount == 0)
//				{
					qrAddSupplier->ParamByName("Default_Order_Unit")->AsString	= "T"; // First supplier unit
//				}
//				else
//				{
//					qrAddSupplier->ParamByName("Default_Order_Unit")->AsString	= "F";
//				}
				if (qrSuppliers->RecordCount == 0)	// Adding the first supplier. Make it the preferred.
				{
					qrAddSupplier->ParamByName("Preferred_Supplier")->AsString	= "T";
				}
				else
				{
					qrAddSupplier->ParamByName("Preferred_Supplier")->AsString	= "F";
				}
				qrAddSupplier->ExecSQL();
				qrAddSupplier->Close();

				dtSuppliersStock->Close();
				qrSuppliers->Close();

				qrSuppliers->Open();
				qrSuppliers->Locate("SUPPLIER_KEY", frmSelectSupplier->ContactKey, TLocateOptions());

				dtSuppliersStock->Open();
				dtSuppliersStock->Locate("Supplier_Unit", frmAddSupplierUnit->cbUnits->Text, TLocateOptions());
				ResizeGrids();
				dbcOrderQty->SetFocus();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnAddOrderUnitClick(TObject *Sender)
{
	if (qrSuppliers->RecordCount == 0)
	{
                btnAddSupplierClick(NULL);
		return;
	}
	if (dtSuppliersStock->State == dsEdit || dtSuppliersStock->State == dsInsert)
	{
		dtSuppliersStock->Post();
	}
	frmAddSupplierUnit->SupplierName = qrSuppliers->FieldByName("Supplier_Name")->AsString;
	frmAddSupplierUnit->ItemName = dtStockEdit->FieldByName("Description")->AsString;
	if (frmAddSupplierUnit->ShowModal() == mrOk)
	{
		if (dtSuppliersStock->Locate("Supplier_Unit", frmAddSupplierUnit->cbUnits->Text, TLocateOptions() << loCaseInsensitive))
		{
			Application->MessageBox("This supplier unit already exists.", "Warning", MB_ICONWARNING + MB_OK);
		}
		else
		{
			bool PreferredSupplier														= dtSuppliersStock->FieldByName("Preferred_Supplier")->AsBoolean;
			dtSuppliersStock->Append();
			dtSuppliersStock->FieldByName("Stock_Key")->AsInteger				= qrSuppliers->FieldByName("Stock_Key")->AsInteger;
			dtSuppliersStock->FieldByName("Supplier_Key")->AsInteger			= qrSuppliers->FieldByName("Supplier_Key")->AsInteger;
			dtSuppliersStock->FieldByName("Supplier_Unit")->AsString			= frmAddSupplierUnit->cbUnits->Text;
			dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString	= PreferredSupplier?"T":"F";
			dtSuppliersStock->FieldByName("Default_Order_Unit")->AsString	= "F";	// Must already be one.
			dtSuppliersStock->FieldByName("Min_Order_Qty")->AsFloat			= 1;
			dtSuppliersStock->Post();
			dtSuppliersStock->Locate("Supplier_Unit", frmAddSupplierUnit->cbUnits->Text, TLocateOptions());
		}
		dbcOrderQty->SetFocus();
		ResizeGrids();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnDeleteOrderUnitClick(TObject *Sender)
{
	if (dtSuppliersStock->RecordCount > 0)
	{
		if (Application->MessageBox("Are you sure?", "Question", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
		{
			dtSuppliersStock->Delete();

			int OldSupplierKey = qrSuppliers->FieldByName("Supplier_Key")->AsInteger;

			dtSuppliersStock->Close();
			qrSuppliers->Close();
			qrSuppliers->Open();
			qrSuppliers->Locate("Supplier_Key", OldSupplierKey, TLocateOptions());
			dtSuppliersStock->Open();
			ResizeGrids();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnRemoveSupplierClick(TObject *Sender)
{
	if (qrSuppliers->RecordCount > 0)
	{
		if (Application->MessageBox("Are you sure?", "Question", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
		{
			qrRemoveSupplier->Close();
			qrRemoveSupplier->ParamByName("SUPPLIER_KEY")->AsInteger	= qrSuppliers->FieldByName("SUPPLIER_KEY")->AsInteger;
			qrRemoveSupplier->ParamByName("STOCK_KEY")->AsInteger		= dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
			qrRemoveSupplier->ExecSQL();

			dtSuppliersStock->Close();
			qrSuppliers->Close();
			qrSuppliers->Open();
			dtSuppliersStock->Open();
			if (qrSuppliers->Eof)
			{
				chbPreferredSupplier->Checked = false;
			}
			if (dtSuppliersStock->Eof)
			{
				chbDefaultOrderUnit->Checked = false;
			}

			ResizeGrids();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnAddLocationClick(TObject *Sender)
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
		dtStockLocationEdit->Open();
		if (dtStockLocationEdit->State == dsEdit || dtStockLocationEdit->State == dsInsert)
		{
			dtStockLocationEdit->Post();
		}
		if (dtStockLocationEdit->Locate("LOCATION", LocationName, TLocateOptions() << loCaseInsensitive))
		{
			Application->MessageBox("This location already exists.", "Warning", MB_ICONWARNING + MB_OK);
			dtStockLocationEdit->Edit();
		}
		else
		{
			bool DefaultLocation = dtStockLocationEdit->Eof;
			dtStockLocationEdit->BeforeInsert = NULL;
			try
			{
				dtStockLocationEdit->Insert();
			}
			__finally
			{
				dtStockLocationEdit->BeforeInsert = dtStockLocationEditBeforeInsert;
			}
			dtStockLocationEdit->FieldByName("STOCK_KEY")->AsInteger			= dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
			dtStockLocationEdit->FieldByName("LOCATION")->AsString			= LocationName;

			dtStockLocationEdit->FieldByName("ON_HAND")->AsFloat				= 0;
//			dtStockLocationEdit->FieldByName("ON_ORDER")->AsFloat				= 0;
			dtStockLocationEdit->FieldByName("MIN_LEVEL")->AsFloat			= 0;
			dtStockLocationEdit->FieldByName("MAX_LEVEL")->AsFloat			= 0;
			dtStockLocationEdit->FieldByName("DEFAULT_LOCATION")->AsString	= DefaultLocation?"T":"F";

			// Make sure they appear in the grid in order!
			dtStockLocationEdit->Post();
			dtStockLocationEdit->Close();
			dtStockLocationEdit->Open();
			dtStockLocationEdit->Locate("LOCATION", LocationName, TLocateOptions() << loCaseInsensitive);
			dtStockLocationEdit->Edit();
		}
		ResizeGrids();
		dbeMinLevel->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnDeleteLocationClick(TObject *Sender)
{
	if (!dtStockLocationEdit->IsEmpty())
   {
		if (Application->MessageBox("Are you sure?", "Question", MB_ICONQUESTION + MB_OKCANCEL) == ID_OK)
   	{
      	dtStockLocationEdit->Delete();
      	ResizeGrids();
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnNextCodeClick(TObject *Sender)
{
/*	sqlAddLK->Close();
   sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_STOCKCODE, 1) FROM RDB$DATABASE";
   sqlAddLK->ExecQuery();
   dtStockEdit->FieldByName("CODE")->AsString = sqlAddLK->Fields[0]->AsInteger;
	sqlAddLK->Close();
   dbeDescription->SetFocus();*/

	dtStockEdit->DisableControls();
	int NextCode = 0;
	do
	{
		sqlAddLK->Close();
		sqlAddLK->SQL->Text = "SELECT GEN_ID(GEN_STOCKCODE, 1) FROM RDB$DATABASE";
		sqlAddLK->ExecQuery();
		NextCode = sqlAddLK->Fields[0]->AsInteger;

		sqlAddLK->Close();
		sqlAddLK->SQL->Text = "Select Code From Stock Where Code = :Code";
		sqlAddLK->ParamByName("Code")->AsString = IntToStr(NextCode);
		sqlAddLK->ExecQuery();
	}
	while (!sqlAddLK->Eof);
	sqlAddLK->Close();

	if (NextCode != 0)
	{
		dtStockEdit->FieldByName("CODE")->AsString = NextCode;
	}
	dtStockEdit->EnableControls();
	dbeDescription->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::PageControl1Changing(TObject *Sender,
		bool &AllowChange)
{
/*	if (Mode == amAddSupplierUnit || Mode == amAddLocation)
	{
		AllowChange = false;
	}
	else if (PageControl1->ActivePage == tsItemDetails)
   {
		btnP1Next->SetFocus();
      AllowChange = CheckDetails();

		if (AllowChange)
		{
         if (rbUnitClass->Checked)
         {
            if (dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString == "")
            {
               dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString = "Unit";
					if (dbcbPurchaseUnit->Items->IndexOf("Unit") == -1)
               {
						dbcbPurchaseUnit->Items->Add("Unit");
               }
            }
				if (dtStockEdit->FieldByName("ITEM_SIZE")->AsFloat == 0)
            {
               dtStockEdit->FieldByName("ITEM_SIZE")->AsFloat = 1;
            }
            if (dtStockEdit->FieldByName("SALE_UNIT")->AsString == "")
            {
               dtStockEdit->FieldByName("SALE_UNIT")->AsString = "Unit";
				}
			}
         else if (rbBottleClass->Checked)
         {
            if (dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString == "")
            {
					dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString = "Bottle";
					if (dbcbPurchaseUnit->Items->IndexOf("Bottle") == -1)
               {
						dbcbPurchaseUnit->Items->Add("Bottle");
               }
            }
				if (dtStockEdit->FieldByName("ITEM_SIZE")->AsFloat == 0)
            {
               dtStockEdit->FieldByName("ITEM_SIZE")->AsFloat = StrToInt(cbBottleSize->Text);
            }
				if (dtStockEdit->FieldByName("SALE_UNIT")->AsString == "")
            {
					dtStockEdit->FieldByName("SALE_UNIT")->AsString = "mls";
				}
			}
         else if (rbBulkClass->Checked)
         {
				if (dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString == "")
            {
               dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString = "Litre";
					if (dbcbPurchaseUnit->Items->IndexOf("Litre") == -1)
               {
						dbcbPurchaseUnit->Items->Add("Litre");
               }
            }
            if (dtStockEdit->FieldByName("ITEM_SIZE")->AsFloat == 0)
				{
					dtStockEdit->FieldByName("ITEM_SIZE")->AsFloat = 1;
            }
            if (dtStockEdit->FieldByName("SALE_UNIT")->AsString == "")
            {
					dtStockEdit->FieldByName("SALE_UNIT")->AsString = "Litre";
            }
         }
      }
   }
	else if (PageControl1->ActivePage == tsPurchaseUnits)
	{
      btnP2Next->SetFocus();
      AllowChange = CheckPurchaseUnit();
	}
   else if (PageControl1->ActivePage == tsSalesUnits)
   {
      btnP4Next->SetFocus();
      AllowChange = CheckSalesUnit();
	}
   else if (PageControl1->ActivePage == tsLocations)
   {
      btnP5Next->SetFocus();
		AllowChange = CheckLocations();
   }
   else if (PageControl1->ActivePage == tsSuppliers)
   {
		btnP6Previous->SetFocus();
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::PageControl1Change(TObject *Sender)
{
/*	if (PageControl1->ActivePage == tsPurchaseUnits)
	{
		dbcbPurchaseUnit->ItemIndex = dbcbPurchaseUnit->Items->IndexOf(dtStockEdit->FieldByName("STOCKTAKE_UNIT")->AsString);
	}
	else if (PageControl1->ActivePage == tsSalesUnits)
	{
		dbcSaleUnit->ItemIndex = dbcSaleUnit->Items->IndexOf(dtStockEdit->FieldByName("SALE_UNIT")->AsString);
		if (dbcSaleUnit->ItemIndex == -1)
		{
			dbcSaleUnit->ItemIndex = dbcSaleUnit->Items->Add(dtStockEdit->FieldByName("SALE_UNIT")->AsString);
		}
	}         */
	if (PageControl1->ActivePage == tsBarcodes)
	{
		dtItemBarcode->Close();
		dtItemBarcode->ParamByName("STOCK_KEY")->AsInteger = dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
		dtItemBarcode->Open();
		dtSupplierBarcode->Close();
		dtSupplierBarcode->ParamByName("STOCK_KEY")->AsInteger = dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
		dtSupplierBarcode->Open();
		ResizeBarcodeGrids();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::qrSuppliersAfterScroll(TDataSet *DataSet)
{
	ResizeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dtSuppliersStockAfterOpen(TDataSet *DataSet)
{
	((TFloatField *)dtSuppliersStock->FieldByName("LATEST_COST"))->currency = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dtStockLocationEditBeforeInsert(
      TDataSet *DataSet)
{
	// Disallow inserts. Can't add new location without pressing 'Add' button or copying
	// or creating default location.
	Abort();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::chbDefaultOrderUnitClick(TObject *Sender)
{
	int CurrentKey = dtSuppliersStock->FieldByName("Supplier_Stock_Key")->AsInteger;
	dtSuppliersStock->DisableControls();
	try
	{
		for (dtSuppliersStock->First(); !dtSuppliersStock->Eof; dtSuppliersStock->Next())
		{
			dtSuppliersStock->Edit();
			if (!chbDefaultOrderUnit->Checked && dtSuppliersStock->FieldByName("Supplier_Stock_Key")->AsInteger == CurrentKey)
			{
				dtSuppliersStock->FieldByName("Default_Order_Unit")->AsString = "T";
			}
			else
			{
				dtSuppliersStock->FieldByName("Default_Order_Unit")->AsString = "F";
			}
			dtSuppliersStock->Post();
		}
		dtSuppliersStock->Locate("Supplier_Stock_Key", CurrentKey, TLocateOptions());
	}
	__finally
	{
		dtSuppliersStock->EnableControls();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dtSuppliersStockAfterScroll(
      TDataSet *DataSet)
{
	try
	{
		chbDefaultOrderUnit->OnClick = NULL;
		chbDefaultOrderUnit->Checked = (dtSuppliersStock->FieldByName("Default_Order_Unit")->AsString == "T");
	}
	__finally
	{
		chbDefaultOrderUnit->OnClick = chbDefaultOrderUnitClick;
	}
	try
	{
		chbPreferredSupplier->OnClick = NULL;
		chbPreferredSupplier->Checked = (dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString == "T");
	}
	__finally
	{
		chbPreferredSupplier->OnClick = chbPreferredSupplierClick;
	}
	Label32->Caption = dtSuppliersStock->FieldByName("Supplier_Unit")->AsString;
	Label41->Caption = dtSuppliersStock->FieldByName("Supplier_Unit")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::chbPreferredSupplierClick(TObject *Sender)
{
	int CurrentKey = dtSuppliersStock->FieldByName("Supplier_Stock_Key")->AsInteger;
	dtSuppliersStock->DisableControls();
	try
	{
		qrClearPreferredSupplier->Close();
		qrClearPreferredSupplier->ParamByName("Stock_Key")->AsInteger = dtStockEdit->FieldByName("Stock_Key")->AsInteger;
		qrClearPreferredSupplier->ExecQuery();

		dtSuppliersStock->Close();	// Refresh this dataset
		dtSuppliersStock->AfterScroll = NULL;
		try
		{
			for (dtSuppliersStock->Open(); !dtSuppliersStock->Eof; dtSuppliersStock->Next())
			{
				dtSuppliersStock->Edit();
				if (chbPreferredSupplier->Checked)
				{
					dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString = "T";
				}
				else
				{
					dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString = "F";
				}
				dtSuppliersStock->Post();
			}
		}
		__finally
		{
			dtSuppliersStock->AfterScroll = dtSuppliersStockAfterScroll;
		}
		dtSuppliersStock->Locate("Supplier_Stock_Key", CurrentKey, TLocateOptions());
	}
	__finally
	{
		dtSuppliersStock->EnableControls();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnNext1Click(TObject *Sender)
{
	PageControl1->ActivePage = PageControl1->FindNextPage(PageControl1->ActivePage, true, true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnPrev1Click(TObject *Sender)
{
	PageControl1->ActivePage = PageControl1->FindNextPage(PageControl1->ActivePage, false, true);
}
//---------------------------------------------------------------------------
/*void __fastcall TfrmAddStock::btnScanItemClick(TObject *Sender)
{
	if (frmGetBarcode->ShowModal() == mrOk)
	{
		dtStockEdit->FieldByName("Barcode")->AsString = frmGetBarcode->Barcode;
	}
	SelectNext((TWinControl *)Sender, true, true);
} */
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::chbDefaultLocationClick(TObject *Sender)
{
//	int CurrentKey = dtStockLocationEdit->FieldByName("Stock_Key")->AsInteger;
	AnsiString CurrentLocation = dtStockLocationEdit->FieldByName("Location")->AsString;
	dtStockLocationEdit->DisableControls();
	try
	{
//		qrClearDefaultLocation->Close();
//		qrClearDefaultLocation->ParamByName("Stock_Key")->AsInteger = dtStockEdit->FieldByName("Stock_Key")->AsInteger;
//		qrClearDefaultLocation->ExecQuery();
		dtStockLocationEdit->AfterScroll = NULL;
		try
		{
			dtStockLocationEdit->Close();	// Refresh this dataset
			for (dtStockLocationEdit->Open(); !dtStockLocationEdit->Eof; dtStockLocationEdit->Next())
			{
				dtStockLocationEdit->Edit();
				if (dtStockLocationEdit->FieldByName("Location")->AsString == CurrentLocation)
				{
					dtStockLocationEdit->FieldByName("Default_Location")->AsString = "T";
				}
				else
				{
					dtStockLocationEdit->FieldByName("Default_Location")->AsString = "F";
				}
				dtStockLocationEdit->Post();
			}
		}
		__finally
		{
			dtStockLocationEdit->AfterScroll = dtStockLocationEditAfterScroll;
		}
		dtStockLocationEdit->Locate("Location", CurrentLocation, TLocateOptions());
	}
	__finally
	{
		dtStockLocationEdit->EnableControls();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dtStockLocationEditAfterScroll(
      TDataSet *DataSet)
{
	try
	{
		chbDefaultLocation->OnClick = NULL;
		chbDefaultLocation->Checked = (dtStockLocationEdit->FieldByName("Default_Location")->AsString == "T");
	}
	__finally
	{
		chbDefaultLocation->OnClick = chbDefaultLocationClick;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dtSuppliersStockBeforePost(TDataSet *DataSet)
{
	if (dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString == "")
	{
		dtSuppliersStock->FieldByName("Preferred_Supplier")->AsString = "F";
	}
	if (dtSuppliersStock->FieldByName("Default_Order_Unit")->AsString == "")
	{
		dtSuppliersStock->FieldByName("Default_Order_Unit")->AsString = "F";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dbeStockCodeKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		if (dbeStockCode->Text == "")
		{
			btnNextCode->SetFocus();
		}
		else
		{
			dbeDescription->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------
/*void __fastcall TfrmAddStock::dbeBarcodeKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		if (dbeBarcode->Text == "")
		{
			btnScanItem->SetFocus();
		}
		else
		{
			dbcEnabled->SetFocus();
		}
	}
} */
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::chbDefaultOrderUnitKeyPress(TObject *Sender,
		char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		btnOk->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnAddItemBarcodeClick(TObject *Sender)
{
	AnsiString Barcode = "";
	if (InputQuery("New Barcode", "Please enter the new Barcode.", Barcode))
	{
		if (((TButton *)Sender)->Name == "btnAddItemBarcode")
			AddItemBarcode(Barcode);
		else
			AddSupplierBarcode(Barcode);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnScanItemBarcodeClick(TObject *Sender)
{
	if (frmGetBarcode->ShowModal() == mrOk)
	{
		if (((TBitBtn *)Sender)->Name == "btnScanItemBarcode")
			AddItemBarcode(frmGetBarcode->Barcode);
		else
			AddSupplierBarcode(frmGetBarcode->Barcode);
	}
}
//---------------------------------------------------------------------------
bool TfrmAddStock::CheckBarcodeBeforeInsert( AnsiString Barcode )
{
    //AnsiString e(":");

	// Barcode can only appear once
	sqlBarcode->Close();
	// Barcode belongs to another Stock.StockKey Key
	sqlBarcode->SQL->Text = "Select Barcode.Stock_Key, Barcode.Barcode, Stock.Description "
									"from BARCODE, Stock where "
									"  BarCode.STOCK_KEY = Stock.Stock_Key "
									"  and Barcode.Barcode = :Barcode";
	sqlBarcode->ParamByName("Barcode")->AsString  = Barcode;
	sqlBarcode->ExecQuery();
	if (!sqlBarcode->Eof)
	{
		Application->MessageBox(("This Barcode is already used by Item '" + sqlBarcode->FieldByName("Description")->AsString + "'" ).c_str(), "Error", MB_ICONERROR + MB_OK);
		sqlBarcode->Close();
		return false;
	}
	sqlBarcode->Close();
	// Barcode belongs to another SupplierStock.Supplier_Stock_Key Key
	sqlBarcode->SQL->Text = "Select SupplierStock.Supplier_Unit, Barcode.Barcode, Stock.Description, Contact.Company_Name from "
									"BARCODE Join SupplierStock On "
									"   BarCode.Supplier_Stock_Key = SupplierStock.Supplier_Stock_Key "
									"join Stock On "
									"   SupplierStock.Stock_Key = Stock.Stock_Key "
									"join Contact On "
									"   SupplierStock.Supplier_Key = Contact.Contact_LK "
									"where Barcode.Barcode = :Barcode ";
	sqlBarcode->ParamByName("Barcode")->AsString  = Barcode;
	sqlBarcode->ExecQuery();
	if (!sqlBarcode->Eof)
	{
		Application->MessageBox(("This Barcode is already used by Item '" + sqlBarcode->FieldByName("Description")->AsString + "'\r"
										 "and Supplier/Unit '" + sqlBarcode->FieldByName("Company_Name")->AsString + "/"  + sqlBarcode->FieldByName("Supplier_Unit")->AsString + "'").c_str(), "Error", MB_ICONERROR + MB_OK);
		sqlBarcode->Close();
		return false;
	}
	sqlBarcode->Close();
	return true;
}
//---------------------------------------------------------------------------
void TfrmAddStock::AddItemBarcode( AnsiString Barcode )
{
	if ( !CheckBarcodeBeforeInsert(Barcode) )
	{
		return;
	}
	dtItemBarcode->Append();
	dtItemBarcode->FieldByName("STOCK_KEY")->AsInteger          = dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
	dtItemBarcode->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger = -1;
	dtItemBarcode->FieldByName("BARCODE")->AsString             = Barcode;
	dtItemBarcode->Post();
	// Refresh
	dtItemBarcode->Close();
	dtItemBarcode->Open();
	ResizeBarcodeGrids();
}
//---------------------------------------------------------------------------
void TfrmAddStock::AddSupplierBarcode( AnsiString Barcode )
{
	if ( !CheckBarcodeBeforeInsert(Barcode))
	{
		return;
	}
	qrSupplierUnits->Close();
	qrSupplierUnits->ParamByName("STOCK_KEY")->AsInteger = dtStockEdit->FieldByName("STOCK_KEY")->AsInteger;
	qrSupplierUnits->Open();
	qrSupplierUnits->FetchAll();
	if (qrSupplierUnits->RecordCount == 0)
	{
		Application->MessageBox("There are no Suppliers added for this item.", "Error", MB_ICONERROR + MB_OK);
		qrSupplierUnits->Close();
		return;
	}
	else
	{
		// show the AddSupplierBarCode Form if > 1, if qty = 1 then just add it
		if (qrSupplierUnits->RecordCount > 1)
		{
			std::auto_ptr<TfrmAddSupplierBarcode>frmAddSupplierBarcode(new TfrmAddSupplierBarcode(NULL));
			frmAddSupplierBarcode->SetDataSet(qrSupplierUnits);
			if (frmAddSupplierBarcode->ShowModal() == mrCancel)
			{
				qrSupplierUnits->Close();
				return;
			}
		}
	}
	dtSupplierBarcode->Append();
	dtSupplierBarcode->FieldByName("STOCK_KEY")->AsInteger          = -1;
	dtSupplierBarcode->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger = qrSupplierUnits->FieldByName("SUPPLIER_STOCK_KEY")->AsInteger;
	dtSupplierBarcode->FieldByName("BARCODE")->AsString             = Barcode;
	dtSupplierBarcode->Post();
	// Refresh
	dtSupplierBarcode->Close();
	dtSupplierBarcode->Open();
	qrSupplierUnits->Close();
	ResizeBarcodeGrids();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnDeleteItemBarcodeClick(TObject *Sender)
{
    AnsiString cachedBarcode =
      dtItemBarcode->FieldByName("barcode")->AsString;
    AnsiString warningMessage =
      "Do you wish to delete Barcode " + cachedBarcode;

	if (Application->MessageBox(warningMessage.c_str(),
                                "Warning",
                                MB_ICONWARNING + MB_YESNO) == ID_YES)
	{
        /* Delete the record for real ... */
        TIBDatabase *d = dmStockData->dbStock;
        std::auto_ptr<TIBTransaction> t(new TIBTransaction(NULL));
        std::auto_ptr<TIBQuery> q(new TIBQuery(NULL));

        t->AddDatabase(d);
        q->Database = d;
        q->Transaction = t.get();

        q->SQL->Add("delete from barcode "
                    "where barcode = :barcode and stock_key = :stock_key;");
        q->ParamByName("barcode")->AsString = cachedBarcode;

        /* I'm relying on the fact that STOCK_KEYs in the
           Barcode table are, y'know, key-like and are unique ... */
        q->ParamByName("stock_key")->AsString =
          dtItemBarcode->FieldByName("stock_key")->AsString;

        t->StartTransaction();
        q->ExecSQL();
        t->Commit();

        /* Continue to remove it from the dataset; I'm not sure
           what other code depended on it being erased ... */
		dtItemBarcode->Delete();
		ResizeBarcodeGrids();
        /* Note that removing something from a TIBDataSet __doesn't__
           actually remove it from the database that provides the
           records?! */
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::btnDeleteSupplierBarcodeClick(TObject *Sender)
{
	if (Application->MessageBox(("Do you wish to delete Barcode " + dtSupplierBarcode->FieldByName("BARCODE")->AsString).c_str(), "Warning", MB_ICONWARNING + MB_YESNO) == ID_YES)
	{
		dtSupplierBarcode->Delete();
		ResizeBarcodeGrids();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dbeGSTExit(TObject *Sender)
{
    if (dbeGST->Text.IsEmpty()
        || StrToFloatDef(dbeGST->Text,
                         -1000.0) < 0)
        dbeGST->Text = "15.0";
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dsSuppliersStockDataChange(TObject *Sender, TField *Field)
{
    //We need to populate the Supplier unit cost text box, by calculation
    //Supplier Unit Cost = CostExc/StockQty

    if(Field)
    {
        AnsiString fieldName = Field->DisplayName;
        if(fieldName == "QTY" || fieldName == "LATEST_COST")
        {
            float qty = dtSuppliersStock->FieldByName("QTY")->AsFloat;
            float cost = dtSuppliersStock->FieldByName("LATEST_COST")->AsFloat;

            if(cost > 0 && qty > 0)
            {
                //float supplierCost = cost/qty;
                AnsiString supplierCost = cost/qty;
                if(supplierCost.Pos(".") != 0)
                {
                    if(supplierCost.Length() - supplierCost.Pos(".") > CurrentConnection.SettingDecimalPlaces )
                       supplierCost = supplierCost.SubString(0,supplierCost.Pos(".") + CurrentConnection.SettingDecimalPlaces);
                }
                dbeSupplierCost->Text = supplierCost;
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::cbStockSubGroupKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
      	if (!cbStockSubGroup->DroppedDown)
	{
		if (Key == VK_DOWN)
		{
			cbStockSubGroup->DroppedDown = true;
			Key = NULL;
		}
		else if (Key == VK_RIGHT)
		{
			cbStockSubGroup->SetFocus();
			Key = NULL;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::cbStockSubGroupKeyPress(TObject *Sender,
      char &Key)
{
if (Key == VK_RETURN && !cbStockSubGroup->DroppedDown)
	{
		dbeGST->SetFocus();
	}

}
//---------------------------------------------------------------------------


void __fastcall TfrmAddStock::btnAddStockSubGroupClick(TObject *Sender)
{
AnsiString SubGroupName = "";
	if (InputQuery("New Stock Sub-Group", "Please enter the name of the new Sub-Group.", SubGroupName))
	{
		if (SubGroupName != "")
		{
			bool Found = false;
			for (int i=0; i<cbStockSubGroup->Items->Count; i++)
			{
				if (SubGroupName.UpperCase() == cbStockSubGroup->Items->Strings[i].UpperCase())
				{
					cbStockSubGroup->ItemIndex = i;
					dtStockEdit->FieldByName("STOCK_SUB_GROUP")->AsString = cbStockSubGroup->Items->Strings[i];
					Found = true;
					break;
				}
			}
			if (!Found)
			{
				cbStockSubGroup->ItemIndex = cbStockSubGroup->Items->Add(SubGroupName);
				dtStockEdit->FieldByName("STOCK_SUB_GROUP")->AsString = SubGroupName;
			}
		}
	}
}
//---------------------------------------------------------------------------

bool TfrmAddStock::SubsitutePopulateData()
{
   bool IsSameStockKey=false;
int temStockKey=0;
TfrmStock* frmStock1 = new TfrmStock(NULL);
          temStockKey=0;
   frmStock1->Location = Location;
	frmStock1->StockMode = smSelect;
	frmStock1->NewItemName = NewItemName;
	if (frmStock1->ShowModal() == true)
	{
		temStockKey		= frmStock1->StockKey;
		qrStockSubstitute->SQL->Text =
			"SELECT "
				"Code,"
				"DESCRIPTION,"
				"STOCKTAKE_UNIT "
			"FROM "
				"STOCK "
			"WHERE "
				"Stock_Key = :Stock_Key";
		qrStockSubstitute->ParamByName("Stock_Key")->AsString = temStockKey;
		qrStockSubstitute->Open();
		if (!qrStockSubstitute->IsEmpty() && StockKey!=temStockKey)
		{
			SubsituteDiscription = qrStockSubstitute->FieldByName("DESCRIPTION")->AsString;
		   //	Unit			= qrStock->FieldByName("STOCKTAKE_UNIT")->AsString;
		//   StockCode	= qrStockSubstitute->FieldByName("Code")->AsString;
		   	//ModalResult = mrOk;
            IsSameStockKey=false;
		}
      else
      {
      	Application->MessageBox("You can not select the same stock for Substitute.", "Warning", MB_ICONWARNING + MB_OK);

         IsSameStockKey=true;

		   //	Unit			= "";
		   //	StockCode	= NULL;
      }
	   //	SubsituteDiscription =    Description;
		qrStockSubstitute->Close();
		SubsituteStockKey  =temStockKey;

	}
      return IsSameStockKey;
       // if(Transaction->InTransaction)
         //  Transaction->Commit();
}



void __fastcall TfrmAddStock::btnGoClick(TObject *Sender)
{
		SubsituteStockKey=0;
		SubsituteDiscription="";
	   if(!SubsitutePopulateData())
       {
		edSearch->Text =    SubsituteDiscription;
		SubstituteStockkey1=  SubsituteStockKey;
		qrSubstituteId->ParamByName("STOCK_KEY")->AsInteger = StockKey;
		qrSubstituteId->ParamByName("SUBSTITUTE_STOCK_KEY")->AsInteger = SubstituteStockkey1;
		qrSubstituteId->Open();
		int	substituteId=qrSubstituteId->FieldByName("SUBSTITUTE_KEY")->AsInteger;

  if(substituteId>0)
{
         qrpopulateSubstitute->Close();

		qrpopulateSubstitute ->ParamByName("SUBSTITUTE_KEY")->AsInteger = substituteId;
        qrpopulateSubstitute->Open();

		DBCmbsubststockitem1->ItemIndex= qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND1")->AsInteger;
		DBCmbsubsttem1->ItemIndex=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND2")->AsInteger;
		txtvariance1->Value=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND3")->AsInteger;


  }
  }                       
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::btnGo1Click(TObject *Sender)
{
		SubsituteStockKey=0;
		SubsituteDiscription="";
		 if(!SubsitutePopulateData())
       {
		edSearch1->Text =    SubsituteDiscription;
		SubstituteStockkey2=  SubsituteStockKey;
		qrSubstituteId->ParamByName("STOCK_KEY")->AsInteger = StockKey;
		qrSubstituteId->ParamByName("SUBSTITUTE_STOCK_KEY")->AsInteger = SubstituteStockkey2;
		qrSubstituteId->Open();
		int	substituteId=qrSubstituteId->FieldByName("SUBSTITUTE_KEY")->AsInteger;

  if(substituteId>0)
{
	qrpopulateSubstitute ->ParamByName("SUBSTITUTE_KEY")->AsInteger = substituteId;
	qrpopulateSubstitute->Open();
   	DBCmbsubststockitem2->ItemIndex= qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND1")->AsInteger;
	DBCmbsubsttem2->ItemIndex=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND2")->AsInteger;
	txtvariance2->Value=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND3")->AsInteger;
}
}       }
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::btnGo3Click(TObject *Sender)
{
		SubsituteStockKey=0;
		SubsituteDiscription="";
		 if(!SubsitutePopulateData())
       {
		edSearch2->Text =    SubsituteDiscription;
		SubstituteStockkey3=  SubsituteStockKey;
		qrSubstituteId->ParamByName("STOCK_KEY")->AsInteger = StockKey;
		qrSubstituteId->ParamByName("SUBSTITUTE_STOCK_KEY")->AsInteger = SubstituteStockkey3;
		qrSubstituteId->Open();
		int	substituteId=qrSubstituteId->FieldByName("SUBSTITUTE_KEY")->AsInteger;

  if(substituteId>0)
{
		qrpopulateSubstitute ->ParamByName("SUBSTITUTE_KEY")->AsInteger = substituteId;
		qrpopulateSubstitute->Open();
		DBCmbsubststockitem3->ItemIndex= qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND1")->AsInteger;
		DBCmbsubsttem3->ItemIndex=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND2")->AsInteger;
		txtvariance3->Value=  qrpopulateSubstitute->FieldByName("SUBSTITUTE_COND3")->AsInteger;
  }     }
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::chbminLevelClick(TObject *Sender)
{      qrSetMinLevel->ParamByName("STOCK_KEY")->AsInteger	= StockKey;
		if (!chbminLevel->Checked )
			{

				qrSetMinLevel->ParamByName("REVERT_TO_ORIGINAL")->AsString	= "T" ;
			}
			else
			{
				qrSetMinLevel->ParamByName("REVERT_TO_ORIGINAL")->AsString	= "F" ;;
			}
				qrSetMinLevel->ExecSQL();
		qrSetMinLevel->Close();

}

//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::btnAddReductionUnitClick(TObject *Sender)
{
   AnsiString UnitName = "";
	if (InputQuery("New Unit", "Please enter the name of the new unit.", UnitName))
	{
		if (UnitName != "")
		{
			bool Found = false;
			for (int i=0; i<dbcbReductionUnit->Items->Count; i++)
			{
				if (UnitName.UpperCase() == dbcbReductionUnit->Items->Strings[i].UpperCase())
				{
					dbcbReductionUnit->ItemIndex = i;
					dtStockEdit->FieldByName("REDUCTION_UNIT")->AsString = dbcbReductionUnit->Items->Strings[i];
					Found = true;
					break;
				}
			}
			if (!Found)
			{
				dbcbReductionUnit->ItemIndex = dbcbReductionUnit->Items->Add(UnitName);
				dtStockEdit->FieldByName("REDUCTION_UNIT")->AsString = UnitName;
			}
		}
	}
}
//---------------------------------------------------------------------------
bool TfrmAddStock::CheckReductionUnit()
{
     if(dbcbReductionUnit->ItemIndex >=0 && dbeConversionFactor->Text.Trim().Length() == 0)
      {
         Application->MessageBox("You must enter a conversion value!",
                                "Error", MB_ICONERROR + MB_OK);
        return false;
      }

    if (dbcbReductionUnit->ItemIndex >=0 &&
       (!dbeConversionFactor->Text.Length() ||
        StrToFloatDef(dbeConversionFactor->Text, -1.0) < 0.0))
       {
            Application->MessageBox("You cannot specify a negative conversion factor.",
                                    "Bad conversion factor value",
                                    MB_ICONERROR + MB_OK);
        dbeConversionFactor->Text = "0.0";
        return false;
    }
   return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dbeCostKeyPress(TObject *Sender, char &Key)
{
    if (Key == VK_RETURN)
	{
		Key = NULL;
		SelectNext((TWinControl *)Sender, true, true);
	}
    if (Key == '-')
	{
		Key = NULL;
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dbeLatestCostKeyPress(TObject *Sender,
      char &Key)
{

    if (Key == VK_RETURN)
	{
		Key = NULL;
	   	SelectNext((TWinControl *)Sender, true, true);

	}
    if (Key == '-')
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dbeAveCostKeyPress(TObject *Sender,
      char &Key)
{
    if (Key == VK_RETURN)
	{
		Key = NULL;
		SelectNext((TWinControl *)Sender, true, true);
	}
    if (Key == '-')
	{
		Key = NULL;
	}
}
void __fastcall TfrmAddStock::dbeCostChange(TObject *Sender)
{
   CheckNegativeValue(dbeCost);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddStock::dbcOrderQtyChange(TObject *Sender)
{
   CheckNegativeValue(dbcOrderQty);
}
//---------------------------------------------------------------------------
void TfrmAddStock::CheckNegativeValue(TDBEdit *dbetextbox)
{

     if(dbetextbox->Text.Trim().Length() > 0)
     {
         AnsiString cost = dbetextbox->Text.TrimLeft();
         cost = cost.SubString(0, 1);
         bool moveToNext = false;
         dbetextbox->Text = FormatForDecimalPlaces(dbetextbox->Text,moveToNext);
         if(moveToNext)
            dbetextbox->SelStart = dbetextbox->Text.Length();
            //SelectNext((TWinControl *)dbetextbox, true, true);

         if(cost == '-')
         {
            dbetextbox->Text = "";
         }
     }
}


void __fastcall TfrmAddStock::dbeLatestCostChange(TObject *Sender)
{
   CheckNegativeValue(dbeLatestCost);
}
//-----------------------------------------------
AnsiString TfrmAddStock::FormatForDecimalPlaces(AnsiString inputValue, bool &moveToNext)
{
    moveToNext = false;
    AnsiString outValue = inputValue;
    AnsiString value = inputValue;
    int originalLength = inputValue.Length();
    if(value.Pos(".") != 0)
    {
       outValue = value.SubString(0,value.Pos(".") + CurrentConnection.SettingDecimalPlaces);
       if(outValue.Length() < originalLength)
          moveToNext = true;
    }
    return outValue;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dbeAveCostChange(TObject *Sender)
{
   CheckNegativeValue(dbeAveCost);
}
//------------------------------------------------------------------------
void __fastcall TfrmAddStock::dbeMaxLevelChange(TObject *Sender)
{
     //CheckNegativeValue(dbeMaxLevel);
    bool moveToNext = false;
    dbeMaxLevel->Text = FormatForDecimalPlaces(dbeMaxLevel->Text, moveToNext);
    if(moveToNext)
    dbeMaxLevel->SelStart = dbeMaxLevel->Text.Length();
       //SelectNext((TWinControl *)((TDBEdit *)Sender), true, true);
}
//----------------------------------------------------------------

void __fastcall TfrmAddStock::dbeMinLevelChange(TObject *Sender)
{
    //CheckNegativeValue(dbeMinLevel);
    bool moveToNext = false;
    dbeMinLevel->Text = FormatForDecimalPlaces(dbeMinLevel->Text, moveToNext);
    if(moveToNext)
       dbeMinLevel->SelStart = dbeMinLevel->Text.Length();
       //SelectNext((TWinControl *)((TDBEdit *)Sender), true, true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dbeAssessedValueChange(TObject *Sender)
{
   CheckNegativeValue(dbeAssessedValue);
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dbeAssessedValueKeyPress(TObject *Sender,
      char &Key)
{
    
    if (Key == VK_RETURN)
	{
		Key = NULL;
		SelectNext((TWinControl *)Sender, true, true);
	}
    if (Key == '-')
	{
		Key = NULL;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dbeConversionFactorKeyPress(TObject *Sender,
      char &Key)
{
	if (Key == VK_RETURN)
	{
		Key = NULL;
		SelectNext((TWinControl *)Sender, true, true);
	}
    if(Key == '-')
    {
       Key = NULL;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dbeConversionFactorChange(TObject *Sender)
{
   CheckNegativeValue(dbeConversionFactor);
}
//-----------------------------------------------------------------------
void __fastcall TfrmAddStock::dbeMinOrderQtyChange(TObject *Sender)
{
    bool moveToNext = false;
    dbeMinOrderQty->Text = FormatForDecimalPlaces(dbeMinOrderQty->Text, moveToNext);
    if(moveToNext)
       dbeMinOrderQty->SelStart = dbeMinOrderQty->Text.Length();
       //SelectNext((TWinControl *)((TDBEdit *)Sender), true, true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddStock::dbeGSTChange(TObject *Sender)
{
    bool moveToNext = false;
    dbeGST->Text = FormatForDecimalPlaces(dbeGST->Text, moveToNext);
    if(moveToNext)
       dbeGST->SelStart = dbeGST->Text.Length();
       //SelectNext((TWinControl *)((TDBEdit *)Sender), true, true);
}
//---------------------------------------------------------------------------
