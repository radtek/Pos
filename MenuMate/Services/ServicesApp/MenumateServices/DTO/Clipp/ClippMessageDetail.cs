using System.Collections.Generic;

namespace MenumateServices.DTO.Clipp
{
    public class ClippMessageDetail
    {
        public string ClippUserRef { get; set; }
        public string FirstName { get; set; }
        public string LastName { get; set; }
        public string PhotoUrl { get; set; }
        public string ClippTabRef { get; set; }
        public string ClippPaymentRef { get; set; }
        public decimal TabLimit { get; set; }
        public string PosTabRef { get; set; }
        public decimal TotalPaymentAmount { get; set; }
        public decimal TipAmount { get; set; }
        public decimal ProcessingFeeAmount { get; set; }
        public decimal DiscountAmount { get; set; }
        public List<Discount> Discounts { get; set; }
    }
}