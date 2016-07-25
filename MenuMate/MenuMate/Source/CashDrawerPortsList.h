//---------------------------------------------------------------------------

#ifndef CashDrawerPortsListH
#define CashDrawerPortsListH
//---------------------------------------------------------------------------

#include "System.hpp"
#include <map>

// all supported cash drawer ports
enum CashDrawerPorts
{
    None = 0,
    Com1,
    Com2,
    Com3,
    Com4,
    Com5,
    Com6,
    Com7,
    Com8,
    Com9,
    Com10,
    Com11,
    Com12,
    Com13,
    Com14,
    Com15,
    Com16,
    ReceiptPrinter,
    CashdrawerPrinter,
    JavlinPOS_66X,
    Gladius,
    PT6200,
    WILLPOS_A10,
    WILLPOS_A20,
    Flytech_CD4B8B78,
    Flytech_CDB4B8x2,
    Protech_Panel_PC,
    Protech_PS3100,
    VPOS,
    EBN_XPOS,
    AdvancePOS_CM5200,
    ToriPOS_815,
    AerPOS_3435 ,
    VPOS485_D36
};

// cash drawer ports map

typedef std::map<CashDrawerPorts,UnicodeString> CashDrawerPortsMap;

class TCashDrawerPortsList
{
    public:
        TCashDrawerPortsList();
        CashDrawerPortsMap GetCashDrawerPorts();
    private:
        void initCashDrawerPorts();
        void addCashDrawerPort( CashDrawerPorts portType, UnicodeString portStrDescription );
        CashDrawerPortsMap cashDrawerPortsMap;
};

#endif
