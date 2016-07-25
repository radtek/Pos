//---------------------------------------------------------------------------

#ifndef DiscountGroupH
#define DiscountGroupH

#include <Classes.hpp>

#define SCD_DISCOUNT_GROUP "Senior Citizen"
#define PWD_DISCOUNT_GROUP "Person with Disability"
#define VIP_DISCOUNT_GROUP "VIP"
#define EMPLOYEE_DISCOUNT_GROUP "Employee"
#define GPC_DISCOUNT_GROUP "GPC"
#define GRP1_DISCOUNT_GROUP "Discount 1"
#define GRP2_DISCOUNT_GROUP "Discount 2"
#define GRP3_DISCOUNT_GROUP "Discount 3"
#define GRP4_DISCOUNT_GROUP "Discount 4"
#define GRP5_DISCOUNT_GROUP "Discount 5"
#define GRP6_DISCOUNT_GROUP "Discount 6"
#define COMPLIMENTARY_GROUP "Complimentary"
#define NONCHARGABLE_GROUP "Non-Chargeable"
#define CORPORATE_GROUP "Corporate Discount"
#define PROMO_DISCOUNT_GROUP "Promo Discount"
#define OTHER_DISCOUNT_GROUP "Other Discount"
//---------------------------------------------------------------------------

class TDiscountGroup
{
    public:
        TDiscountGroup(int dbkey);
        TDiscountGroup(int dbKey, UnicodeString name, bool isDefault);
        int DBKey;
        UnicodeString Name;
        bool IsDefault;
        int DiscountGroupType;
        bool TaxExempt;
};
#endif
