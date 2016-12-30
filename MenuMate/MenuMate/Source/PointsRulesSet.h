//---------------------------------------------------------------------------

#ifndef PointsRulesSetH
#define PointsRulesSetH

#define EXPENSECARD "E"
#define FUNCTIONCARD "F"

#include <system.hpp>

/*This EMUN is compressed into an bitset sotred on thesmartcards in an integer so dont
  add more than 32 options here.*/
#define eprFinancial                            0x0000080
#define eprAllowDiscounts                       0x0000040
#define eprNoPointsRedemption                   0x0000020
#define eprNoPointsPurchases                    0x0000010
#define eprOnlyEarnsPointsWhileRedeemingPoints  0x0000008
#define eprEarnsPointsWhileRedeemingPoints      0x0000004
#define eprNeverEarnsPoints                     0x0000002
#define eprAllowedNegitive                      0x0000001

typedef Set<int,eprAllowedNegitive,eprNoPointsRedemption> TPointsRulesSet;
typedef Set<int,eprAllowedNegitive,eprFinancial> TPointsRulesSubsSet;

#endif
