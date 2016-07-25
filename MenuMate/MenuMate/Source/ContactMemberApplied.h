//---------------------------------------------------------------------------

#ifndef ContactMemberAppliedH
#define ContactMemberAppliedH

#include "MMContactInfo.h"
//---------------------------------------------------------------------------
enum eMemberSource
{
   emsManual, emsSmartCard, emsSmartCardOffline,emsBarcode
};

class TContactMemberApplied
{
private:
public:
   TMMContactInfo Member;
   eMemberSource MemberSource;

   void Clear();
   void Assign(TContactMemberApplied inMembership);
   void Assign(TMMContactInfo inMember, eMemberSource inMemberSource);
   bool Applied();
   TContactMemberApplied();
};

#endif
