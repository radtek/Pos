namespace MenumateServices.DTO.Clipp
{
    public class ClippErrorDetail
    {
        public string ClippTabRef { get; set; }
        public string MessageId { get; set; }
        public decimal TotalAmount { get; set; }
       
        public string ErrorCode { get; set; }
        public string ErrorDescription { get; set; }
    }
}