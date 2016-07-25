//---------------------------------------------------------------------------

#ifndef SecurityReferenceH
#define SecurityReferenceH
//---------------------------------------------------------------------------
#include "Classes.hpp"

enum TSecurityTypes { secOrderedBy, secCredit, secCancel,secPriceAdjust,secFloatSet,
                      secFloatAdjust,secZedded,secOpenDraw,secBilledBy,secDiscountedBy,
							 secSecurePayment,secTransferred,secWriteOff,secMemberDiscount,secLocationDiscount,secNone, secCheckRemoval, secReprintReceipt, secTransferredPrelim};

enum TSecurityFields { secfUserKey, secfEvent, secfFrom, secfTo,secfTimeStamp};
const char* const SecurityTypes[] = {"Ordered By",
                                       "Credit",
                                       "Cancel",
                                       "Price Adjust",
                                       "Float Set",
                                       "Float Adjustment",
                                       "Till Z Off",
                                       "Manually Opened Cash Drawer",
                                       "Billed By",
                                       "Discounted By",
                                       "Secure Payment",
                                       "Transferred",
                                       "WriteOff",
													"Member Discount",
													"Location Discount",
                                       NULL,
										"Check Removal",
										"Reprint Receipt",
										"Transferred Prelim",
                                        "Allow Reprint Orders"};

class TSecurityReference
{
	public:
	__int64 UserKey;
	UnicodeString TerminalName;
	UnicodeString Event;
	UnicodeString From;
	UnicodeString To;
	UnicodeString Note;
	TDateTime TimeStamp;
	void SecRefCopy(TSecurityReference *in);
	TSecurityReference();
};

#endif
