namespace MenumateServices.DTO.Clipp
{
    public class ClippTabDetailRequest 
    {
        //These parameters are url parameters..
        public string MessageId { get; set; }
        public string ClippTabRef { get; set; }
        public decimal Amount { get; set; }

        public ClippTabDetail TabData { get; set; }
    }
}