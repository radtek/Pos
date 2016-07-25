//---------------------------------------------------------------------------


#pragma hdrstop

#include "CashDrawFactory.h"
#include "CashDrawAdvancePOS.h"
#include "CashDrawComPort.h"
#include "PortCashDraw.h"
#include "PrinterCashDraw.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

/**
 * Builds a cash draw controller of the type selected in setup.
 *
 * @param selectedCashDrawPort Enum to determine the type of cash draw to return.
 * @param inPortLength Size of the buffer for com port kickers and cash draw printer.
 * @param inPortHighChars
 * @param inReceiptPrinter The
 * @return
 */
TCashDraw* TCashDrawFactory::CreateCashDraw(CashDrawerPorts selectedCashDrawPort, int inPortLength, bool inPortHighChars, TPrinterPhysical& inReceiptPrinter)
{
    TCashDraw* cashDraw = NULL;
    switch(selectedCashDrawPort)
    {
        case Com1:
            cashDraw = new TCashDrawComPort(1, inPortLength, inPortHighChars);
            break;
        case Com2:
            cashDraw = new TCashDrawComPort(2, inPortLength, inPortHighChars);
            break;
        case Com3:
            cashDraw = new TCashDrawComPort(3, inPortLength, inPortHighChars);
            break;
        case Com4:
            cashDraw = new TCashDrawComPort(4, inPortLength, inPortHighChars);
            break;
        case Com5:
            cashDraw = new TCashDrawComPort(5, inPortLength, inPortHighChars);
            break;
        case Com6:
            cashDraw = new TCashDrawComPort(6, inPortLength, inPortHighChars);
            break;
        case Com7:
            cashDraw = new TCashDrawComPort(7, inPortLength, inPortHighChars);
            break;
        case Com8:
            cashDraw = new TCashDrawComPort(8, inPortLength, inPortHighChars);
            break;
        case Com9:
            cashDraw = new TCashDrawComPort(9, inPortLength, inPortHighChars);
            break;
        case Com10:
            cashDraw = new TCashDrawComPort(10, inPortLength, inPortHighChars);
            break;
        case Com11:
            cashDraw = new TCashDrawComPort(11, inPortLength, inPortHighChars);
            break;
        case Com12:
            cashDraw = new TCashDrawComPort(12, inPortLength, inPortHighChars);
            break;
        case Com13:
            cashDraw = new TCashDrawComPort(13, inPortLength, inPortHighChars);
            break;
        case Com14:
            cashDraw = new TCashDrawComPort(14, inPortLength, inPortHighChars);
            break;
        case Com15:
            cashDraw = new TCashDrawComPort(15, inPortLength, inPortHighChars);
            break;
        case Com16:
            cashDraw = new TCashDrawComPort(16, inPortLength, inPortHighChars);
            break;
        case ReceiptPrinter:
            cashDraw = new TCashDrawReceiptPrinter(inReceiptPrinter);
            break;
        case CashdrawerPrinter:
            cashDraw = new TCashDrawCashDrawPrinter(inPortLength);
            break;
        case JavlinPOS_66X:
            cashDraw = new TCashDrawJavlin();
            break;
        case Gladius:
            cashDraw = new TCashDrawGladius();
            break;
        case PT6200:
            cashDraw = new TCashDrawPT6200();
            break;
        case WILLPOS_A10:
            cashDraw = new TCashDrawWillPosA10();
            break;
        case WILLPOS_A20:
            cashDraw = new TCashDrawWillPosA20();
            break;
        case Flytech_CD4B8B78:
            cashDraw = new TCashDrawFlytechCD4B8B78();
            break;
        case Flytech_CDB4B8x2:
            cashDraw = new TCashDrawFlytechCDB4B8x2();
            break;
        case Protech_Panel_PC:
            cashDraw = new TCashDrawProtechPanelPc();
            break;
        case Protech_PS3100:
            cashDraw = new TCashDrawProtechPS3100();
            break;
        case VPOS:
            cashDraw = new TCashDrawVPos335();
            break;
        case EBN_XPOS:
            cashDraw = new TCashDrawEbnXPos752();
            break;
        case AdvancePOS_CM5200:
            cashDraw = new TCashDrawAdvancePOS();
            break;
        case ToriPOS_815:
            cashDraw = new TCashDrawToripos815();
            break;
        case AerPOS_3435:
            cashDraw = new TCashDrawAerpos3435();
            break;
        case VPOS485_D36:
            cashDraw = new TCashDrawVPos485D36();
            break;
        default:
            break;
    }
    return cashDraw;
}
//---------------------------------------------------------------------------
