//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SetupLocations.h"
#include "Utilities.h"
#include "MMData.h"
#include "StockData.h"
#include "LocationRename.h"
#define LOCATION_NAME_INDEX   0
#define SALES_LOCATION_INDEX  1
#define STOCK_LOCATION_INDEX  2
#define PURCHASE_ORDER_INDEX   3
#define SUPPLIER_LOCATION_INDEX  4
#define STOCK_REQUEST_INDEX  5
#define STOCK_REQUEST_AUTO_INDEX  6
#define TYPE_CHECKED				" "
#define TYPE_UNCHECKED			""
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSetupLocations *frmSetupLocations;
//---------------------------------------------------------------------------
__fastcall TfrmSetupLocations::TfrmSetupLocations(TComponent* Owner)
: TForm(Owner)
{
	StringGrid1->DoubleBuffered = true;
	SGMouseDown = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::FormShow(TObject *Sender)
{
	if (!Transaction->InTransaction) Transaction->StartTransaction();
	sqlLocation->Close();
	StringGrid1->RowCount = 2;
	StringGrid1->Cells[LOCATION_NAME_INDEX][0] = "Location";
	StringGrid1->Cells[SALES_LOCATION_INDEX][0] = "Sales Location";
	StringGrid1->Cells[STOCK_LOCATION_INDEX][0] = "Stock Location";
	StringGrid1->Cells[PURCHASE_ORDER_INDEX][0] = "Purchase Order";
	StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][0] = "Supplier Order";
	StringGrid1->Cells[STOCK_REQUEST_INDEX][0] = "Stock Request";
    StringGrid1->Cells[STOCK_REQUEST_AUTO_INDEX][0] = "Stock Request";

	for (sqlLocation->ExecQuery(); !sqlLocation->Eof; sqlLocation->Next())   //sqlCategory
	{
		StringGrid1->Cells[LOCATION_NAME_INDEX][StringGrid1->RowCount-1] = sqlLocation->FieldByName("Name")->AsString;
		StringGrid1->Objects[LOCATION_NAME_INDEX][StringGrid1->RowCount-1] = (TObject *)sqlLocation->FieldByName("Location_Key")->AsInteger;
		if (sqlLocation->FieldByName("Location_Type")->AsInteger == 0)
		{
			StringGrid1->Cells[SALES_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[STOCK_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
		}
		else if (sqlLocation->FieldByName("Location_Type")->AsInteger == 1)
		{
			StringGrid1->Cells[SALES_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
			StringGrid1->Cells[STOCK_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
		}
		else if (sqlLocation->FieldByName("Location_Type")->AsInteger == 2)
		{
			StringGrid1->Cells[SALES_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[STOCK_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
		}
		if (sqlLocation->FieldByName("CATEGORY_TYPE")->AsInteger == 0)
		{
			StringGrid1->Cells[PURCHASE_ORDER_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
			StringGrid1->Cells[STOCK_REQUEST_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
		}
		else if (sqlLocation->FieldByName("CATEGORY_TYPE")->AsInteger == 1)
		{StringGrid1->Cells[PURCHASE_ORDER_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
			StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[STOCK_REQUEST_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
		}
		else  if (sqlLocation->FieldByName("CATEGORY_TYPE")->AsInteger == 2)
		{
			StringGrid1->Cells[PURCHASE_ORDER_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
			StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
			StringGrid1->Cells[STOCK_REQUEST_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
		}
		else  if (sqlLocation->FieldByName("CATEGORY_TYPE")->AsInteger == 3)
		{
			StringGrid1->Cells[PURCHASE_ORDER_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[STOCK_REQUEST_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
            }
		else  if (sqlLocation->FieldByName("CATEGORY_TYPE")->AsInteger == 4)
		{
			StringGrid1->Cells[PURCHASE_ORDER_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
			StringGrid1->Cells[STOCK_REQUEST_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;}
		else  if (sqlLocation->FieldByName("CATEGORY_TYPE")->AsInteger == 5)
		{ StringGrid1->Cells[PURCHASE_ORDER_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
			StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[STOCK_REQUEST_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
		}
		else  if (sqlLocation->FieldByName("CATEGORY_TYPE")->AsInteger == 6)
		{ StringGrid1->Cells[PURCHASE_ORDER_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
			StringGrid1->Cells[STOCK_REQUEST_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;
		}
		else  if (sqlLocation->FieldByName("CATEGORY_TYPE")->AsInteger == 99)
		{ StringGrid1->Cells[PURCHASE_ORDER_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
			StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
			StringGrid1->Cells[STOCK_REQUEST_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;
		}

        	if (sqlLocation->FieldByName("STOCKREQUESTAUTO_KEY")->AsInteger == 0)
		{
			StringGrid1->Cells[STOCK_REQUEST_AUTO_INDEX][StringGrid1->RowCount-1] = TYPE_UNCHECKED;

		}
         else	if (sqlLocation->FieldByName("STOCKREQUESTAUTO_KEY")->AsInteger == 1)
		{
			StringGrid1->Cells[STOCK_REQUEST_AUTO_INDEX][StringGrid1->RowCount-1] = TYPE_CHECKED;

		}
		StringGrid1->RowCount++;
	}
	StringGrid1->RowCount--;
	StringGrid1->ColWidths[SALES_LOCATION_INDEX] = StringGrid1->Canvas->TextExtent(StringGrid1->Cells[SALES_LOCATION_INDEX][0]).cx + 15;
	StringGrid1->ColWidths[STOCK_LOCATION_INDEX] = StringGrid1->Canvas->TextExtent(StringGrid1->Cells[STOCK_LOCATION_INDEX][0]).cx + 15;
	StringGrid1->ColWidths[PURCHASE_ORDER_INDEX] = StringGrid1->Canvas->TextExtent(StringGrid1->Cells[PURCHASE_ORDER_INDEX][0]).cx + 15;
	StringGrid1->ColWidths[SUPPLIER_LOCATION_INDEX] = StringGrid1->Canvas->TextExtent(StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][0]).cx + 15;
	StringGrid1->ColWidths[STOCK_REQUEST_INDEX] = StringGrid1->Canvas->TextExtent(StringGrid1->Cells[STOCK_REQUEST_INDEX][0]).cx + 15;
    	StringGrid1->ColWidths[STOCK_REQUEST_AUTO_INDEX] = StringGrid1->Canvas->TextExtent(StringGrid1->Cells[STOCK_REQUEST_AUTO_INDEX][0]).cx + 15;
	StringGrid1->ColWidths[LOCATION_NAME_INDEX]  =	StringGrid1->ClientWidth -
	StringGrid1->ColWidths[SALES_LOCATION_INDEX] -
	StringGrid1->ColWidths[STOCK_LOCATION_INDEX] - StringGrid1->ColWidths[PURCHASE_ORDER_INDEX]-
	StringGrid1->ColWidths[SUPPLIER_LOCATION_INDEX]- StringGrid1->ColWidths[STOCK_REQUEST_INDEX] - StringGrid1->ColWidths[STOCK_REQUEST_AUTO_INDEX] -
	(StringGrid1->GridLineWidth * 6);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::StringGrid1DrawCell(TObject *Sender,
int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	if (State.Contains(gdFocused))
	{
		StringGrid1->Canvas->DrawFocusRect(Rect);
	}
	if (!State.Contains(gdFixed))
	{
		if (ACol > 0)
		{
			InflateRect(&Rect, -2, -2);
			if (StringGrid1->Cells[ACol][ARow] == TYPE_CHECKED)
			{
				DrawFrameControl(StringGrid1->Canvas->Handle, &Rect, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_CHECKED);
			}
			else
			{
				DrawFrameControl(StringGrid1->Canvas->Handle, &Rect, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::StringGrid1MouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	SGMouseDown = true;
	int Col, Row;
	StringGrid1->MouseToCell(X, Y, Col, Row);
	if (Col > 0 && Row > 0)
	{
		TGridRect SelectCell;
		SelectCell.Left = 0;
		SelectCell.Top = Row;
		SelectCell.Right = 0;
		SelectCell.Bottom = Row;
		StringGrid1->Selection = SelectCell;


		if (StringGrid1->Cells[Col][Row] == TYPE_UNCHECKED)
		{
			StringGrid1->Cells[Col][Row] = TYPE_CHECKED;
		}
		else
		{
			StringGrid1->Cells[Col][Row] = TYPE_UNCHECKED;
		}
         if(StringGrid1->Cells[STOCK_REQUEST_AUTO_INDEX][Row]== TYPE_UNCHECKED )
        {
          StringGrid1->Cells[STOCK_REQUEST_INDEX][Row]  =  TYPE_UNCHECKED;
        }
		// If unchecked both...
		if (StringGrid1->Cells[SALES_LOCATION_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[STOCK_LOCATION_INDEX][Row] == TYPE_UNCHECKED)
		{
			if (Col == SALES_LOCATION_INDEX)
			{
				StringGrid1->Cells[STOCK_LOCATION_INDEX][Row] = TYPE_CHECKED;
			}
			else
			{
				StringGrid1->Cells[SALES_LOCATION_INDEX][Row] = TYPE_CHECKED;
			}
		}
		sqlUpdateLocation->Close();
		sqlUpdateLocation->ParamByName("Location_Key")->AsInteger = int(StringGrid1->Objects[LOCATION_NAME_INDEX][Row]);
		sqlUpdateLocation->ParamByName("Name")->AsString = StringGrid1->Cells[LOCATION_NAME_INDEX][Row];
		if (StringGrid1->Cells[SALES_LOCATION_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[STOCK_LOCATION_INDEX][Row] == TYPE_UNCHECKED)
		{
			sqlUpdateLocation->ParamByName("Location_Type")->AsInteger = 0;
		}
		else if (StringGrid1->Cells[SALES_LOCATION_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[STOCK_LOCATION_INDEX][Row] == TYPE_CHECKED)
		{
			sqlUpdateLocation->ParamByName("Location_Type")->AsInteger = 1;
		}
		else if (StringGrid1->Cells[SALES_LOCATION_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[STOCK_LOCATION_INDEX][Row] == TYPE_CHECKED)
		{
			sqlUpdateLocation->ParamByName("Location_Type")->AsInteger = 2;
		}
		//---------------------------------------------------------
		if (StringGrid1->Cells[PURCHASE_ORDER_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[STOCK_REQUEST_INDEX][Row] == TYPE_UNCHECKED)
		{
			sqlUpdateLocation->ParamByName("CATEGORY_TYPE")->AsInteger = 0;
		}
		if (StringGrid1->Cells[PURCHASE_ORDER_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[STOCK_REQUEST_INDEX][Row] == TYPE_UNCHECKED)
		{
			sqlUpdateLocation->ParamByName("CATEGORY_TYPE")->AsInteger = 1;
		}
		if (StringGrid1->Cells[PURCHASE_ORDER_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[STOCK_REQUEST_INDEX][Row] == TYPE_CHECKED)
		{
			sqlUpdateLocation->ParamByName("CATEGORY_TYPE")->AsInteger = 2;   //
		}
		if (StringGrid1->Cells[PURCHASE_ORDER_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[STOCK_REQUEST_INDEX][Row] == TYPE_UNCHECKED)
		{
			sqlUpdateLocation->ParamByName("CATEGORY_TYPE")->AsInteger = 3;
		}
		if (StringGrid1->Cells[PURCHASE_ORDER_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[STOCK_REQUEST_INDEX][Row] == TYPE_CHECKED)
		{
			sqlUpdateLocation->ParamByName("CATEGORY_TYPE")->AsInteger = 4;   //
		}
		if (StringGrid1->Cells[PURCHASE_ORDER_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[STOCK_REQUEST_INDEX][Row] == TYPE_CHECKED)
		{
			sqlUpdateLocation->ParamByName("CATEGORY_TYPE")->AsInteger = 5;      //
		}
		if (StringGrid1->Cells[PURCHASE_ORDER_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][Row] == TYPE_CHECKED && StringGrid1->Cells[STOCK_REQUEST_INDEX][Row] == TYPE_CHECKED)
		{
			sqlUpdateLocation->ParamByName("CATEGORY_TYPE")->AsInteger = 6;      //
		}
		if (StringGrid1->Cells[PURCHASE_ORDER_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[SUPPLIER_LOCATION_INDEX][Row] == TYPE_UNCHECKED && StringGrid1->Cells[STOCK_REQUEST_INDEX][Row] == TYPE_UNCHECKED)
		{
			sqlUpdateLocation->ParamByName("CATEGORY_TYPE")->AsInteger = 99;
		}


        	if (StringGrid1->Cells[STOCK_REQUEST_AUTO_INDEX][Row] == TYPE_UNCHECKED )
		{
			sqlUpdateLocation->ParamByName("STOCKREQUESTAUTO_KEY")->AsInteger = 0;
		}
        	if (StringGrid1->Cells[STOCK_REQUEST_AUTO_INDEX][Row] == TYPE_CHECKED )
		{
			sqlUpdateLocation->ParamByName("STOCKREQUESTAUTO_KEY")->AsInteger = 1;
		}
		//CATEGORY_TYPE
		sqlUpdateLocation->ExecQuery();
		Transaction->Commit();
		FormShow(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::StringGrid1MouseUp(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	SGMouseDown = false;
	int Col, Row;
	StringGrid1->MouseToCell(X, Y, Col, Row);
	if (Col > 0 && Row > 0)
	{
		TGridRect SelectCell;
		SelectCell.Left = 0;
		SelectCell.Top = Row;
		SelectCell.Right = 0;
		SelectCell.Bottom = Row;
		StringGrid1->Selection = SelectCell;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::StringGrid1MouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{
	if (SGMouseDown)
	{
		int Col, Row;
		StringGrid1->MouseToCell(X, Y, Col, Row);
		if (Col > 0 && Row > 0)
		{
			TGridRect SelectCell;
			SelectCell.Left = 0;
			SelectCell.Top = Row;
			SelectCell.Right = 0;
			SelectCell.Bottom = Row;
			StringGrid1->Selection = SelectCell;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::StringGrid1KeyPress(TObject *Sender,
char &Key)
{
	/*	if (Key == ' ' && StringGrid1->Selection.Left > 0 && StringGrid1->Selection.Top > 0)
	{
		if (StringGrid1->Cells[StringGrid1->Selection.Left][StringGrid1->Selection.Top] == TYPE_UNCHECKED)
		{
			StringGrid1->Cells[StringGrid1->Selection.Left][StringGrid1->Selection.Top] = TYPE_CHECKED;
		}
		else
		{
			StringGrid1->Cells[StringGrid1->Selection.Left][StringGrid1->Selection.Top] = TYPE_UNCHECKED;
		}
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::btnOkClick(TObject *Sender)
{
	/*	for (int i=1; i<StringGrid1->RowCount; i++)
	{
		sqlUpdateLocation->Close();
		sqlUpdateLocation->ParamByName("Location_Key")->AsInteger = int(StringGrid1->Objects[LOCATION_NAME_INDEX][i]);
		sqlUpdateLocation->ParamByName("Name")->AsString = StringGrid1->Cells[LOCATION_NAME_INDEX][i];
		if (StringGrid1->Cells[SALES_LOCATION_INDEX][i] == TYPE_CHECKED && StringGrid1->Cells[STOCK_LOCATION_INDEX][i] == TYPE_UNCHECKED)
		{
			sqlUpdateLocation->ParamByName("Location_Type")->AsInteger = 0;
		}
		else if (StringGrid1->Cells[SALES_LOCATION_INDEX][i] == TYPE_UNCHECKED && StringGrid1->Cells[STOCK_LOCATION_INDEX][i] == TYPE_CHECKED)
		{
			sqlUpdateLocation->ParamByName("Location_Type")->AsInteger = 1;
		}
		else if (StringGrid1->Cells[SALES_LOCATION_INDEX][i] == TYPE_CHECKED && StringGrid1->Cells[STOCK_LOCATION_INDEX][i] == TYPE_CHECKED)
		{
			sqlUpdateLocation->ParamByName("Location_Type")->AsInteger = 2;
		}
		sqlUpdateLocation->ExecQuery();
	}*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::FormClose(TObject *Sender,
TCloseAction &Action)
{
	try
	{
		if (Transaction->InTransaction) Transaction->Commit();
	}
	catch (Exception &E)
	{
		Application->ShowException(&E);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::btnAddClick(TObject *Sender)
{
	AnsiString Location = "";
	if (InputQuery("New Location", "Please enter the name of the new Location.", Location))
	{
		if (Location != "")
		{
			qrFindLocation->Close();
			qrFindLocation->ParamByName("Name")->AsString = Location.UpperCase();
			qrFindLocation->ExecQuery();
			if (qrFindLocation->Eof)
			{
				//Add the location!
				sqlAddLK->Close();
				sqlAddLK->ExecQuery();
				qrAddLocation->Close();
				qrAddLocation->ParamByName("Location_Key")->AsInteger		= sqlAddLK->Fields[0]->AsInteger;
				qrAddLocation->ParamByName("Location_Type")->AsInteger	= 1;
				qrAddLocation->ParamByName("Name")->AsString					= Location;
				qrAddLocation->ParamByName("CATEGORY_TYPE")->AsString					= 99;
                qrAddLocation->ParamByName("STOCKREQUESTAUTO_KEY")->AsInteger					= 0;
				qrAddLocation->ExecQuery();
				Transaction->Commit();
				FormShow(NULL);
				for (int i=1; i<StringGrid1->RowCount; i++)
				{
					if (StringGrid1->Cells[LOCATION_NAME_INDEX][i] == Location)
					{
						TGridRect SelectCell;
						SelectCell.Left			= 0;
						SelectCell.Top				= i;
						SelectCell.Right			= 0;
						SelectCell.Bottom			= i;
						StringGrid1->Selection	= SelectCell;
						break;
					}
				}
			}
			else
			{
				for (int i=1; i<StringGrid1->RowCount; i++)
				{
					if (StringGrid1->Cells[LOCATION_NAME_INDEX][i] == qrFindLocation->FieldByName("Name")->AsString)
					{
						TGridRect SelectCell;
						SelectCell.Left			= 0;
						SelectCell.Top				= i;
						SelectCell.Right			= 0;
						SelectCell.Bottom			= i;
						StringGrid1->Selection	= SelectCell;
						break;
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::btnDeleteClick(TObject *Sender)
{
	if (StringGrid1->Selection.Top > StringGrid1->FixedRows-1)
	{
		if (Application->MessageBox("Are you sure you wish to delete this location?\r"
					"(Terminals will need to be restarted for the change to take effect)",
					"Delete Location?", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
		{
			qrDeleteLocation->Close();
			qrDeleteLocation->ParamByName("Location_Key")->AsInteger = int(StringGrid1->Objects[LOCATION_NAME_INDEX][StringGrid1->Selection.Top]);
			qrDeleteLocation->ExecQuery();
			Transaction->Commit();
			FormShow(NULL);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::btnRenameClick(TObject *Sender)
{
	if (StringGrid1->Selection.Top > StringGrid1->FixedRows-1)
	{
		AnsiString Location = "";
		if (InputQuery("New Location", "Please enter the name of the new Location.", Location))
		{
			if (Location != "")
			{
				qrFindLocation->Close();
				qrFindLocation->ParamByName("Name")->AsString = Location.UpperCase();
				qrFindLocation->ExecQuery();
				if (Location.UpperCase() == StringGrid1->Cells[LOCATION_NAME_INDEX][StringGrid1->Selection.Top].UpperCase() ||
						qrFindLocation->Eof)
				{
					if (Application->MessageBox("Are you sure you wish to rename this location?\r"
								"(Terminals will need to be restarted for the change to take effect)",
								"Rename Location?", MB_OKCANCEL + MB_ICONQUESTION) == ID_OK)
					{
						qrRenameLocation->Close();
						qrRenameLocation->ParamByName("Location_Key")->AsInteger = int(StringGrid1->Objects[LOCATION_NAME_INDEX][StringGrid1->Selection.Top]);
						qrRenameLocation->ParamByName("Name")->AsString = Location;
						qrRenameLocation->ExecQuery();
						Transaction->Commit();
						FormShow(NULL);
					}
				}
				else
				{
					Application->MessageBox("You must enter a new name for this location", "Error", MB_OK + MB_ICONERROR);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::btnSyncClick(TObject *Sender)
{
	std::auto_ptr<TfrmUpdateLocations> updateForm(new TfrmUpdateLocations(NULL));
	updateForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupLocations::btnInfoClick(TObject *Sender)
{
	if (StringGrid1->Selection.Top > StringGrid1->FixedRows-1)
	{
		qrLocInfo->Close();
		qrLocInfo->ParamByName("location_key")->AsInteger = int(StringGrid1->Objects[LOCATION_NAME_INDEX][StringGrid1->Selection.Top]);
		qrLocInfo->ExecQuery();
		std::vector<AnsiString> devices;
		while(!qrLocInfo->Eof)
		{
			devices.push_back(qrLocInfo->FieldByName("device_name")->AsString + " (" + qrLocInfo->FieldByName("product")->AsString + ")");
			qrLocInfo->Next();
		}
		AnsiString msg = "There are no terminals connected to this location.\r\r";
		if(devices.size() > 0)
		{
			msg = "The terminals connected to this location are as follows:\r\r";
			for(unsigned i=0; i<devices.size(); ++i)
			{
				msg = msg + devices[i] + "\r";
			}
			msg = msg + "\r";
		}
		StockTransaction->StartTransaction();
		try
		{
			qrStockLocInfo->ParamByName("Location")->AsString = StringGrid1->Cells[LOCATION_NAME_INDEX][StringGrid1->Selection.Top];
			qrStockLocInfo->ExecQuery();
			if(qrStockLocInfo->Eof)
			{
				msg = msg + "There are no stock items using this location.";
			}
			else
			{
				msg = msg + "There are " + qrStockLocInfo->Fields[0]->AsString + " stock items using this location.";
			}
		}
		__finally
		{
			StockTransaction->Commit();
		}
		Application->MessageBox(msg.c_str(), "Information", MB_OK + MB_ICONINFORMATION);
	}
}
//---------------------------------------------------------------------------

