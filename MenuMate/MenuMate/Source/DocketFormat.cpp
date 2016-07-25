//---------------------------------------------------------------------------


#pragma hdrstop

#include "DocketFormat.h"
#include "GlobalSettings.h"
#include "ManagerVariable.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TDocketFormat::TDocketFormat()
{
   UnRavelKitchenPrinting = false;
   BulletSide = " > ";
   BulletOpt = "    ";
   BulletSetMenuItem = "    ";
   NoteHeader = "Note: ";
   SideHeader = "  Sides: ";
   MixHeader = "  Serve In: ";
   DishesPerSeatMultiplerString = " on ";
}

void TDocketFormat::Load(Database::TDBTransaction &DBTransaction)
{
   /* Create our on Variable Manager to try and avoid Threading conflicts */
   std::auto_ptr<TManagerVariable> PrinterVariables(new TManagerVariable());
   PrinterVariables->LocationProfileKey = TManagerVariable::Instance().LocationProfileKey;
   PrinterVariables->DeviceProfileKey = TManagerVariable::Instance().DeviceProfileKey;

   UnRavelKitchenPrinting = TGlobalSettings::Instance().UnRavelKitchenPrinting;
   BulletSide = PrinterVariables->GetStr(DBTransaction, vmKitchenBulletSide, " > ");
   BulletOpt = PrinterVariables->GetStr(DBTransaction, vmKitchenBulletOpt, "    ");
   BulletSetMenuItem = PrinterVariables->GetStr(DBTransaction, vmKitchenBulletSetMenuItem, "  ");
   NoteHeader = PrinterVariables->GetStr(DBTransaction, vmKitchenHeaderNote, "Note: ");
   SideHeader = PrinterVariables->GetStr(DBTransaction, vmKitchenHeaderSide, "  Sides: ");
   MixHeader = PrinterVariables->GetStr(DBTransaction, vmKitchenHeaderMix, "  Serve In: ");
   DishesPerSeatMultiplerString = PrinterVariables->GetStr(DBTransaction, vmKitchenPerSeatMultiplerString, " on ");
}