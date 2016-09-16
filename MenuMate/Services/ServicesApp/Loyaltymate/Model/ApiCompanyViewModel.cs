using System.Collections.Generic;

namespace Loyaltymate.Model
{
    public class ApiCompanyViewModel
    {
        public long CompanyId { get; set; }
        public string CompanyCode { get; set; }
        public string SyndicateCode { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string CompanySubDomain { get; set; }
        public bool HasPocketVouchersAvailable { get; set; }
        public bool HasGiftCardsAvailable { get; set; }
        public IList<ApiTierLevelViewModel> TierLevels { get; set; }
        public IList<ApiDiscountViewModel> Discounts { get; set; }
    }
}