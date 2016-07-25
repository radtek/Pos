//---------------------------------------------------------------------------

#ifndef LoyaltyPrintingH
#define LoyaltyPrintingH

#include "Request.h"
#include "MMLogging.h"
#include "PrintFormat.h"
#include "Money.h"
#include "ManagerPhysicalPrinter.h"
#include "VirtualPrinterManager.h"
#include "MM_DBCore.h"
#include "ManagerChitNumber.h"
#include "ReqPrintJob.h"

#include "enumPrinting.h"
#include "PrintingSections.h"

// Helper Class for generating activation receipts for loyalty cards
//------------------------------------------------------------------------------
class TLoyaltyPrinting
{
    private:
        static TLoyaltyPrinting* instance;
        TLoyaltyPrinting();
        ~TLoyaltyPrinting();

        //TODO: remove this mock method
        TPSectionInstruction buildMockInstruction(
                                                int order,
                                                ePrintOutFormatInstructions instruction,
                                                UnicodeString caption,
                                                bool lineAbove,
                                                bool lineBelow,
                                                bool visible,
                                                bool cut
                                                    );

	public:
        static TLoyaltyPrinting* Instance()
        {
            if(instance == NULL)
                instance = new TLoyaltyPrinting();

            return instance;
        }
	    bool GetPrintouts(Database::TDBTransaction &DBTransaction,TReqPrintJob *Request, TPrinterPhysical Printer);

        // Returns the Print job stored in the PrintJobs TList so we can add to it.
        void SetHeaderFooter(TStrings *inHeader ,TStrings *inFooter);
        UnicodeString MemberNumber;
        UnicodeString MemberName;

        void LoadLoyaltyPrintingTemplate(Database::TDBTransaction &DBTransaction,int PrinterKey,TSectionInstructStorage &inTemplate);
        void SaveLoyaltyPrintingTemplate(Database::TDBTransaction &DBTransaction,int PrinterKey,TSectionInstructStorage &inTemplate);
};
//------------------------------------------------------------------------------

#endif
