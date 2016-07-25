//---------------------------------------------------------------------------


#pragma hdrstop

#include "CashDrawerPortsList.h"

//---------------------------------------------------------------------------

TCashDrawerPortsList::TCashDrawerPortsList()
{
    initCashDrawerPorts();
}
//---------------------------------------------------------------------------

CashDrawerPortsMap TCashDrawerPortsList::GetCashDrawerPorts()
{
    return cashDrawerPortsMap;
}
//---------------------------------------------------------------------------

void TCashDrawerPortsList::initCashDrawerPorts()
{
    cashDrawerPortsMap.clear();

    addCashDrawerPort( None,    "None" );
    addCashDrawerPort( Com1,   "Com 1" );
    addCashDrawerPort( Com2,   "Com 2" );
    addCashDrawerPort( Com3,   "Com 3" );
    addCashDrawerPort( Com4,   "Com 4" );
    addCashDrawerPort( Com5,   "Com 5" );
    addCashDrawerPort( Com6,   "Com 6" );
    addCashDrawerPort( Com7,   "Com 7" );
    addCashDrawerPort( Com8,   "Com 8" );
    addCashDrawerPort( Com9,   "Com 9" );
    addCashDrawerPort( Com10,  "Com 10" );
    addCashDrawerPort( Com11,  "Com 11" );
    addCashDrawerPort( Com12,  "Com 12" );
    addCashDrawerPort( Com13,  "Com 13" );
    addCashDrawerPort( Com14,  "Com 14" );
    addCashDrawerPort( Com15,  "Com 15" );
    addCashDrawerPort( Com16,  "Com 16" );
    addCashDrawerPort( ReceiptPrinter,      "Receipt Printer" );
    addCashDrawerPort( CashdrawerPrinter,   "Cashdrawer Printer" );
    addCashDrawerPort( JavlinPOS_66X,       "Javlin / POS 66X Cashdrawer Port" );
    addCashDrawerPort( Gladius,             "Gladius Cashdrawer Port" );
    addCashDrawerPort( PT6200,              "PT6200" );
    addCashDrawerPort( WILLPOS_A10,         "WILLPOS A10" );
    addCashDrawerPort( WILLPOS_A20,         "WILLPOS A20" );
    addCashDrawerPort( Flytech_CD4B8B78,    "Flytech CD4B8B78" );
    addCashDrawerPort( Flytech_CDB4B8x2,    "Flytech CDB4B8x2" );
    addCashDrawerPort( Protech_Panel_PC,    "Protech Panel PC" );
    addCashDrawerPort( Protech_PS3100,      "Protech PS3100" );
    addCashDrawerPort( VPOS,                "Element485 C48" );
    addCashDrawerPort( EBN_XPOS,            "EBN_POS" );
    addCashDrawerPort( AdvancePOS_CM5200,   "AdvancePOS CM5200" );
    addCashDrawerPort( ToriPOS_815,         "ToriPOS 815" );
    addCashDrawerPort( AerPOS_3435,         "AerPOS AP-3435" );
    addCashDrawerPort( VPOS485_D36,         "Element485 D36" );

}
//---------------------------------------------------------------------------

void TCashDrawerPortsList::addCashDrawerPort( CashDrawerPorts portType, UnicodeString portStrDescription )
{
    cashDrawerPortsMap[portType] = portStrDescription;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
