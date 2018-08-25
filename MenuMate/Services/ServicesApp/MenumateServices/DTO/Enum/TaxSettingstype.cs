using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.DTO.Enum
{
    public enum TaxSettingType
    {
        ItemPriceIncludeTax = 3,
        ItemPriceIncludeServiceCharge,
        CalculateTaxBeforeDiscount,
        CalculateTaxAfterDiscount,
        CalculateScPreDiscountedPrice,
        ReCalculateScAfterDiscount,
        ApplyServiceChargeTax,
        ServiceChargeTaxRate
    }
}
