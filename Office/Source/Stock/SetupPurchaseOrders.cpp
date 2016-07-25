//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SetupPurchaseOrders.h"
#include "Connections.h"
#include "Consts.h"
#include "MMRegistry.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSetupPurchaseOrders *frmSetupPurchaseOrders;
//---------------------------------------------------------------------------
__fastcall TfrmSetupPurchaseOrders::TfrmSetupPurchaseOrders(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupPurchaseOrders::FormShow(TObject *Sender)
{
	memCompanyDetails->Text		= CurrentConnection.ReportHeader;
	memDeliveryAddress->Text	= CurrentConnection.DeliveryAddress;
	memInstructions->Text		= CurrentConnection.OrderInstructions;

    cbShowItemCost->Checked =
      !CurrentConnection.DontShowItemCostInPurchaseOrder;

	memCompanyDetails->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupPurchaseOrders::btnSaveClick(TObject *Sender)
{
	CurrentConnection.ReportHeader		= memCompanyDetails->Text;
	CurrentConnection.DeliveryAddress	= memDeliveryAddress->Text;
	CurrentConnection.OrderInstructions	= memInstructions->Text;

	AnsiString Key = OfficeKey + "\\" + CurrentConnection.CompanyName;

	RegistryWrite(Key, "CompanyDetails",		CurrentConnection.ReportHeader);
	RegistryWrite(Key, "DeliveryAddress",		CurrentConnection.DeliveryAddress);
	RegistryWrite(Key, "OrderInstructions",	CurrentConnection.OrderInstructions);

    CurrentConnection.DontShowItemCostInPurchaseOrder = !cbShowItemCost->Checked;
    RegistryWrite(Key, "DontShowItemCost", !cbShowItemCost->Checked);

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSetupPurchaseOrders::cbShowItemCostClick(
      TObject *Sender)
{
    CurrentConnection.DontShowItemCostInPurchaseOrder =
      !cbShowItemCost->Checked;    
}
//---------------------------------------------------------------------------

