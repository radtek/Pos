namespace MenumateServices.DTO.Clipp
{
    public class PaymentDetailResponse
    {
        public string ClippTabRef { get; set; }
        public string PosTabRef { get; set; }
        public string ClippPaymentRef { get; set; }
        public decimal TotalPaymentAmount { get; set; }
        public decimal ProcessingFeeAmount { get; set; }
    }
}