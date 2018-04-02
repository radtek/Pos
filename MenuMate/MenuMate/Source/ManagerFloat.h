//---------------------------------------------------------------------------

#ifndef ManagerFloatH
#define ManagerFloatH
//---------------------------------------------------------------------------
#include "Forms.hpp"
#include "MM_DBCore.h"
#include "MMContactInfo.h"
#include "Contact.h"
#include "ListManager.h"
#include "FloatSkimData.h"



class TManagerFloat
{
   private :
	  TForm *Owner;
 //	  TFloatSkimData FloatSkimData;
   public :
   TManagerFloat(TForm * inOwner);
   void AlterFloat(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
   void SetFloat(Database::TDBTransaction &DBTransaction,TMMContactInfo &UserInfo);
   void OpenCashDrawerAccordingToPrinter(Database::TDBTransaction &DBTransaction);
};


#endif
