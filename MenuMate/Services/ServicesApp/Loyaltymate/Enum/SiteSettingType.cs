﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Loyaltymate.Enum
{
    public enum SiteSettingType
    {
        PickUp = 0,
        DineIn,
        TakeAway,
        ItemPriceIncludeTax,
        ItemPriceIncludeServiceCharge,
        CalculateTaxBeforeDiscount,
        CalculateTaxAfterDiscount,
        CalculateScPreDiscountedPrice,
        ReCalculateScAfterDiscount,
        ApplyServiceChargeTax,
        ServiceChargeTaxRate
    }
}
