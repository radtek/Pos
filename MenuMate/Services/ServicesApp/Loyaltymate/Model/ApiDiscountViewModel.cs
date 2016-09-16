
using Loyaltymate.Enum;
namespace Loyaltymate.Model
{
    public class ApiDiscountViewModel
    {
        public long CompanyId { get; set; }
        public long DiscountId { get; set; }
        public string Name { get; set; }
        public string Code { get; set; }
        public DiscountType DiscountType { get; set; }
        public ImplicationType ImplicationType { get; set; }
        public decimal Value { get; set; }
        public decimal RoundToDecimalPlaces { get; set; }
        public DisplayOption DisplayAs { get; set; }
        public string Description { get; set; }
        public int? DiscountGroup { get; set; }
        public bool IsMembersOnlyDiscount { get; set; }
        public bool IsMemberExemptDiscount { get; set; }
        public int? PriorityOrder { get; set; }
        public int? AppearanceOrder { get; set; }
        public bool IsCategoryFilterApplicable { get; set; }
        public decimal? MaximumValue { get; set; }
        public int? MinimumNumberOfItemsAllowed { get; set; }
        public int? MaximumNumberOfItemsAllowed { get; set; }
        public int? DailyUsageAllowedPerMember { get; set; }
        public ProductPriority ProductPriority { get; set; }
    }
}