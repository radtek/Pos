namespace MenumateServices.DTO.Clipp
{
    public class PaymentDetailRequest
    {
        public string ClippTabRef { get; set; }
        public string ClippMessageId { get; set; }
        public decimal Amount { get; set; }
        public string ErrorData { get; set; }
    }
}