//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LocationRename.h"
#include "MMData.h"
#include "StockData.h"
#include "MM_StockTransactions.h"
#include <algorithm>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmUpdateLocations *frmUpdateLocations;
//---------------------------------------------------------------------------
__fastcall TfrmUpdateLocations::TfrmUpdateLocations(TComponent* Owner)
	: TForm(Owner), dbStockTransaction(dmStockData->dbStock), dbMMTransaction(dmMMData->dbMenuMate)
{
	stockListQuery		= dbStockTransaction.Query(dbStockTransaction.AddQuery());
	updateStockQuery	= dbStockTransaction.Query(dbStockTransaction.AddQuery());
	mmLocationsQuery	= dbMMTransaction.Query(dbMMTransaction.AddQuery());
	mmUpdateStock		= dbMMTransaction.Query(dbMMTransaction.AddQuery());

	stockListQuery->SQL->Text =
		"Select "
			"StockCategory.Stock_Category,"
			"StockCategory.Stock_Category_Key,"
			"StockGroup.Stock_Group,"
			"StockGroup.Stock_Group_Key,"
			"Stock.Description,"
			"Stock.Code,"
			"Stock.Stock_Key,"
			"StockLocation.Location,"
			"StockLocation.On_Hand,"
			"StockLocation.Initialised "
		"From "
			"StockCategory Inner Join StockGroup on "
				"StockCategory.Stock_Category_Key = StockGroup.Stock_Category_Key "
			"Left Join Stock on "
				"StockGroup.Stock_Group_Key = Stock.Stock_Group_Key "
			"Left Join StockLocation on "
				"Stock.Stock_Key = StockLocation.Stock_Key "
/*		"Where "
			"(StockGroup.Deleted = 'F' or StockGroup.Deleted is null) and "
			"(StockCategory.Deleted = 'F' or StockCategory.Deleted is null) and "
			"(Stock.Deleted = 'F' or Stock.Deleted is null) "*/
		"Order By "
			"StockCategory.Sort_Order,"
			"StockGroup.Sort_Order,"
			"Stock.Description,"
			"StockLocation.Location";

	mmLocationsQuery->SQL->Text =
		"Select "
			"Name "
		"From "
			"Locations "
		"Where "
			"Location_Type = 1 or Location_Type = 2 "
		"Order By "
			"Name";

	pcReportProperties->ActivePage = tsSelectItems;
	loadTree();
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::loadTree()
{
	dbStockTransaction.Start();
	try
	{
		TTreeNode *CategoryNode				= NULL;
		TTreeNode *GroupNode					= NULL;
		TTreeNode *StockNode					= NULL;

		int LastCategoryKey					= 0;
		int LastGroupKey						= 0;
		int LastStockKey						= 0;

		tvFilterTree->Items->Clear();
		stockKeyToCodeMap.clear();
		for (stockListQuery->ExecQuery(); !stockListQuery->Eof; stockListQuery->Next())
		{
			if (!CategoryNode || LastCategoryKey != stockListQuery->FieldByName("Stock_Category_Key")->AsInteger)
			{
				CategoryNode						= tvFilterTree->Items->Add(NULL, stockListQuery->FieldByName("Stock_Category")->AsString);

				CategoryNode->ImageIndex		= ICON_CLOSED_FOLDER_INDEX;
				CategoryNode->SelectedIndex	= ICON_OPEN_FOLDER_INDEX;
				CategoryNode->StateIndex		= STATE_IMAGE_SMALL_UNCHECKED;

				LastCategoryKey					= stockListQuery->FieldByName("Stock_Category_Key")->AsInteger;
			}
			if ((!GroupNode || LastGroupKey != stockListQuery->FieldByName("Stock_Group_Key")->AsInteger) &&
					!stockListQuery->FieldByName("Stock_Group_Key")->IsNull)
			{
				GroupNode							= tvFilterTree->Items->AddChild(CategoryNode, stockListQuery->FieldByName("Stock_Group")->AsString);
				GroupNode->ImageIndex			= ICON_CLOSED_FOLDER_INDEX;
				GroupNode->SelectedIndex		= ICON_OPEN_FOLDER_INDEX;
				GroupNode->StateIndex			= STATE_IMAGE_SMALL_UNCHECKED;

				LastGroupKey						= stockListQuery->FieldByName("Stock_Group_Key")->AsInteger;
			}
			if ((!StockNode || LastStockKey != stockListQuery->FieldByName("Stock_Key")->AsInteger) &&
					!stockListQuery->FieldByName("Stock_Key")->IsNull)
			{
				StockNode							= tvFilterTree->Items->AddChild(GroupNode, stockListQuery->FieldByName("Description")->AsString);
				StockNode->ImageIndex			= ICON_BOX_INDEX;
				StockNode->SelectedIndex		= ICON_BOX_INDEX;
				StockNode->StateIndex			= STATE_IMAGE_SMALL_UNCHECKED;
				StockNode->Data					= (void*)stockListQuery->FieldByName("Stock_Key")->AsInteger;

				stockKeyToCodeMap[stockListQuery->FieldByName("Stock_Key")->AsInteger] =
					StockDetails(stockListQuery->FieldByName("Stock_Key")->AsInteger,
									 stockListQuery->FieldByName("Code")->AsString,
									 stockListQuery->FieldByName("Description")->AsString);

				LastStockKey						= stockListQuery->FieldByName("Stock_Key")->AsInteger;
			}
			if(stockListQuery->FieldByName("Location")->AsString != "")
			{
				TTreeNode *locationNode = tvFilterTree->Items->AddChild(StockNode, stockListQuery->FieldByName("Location")->AsString +
					" (" + FormatFloat("0.####", stockListQuery->FieldByName("On_Hand")->AsDouble) + " on hand)");
				locationNode->ImageIndex		= -1;
				locationNode->SelectedIndex	= -1;
				locationNode->StateIndex		= -1;

				stockKeyToCodeMap[stockListQuery->FieldByName("Stock_Key")->AsInteger].locations[stockListQuery->FieldByName("Location")->AsString] =
						LocationDetails(stockListQuery->FieldByName("Location")->AsString,
											 stockListQuery->FieldByName("On_Hand")->AsDouble,
											 stockListQuery->FieldByName("Initialised")->AsString == "T");
			}
		}
	}
	__finally
	{
		dbStockTransaction.Commit();
	}
	// Load possible locations
	dbMMTransaction.Start();
	try
	{
		for (mmLocationsQuery->ExecQuery(); !mmLocationsQuery->Eof; mmLocationsQuery->Next())
		{
			cbLocation->Items->Add(mmLocationsQuery->FieldByName("Name")->AsString);
			lbMMLocations->Items->Add(mmLocationsQuery->FieldByName("Name")->AsString);
		}
	}
	__finally
	{
		dbMMTransaction.Commit();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::tvFilterTreeKeyPress(TObject *Sender, char &Key)
{
	TTreeNode *node = tvFilterTree->Selected;
	if (node && Key == ' ')
	{
		toggleCheck(node);
		Key = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::tvFilterTreeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	 THitTests HitTests = tvFilterTree->GetHitTestInfoAt(X, Y);
	 bool CheckBoxClicked = HitTests.Contains(htOnStateIcon);

	 if (CheckBoxClicked)
	 {
		  TTreeNode *node = tvFilterTree->GetNodeAt(X, Y);
		  if (node)
		  {
				toggleCheck(node);
		  }
	 }
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::toggleCheck(TTreeNode *Node)
{
  if (Node->Level == 0)
  {
		if (Node->StateIndex == STATE_IMAGE_SMALL_UNCHECKED || Node->StateIndex == STATE_IMAGE_SMALL_GRAY_CHECKED)
		{
			Node->StateIndex = STATE_IMAGE_SMALL_CHECKED;
		}
		else if (Node->StateIndex == STATE_IMAGE_SMALL_CHECKED)
		{
			Node->StateIndex = STATE_IMAGE_SMALL_UNCHECKED;
		}
		TTreeNode *GroupNode = Node->getFirstChild();
		while (GroupNode)
		{
			GroupNode->StateIndex = Node->StateIndex;

			TTreeNode *StockNode = GroupNode->getFirstChild();
			while (StockNode)
			{
				StockNode->StateIndex = GroupNode->StateIndex;
				StockNode = StockNode->getNextSibling();
			}
			GroupNode = GroupNode->getNextSibling();
		}
  }
  else if (Node->Level == 1)
  {
		if (Node->StateIndex == STATE_IMAGE_SMALL_UNCHECKED || Node->StateIndex == STATE_IMAGE_SMALL_GRAY_CHECKED)
		{
			Node->StateIndex = STATE_IMAGE_SMALL_CHECKED;
		}
		else if (Node->StateIndex == STATE_IMAGE_SMALL_CHECKED)
		{
			Node->StateIndex = STATE_IMAGE_SMALL_UNCHECKED;
		}
		setChildState(Node);

		TTreeNode *StockNode = Node->getFirstChild();
		while (StockNode)
		{
			StockNode->StateIndex = Node->StateIndex;
			StockNode = StockNode->getNextSibling();
		}
  }
  else if (Node->Level == 2)
  {
		if (Node->StateIndex == STATE_IMAGE_SMALL_UNCHECKED)
		{
			Node->StateIndex = STATE_IMAGE_SMALL_CHECKED;
		}
		else if (Node->StateIndex == STATE_IMAGE_SMALL_CHECKED)
		{
			Node->StateIndex = STATE_IMAGE_SMALL_UNCHECKED;
		}
		setChildState(Node);
		setChildState(Node->Parent);
	}
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::setChildState(TTreeNode *node)
{
	bool allSelected = true;
	bool someSelected = false;
	TTreeNode *childNode = node->Parent->getFirstChild();
	while (childNode)
	{
		if (childNode->StateIndex == STATE_IMAGE_SMALL_CHECKED)
		{
			someSelected = true;
		}
		else if(childNode->StateIndex == STATE_IMAGE_SMALL_GRAY_CHECKED)
		{
			someSelected = true;
			allSelected = false;
		}
		else
		{
			allSelected = false;
		}
		childNode = childNode->getNextSibling();
	}
	if (allSelected)
	{
		node->Parent->StateIndex = STATE_IMAGE_SMALL_CHECKED;
	}
	else if (someSelected)
	{
		node->Parent->StateIndex = STATE_IMAGE_SMALL_GRAY_CHECKED;
	}
	else
	{
		node->Parent->StateIndex = STATE_IMAGE_SMALL_UNCHECKED;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::btnProductsRenameClick(TObject *Sender)
{
	Database::TDBControl dbControl(dmMMData->dbMenuMate);
	int connectionCount = Database::TDBUtilities(dbControl).UserCount();
	if(connectionCount > 1)
	{
		Application->MessageBox(AnsiString("Please make sure there are no connections to the database first!\r"
			"(There are currently " + IntToStr(connectionCount-1) + " other connections.)").c_str(), "Error", MB_ICONERROR + MB_OK);
		return;
	}

	selectedStock.clear();
	std::set<AnsiString> selectableLocations;
//	std::map<AnsiString, AnsiString> inStocktakeLocations;

	for(int iCat=0; iCat<tvFilterTree->Items->Count; iCat++)
	{
		TTreeNode* catNode = tvFilterTree->Items->Item[iCat];

		if(catNode->StateIndex == STATE_IMAGE_SMALL_GRAY_CHECKED ||
			catNode->StateIndex == STATE_IMAGE_SMALL_CHECKED)
		{
			for(int iGrp=0; iGrp<catNode->Count; iGrp++)
			{
				TTreeNode* grpNode = catNode->Item[iGrp];

				if(grpNode->StateIndex == STATE_IMAGE_SMALL_GRAY_CHECKED ||
					grpNode->StateIndex == STATE_IMAGE_SMALL_CHECKED)
				{
					for(int iProd=0; iProd<grpNode->Count; iProd++)
					{
						TTreeNode* prodNode = grpNode->Item[iProd];

						if(prodNode->StateIndex == STATE_IMAGE_SMALL_CHECKED)
						{
							selectedStock[(int)prodNode->Data] = stockKeyToCodeMap[(int)prodNode->Data];

							StockDetails::Locations::const_iterator iLoc;
							for(iLoc=stockKeyToCodeMap[(int)prodNode->Data].locations.begin();
								 iLoc!=stockKeyToCodeMap[(int)prodNode->Data].locations.end(); ++iLoc)
							{
								/*if(iLoc->second.inStocktake)
								{
									inStocktakeLocations[prodNode->Text] = iLoc->second.name;
								}
								else*/
								{
									selectableLocations.insert(iLoc->first);
								}
							}
						}
					}
				}
			}
		}
	}
/*	if(inStocktakeLocations.size() > 0)
	{
		AnsiString msg = "Some of the selected items are currently initialized for stocktake.";

		if(inStocktakeLocations.size() < 40) // Do't display too many!
		{
			std::map<AnsiString, AnsiString>::const_iterator i = inStocktakeLocations.begin();

			msg += "\r\r" + i->first;
			++i;
			if(i!=inStocktakeLocations.end())
			{
				msg + ", ";
				for(; i!=inStocktakeLocations.end(); ++i)
				{
					msg += ", " + i->first;
				}
			}
		}
		Application->MessageBox(msg.c_str(), "Error", MB_ICONERROR + MB_OK);
	}
	else*/ if(selectableLocations.size() == 0)
	{
		Application->MessageBox("There are no locations to rename!", "Error", MB_ICONERROR + MB_OK);
	}
	else
	{
		lbLocations->Items->Clear();
		pcReportProperties->ActivePage = tsLocations;
		std::set<AnsiString>::const_iterator iLoc;
		for(iLoc=selectableLocations.begin(); iLoc!=selectableLocations.end(); ++iLoc)
		{
			int index = cbLocation->Items->IndexOf(*iLoc);
			if(index == -1 || cbLocation->Items->Strings[index] != *iLoc)
			{
            // case-sensitive lookup
				lbLocations->Items->AddObject(*iLoc, (TObject*)1);
			}
			else
			{
				lbLocations->Items->AddObject(*iLoc, NULL);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::btnAddLocationsClick(TObject *Sender)
{
	selectedStock.clear();

	for(int iCat=0; iCat<tvFilterTree->Items->Count; iCat++)
	{
		TTreeNode* catNode = tvFilterTree->Items->Item[iCat];

		if(catNode->StateIndex == STATE_IMAGE_SMALL_GRAY_CHECKED ||
			catNode->StateIndex == STATE_IMAGE_SMALL_CHECKED)
		{
			for(int iGrp=0; iGrp<catNode->Count; iGrp++)
			{
				TTreeNode* grpNode = catNode->Item[iGrp];

				if(grpNode->StateIndex == STATE_IMAGE_SMALL_GRAY_CHECKED ||
					grpNode->StateIndex == STATE_IMAGE_SMALL_CHECKED)
				{
					for(int iProd=0; iProd<grpNode->Count; iProd++)
					{
						TTreeNode* prodNode = grpNode->Item[iProd];

						if(prodNode->StateIndex == STATE_IMAGE_SMALL_CHECKED)
						{
							selectedStock[(int)prodNode->Data] = stockKeyToCodeMap[(int)prodNode->Data];
						}
					}
				}
			}
		}
	}
	pcReportProperties->ActivePage = tsLocationToAdd;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::btnLocationsNextClick(TObject *Sender)
{
	if(lbLocations->ItemIndex != -1 && cbLocation->Text != "")
	{
		AnsiString selectedName = lbLocations->Items->Strings[lbLocations->ItemIndex];
		AnsiString newName = cbLocation->Text;
		AnsiString newNameUpper = newName.UpperCase();

		Database::TDBTransaction transaction(dmStockData->dbStock);
		TIBSQL* checkForStocktake = transaction.Query(transaction.AddQuery());
		transaction.Start();
		checkForStocktake->Close();
		checkForStocktake->SQL->Text =
			"Select * From StockTake Where Location = :Location And Committed_Time is null";
		checkForStocktake->ParamByName("Location")->AsString = selectedName;
		checkForStocktake->ExecQuery();
		if(!checkForStocktake->Eof)
		{
			Application->MessageBox("There is an uncommitted stocktake in this location.", "Error", MB_ICONERROR + MB_OK);
			transaction.Commit();
			return;
		}
		else
		{
			transaction.Commit();
		}

/*
		std::map<int, StockDetails>::iterator iProd;
		for(iProd=selectedStock.begin(); iProd!=selectedStock.end(); ++iProd)
		{
			StockDetails& stockDetails = iProd->second;

			StockDetails::Locations::iterator iLoc;
			bool doTransfer = false;
			for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
			{
				LocationDetails& locationDetails = iLoc->second;

				if(locationDetails.name == selectedName)
				{
					// This is the one we're trying to rename.
					if(locationDetails.name == newName)
					{
						// It's already the same!
						locationDetails.operationType = LocationDetails::None;
					}
					else if(locationDetails.name.UpperCase() == newNameUpper && locationDetails.name != newName)
					{
						// This one is like the one we're renaming to but slightly different.
						locationDetails.operationType = LocationDetails::Unknown;
						doTransfer = true;
					}
					// Otherwise it is completely different
				}
				else
				{
					locationDetails.operationType = LocationDetails::None;
				}
			}
			if(doTransfer)
			{
				stockToUpdate[iProd->first] = iProd->second;
			}
		}
*/
		// Just find stuff to work with.
		// Set OperationType to None if there is no operation for the location though.
		std::vector<AnsiString> inStocktakeLocations;

		std::map<int, StockDetails>::iterator iProd;
		for(iProd=selectedStock.begin(); iProd!=selectedStock.end(); ++iProd)
		{
			StockDetails& stockDetails = iProd->second;

			StockDetails::Locations::iterator iLoc;
			bool doTransfer = false;
			for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
			{
				LocationDetails& locationDetails = iLoc->second;

				if((locationDetails.name == selectedName || locationDetails.name.UpperCase() == newNameUpper) &&
					 locationDetails.name != newName)
				{
					// This one is either the one we're looking to update or
					// it's similar to what we're changing to (but not exactly the same)
					locationDetails.operationType = LocationDetails::Unknown;
					doTransfer = true;

					if(iLoc->second.inStocktake)
					{
						inStocktakeLocations.push_back(stockDetails.description);
					}
				}
				else
				{
					locationDetails.operationType = LocationDetails::None;
				}
			}
			if(doTransfer)
			{
				stockToUpdate[iProd->first] = iProd->second;
			}
		}
		if(stockToUpdate.size() > 0)
		{
			if(inStocktakeLocations.size() > 0)
			{
				AnsiString msg = "Some of the selected items are currently initialized for stocktake.";

				if(inStocktakeLocations.size() < 40) // Do't display too many!
				{
					std::vector<AnsiString>::const_iterator i = inStocktakeLocations.begin();

					msg += "\r\r" + *i;
					++i;
					if(i!=inStocktakeLocations.end())
					{
						msg + ", ";
						for(; i!=inStocktakeLocations.end(); ++i)
						{
							msg += ", " + *i;
						}
					}
				}
				Application->MessageBox(msg.c_str(), "Error", MB_ICONERROR + MB_OK);
			}
			else
			{
				memProcess->Lines->Clear();
				chbConfirm->Checked = false;

				std::map<int, StockDetails>::iterator iProd;
				for(iProd=stockToUpdate.begin(); iProd!=stockToUpdate.end(); ++iProd)
				{
					StockDetails& stockDetails = iProd->second;

					StockDetails::Locations::iterator iLoc;

					// Need to do something here if the new location doesn't exist.
					if(!stockDetails.containsLocation(newName))
					{
						// New location doesn't exist!
						//
						// Will rename a location if only differing by case.
						// Otherwise will create if the new location doesn't exist
						// Must only do 1 rename OR 1 create!
						bool willRename = false;
						bool willCreate = false;
						for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
						{
							LocationDetails& location = iLoc->second;

							if(location.name != newName && location.name.UpperCase() == newName.UpperCase())
							{
								// The name is similar but an exact one doesn't exist.
								// We can create the required location by renaming this one.
								location.operationType = LocationDetails::Rename;
								willRename = true;
								break;
							}
						}
						if(!willRename)
						{
							for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
							{
								LocationDetails& location = iLoc->second;

								if(location.name != newName && location.operationType == LocationDetails::Unknown)
								{
									// Only one will create!
									location.operationType = LocationDetails::CreateAndTransfer;
									willCreate = true;
									break;
								}
							}
						}
						if(!willRename && !willCreate)
						{
							throw Exception("Oops, something went wrong! New location will not exist!");
						}
					}
					// New location will now exists
					//
					// Will transfer everything necessary
					for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
					{
						LocationDetails& location = iLoc->second;

						if(location.name != newName && location.operationType == LocationDetails::Unknown)
						{
							location.operationType = LocationDetails::Transfer;
							break;
						}
					}
				}
				// First, look for create or rename to make sure destination exists
				for(iProd=stockToUpdate.begin(); iProd!=stockToUpdate.end(); ++iProd)
				{
					const StockDetails& stockDetails = iProd->second;
					StockDetails::Locations::const_iterator iLoc;
					for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
					{
						const LocationDetails& location = iLoc->second;
						if(location.operationType == LocationDetails::Rename)
						{
							memProcess->Lines->Add(stockDetails.description + ": Rename " + location.name + "->" + newName);
						}
						else if(location.operationType == LocationDetails::CreateAndTransfer)
						{
							memProcess->Lines->Add(stockDetails.description + ": Create " + newName);
						}
					}
				}
				// Now set up for some transferring
				for(iProd=stockToUpdate.begin(); iProd!=stockToUpdate.end(); ++iProd)
				{
					const StockDetails& stockDetails = iProd->second;
					StockDetails::Locations::const_iterator iLoc;
					for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
					{
						const LocationDetails& location = iLoc->second;
						if(location.operationType == LocationDetails::CreateAndTransfer || location.operationType == LocationDetails::Transfer)
						{
							memProcess->Lines->Add(stockDetails.description + ": Transfer " + location.name + "->" + newName);
							memProcess->Lines->Add(stockDetails.description + ": Delete " + location.name);
						}
						else if(location.operationType == LocationDetails::Unknown)
						{
							throw Exception("Oops, something went wrong! Location operation unknown!");
						}
					}
				}
				pcReportProperties->ActivePage = tsClashes;
			}
		}
		else
		{
			Application->MessageBox("There are no locations to update!", "Error", MB_ICONERROR + MB_OK);
		}
	}
	else
	{
		Application->MessageBox("You must select a source and destination location.", "Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::btnFinishClick(TObject *Sender)
{
	if(chbConfirm->Checked)
	{
		btnClose->Enabled = false;
		try
		{
			Stock::TTransferStock createTransfers(dmStockData->dbStock);
			AnsiString newName;
			if(lbMMLocations->ItemIndex != -1)
			{
				newName = lbMMLocations->Items->Strings[lbMMLocations->ItemIndex];
			}
			else
			{
				newName = cbLocation->Text;
			}
			if(newName == "")
			{
				throw Exception("I don't know what to do?");
			}

			ProgressBar1->Position = 0;
			ProgressBar1->Max = stockToUpdate.size() * 3;

			dbStockTransaction.Start();
			try
			{
				// First, look for create or rename to make sure destination exists
				std::map<int, StockDetails>::iterator iProd;
				for(iProd=stockToUpdate.begin(); iProd!=stockToUpdate.end(); ++iProd)
				{
					const StockDetails& stockDetails = iProd->second;
					StockDetails::Locations::const_iterator iLoc;
					for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
					{
						const LocationDetails& location = iLoc->second;
						if(location.operationType == LocationDetails::Rename)
						{
							renameLocation(stockDetails.key, location.name, newName);
						}
						else if(location.operationType == LocationDetails::CreateAndTransfer ||
								  location.operationType == LocationDetails::Create)
						{
							std::set<int> StockKeys;
							StockKeys.insert(stockDetails.key);
							createTransfers.CreateLocations(StockKeys, newName);
						}
					}
					ProgressBar1->Position++;
				}
				dbStockTransaction.Commit();
			}
			catch(Exception &e)
			{
				try
				{
					dbStockTransaction.Rollback();
				}
				__finally
				{
					ProgressBar1->Position = 0;
					Application->MessageBox("There was a problem renaming or creating the stock locations.", "Error", MB_ICONERROR + MB_OK);
					return;
				}
			}
			// Now do some transferring
			std::vector<Stock::TTransferInfo> transfers;

			std::map<int, StockDetails>::iterator iProd;
			for(iProd=stockToUpdate.begin(); iProd!=stockToUpdate.end(); ++iProd)
			{
				const StockDetails& stockDetails = iProd->second;
				StockDetails::Locations::const_iterator iLoc;
				for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
				{
					const LocationDetails& location = iLoc->second;
					if(location.operationType == LocationDetails::CreateAndTransfer || location.operationType == LocationDetails::Transfer)
					{
						Stock::TTransferInfo			transferInfo;
						transferInfo.StockKey		= stockDetails.key;
						transferInfo.Quantity		= location.quantity;
						transferInfo.Source			= location.name;
						transferInfo.Destination	= newName;
						transfers.push_back(transferInfo);
					}
				}
				ProgressBar1->Position++;
			}
			Stock::TTransactionBatchInfo	batchInfo(Stock::ttTransfer);

			batchInfo.User_ID		= "ADMIN";
			batchInfo.User_Name	= "Administration";
			if (!createTransfers.TransferStock(batchInfo, transfers))
			{
				Application->MessageBox("There was a problem transferring the stock.", "Error", MB_ICONERROR + MB_OK);
				return;
			}
			dbStockTransaction.Start();
			dbMMTransaction.Start();
			try
			{
				for(iProd=stockToUpdate.begin(); iProd!=stockToUpdate.end(); ++iProd)
				{
					const StockDetails& stockDetails = iProd->second;
					StockDetails::Locations::const_iterator iLoc;
					for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
					{
						const LocationDetails& location = iLoc->second;
						if(location.operationType == LocationDetails::Rename ||
							location.operationType == LocationDetails::CreateAndTransfer ||
							location.operationType == LocationDetails::Transfer)
						{
							renamePurchaseStock(stockDetails.code, location.name, newName);
							renameMMRecipeStock(stockDetails.code, location.name, newName);
							renameMMOrderRecipeStock(stockDetails.code, location.name, newName);
							renameMMStockTrack(stockDetails.code, location.name, newName);
							renameMMStockCosts(stockDetails.code, location.name, newName);
						}
						if(location.operationType == LocationDetails::CreateAndTransfer || location.operationType == LocationDetails::Transfer)
						{
							deleteLocation(stockDetails.key, location.name);
						}
					}
					ProgressBar1->Position++;
				}
				dbStockTransaction.Commit();
				dbMMTransaction.Commit();
				ProgressBar1->Position = 0;
				Close();
			}
			catch(Exception &e)
			{
				try
				{
					try
					{
						dbStockTransaction.Rollback();
					}
					__finally
					{
						dbMMTransaction.Rollback();
					}
				}
				__finally
				{
					ProgressBar1->Position = 0;
					Application->MessageBox("There was a problem transferring the stock.", "Error", MB_ICONERROR + MB_OK);
				}
			}
		}
		__finally
		{
			btnClose->Enabled = true;
		}
	}
	else
	{
		Application->MessageBox("Please confirm your intent by ticking the box.", "Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::deleteLocation(int stockKey, const AnsiString& location)
{
	updateStockQuery->Close();
	updateStockQuery->SQL->Text =
		"Delete From "
			"StockLocation "
		"Where "
			"Stock_Key = :Stock_Key And "
			"Location = :Location And "
			"On_Hand = 0";

	updateStockQuery->ParamByName("Stock_Key")->AsInteger = stockKey;
	updateStockQuery->ParamByName("Location")->AsString = location;
	updateStockQuery->ExecQuery();
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::renameLocation(int stockKey, const AnsiString& oldLocation, const AnsiString& newLocation)
{
	updateStockQuery->Close();
	updateStockQuery->SQL->Text =
		"Update StockLocation Set "
			"Location = :NewLocation "
		"Where "
			"Stock_Key = :Stock_Key And "
			"Location = :OldLocation";

	updateStockQuery->ParamByName("NewLocation")->AsString = newLocation;
	updateStockQuery->ParamByName("Stock_Key")->AsInteger = stockKey;
	updateStockQuery->ParamByName("OldLocation")->AsString = oldLocation;
	updateStockQuery->ExecQuery();
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::renamePurchaseStock(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation)
{
	updateStockQuery->Close();
	updateStockQuery->SQL->Text =
		"Update "
			"PurchaseStock "
		"Set "
			"Location = :NewLocation "
		"Where "
			"Code = :Code And "
			"Location = :OldLocation And "
			"(Qty_On_Order > 0 Or "
				"PurchaseStock.Order_FK In ("
					"Select "
						"PurchaseOrder.Order_LK "
					"From "
						"PurchaseOrder "
					"Where "
						"PurchaseOrder.Posted = 'F'))";

	updateStockQuery->ParamByName("NewLocation")->AsString = newLocation;
	updateStockQuery->ParamByName("OldLocation")->AsString = oldLocation;
	updateStockQuery->ParamByName("Code")->AsString = stockCode;
	updateStockQuery->ExecQuery();
	updateStockQuery->Close();
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::renameMMRecipeStock(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation)
{
	mmUpdateStock->Close();
	mmUpdateStock->SQL->Text =
		"Update "
			"Recipe "
		"Set "
			"Stock_Location = :NewLocation "
		"Where "
			"Stock_Code = :Code And "
			"Upper(Stock_Location) = :OldLocation";

	mmUpdateStock->ParamByName("NewLocation")->AsString = newLocation;
	mmUpdateStock->ParamByName("OldLocation")->AsString = oldLocation.UpperCase();
	mmUpdateStock->ParamByName("Code")->AsString = stockCode;
	mmUpdateStock->ExecQuery();
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::renameMMOrderRecipeStock(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation)
{
	mmUpdateStock->Close();
	mmUpdateStock->SQL->Text =
		"Update "
			"OrderRecipe "
		"Set "
			"Stock_Location = :NewLocation "
		"Where "
			"Stock_Code = :Code And "
			"Upper(Stock_Location) = :OldLocation";

	mmUpdateStock->ParamByName("NewLocation")->AsString = newLocation;
	mmUpdateStock->ParamByName("OldLocation")->AsString = oldLocation.UpperCase();
	mmUpdateStock->ParamByName("Code")->AsString = stockCode;
	mmUpdateStock->ExecQuery();
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::renameMMStockTrack(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation)
{
	mmUpdateStock->Close();
	mmUpdateStock->SQL->Text =
		"Update "
			"StockTrack "
		"Set "
			"Stock_Location = :NewLocation "
		"Where "
			"Stock_Code = :Code And "
			"Upper(Stock_Location) = :OldLocation";

	mmUpdateStock->ParamByName("NewLocation")->AsString = newLocation;
	mmUpdateStock->ParamByName("OldLocation")->AsString = oldLocation.UpperCase();
	mmUpdateStock->ParamByName("Code")->AsString = stockCode;
	mmUpdateStock->ExecQuery();
}
//---------------------------------------------------------------------------
void TfrmUpdateLocations::renameMMStockCosts(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation)
{
	mmUpdateStock->Close();
	mmUpdateStock->SQL->Text =
		"Select * From StockCosts "
		"Where "
			"Stock_Code = :Code And "
			"Location = :NewLocation";
	mmUpdateStock->ParamByName("NewLocation")->AsString = newLocation;
	mmUpdateStock->ParamByName("Code")->AsString = stockCode;
	mmUpdateStock->ExecQuery();
	
	if(mmUpdateStock->Eof)
	{
		mmUpdateStock->Close();
		mmUpdateStock->SQL->Text =
			"Update "
				"StockCosts "
			"Set "
				"Location = :NewLocation "
			"Where "
				"Stock_Code = :Code And "
				"Upper(Location) = :OldLocation";

		mmUpdateStock->ParamByName("NewLocation")->AsString = newLocation;
		mmUpdateStock->ParamByName("OldLocation")->AsString = oldLocation.UpperCase();
		mmUpdateStock->ParamByName("Code")->AsString = stockCode;
		mmUpdateStock->ExecQuery();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::lbLocationsDrawItem(TWinControl *Control,
		int Index, TRect &Rect, TOwnerDrawState State)
{
	if(lbLocations->Items->Count > Index)
	{
		TCanvas *pCanvas = ((TListBox *)Control)->Canvas;
		pCanvas->FillRect(Rect); // clear the rectangle

		int horizOffset = 2;
		if(lbLocations->Items->Objects[Index] != NULL)
		{
			int vertOffset = (((TListBox *)Control)->ItemHeight - dmUtilities->imlTreeImages->Height) / 2;

			dmUtilities->imlTreeImages->Draw(lbLocations->Canvas, Rect.Left + horizOffset, Rect.Top + vertOffset, ICON_EXCLAMATION_INDEX);
			horizOffset += dmUtilities->imlTreeImages->Width + 4;
		}
		// display the text
		AnsiString text = ((TListBox *)Control)->Items->Strings[Index];
		int vertOffset = (((TListBox *)Control)->ItemHeight - pCanvas->TextHeight(text)) / 2;
		pCanvas->TextOut(Rect.Left + horizOffset, Rect.Top + vertOffset, text);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::btnAddLocationBackClick(TObject *Sender)
{
	pcReportProperties->ActivePage = tsSelectItems;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::btnLocationsBackClick(TObject *Sender)
{
	pcReportProperties->ActivePage = tsSelectItems;
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::btnAddLocationNextClick(TObject *Sender)
{
	if(lbMMLocations->ItemIndex != -1)
	{
		AnsiString selectedName = lbMMLocations->Items->Strings[lbMMLocations->ItemIndex];

		if(selectedName == "")
		{
			throw Exception("I don't know what to do?");
		}

		std::map<int, StockDetails>::iterator iProd;
		for(iProd=selectedStock.begin(); iProd!=selectedStock.end(); ++iProd)
		{
			StockDetails& stockDetails = iProd->second;

			if(!stockDetails.containsLocation(selectedName))
			{
				stockToUpdate[iProd->first] = iProd->second;
			}
		}
		if(stockToUpdate.size() > 0)
		{
			memProcess->Lines->Clear();
			chbConfirm->Checked = false;

			std::map<int, StockDetails>::iterator iProd;
			for(iProd=stockToUpdate.begin(); iProd!=stockToUpdate.end(); ++iProd)
			{
				StockDetails& stockDetails = iProd->second;

				StockDetails::Locations::iterator iLoc;

				bool willRename = false;
				bool willCreate = false;
				for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
				{
					LocationDetails& location = iLoc->second;

					if(location.name != selectedName && location.name.UpperCase() == selectedName.UpperCase())
					{
						// The name is similar but an exact one doesn't exist.
						// We can create the required location by renaming this one.
						location.operationType = LocationDetails::Rename;
						willRename = true;
						break;
					}
				}
				if(!willRename)
				{
					willCreate = true;
					stockDetails.locations[selectedName] = LocationDetails(selectedName, 0, false);
					stockDetails.locations[selectedName].operationType = LocationDetails::Create;
				}
				if(!willRename && !willCreate)
				{
					throw Exception("Oops, something went wrong! New location will not exist!");
				}
				// New location will now exists
				//
				// Will transfer everything necessary
				for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
				{
					LocationDetails& location = iLoc->second;

					if(location.name != selectedName && location.name.UpperCase() == selectedName.UpperCase() &&
						location.operationType == LocationDetails::Unknown)
					{
						location.operationType = LocationDetails::Transfer;
					}
					else if(location.operationType == LocationDetails::Unknown)
					{
						location.operationType = LocationDetails::None;
					}
				}
			}
			// First, look for create or rename to make sure destination exists
			for(iProd=stockToUpdate.begin(); iProd!=stockToUpdate.end(); ++iProd)
			{
				const StockDetails& stockDetails = iProd->second;
				StockDetails::Locations::const_iterator iLoc;
				for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
				{
					const LocationDetails& location = iLoc->second;
					if(location.operationType == LocationDetails::Rename)
					{
						memProcess->Lines->Add(stockDetails.description + ": Rename " + location.name + "->" + selectedName);
					}
					else if(location.operationType == LocationDetails::Create)
					{
						memProcess->Lines->Add(stockDetails.description + ": Create " + selectedName);
					}
				}
			}
			// Now set up for some transferring
			for(iProd=stockToUpdate.begin(); iProd!=stockToUpdate.end(); ++iProd)
			{
				const StockDetails& stockDetails = iProd->second;
				StockDetails::Locations::const_iterator iLoc;
				for(iLoc=stockDetails.locations.begin(); iLoc!=stockDetails.locations.end(); ++iLoc)
				{
					const LocationDetails& location = iLoc->second;
					if(location.operationType == LocationDetails::CreateAndTransfer || location.operationType == LocationDetails::Transfer)
					{
						memProcess->Lines->Add(stockDetails.description + ": Transfer " + location.name + "->" + selectedName);
						memProcess->Lines->Add(stockDetails.description + ": Delete " + location.name);
					}
					else if(location.operationType == LocationDetails::Unknown)
					{
						throw Exception("Oops, something went wrong! Location operation unknown!");
					}
				}
			}
			pcReportProperties->ActivePage = tsClashes;
		}
		else
		{
			Application->MessageBox("There is nothing to do!", "Error", MB_ICONERROR + MB_OK);
		}
	}
	else
	{
		Application->MessageBox("You must select a location to add.", "Error", MB_ICONERROR + MB_OK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmUpdateLocations::btnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

