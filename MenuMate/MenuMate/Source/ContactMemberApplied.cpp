//---------------------------------------------------------------------------


#pragma hdrstop

#include "ContactMemberApplied.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


void TContactMemberApplied::Clear()
{
   Member.Clear();
}

void TContactMemberApplied::Assign(TContactMemberApplied inMemberApplied)
{
   Member = inMemberApplied.Member;
   MemberSource = inMemberApplied.MemberSource;
}

void TContactMemberApplied::Assign(TMMContactInfo inMember, eMemberSource inMemberSource)
{
   Member = inMember;
   MemberSource = inMemberSource;
}

bool TContactMemberApplied::Applied()
{
	return Member.Valid();
}

/* void TContactMemberApplied::Assign(TContactPoints inPoints)
{
Points.Assign(inPoints);
} */

TContactMemberApplied::TContactMemberApplied()
{
   MemberSource = emsManual;
   Member.Points.Clear();
}