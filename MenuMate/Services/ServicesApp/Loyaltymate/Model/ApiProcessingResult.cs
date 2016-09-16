using Loyaltymate.Enum;

namespace Loyaltymate.Model
{
    public class ApiProcessingResult
    {
        public string UniqueReferenceNumber { get; set; }
        public ElementType ElementType { get; set; }
        public bool IsProcessedSuccessfully { get; set; }
        public string Error { get; set; }
    }
}