//---------------------------------------------------------------------------

#ifndef SalesForceInterfaceH
#define SalesForceInterfaceH
#include "SalesForceWSDL.h"
//---------------------------------------------------------------------------
struct SalesForceFields
{
    bool Mobi2Go ;
    bool Stock;
    bool Thorlink;
    bool WebMate;
    bool Xero;
    bool ChefMate;
    bool PalmMate;
    bool Loyalty;
    UnicodeString VersionNumber;
    UnicodeString AccountName;
};
class TSalesForceInterface
{
	private:
    _di_ISalesForceIntegrationWebService sfClient;

   public :
   void initSFClient();
    void UpdateSalesForceFields(SalesForceFields sfFields);
   TSalesForceInterface();
};
#endif
